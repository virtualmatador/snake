//
//  error.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 4/12/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#include <sstream>

#include "../cross/core/src/helper.h"

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
    handlers_["https"] = [&](const char* command, const char* info)
    {
        if (strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
            Https();
    };
    handlers_["http"] = [&](const char* command, const char* info)
    {
        if (strlen(command) == 0)
            return;
        else if (std::strcmp(command, "echo") == 0)
            Response(info);
    };
    bridge::LoadWebView(index_, (__int32_t)core::VIEW_INFO::Default, "error");
}

main::Error::~Error()
{
}

void main::Error::Escape()
{
    bridge::Exit();
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
    bridge::NeedRestart();
}

void main::Error::Https()
{
    std::list<std::pair<std::string, std::string>> params;
    params.push_back({"name", "ali&me=kazem&a=:\"<%>"});
    helper::Http("https://postman-echo.com/post", params, "http", "echo");
}


void main::Error::Response(const char* response)
{
    std::ostringstream composer;
    composer << "document.getElementById(\"id-response\").innerHTML=\"" << helper::EscapeHtml(response) << "\";";
    bridge::CallFunction(composer.str().c_str());
}
