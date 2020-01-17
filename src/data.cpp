//
//  data.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <iostream>
#include <sstream>

#include "../cross/core/src/bridge.h"

#include "data.h"

#include "toolbox.hpp"
void main::Data::Load()
{
    std::istringstream parser;
    parser.str(bridge::GetPreference("GAME"));
    try
    {
        toolbox::Load(parser, lives_, 0, 5);
        toolbox::Load(parser, level_, 1, 10);
        toolbox::Load(parser, score_, 0, 10000);
        toolbox::Load(parser, frame_, 0, MAX_FRAME - level_ - 1);
        toolbox::Load(parser, side_, 0, 3);
        toolbox::Load(parser, sound_, false, true);
        toolbox::Load(parser, food_[0], 0, 9);
        toolbox::Load(parser, food_[1], 0, 19);
        int snake_size;
        toolbox::Load(parser, snake_size, 3, CX * CY - 1);
        parts_.clear();
        for (int i = 0; i < snake_size; ++i)
        {
            decltype(parts_)::value_type part;
            toolbox::Load(parser, part[0], 0, 9);
            toolbox::Load(parser, part[1], 0, 19);
            parts_.push_back(part);
        }
        for (auto left_it = parts_.begin(); left_it != parts_.end(); ++left_it)
        {
            for (auto right_it = std::next(left_it); right_it != parts_.end(); ++right_it)
            {
                if (*left_it == *right_it)
                {
                    throw "";
                }
            }
        }
    }
    catch(...)
    {
        Reset();
    }
}

void main::Data::Save()
{
    std::ostringstream composer;
    try
    {
        toolbox::Save(composer, lives_);
        toolbox::Save(composer, level_);
        toolbox::Save(composer, score_);
        toolbox::Save(composer, frame_);
        toolbox::Save(composer, side_);
        toolbox::Save(composer, sound_);
        toolbox::Save(composer, food_[0]);
        toolbox::Save(composer, food_[1]);
        toolbox::Save(composer, (int)parts_.size());
        for (const auto part : parts_)
        {
            toolbox::Save(composer, part[0]);
            toolbox::Save(composer, part[1]);
        }
    }
    catch(...)
    {
        composer.clear();
    }
    bridge::SetPreference("GAME", composer.str().c_str());
}

void main::Data::Reset()
{
    lives_ = 5;
    level_ = 1;
    score_ = 0;
    frame_ = 0;
    sound_ = true;
    ResetSnake();
    ResetFood();
}

void main::Data::ResetSnake()
{
    side_ = 1;
    parts_.clear();
    parts_.push_back({CY / 2, 3});
    parts_.push_back({CY / 2, 2});
    parts_.push_back({CY / 2, 1});
}

void main::Data::ResetFood()
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

