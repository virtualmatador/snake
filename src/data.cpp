//
//  data.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <cstdlib>
#include <istream>
#include <ostream>
#include <utility>

#include "data.h"

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

void main::Data::Load(std::istream& input)
{
    Reset();
    input >> std::dec >> std::noboolalpha >> std::skipws;
    int version = 0;
    if (!(input >> version))
        return;
    if (version != save_version_)
    {
        incompatible_save_ = true;
        return;
    }

    int lives, eat, score, side, snake_size;
    bool sound;
    std::array<int, 2> food;
    if (!(input >> lives >> eat >> score >> side >> sound
                >> food[0] >> food[1] >> snake_size) ||
        lives < 0 || lives > lives_max_ || eat < 0 || eat > eat_max_ ||
        score < 0 || score > score_max_ || side == 0 || side < -2 || side > 2 ||
        food[0] < 0 || food[0] >= c_y_ || food[1] < 0 || food[1] >= c_x_ ||
        snake_size < 3 || snake_size >= c_x_ * c_y_)
        return;

    decltype(parts_) parts;
    std::array<std::array<bool, c_x_>, c_y_> occupied{};
    occupied[food[0]][food[1]] = true;
    for (int i = 0; i < snake_size; ++i)
    {
        std::array<int, 2> part;
        if (!(input >> part[0] >> part[1]) ||
            part[0] < 0 || part[0] >= c_y_ || part[1] < 0 || part[1] >= c_x_ ||
            occupied[part[0]][part[1]])
            return;
        if (!parts.empty() &&
            std::abs(parts.back()[0] - part[0]) +
            std::abs(parts.back()[1] - part[1]) != 1)
            return;
        occupied[part[0]][part[1]] = true;
        parts.push_back(part);
    }

    const auto& head = parts.front();
    const auto& neck = *std::next(parts.begin());
    if ((side == -1 && neck[0] < head[0]) ||
        (side == 1 && neck[0] > head[0]) ||
        (side == -2 && neck[1] < head[1]) ||
        (side == 2 && neck[1] > head[1]))
        return;

    input >> std::ws;
    if (!input.eof() || input.bad())
        return;

    lives_ = lives;
    eat_ = eat;
    score_ = score;
    side_ = side;
    sound_ = sound;
    food_ = food;
    parts_ = std::move(parts);
}

void main::Data::Save(std::ostream& output) const
{
    if (incompatible_save_)
    {
        output.setstate(std::ios::failbit);
        return;
    }

    output << std::dec << std::noboolalpha << std::noshowbase << std::noshowpos;
    output.width(0);
    output << save_version_ << '\n'
           << lives_ << ' ' << eat_ << ' ' << score_ << ' ' << side_ << ' ' << sound_ << '\n'
           << food_[0] << ' ' << food_[1] << '\n'
           << parts_.size() << '\n';
    for (const auto& part : parts_)
        output << part[0] << ' ' << part[1] << '\n';
}

void main::Data::Reset()
{
    incompatible_save_ = false;
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
    {
        for (int i = 1; i < c_y_ * c_x_ / 2; ++i)
            parts_.pop_front();
        const auto last_removed = parts_.front();
        parts_.pop_front();
        const auto & head = parts_.front();
        if (last_removed[0] < head[0])
            side_ = -1;
        else if (last_removed[0] > head[0])
            side_ = 1;
        else if (last_removed[1] < head[1])
            side_ = -2;
        else
            side_ = 2;
    }
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
