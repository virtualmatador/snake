//
//  main.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 7/13/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#include "../cross/core/main.h"

#include "begin.h"
#include "error.h"


PROGRESS progress_;

void life_cycle::Begin()
{
}

void life_cycle::End()
{
}

void life_cycle::Create()
{
}

void life_cycle::Destroy()
{
}

void life_cycle::Start()
{
    try
    {
       progress_ = (PROGRESS)std::stoi(bridge::GetPreference("progress"));
    }
    catch (...)
    {
        progress_ = PROGRESS::BEGIN;
    }
}

void life_cycle::Stop()
{
    std::ostringstream composer;
    composer << (int)progress_;
    bridge::SetPreference("progress", composer.str().c_str());
}

void life_cycle::Restart()
{
    switch (progress_)
    {
        case PROGRESS::BEGIN:
            core::Stage::stage_ = std::make_unique<main::Begin>();
        break;
        case PROGRESS::ERROR:
            core::Stage::stage_ = std::make_unique<main::Error>();
        break;
    }
}
