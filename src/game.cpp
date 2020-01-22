//
//  begin.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <time.h>

#include "stereogram/stereogram.hpp"

#include "game.h"
#include "data.h"
#include "progress.h"

main::Game::Game()
    : core::Runner(
        (__int32_t)core::VIEW_INFO::Landscape |
        (__int32_t)core::VIEW_INFO::ScreenOn |
        (__int32_t)core::VIEW_INFO::CloseMenu,
        480,
        "move turn food die win")
    , pause_{true}
    , frame_{0}
    , pattern_frame_{FR - 1}
    , column_{0}
    , cell_{0}
    , margin_{0, 0}
    , touch_{0, 0}
{
    std::srand(time(NULL));
    frame_lenght_ = std::operator""ms((unsigned long long)50);
}

main::Game::~Game()
{
}

void main::Game::Escape()
{
    main::progress_ = main::PROGRESS::MENU;
    bridge::NeedRestart();
}

void main::Game::Initial()
{
    column_ = dpi_ / 2;
    cell_ = std::min((width_ - column_ * 2) / (data_.c_x_ + 1), height_ / (data_.c_y_ + 1));
    border_ = cell_ / 3;
    margin_[0] = (height_ - cell_ * data_.c_y_ - border_ * 2) / 2;
    margin_[1] = (width_ - cell_ * data_.c_x_ - border_ * 2) / 2;
    pattern_target_ = stereogram::CreateRandomPattern<4>(column_, order_rgba_);
    pattern_current_.resize(pattern_target_.size());
    pattern_base_.resize(pattern_target_.size());
}

void main::Game::Step(__uint32_t* pixels)
{
    if (!pause_)
    {
        if (++frame_ == data_.GetDelay())
            Play(false);
    }
    if (++pattern_frame_ == FR)
    {
        std::swap(pattern_base_, pattern_target_);
        pattern_target_ = stereogram::CreateRandomPattern<4>(column_, order_rgba_);
        pattern_frame_ = 0;
    }
    for (int i = 0; i < pattern_current_.size(); ++i)
        pattern_current_[i] = ((int) pattern_target_[i] * pattern_frame_ + (int) pattern_base_[i] * (FR - pattern_frame_)) / FR;
    ApplyBoard(pixels);
    stereogram::Convert<4, 16>((unsigned char*)pixels, column_, width_, height_, pattern_current_.data());
}

void main::Game::TouchBegin(const float x, const float y)
{
    touch_[0] = y;
    touch_[1] = x;
}

void main::Game::TouchMove(const float x, const float y)
{
}

void main::Game::TouchEnd(const float x, const float y)
{
    touch_[0] = y - touch_[0];
    touch_[1] = x - touch_[1];
    if (std::pow(touch_[0] * touch_[0] + touch_[1] * touch_[1], 0.5) < 10)
    {
        if (pause_)
        {
            if (data_.lives_ > 0)
            {
                frame_ = 0;
                pause_ = false;
            }
        }
        else
            pause_ = true;
    }
    else if (!pause_)
    {
        int side = 0;
        if (std::abs(touch_[0]) > std::abs(touch_[1]))
        {
            if (touch_[0] < 0)
            {
                if (data_.side_ != 1)
                    side = -1;
            }
            else
            {
                if (data_.side_ != -1)
                    side = 1;
            }
        }
        else
        {
            if (touch_[1] < 0)
            {
                if (data_.side_ != 2)
                    side = -2;
            }
            else
            {
                if (data_.side_ != -2)
                    side = 2;
            }
        }
        if (side != 0)
        {
            data_.side_ = side;
            Play(true);
        }
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
                return false;
        }
        data_.parts_.push_front(head);
        return 1;
    }
    return -1;
}

void main::Game::Play(bool turn)
{
    int audio;
    switch (Move())
    {
    case 0:
        pause_ = true;
        if (--data_.lives_ > 0)
            data_.ResetSnakeFood();
        audio = 3;
        break;
    case 1:
        audio = turn ? 1 : 0;
        break;
    case 2:
        audio = 2;
        break;
    case 3:
        data_.score_ = data_.score_ * (data_.lives_max_ + 1) / (data_.lives_max_ + 1 - data_.lives_);
        data_.lives_ = 0;
        pause_ = true;
        audio = 4;
        break;
    }
    if (data_.sound_)
        bridge::PlayAudio(audio);
    frame_ = 0;
}

void main::Game::ApplyBoard(__uint32_t *pixels)
{
    memset(pixels, color_empty_, width_ * height_ * 4);
    DrawBorder(pixels, color_border_);
    auto it_pre = data_.parts_.begin();
    for (auto it = std::next(it_pre); it != data_.parts_.end(); ++it)
    {
        DrawTail(pixels, *it_pre, (*it)[0] - (*it_pre)[0] + 2 * ((*it)[1] - (*it_pre)[1]));
        it_pre = it;
    }
    DrawHead(pixels);
    DrawFood(pixels);
}

void main::Game::DrawBorder(__uint32_t* pixels, const __uint32_t color)
{
    for (int i = margin_[0]; i < margin_[0] + border_; ++i)
        for (int j = margin_[1]; j < margin_[1] + cell_ * data_.c_x_ + border_ * 2; ++j)
            pixels[i * width_ + j] = color;
    for (int i = margin_[0] + border_; i < margin_[0] + border_ +  cell_ * data_.c_y_; ++i)
    {
        for (int j = margin_[1]; j < margin_[1] + border_; ++j)
            pixels[i * width_ + j] = color;
        for (int j = margin_[1] + cell_ * data_.c_x_ + border_; j < margin_[1] + cell_ * data_.c_x_ + border_ * 2; ++j)
            pixels[i * width_ + j] = color;
    }
    for (int i = margin_[0] + cell_ * data_.c_y_ + border_; i < margin_[0] + cell_ * data_.c_y_ + 2 * border_; ++i)
        for (int j = margin_[1]; j < margin_[1] + cell_ * data_.c_x_ + border_ * 2; ++j)
            pixels[i * width_ + j] = color;    
}

void main::Game::DrawFood(__uint32_t* pixels)
{
    int center[2];
    center[0] = margin_[0] + border_ + data_.food_[0] * cell_ + cell_ / 2;
    center[1] = margin_[1] + border_ + data_.food_[1] * cell_ + cell_ / 2;
    for (int i = margin_[0] + border_ + data_.food_[0] * cell_; i < margin_[0] + border_ + cell_ + data_.food_[0] * cell_; ++i)
        for (int j = margin_[1] + border_ + data_.food_[1] * cell_; j < margin_[1] + border_ + cell_ + data_.food_[1] * cell_; ++j)
            if (std::pow((i - center[0]) * (i - center[0]) + (j - center[1]) * (j - center[1]), 0.5) <= cell_ / 2)
                pixels[i * width_ + j] = color_food_;
}

void main::Game::DrawHead(__uint32_t* pixels)
{
    for (int i = margin_[0] + border_ + data_.parts_.front()[0] * cell_; i < margin_[0] + border_ + cell_ + data_.parts_.front()[0] * cell_; ++i)
        for (int j = margin_[1] + border_ + data_.parts_.front()[1] * cell_; j < margin_[1] + border_ + cell_ + data_.parts_.front()[1] * cell_; ++j)
            pixels[i * width_ + j] = color_head_;
}

void main::Game::DrawTail(__uint32_t* pixels, const std::array<int, 2> & tail, const int side)
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
