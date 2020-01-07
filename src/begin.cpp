//
//  begin.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#include <iostream>
#include <sstream>

#include "steriogram/steriogram.hpp"

#include "toolbox.hpp"

#include "begin.h"


main::Begin::Begin()
    : core::Runner(
        (__int32_t)core::VIEW_INFO::Landscape |
        (__int32_t)core::VIEW_INFO::ScreenOn |
        (__int32_t)core::VIEW_INFO::CloseMenu, 480)
    , column_{0}
    , cell_size_{0}
    , margin_{0, 0}
    , touch_{0, 0}
    , frame_{0}
{
    std::istringstream parser;
    parser.str(bridge::GetPreference("Begin"));
    try
    {
        toolbox::Load(parser, level_, 0, 5);
        toolbox::Load(parser, lives_, 0, 5);
        toolbox::Load(parser, food_[0], 0, 20);
        toolbox::Load(parser, food_[1], 0, 10);
        toolbox::Load(parser, head_[0], 0, 20);
        toolbox::Load(parser, head_[1], 0, 10);
        toolbox::Load(parser, side_, (unsigned char)0, (unsigned char)4);
        unsigned char snake_size;
        toolbox::Load(parser, snake_size, (unsigned char)3, (unsigned char)200);
        int y = head_[0];
        int x = head_[1];
        for (unsigned char i = 0; i < snake_size; ++i)
        {
            unsigned char part;
            toolbox::Load(parser, part, (unsigned char)0, (unsigned char)4);
            MoveCell(part, y, x);
            if (x < 0 || x >= CX || y < 0 || y >= CY)
                throw "";
            snake_.push_back(part);
        }
    }
    catch(...)
    {
        level_ = 0;
        lives_ = 5;
        food_[0] = 0;
        food_[1] = 3;
        head_[0] = 3;
        head_[1] = 0;
        snake_.push_back(3);
        snake_.push_back(3);
        side_ = 3;
    }
}

