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

main::Data main::data_;

const int main::Data::lives_max_;
const int main::Data::frame_max_;
const int main::Data::eat_max_;
const int main::Data::eat_level_;
const int main::Data::score_max_;
const int main::Data::c_y_;
const int main::Data::c_x_;

main::Data::Data()
    : random_{seeder_()}
{
}

main::Data::~Data()
{
}

void main::Data::Load()
{
    try
    {
        toolbox::Load("GAME_LIVES", lives_, 0, lives_max_ + 1);
        toolbox::Load("GAME_EAT", eat_, 0, eat_max_ + 1);
        toolbox::Load("GAME_SCORE", score_, 0, score_max_ + 1);
        toolbox::Load("GAME_SIDE", side_, -2, 3);
        toolbox::Load("GAME_SOUND", sound_, false, false);
        toolbox::Load("GAME_FOOD_Y", food_[0], 0, c_y_);
        toolbox::Load("GAME_FOOD_X", food_[1], 0, c_x_);
        int snake_size;
        toolbox::Load("GAME_SNAKE_SIZE", snake_size, 3, c_x_ * c_y_);
        parts_.clear();
        for (int i = 0; i < snake_size; ++i)
        {
            decltype(parts_)::value_type part;
            {
                std::ostringstream composer;
                composer << "GAME_SNAKE_Y_" << i;
                toolbox::Load(composer.str().c_str(), part[0], 0, c_y_);
            }
            {
                std::ostringstream composer;
                composer << "GAME_SNAKE_X_" << i;
                toolbox::Load(composer.str().c_str(), part[1], 0, c_x_);
            }
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
        Reset();
    }
}

void main::Data::Save()
{
    toolbox::Save("GAME_LIVES", lives_);
    toolbox::Save("GAME_EAT", eat_);
    toolbox::Save("GAME_SCORE", score_);
    toolbox::Save("GAME_SIDE", side_);
    toolbox::Save("GAME_SOUND", sound_);
    toolbox::Save("GAME_FOOD_Y", food_[0]);
    toolbox::Save("GAME_FOOD_X", food_[1]);
    toolbox::Save("GAME_SNAKE_SIZE", (int)parts_.size());
    int i = 0;
    for (const auto part : parts_)
    {
        {
            std::ostringstream composer;
            composer << "GAME_SNAKE_Y_" << i;
            toolbox::Save(composer.str().c_str(), part[0]);
        }
        {
            std::ostringstream composer;
            composer << "GAME_SNAKE_X_" << i;
            toolbox::Save(composer.str().c_str(), part[1]);
        }
         ++i;
    }
}

void main::Data::Reset()
{
    lives_ = lives_max_;
    eat_ = 0;
    score_ = 0;
    sound_ = true;
    ResetSnakeFood();
}

void main::Data::ResetSnakeFood()
{
    side_ = 2;
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
    int dist = std::uniform_int_distribution<int>(0, c_y_ * c_x_ - (int)parts_.size() - 1)(random_);
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
