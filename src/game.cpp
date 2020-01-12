//
//  begin.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <time.h>

#include "steriogram/steriogram.hpp"

#include "game.h"
#include "data.h"
#include "progress.h"

main::Game::Game()
    : core::Runner(
        (__int32_t)core::VIEW_INFO::Landscape |
        (__int32_t)core::VIEW_INFO::ScreenOn |
        (__int32_t)core::VIEW_INFO::CloseMenu, 480)
    , pause_{true}
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
    cell_ = std::min((width_ - column_ * 2) / (CX + 1), height_ / (CY + 1));
    border_ = cell_ / 3;
    margin_[0] = (height_ - cell_ * CY - border_ * 2) / 2;
    margin_[1] = (width_ - cell_ * CX - border_ * 2) / 2;
    pattern_target_ = steriogram::CreateRandomPattern<4>(column_, order_rgba_);
    pattern_current_.resize(pattern_target_.size());
    pattern_base_.resize(pattern_target_.size());
}

void main::Game::Step(__uint32_t* pixels)
{
    if (!pause_)
    {
        if (++data_.frame_ >= MAX_FRAME - data_.level_)
            Play();
    }
    if (++pattern_frame_ == FR)
    {
        std::swap(pattern_base_, pattern_target_);
        pattern_target_ = steriogram::CreateRandomPattern<4>(column_, order_rgba_);
        pattern_frame_ = 0;
    }
    for (int i = 0; i < pattern_current_.size(); ++i)
        pattern_current_[i] = ((int) pattern_target_[i] * pattern_frame_ + (int) pattern_base_[i] * (FR - pattern_frame_)) / FR;
    ApplyBoard(pixels);
    steriogram::Convert<4, 16>((unsigned char*)pixels, column_, width_, height_, pattern_current_.data());
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
                pause_ = false;
        }
        else
            pause_ = true;
    }
    else if (!pause_)
    {
        if (std::abs(touch_[0]) > std::abs(touch_[1]))
        {
            if (touch_[0] < 0)
            {
                if (data_.side_ != 3)
                {
                    data_.side_ = 2;
                    Play();
                }
            }
            else
            {
                if (data_.side_ != 2)
                {
                    data_.side_ = 3;
                    Play();
                }
            }
        }
        else
        {
            if (touch_[1] < 0)
            {
                if (data_.side_ != 1)
                {
                    data_.side_ = 0;
                    Play();
                }
        }
            else
            {
                if (data_.side_ != 0)
                {
                    data_.side_ = 1;
                    Play();
                }
            }
        }
    }
}

bool main::Game::Move()
{
    auto head = data_.parts_.front();
    switch (data_.side_)
    {
    case 0:
        if (head[1] > 0)
            --head[1];
        else
            return false;
        break;
    case 1:
        if (head[1] < CX - 1)
            ++head[1];
        else
            return false;
        break;
    case 2:
        if (head[0] > 0)
            --head[0];
        else
            return false;
        break;
    case 3:
        if (head[0] < CY - 1)
            ++head[0];
        else
            return false;
        break;
    }
    if (head == data_.food_)
    {
        data_.score_ += data_.parts_.size();
        if (data_.score_ > data_.level_ * 1000)
        {
            if (data_.level_ < 10)
                ++data_.level_;
        }
        data_.parts_.push_front(head);
        data_.ResetFood();
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
    }
    return true;
}

void main::Game::Play()
{
    if (!Move())
    {
        pause_ = true;
        if (--data_.lives_ > 0)
        {
            data_.ResetSnake();
            data_.ResetFood();
        }
    }
    data_.frame_ = 0;
}

void main::Game::ApplyBoard(__uint32_t *pixels)
{
    memset(pixels, color_empty_, width_ * height_ * 4);
    DrawBorder(pixels, color_border_);
    DrawSquare(pixels, data_.food_, color_food_);
    DrawSquare(pixels, data_.parts_.front(), color_head_);
    for (auto it = std::next(data_.parts_.begin()); it != data_.parts_.end(); ++it)
        DrawSquare(pixels, *it, color_tail_);
}

void main::Game::DrawBorder(__uint32_t* pixels, const __uint32_t color)
{
    for (int i = margin_[0]; i < margin_[0] + border_; ++i)
        for (int j = margin_[1]; j < margin_[1] + cell_ * CX + border_ * 2; ++j)
            pixels[i * width_ + j] = color;
    for (int i = margin_[0] + border_; i < margin_[0] + border_ +  cell_ * CY; ++i)
    {
        for (int j = margin_[1]; j < margin_[1] + border_; ++j)
            pixels[i * width_ + j] = color;
        for (int j = margin_[1] + cell_ * CX + border_; j < margin_[1] + cell_ * CX + border_ * 2; ++j)
            pixels[i * width_ + j] = color;
    }
    for (int i = margin_[0] + cell_ * CY + border_; i < margin_[0] + cell_ * CY + 2 * border_; ++i)
        for (int j = margin_[1]; j < margin_[1] + cell_ * CX + border_ * 2; ++j)
            pixels[i * width_ + j] = color;    
}

void main::Game::DrawSquare(__uint32_t* pixels, const std::array<int, 2> & target, const __uint32_t color)
{
    for (int i = margin_[0] + border_ + target[0] * cell_; i < margin_[0] + border_ + cell_ + target[0] * cell_; ++i)
        for (int j = margin_[1] + border_ + target[1] * cell_; j < margin_[1] + border_ + cell_ + target[1] * cell_; ++j)
            pixels[i * width_ + j] = color;
}
