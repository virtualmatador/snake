//
//  error.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/12/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//
#include "error.h"


main::Error::Error()
{
    handlers_["retry"] = [&](const char* command, const char* info)
    {
        if (strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
            Retry();
    };
    bridge::LoadWebView(index_, (__int32_t)core::VIEW_INFO::Default, "error");
}

main::Error::~Error()
{
}

void main::Error::LoadInput(const char* id)
{
    std::ostringstream js;
    js << "document.getElementById('" << id << "').value = '" << bridge::GetPreference(id) << "'";
    bridge::CallFunction(js.str().c_str());
}

void main::Error::SaveInput(const char* id, const char* value)
{
    bridge::SetPreference(id, value);
}

void main::Error::Retry()
{
    progress_ = PROGRESS::BEGIN;
    bridge::OnRestart();
}
