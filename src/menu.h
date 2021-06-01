//
//  menu.h
//  snake-3D
//
//  Created by Ali Asadpoor on 4/12/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#ifndef SRC_MENU_H
#define SRC_MENU_H

#include "../cross/core/src/stage.h"

namespace main
{
    class Menu: public core::Stage
    {
    public:
        Menu();
        ~Menu();
        void Escape() override;

    private:
        void Play();
        void Reset();
    };
}

#endif //SRC_MENU_H
