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
        (__int32_t)core::VIEW_INFO::ScreenOn,
        std::operator""ms((unsigned long long)33))
{
    std::istringstream parser;
    parser.str(bridge::GetPreference("Begin"));
    parser >> data_;
    if (data_ < 0 || data_ > 100)
        data_ = 50;
}

main::Begin::~Begin()
{
    std::ostringstream composer;
    composer << data_;
    bridge::SetPreference("Begin", composer.str().c_str());
}

void main::Begin::Step(__int32_t* pixels)
{
    if (++data_ == 100)
        data_ = 0;
    memset(pixels, 0, width_ * height_ * 4);
    for (int i = height_ / 4; i < height_ * 3 / 4; ++i)
    {
        for (int j = width_ / 3; j < width_ * 2 / 3; ++j)
        {
            pixels[i * width_ + j + abs(data_ - 50) - 25] = 255 << 24 | (64)  << 16 | (64) << 8 | (64);
        }
    }
    Steriogram<4>((unsigned char*)pixels, width_, height_);
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
