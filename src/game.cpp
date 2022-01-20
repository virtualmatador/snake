//
//  begin.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <algorithm>
#include <cstring>
#include <sstream>

#include <time.h>

#include "stereogram/stereogram.hpp"

#include "game.h"
#include "data.h"
#include "progress.h"

main::Game::Game()
    : run_{true}
    , width_{0}
    , height_{0}
    , order_rgba_{0x00010203}
    , frame_length_{std::chrono::milliseconds(50)}
    , drag_{0}
    , pause_{true}
    , frame_{0}
    , pattern_frame_{animation_frames_ - 1}
    , cell_{0}
    , margin_{0, 0}
    , image_index_{0}
{
    SetHandlers();
    bridge::LoadView(index_,
        (std::int32_t)core::VIEW_INFO::Landscape |
        (std::int32_t)core::VIEW_INFO::ScreenOn |
        (std::int32_t)core::VIEW_INFO::AudioNoSolo,
        "game");
}

main::Game::~Game()
{
    pixels_lock_.lock();
    run_ = false;
    pixels_lock_.unlock();
    if (worker_.joinable())
    {
        worker_.join();
    }
}

void main::Game::SetHandlers()
{
    handlers_["body"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "ready") == 0)
            Setup();
        else if (std::strcmp(command, "image") == 0)
            Run(info);
        else if (std::strcmp(command, "resize") == 0)
            Resize(info);
        else if (std::strcmp(command, "escape") == 0)
            Escape();
        else if (std::strcmp(command, "back") == 0)
            ApplyBack();
    };
    handlers_["game"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "tick") == 0)
            Tick();
        else if (std::strcmp(command, "audio") == 0)
            PlayAudio(info);
        else if (std::strcmp(command, "drag") == 0)
            Drag(info);
    };
}

void main::Game::Setup()
{
    std::ostringstream js;
    js << "widths_['image'] = " << request_width_ << ";";
    bridge::CallFunction(js.str().c_str());
    bridge::CreateImage("image", "body");
    bridge::CallFunction("setup()");
}

void main::Game::Run(const char* dimensions)
{
    Resize(dimensions);
    worker_ = std::thread([this, index = index_]()
    {
        auto frame_time = std::chrono::steady_clock::now();
        for (;;)
        {
            auto now = std::chrono::steady_clock::now();
            std::unique_lock<std::mutex> auto_locker{ pixels_lock_ };
            if (!run_)
            {
                break;
            }
            if (drag_ != 0)
            {
                Turn();
                frame_time = now + frame_length_;
            }
            auto wait = frame_time - now;
            if (wait.count() <= 0)
            {
                frame_time += frame_length_;
                Step();
                bridge::AsyncMessage(index, "game", "tick", "");
            }
            else
            {
                waiter_.wait_for(auto_locker, wait);
            }
        }
    });
}

void main::Game::Tick()
{
    bridge::ResetImage(index_, ++image_index_, "image");
}

void main::Game::Resize(const char* size)
{
    std::istringstream parser(size);
    int32_t width = 0;
    int32_t height = 0;
    parser >> width >> height;
    height = height * request_width_ / width;
    width = request_width_;
    Initial(width, height);
}

void main::Game::FeedUri(const char* uri, std::function<void(
            const std::vector<unsigned char>&)>&& consume)
{
    pixels_lock_.lock();
    consume(pixels_);
    pixels_lock_.unlock();
}

void main::Game::Escape()
{
    main::progress_ = main::PROGRESS::MENU;
    bridge::NeedRestart();
}

void main::Game::Initial(int32_t width, int32_t height)
{
    pixels_lock_.lock();
    width_ = width;
    height_ = height;
    pixels_.resize(width_ * height_ * bpp_ + 54);
    pixels_[0] = 'B';
    pixels_[1] = 'M';
    *(uint32_t*)&pixels_[2] = pixels_.size();
    *(uint32_t*)&pixels_[6] = 0;
    *(uint32_t*)&pixels_[10] = 54;
    *(uint32_t*)&pixels_[14] = 40;
    *(int32_t*)&pixels_[18] = width_;
    *(int32_t*)&pixels_[22] = height_;
    *(uint16_t*)&pixels_[26] = 1;
    *(uint16_t*)&pixels_[28] = 8 * bpp_;
    *(uint32_t*)&pixels_[30] = 0;
    *(uint32_t*)&pixels_[34] = width_ * height_ * bpp_;
    *(int32_t*)&pixels_[38] = 10000;
    *(int32_t*)&pixels_[42] = 10000;
    *(uint32_t*)&pixels_[46] = 0;
    *(uint32_t*)&pixels_[50] = 0;
    pattern_target_ =
        stereogram::CreatePattern<bpp_>((width_ / columns_), order_rgba_);
    pattern_current_.resize(pattern_target_.size());
    pattern_base_.resize(pattern_target_.size());
    cell_ = std::min((width_ - (width_ / columns_) * 2) / (data_.c_x_ + 1),
        height_ / (data_.c_y_ + 1));
    border_ = cell_ / 3;
    margin_[0] = (height_ - cell_ * data_.c_y_ - border_ * 2) / 2;
    margin_[1] = (width_ - cell_ * data_.c_x_ - border_ * 2) / 2;
    pixels_lock_.unlock();
}