main::Begin::~Begin()
{
    std::ostringstream composer;
    try
    {
        toolbox::Save(composer, level_);
        toolbox::Save(composer, ' ');
        toolbox::Save(composer, lives_);
        toolbox::Save(composer, ' ');
        toolbox::Save(composer, food_[0]);
        toolbox::Save(composer, ' ');
        toolbox::Save(composer, food_[1]);
        toolbox::Save(composer, ' ');
        toolbox::Save(composer, head_[0]);
        toolbox::Save(composer, ' ');
        toolbox::Save(composer, head_[1]);
        toolbox::Save(composer, ' ');
        toolbox::Save(composer, side_);
        toolbox::Save(composer, ' ');
        toolbox::Save(composer, (unsigned char)snake_.size());
        toolbox::Save(composer, ' ');
        for (const auto part : snake_)
            toolbox::Save(composer, part);
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

void main::Begin::Initial(__uint32_t* pixels)
{
    memset(pixels, 0, width_ * height_ * 4);
    frame_ = -1;
    frame_lenght_ = std::operator""ms((unsigned long long)50);
    column_ = dpi_ / 2;
    cell_size_ = std::min((width_ - column_ * 2) / CX, height_ / CY);
    margin_[0] = (height_ - cell_size_ * CY) / 2;
    margin_[1] = (width_ - cell_size_ * CX) / 2;
    pattern_target_ = steriogram::CreateRandomPattern<4>(column_, order_rgba_);
    pattern_current_.resize(pattern_target_.size());
    pattern_base_.resize(pattern_target_.size());
}

void main::Begin::Step(__uint32_t* pixels)
{
    if (++frame_ ==  FR * (20 - level_))
        frame_ = 0;
    if (frame_ % (20 - level_) == 0)
        Play();
    if (frame_ % FR == 0)
    {
        std::swap(pattern_base_, pattern_target_);
        pattern_target_ = steriogram::CreateRandomPattern<4>(column_, order_rgba_);
    }
    for (int i = 0; i < pattern_current_.size(); ++i)
        pattern_current_[i] = ((int) pattern_target_[i] * (frame_ % FR) + (int) pattern_base_[i] * (FR - (frame_ % FR))) / FR;
    ApplyBoard(pixels);
    steriogram::Convert<4, 32>((unsigned char*)pixels, column_, width_, height_, pattern_current_.data());
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
    if (std::abs(touch_[0]) > std::abs(touch_[1]))
    {
        if (touch_[0] < 0)
        {
            if (side_ != 3)
                side_ = 2;
        }
        else
        {
            if (side_ != 2)
                side_ = 3;
        }
    }
    else
    {
        if (touch_[1] < 0)
        {
            if (side_ != 1)
            side_ = 0;
        }
        else
        {
            if (side_ != 0)
                side_ = 1;
        }
    }
}

void main::Begin::MoveCell(const int part, int & y, int & x)
{
    switch (part)
    {
        case 0:
            ++x;
            break;
        case 1:
            --x;
            break;
        case 2:
            ++y;
            break;
        case 3:
            --y;
            break;
    }
}

bool main::Begin::MoveHead(const int part, int & y, int & x)
{
    switch (part)
    {
    case 0:
        if (x > 0)
            --x;
        else return false;
        break;
    case 1:
        if (x < CX - 1)
            ++x;
        else
            return false;
        break;
    case 2:
        if (y > 0)
            --y;
        else
            return false;
        break;
    case 3:
        if (y < CY - 1)
            ++y;
        else
            return false;
        break;
    }
    if (food_[0] == y && food_[1] == x)
        snake_.push_back(-1);
    return true;
}

void main::Begin::Play()
{
    if (!MoveHead(side_, head_[0], head_[1]))
    {
        if (--lives_ > 0)
        {
            // Reset scene
            head_[0] = 3;
            head_[1] = 0;
        }
        else
        {
            // GameOver();
        }
    }
    snake_.push_front(side_);
    snake_.pop_back();
    return DrawBoard();
}

void main::Begin::DrawBoard()
{
    for (int i = 0; i < CY; ++i)
        for (int j = 0; j < CX; ++j)
            board_[i][j] = CellType::Empty;
    DrawFood();
    DrawSnake();
}

void main::Begin::DrawSnake()
{
    int y = head_[0];
    int x = head_[1];
    board_[y][x] = CellType::Head;
    for (const auto part: snake_)
    {
        MoveCell(part, y, x);
        board_[y][x] = CellType::Tail;
    }
}

void main::Begin::DrawFood()
{
    board_[food_[0]][food_[1]] = CellType::Food;
}

void main::Begin::ApplyBoard(__uint32_t *pixels)
{
    for (int i = 0; i < margin_[0]; ++i)
        for (int j = 0; j < width_; ++j)
            pixels[i * width_ + j] = 0;
    for (int i = margin_[0]; i < height_- margin_[0]; ++i)
    {
        for (int j = 0; j < margin_[1]; ++j)
            pixels[i * width_ + j] = 0;
        for (int j = width_ - margin_[1]; j < width_; ++j)
            pixels[i * width_ + j] = 0;
    }
    for (int i = height_ - margin_[0]; i < height_; ++i)
        for (int j = 0; j < width_; ++j)
            pixels[i * width_ + j] = 0;
    for (int i = 0; i < CY; ++i)
    {
        for (int j = 0; j < CX; ++j)
        {
            switch (board_[i][j])
            {
            case CellType::Empty:
                DrawSquare(pixels, i, j, 0 << 24 | 0 << 16  | 0 << 8 | 0 << 0);
                break;
            case CellType::Food:
                DrawSquare(pixels, i, j, 96 << 24 | 96 << 16  | 96 << 8 | 96 << 0);
                break;
            case CellType::Head:
                DrawSquare(pixels, i, j, 64 << 24 | 64 << 16  | 64 << 8  | 64 << 0);
                break;
            case CellType::Tail:
                DrawSquare(pixels, i, j, 48 << 24 | 48 << 16  | 48 << 8  | 48 << 0);
                break;
            }
        }
    }
}

void main::Begin::DrawSquare(__uint32_t* pixels, const int y, const int x, const __uint32_t color)
{
    for (int i = margin_[0] + y * cell_size_; i < margin_[0] + cell_size_ + y * cell_size_; ++i)
        for (int j = margin_[1] + x * cell_size_; j < margin_[1] + cell_size_ + x * cell_size_; ++j)
            pixels[i * width_ + j] = color;
}
