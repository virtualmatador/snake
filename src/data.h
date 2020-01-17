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

namespace main
{
    class Data
    {
        static const int lives_max_ = 5;
        static const int frame_max_ = 16;
        static const int eat_max_ = 1000;
        static const int eat_level_ = 100;
        static const int score_max_ = 1000000000;
        static const int c_y_ = 10;
        static const int c_x_ = 20;
        friend class Menu;
        friend class Game;
    public:
        void Load();
        void Save();
        void Reset();
        void ResetSnakeFood();
        void ResetFood();
        int GetDelay();

    private:
        int lives_;
        int eat_;
        int score_;
        int frame_;
        int side_;
        bool sound_;
        std::array<int, 2> food_;
        std::list<std::array<int, 2>> parts_;
    };

    extern Data data_;
}

#endif //SRC_DATA_H