void main::Game::Step()
{
    if (++pattern_frame_ == animation_frames_)
    {
        std::swap(pattern_base_, pattern_target_);
        pattern_target_ =
            stereogram::CreatePattern<bpp_>((width_ / columns_), order_rgba_);
        pattern_frame_ = 0;
    }
    for (int i = 0; i < pattern_current_.size(); ++i)
    {
        pattern_current_[i] = ((int) pattern_target_[i] * pattern_frame_ +
            (int) pattern_base_[i] * (animation_frames_ - pattern_frame_)) /
            animation_frames_;
    }
    if (!pause_)
    {
        if (++frame_ == data_.GetDelay())
            Play(false);
    }
    auto bitmap = &pixels_[54];
    ApplyBoard((PIXEL*)bitmap);
    stereogram::Convert<bpp_, 16>(bitmap, (width_ / columns_), width_, height_,
        pattern_current_.data());
}

void main::Game::Drag(const char* side)
{
    int mX = 0;
    int mY = 0;
    std::istringstream is{ side };
    is >> mX >> mY;
    pixels_lock_.lock();
    if (std::abs(mX) < 1 && std::abs(mY) < 1)
    {
        if (pause_)
        {
            if (data_.lives_ > 0)
            {
                frame_ = 0;
                UpdatePause(false, false);
            }
        }
        else
        {
            UpdatePause(true, false);
        }
    }
    else if (!pause_)
    {
        if (std::abs(mX) > std::abs(mY))
        {
            if (mX < 0)
            {
                drag_ = -2;
            }
            else if (mX > 0)
            {
                drag_ = 2;
            }
        }
        else
        {
            if (mY < 0)
            {
                drag_ = 1;
            }
            else
            {
                drag_ = -1;
            }
        }
    }
    pixels_lock_.unlock();
    waiter_.notify_all();
}

void main::Game::Turn()
{
    int new_side = 0;
    if (drag_ == -1)
    {
        if (data_.side_ != 1)
            new_side = -1;
    }
    else if (drag_ == 1)
    {
        if (data_.side_ != -1)
            new_side = 1;
    }
    if (drag_ == -2)
    {
        if (data_.side_ != 2)
            new_side = -2;
    }
    else if (drag_ == 2)
    {
        if (data_.side_ != -2)
            new_side = 2;
    }
    drag_ = 0;
    if (new_side != 0)
    {
        data_.side_ = new_side;
        Play(true);
    }
}

int main::Game::Move()
{
    auto head = data_.parts_.front();
    switch (data_.side_)
    {
    case -1:
        if (head[0] > 0)
            --head[0];
        else
            return 0;
        break;
    case 1:
        if (head[0] < data_.c_y_ - 1)
            ++head[0];
        else
            return 0;
        break;
    case -2:
        if (head[1] > 0)
            --head[1];
        else
            return 0;
        break;
    case 2:
        if (head[1] < data_.c_x_ - 1)
            ++head[1];
        else
            return 0;
        break;
    }
    if (head == data_.food_)
    {
        data_.score_ += data_.parts_.size();
        data_.parts_.push_front(head);
        data_.ResetFood();
        if (++data_.eat_ == data_.eat_max_)
            return 3;
        return 2;
    }
    else
    {
        data_.parts_.pop_back();
        for (const auto & part : data_.parts_)
        {
            if (head == part)
                return 0;
        }
        data_.parts_.push_front(head);
        return 1;
    }
}

void main::Game::Play(bool turn)
{
    const char* audio;
    switch (Move())
    {
    case 0:
        UpdatePause(true, true);
        if (--data_.lives_ > 0)
            data_.ResetSnakeFood();
        audio = "die";
        break;
    case 1:
        audio = turn ? "turn" : "move";
        break;
    case 2:
        audio = "food";
        break;
    case 3:
        data_.score_ = data_.score_ * (data_.lives_max_ + 1) /
            (data_.lives_max_ + 1 - data_.lives_);
        data_.lives_ = 0;
        UpdatePause(true, true);
        audio = "win";
        break;
    }
    frame_ = 0;
    if (data_.sound_)
    {
        bridge::AsyncMessage(index_, "game", "audio", audio);
    }
}

