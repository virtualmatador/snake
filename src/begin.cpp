//
//  begin.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#include "begin.h"
#include "steriogram/steriogram.hpp"


main::Begin::Begin()
    : core::Runner(
        (__int32_t)core::VIEW_INFO::Landscape |
        (__int32_t)core::VIEW_INFO::ScreenOn, 640,
        std::operator""ms((unsigned long long)33))
    , column_{0}
{
    std::istringstream parser;
    parser.str(bridge::GetPreference("Begin"));
    parser >> data_x_ >> data_y_;
    if (data_x_ < 0 || data_x_ >= 200)
        data_x_ = 100;
    if (data_y_ < 0 || data_y_ >= 100)
        data_y_ = 50;
}

main::Begin::~Begin()
{
    std::ostringstream composer;
    composer << data_x_ << ' ' << data_y_ << ' ';
    bridge::SetPreference("Begin", composer.str().c_str());
}

void main::Begin::Initial()
{
    column_ = dpi_ / 2.5;
    pattern_ = steriogram::CreatePattern<4>(column_);
}

void main::Begin::Step(__int32_t* pixels)
{
    if (++data_x_ >= 200)
        data_x_ = 0;
    if (++data_y_ >= 100)
        data_y_ = 0;
    memset(pixels, 0, width_ * height_ * 4);
    for (int i = height_ / 4; i < height_ * 3 / 4; ++i)
    {
        for (int j = width_ / 3; j < width_ * 2 / 3; ++j)
        {
            pixels[(i + abs(data_y_ - 50) - 25) * width_ + j + abs(data_x_ - 100) - 50] = 255 << 24 | (96)  << 16 | (96) << 8 | (96);
        }
    }
    steriogram::Convert<4, 16>((unsigned char*)pixels, column_, width_, height_, pattern_.data());
}

void main::Begin::TouchBegin(float x, float y)
{
}

void main::Begin::TouchMove(float x, float y)
{
}

void main::Begin::TouchEnd(float x, float y)
{
    progress_ = PROGRESS::ERROR;
    bridge::OnRestart();
}
