//
//  error.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/12/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <sstream>
#include <cstring>

#include "../cross/core/src/helper.h"

#include "menu.h"
#include "data.h"


main::Menu::Menu()
{
    handlers_["body"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "ready") == 0)
        {
            std::ostringstream js;
            js.str("");
            js.clear();
            js << "setLevel(" << data_.eat_ / 100 << ")";
            bridge::CallFunction(js.str().c_str());
            js.str("");
            js.clear();
            js << "setLives(" << data_.lives_ << ")";
            bridge::CallFunction(js.str().c_str());
            js.str("");
            js.clear();
            js << "setScore(" << data_.score_ << ")";
            bridge::CallFunction(js.str().c_str());
            js.str("");
            js.clear();
            js << "setSound(" << data_.sound_ << ")";
            bridge::CallFunction(js.str().c_str());
        }
    };
    handlers_["play"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
            Play();
    };
    handlers_["reset"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
            Reset();
    };
    handlers_["sound"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            if (std::strlen(info) == 0)
                return;
            else if (std::strcmp(info, "true") == 0)
                data_.sound_ = true;
            else if (std::strcmp(info, "false") == 0)
                data_.sound_ = false;
        }
    };
    bridge::LoadWebView(index_, (std::int32_t)core::VIEW_INFO::Default, "menu", "");
}

main::Menu::~Menu()
{
}

void main::Menu::Escape()
{
    bridge::Exit();
}

void main::Menu::Play()
{
    progress_ = PROGRESS::GAME;
    bridge::NeedRestart();
}

void main::Menu::Reset()
{
    data_.Reset();
    bridge::NeedRestart();
}