void main::Game::ApplyBoard(PIXEL* pixels)
{
    for (int i = 0; i < height_; ++i)
    {
        for (int j = 0; j < width_; j++)
        {
            pixels[i * width_ + j] = color_empty_;
        }
    }
    DrawBorder(pixels);
    auto it_pre = data_.parts_.begin();
    for (auto it = std::next(it_pre); it != data_.parts_.end(); ++it)
    {
        DrawTail(pixels, *it_pre, (*it)[0] - (*it_pre)[0] + 2 * ((*it)[1] -
            (*it_pre)[1]));
        it_pre = it;
    }
    DrawHead(pixels);
    DrawFood(pixels);
}

void main::Game::DrawBorder(PIXEL* pixels)
{
    for (int i = margin_[0]; i < margin_[0] + border_; ++i)
        for (int j = margin_[1];
            j < margin_[1] + cell_ * data_.c_x_ + border_ * 2; ++j)
            pixels[i * width_ + j] = color_border_;
    for (int i = margin_[0] + border_;
        i < margin_[0] + border_ +  cell_ * data_.c_y_; ++i)
    {
        for (int j = margin_[1]; j < margin_[1] + border_; ++j)
            pixels[i * width_ + j] = color_border_;
        for (int j = margin_[1] + cell_ * data_.c_x_ + border_;
            j < margin_[1] + cell_ * data_.c_x_ + border_ * 2; ++j)
            pixels[i * width_ + j] = color_border_;
    }
    for (int i = margin_[0] + cell_ * data_.c_y_ + border_;
        i < margin_[0] + cell_ * data_.c_y_ + 2 * border_; ++i)
        for (int j = margin_[1];
            j < margin_[1] + cell_ * data_.c_x_ + border_ * 2; ++j)
            pixels[i * width_ + j] = color_border_;
}

void main::Game::DrawFood(PIXEL* pixels)
{
    int center[2];
    center[0] = margin_[0] + border_ + data_.food_[0] * cell_ + cell_ / 2;
    center[1] = margin_[1] + border_ + data_.food_[1] * cell_ + cell_ / 2;
    for (int i = margin_[0] + border_ + data_.food_[0] * cell_;
        i < margin_[0] + border_ + cell_ + data_.food_[0] * cell_; ++i)
        for (int j = margin_[1] + border_ + data_.food_[1] * cell_;
            j < margin_[1] + border_ + cell_ + data_.food_[1] * cell_; ++j)
            if (std::pow((i - center[0]) * (i - center[0]) +
                (j - center[1]) * (j - center[1]), 0.5) <= cell_ / 2)
                pixels[i * width_ + j] = color_food_;
}

void main::Game::DrawHead(PIXEL* pixels)
{
    for (int i = margin_[0] + border_ + data_.parts_.front()[0] * cell_;
        i < margin_[0] + border_ + cell_ + data_.parts_.front()[0] * cell_; ++i)
        for (int j = margin_[1] + border_ + data_.parts_.front()[1] * cell_;
            j < margin_[1] + border_ + cell_ + data_.parts_.front()[1] * cell_;
            ++j)
            pixels[i * width_ + j] = color_head_;
}

void main::Game::DrawTail(
    PIXEL* pixels, const std::array<int, 2> & tail, const int side)
{
    int extra[2];
    extra[std::abs(side) - 1] = cell_ / 4 + 1;
    extra[2 - std::abs(side)] = 0;
    int margin[2];
    margin[std::abs(side) - 1] = cell_;
    margin[2 - std::abs(side)] = cell_ / 4 + 1;
    int center[2];
    center[0] = margin_[0] + border_ + tail[0] * cell_ + cell_ / 2;
    center[1] = margin_[1] + border_ + tail[1] * cell_ + cell_ / 2;
    int left = center[1] - (side != 2) * margin[1] - extra[1];
    int right = center[1] + (side != -2) * margin[1] + extra[1];
    int top = center[0] - (side != 1) * margin[0] - extra[0];
    int bottom = center[0] + (side != -1) * margin[0] + extra[0];
    for (int i = top; i <= bottom; ++i)
        for (int j = left; j <= right; ++j)
            pixels[i * width_ + j] = color_tail_;
}

void main::Game::UpdatePause(bool state, bool async)
{
    pause_ = state;
    if (async)
    {
        bridge::AsyncMessage(index_, "body", "back", "");
    }
    else
    {
        ApplyBack();
    }
}

void main::Game::ApplyBack()
{
    std::ostringstream js;
    js << "showBack(" << (pause_ ? "true" : "false") << ");";
    bridge::CallFunction(js.str().c_str());
}

void main::Game::PlayAudio(const char* audio)
{
    std::ostringstream js;
    js << "playAudio('" << audio << "');";
    bridge::CallFunction(js.str().c_str());
}
