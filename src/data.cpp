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

const int main::Data::lives_max_;
const int main::Data::frame_max_;
const int main::Data::eat_max_;
const int main::Data::eat_level_;
const int main::Data::score_max_;
const int main::Data::c_y_;
const int main::Data::c_x_;

void main::Data::Load()
{
    std::istringstream parser;
    parser.str(bridge::GetPreference("GAME"));
    try
    {
        toolbox::Load(parser, lives_, 0, lives_max_);
        toolbox::Load(parser, eat_, 0, eat_max_);
        toolbox::Load(parser, score_, 0, score_max_);
        toolbox::Load(parser, frame_, 0, GetDelay() - 1);
        toolbox::Load(parser, side_, 0, 3);
        toolbox::Load(parser, sound_, false, true);
        toolbox::Load(parser, food_[0], 0, c_y_ - 1);
        toolbox::Load(parser, food_[1], 0, c_x_ - 1);
        int snake_size;
        toolbox::Load(parser, snake_size, 3, c_x_ * c_y_ - 1);
        parts_.clear();
        for (int i = 0; i < snake_size; ++i)
        {
            decltype(parts_)::value_type part;
            toolbox::Load(parser, part[0], 0, c_y_ - 1);
            toolbox::Load(parser, part[1], 0, c_x_ - 1);
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
        toolbox::Save(composer, eat_);
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
    lives_ = lives_max_;
    eat_ = 0;
    score_ = 0;
    frame_ = 0;
    sound_ = true;
    ResetSnakeFood();
}

void main::Data::ResetSnakeFood()
{
    side_ = 1;
    parts_.clear();
    parts_.push_back({c_y_ / 2, 3});
    parts_.push_back({c_y_ / 2, 2});
    parts_.push_back({c_y_ / 2, 1});
    ResetFood();
}

void main::Data::ResetFood()
{
    if (parts_.size() == c_y_ * c_x_)
        for (int i = 0; i < c_y_ * c_x_ / 2; ++i)
            parts_.pop_front();
    int dist = std::rand() % (c_y_ * c_x_ - parts_.size());
    for (food_[0] = 0; food_[0] < c_y_; ++food_[0])
    {
        for (food_[1] = 0; food_[1] < c_x_; ++food_[1])
        {
            if ([&]()
            {
                for (const auto & part : parts_)
                    if (part == food_)
                        return false;
                return true;
            }())
            {
                if (--dist == -1)
                    return;
            }
        }
    }
}

int main::Data::GetDelay()
{
    return frame_max_ - (eat_ / eat_level_);
}
