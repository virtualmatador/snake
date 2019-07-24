//
//  begin.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#include "begin.h"
#include "steriogram/steriogram.hpp"


template<int PIXEL_SIZE>
void WritePattern(int column, unsigned char* dest, void* user_data)
{
    double & s = *(double*) user_data;
    for (int y = 0; y < column; ++y)
    {
        for (int x = 0; x < column; ++x)
        {
            if (PIXEL_SIZE > 0)
                dest[(y * column + x) * PIXEL_SIZE + 0] = int(pow(y + column / 3.6 + 10 * s, 1.45 + s) * pow(x + column / 3.4 + 10 * s, 1.55 - s)) % 256;
            if (PIXEL_SIZE > 1)
                dest[(y * column + x) * PIXEL_SIZE + 1] = int(pow(y + column / 4.8 + 10 * s, 1.15 - s) * pow(x + column / 4.6 + 10 * s, 1.25 + s)) % 256;
            if (PIXEL_SIZE > 2)
                dest[(y * column + x) * PIXEL_SIZE + 2] = int(pow(y + column / 2.2 + 10 * s, 1.85 + s) * pow(x + column / 2.4 + 10 * s, 1.75 - s)) % 256;
            if (PIXEL_SIZE > 3)
                dest[(y * column + x) * PIXEL_SIZE + 3] = 255;
        }
    }
}

main::Begin::Begin()
    : core::Runner(
        (__int32_t)core::VIEW_INFO::Landscape |
        (__int32_t)core::VIEW_INFO::ScreenOn,
        std::operator""ms((unsigned long long)33))
{
    std::istringstream parser;
    parser.str(bridge::GetPreference("Begin"));
    parser >> data_x_ >> data_y_ >> salt_;
    if (data_x_ < 0 || data_x_ >= 200)
        data_x_ = 100;
    if (data_y_ < 0 || data_y_ >= 100)
        data_y_ = 50;
    if (salt_ < 0.0 || salt_ >= 0.5)
        salt_ = 0.25;
}

main::Begin::~Begin()
{
    std::ostringstream composer;
    composer << data_x_ << ' ' << data_y_ << ' ' << salt_ ' ';
    bridge::SetPreference("Begin", composer.str().c_str());
}

void main::Begin::Step(__int32_t* pixels)
{
    if (++data_x_ >= 200)
        data_x_ = 0;
    if (++data_y_ >= 100)
        data_y_ = 0;
    if ((salt_ += 0.0002) >= 0.5)
        salt_ = 0.0;
    double salt = abs(salt_ - 0.25) - 0.125;
    memset(pixels, 0, width_ * height_ * 4);
    for (int i = height_ / 4; i < height_ * 3 / 4; ++i)
    {
        for (int j = width_ / 3; j < width_ * 2 / 3; ++j)
        {
            pixels[(i + abs(data_y_ - 50) - 25) * width_ + j + abs(data_x_ - 100) - 50] = 255 << 24 | (96)  << 16 | (96) << 8 | (96);
        }
    }
    steriogram::Convert<4, 16>((unsigned char*)pixels, width_, height_, WritePattern<4>, &salt);
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
