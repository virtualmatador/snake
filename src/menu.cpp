//
//  error.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/12/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <sstream>
#include <cstring>

#include "menu.h"
#include "data.h"
#include "progress.h"

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
            if (data_.incompatible_save_)
                bridge::CallFunction("showSaveVersionError()");
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
            if (data_.incompatible_save_ || std::strlen(info) == 0)
                return;
            else if (std::strcmp(info, "true") == 0)
                data_.sound_ = true;
            else if (std::strcmp(info, "false") == 0)
                data_.sound_ = false;
        }
    };
}

void main::Menu::Attach()
{
    bridge::SetAudioNoSolo(true);
    bridge::SetLayout(false, false);
    bridge::LoadView(Index(), "menu");
}

main::Menu::~Menu()
{
}

void main::Menu::FeedUri(const char* uri, std::function<void(
    const std::vector<unsigned char>&)>&& consume)
{
}

void main::Menu::Escape()
{
    bridge::Exit();
}

void main::Menu::Play()
{
    if (data_.incompatible_save_)
        return;
    progress_ = PROGRESS::GAME;
    RequestStage();
}

void main::Menu::Reset()
{
    data_.Reset();
    RequestStage();
}
