//
//  begin.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <time.h>

#include "steriogram/steriogram.hpp"

#include "toolbox.hpp"

#include "begin.h"


main::Begin::Begin()
    : core::Runner(
        (__int32_t)core::VIEW_INFO::Landscape |
        (__int32_t)core::VIEW_INFO::ScreenOn |
        (__int32_t)core::VIEW_INFO::CloseMenu, 480)
    , pause_{true}
    , pattern_frame_{FR - 1}
    , game_frame_{0}
    , column_{0}
    , cell_{0}
    , margin_{0, 0}
    , touch_{0, 0}
{
    std::srand(time(NULL));
    frame_lenght_ = std::operator""ms((unsigned long long)50);
    std::istringstream parser;
    parser.str(bridge::GetPreference("Begin"));
    try
    {
        toolbox::Load(parser, level_, 0, 5);
        toolbox::Load(parser, lives_, 0, 5);
        toolbox::Load(parser, food_[0], 0, 10);
        toolbox::Load(parser, food_[1], 0, 20);
        toolbox::Load(parser, side_, 0, 4);
        int snake_size;
        toolbox::Load(parser, snake_size, 3, CX * CY);
        for (int i = 0; i < snake_size; ++i)
        {
            decltype(parts_)::value_type part;
            toolbox::Load(parser, part[0], 0, 10);
            toolbox::Load(parser, part[1], 0, 20);
            parts_.push_back(part);
        }
        for (auto left_it = parts_.begin(); left_it != parts_.end(); ++left_it)
        {
            for (auto right_it = std::next(left_it); right_it != parts_.end(); ++right_it)
            {
                if (*left_it == *right_it)
                    throw "";
            }
        }
    }
    catch(...)
    {
        level_ = 0;
        lives_ = 4;
        ResetSnake();
        ResetFood();
    }
}

main::Begin::~Begin()
{
    std::ostringstream composer;
    try
    {
        toolbox::Save(composer, level_);
        toolbox::Save(composer, lives_);
        toolbox::Save(composer, food_[0]);
        toolbox::Save(composer, food_[1]);
        toolbox::Save(composer, side_);
        toolbox::Save(composer, (int)parts_.size());
        for (const auto part : parts_)
        {
            toolbox::Save(composer, part[0]);
            toolbox::Save(composer, part[1]);
        }
    }
    catch(...)
    {
    }
    bridge::SetPreference("Begin", composer.str().c_str());
}

void main::Begin::Escape()
{
    progress_ = PROGRESS::ERROR;
    bridge::NeedRestart();
}

void main::Begin::Initial()
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

void main::Begin::Step(__uint32_t* pixels)
{
    if (!pause_)
    {
        if (++game_frame_ == 20 - level_)
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

void main::Begin::ResetSnake()
{
    side_ = 1;
    parts_.clear();
    parts_.push_back({CY / 2, 3});
    parts_.push_back({CY / 2, 2});
    parts_.push_back({CY / 2, 1});
}

void main::Begin::ResetFood()
{
    int food = std::rand() % (CY * CX - parts_.size());
    for (food_[0] = 0; food_[0] < CY; ++food_[0])
    {
        for (food_[1] = 0; food_[1] < CX; ++food_[1])
        {
            if ([&]()
            {
                for (const auto & part : parts_)
                    if (part == food_)
                        return false;
                return true;
            }())
            {
                if (--food == -1)
                    return;
            }
        }
    }
}

void main::Begin::TouchBegin(const float x, const float y)
{
    touch_[0] = y;
    touch_[1] = x;
}

void main::Begin::TouchMove(const float x, const float y)
{
}

void main::Begin::TouchEnd(const float x, const float y)
{
    touch_[0] = y - touch_[0];
    touch_[1] = x - touch_[1];
    if (std::pow(touch_[0] * touch_[0] + touch_[1] * touch_[1], 0.5) < 10)
        pause_ = !pause_;
    else if (!pause_)
    {
        if (std::abs(touch_[0]) > std::abs(touch_[1]))
        {
            if (touch_[0] < 0)
            {
                if (side_ != 3)
                {
                    side_ = 2;
                    Play();
                }
            }
            else
            {
                if (side_ != 2)
                {
                    side_ = 3;
                    Play();
                }
            }
        }
        else
        {
            if (touch_[1] < 0)
            {
                if (side_ != 1)
                {
                    side_ = 0;
                    Play();
                }
        }
            else
            {
                if (side_ != 0)
                {
                    side_ = 1;
                    Play();
                }
            }
        }
    }
}

bool main::Begin::Move()
{
    auto head = parts_.front();
    switch (side_)
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
    if (head == food_)
    {
        parts_.push_front(head);
        ResetFood();
    }
    else
    {
        parts_.pop_back();
        for (const auto & part : parts_)
        {
            if (head == part)
                return false;
        }
        parts_.push_front(head);
    }
    return true;
}

void main::Begin::Play()
{
    if (!Move())
    {
        pause_ = true;
        if (--lives_ > 0)
        {
            ResetSnake();
            ResetFood();
        }
        else
        {
            // GameOver();
        }
    }
    game_frame_ = 0;
}

void main::Begin::ApplyBoard(__uint32_t *pixels)
{
    memset(pixels, color_empty_, width_ * height_ * 4);
    DrawBorder(pixels, color_border_);
    DrawSquare(pixels, food_, color_food_);
    DrawSquare(pixels, parts_.front(), color_head_);
    for (auto it = std::next(parts_.begin()); it != parts_.end(); ++it)
        DrawSquare(pixels, *it, color_tail_);
}

void main::Begin::DrawBorder(__uint32_t* pixels, const __uint32_t color)
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

void main::Begin::DrawSquare(__uint32_t* pixels, const std::array<int, 2> & target, const __uint32_t color)
{
    for (int i = margin_[0] + border_ + target[0] * cell_; i < margin_[0] + border_ + cell_ + target[0] * cell_; ++i)
        for (int j = margin_[1] + border_ + target[1] * cell_; j < margin_[1] + border_ + cell_ + target[1] * cell_; ++j)
            pixels[i * width_ + j] = color;
}
