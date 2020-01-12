//
//  error.h
//  snake-3D
//
//  Created by Ali Asadpoor on 4/12/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#include "../cross/core/src/stage.h"

#include "progress.h"


namespace main
{
    class Menu: public core::Stage
    {
    public:
        Menu();
        ~Menu();
        void Escape() override;

    private:
        void LoadInput(const char* id);
        void SaveInput(const char* id, const char* value);
        void Play();
        void Reset();
    };
}

#endif //SRC_ERROR_H
