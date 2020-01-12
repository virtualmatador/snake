//
//  data.h
//  snake-3D
//
//  Created by Ali Asadpoor on 1/11/20.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#ifndef SRC_DATA_H
#define SRC_DATA_H

#include <array>
#include <list>

#define CX 20
#define CY 10
#define MAX_FRAME 16

namespace main
{
    class Data
    {
        friend class Menu;
        friend class Game;
    public:
        void Load();
        void Save();
        void Reset();
        void ResetSnake();
        void ResetFood();

    private:
        int lives_;
        int level_;
        int score_;
        int frame_;
        std::array<int, 2> food_;
        std::list<std::array<int, 2>> parts_;
        int side_;
    };

    extern Data data_;
}

#endif //SRC_DATA_H
