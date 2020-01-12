//
//  error.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/12/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#include <sstream>
#include <cstring>

#include "../cross/core/src/helper.h"

#include "menu.h"


main::Menu::Menu()
{
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
    bridge::LoadWebView(index_, (__int32_t)core::VIEW_INFO::Default, "menu");
}

main::Menu::~Menu()
{
}

void main::Menu::Escape()
{
    bridge::Exit();
}

void main::Menu::LoadInput(const char* id)
{
    std::ostringstream js;
    js << "document.getElementById('" << id << "').value = '" << bridge::GetPreference(id) << "'";
    bridge::CallFunction(js.str().c_str());
}

void main::Menu::SaveInput(const char* id, const char* value)
{
    bridge::SetPreference(id, value);
}

void main::Menu::Play()
{
    progress_ = PROGRESS::GAME;
    bridge::NeedRestart();
}

void main::Menu::Reset()
{
}
