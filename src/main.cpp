//
//  main.cpp
//  snake-3D
//
//  Created by Ali Asadpoor on 7/13/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#include <sstream>

#include "../cross/core/src/main.h"

#include "data.h"
#include "game.h"
#include "menu.h"
#include "progress.h"

main::PROGRESS main::progress_ = main::PROGRESS::MENU;

void life_cycle::Begin()
{
    main::data_.Load();
}

void life_cycle::End()
{
    main::data_.Save();
}

void life_cycle::Create()
{
}

void life_cycle::Destroy()
{
    main::data_.Save();
}

void life_cycle::Start()
{
}

void life_cycle::Stop()
{
    main::data_.Save();
}

void life_cycle::Restart()
{
    main::data_.Save();
    switch (main::progress_)
    {
        case main::PROGRESS::MENU:
            core::Stage::stage_ = std::make_unique<main::Menu>();
        break;
        case main::PROGRESS::GAME:
            core::Stage::stage_ = std::make_unique<main::Game>();
        break;
    }
}
