//
//  begin.h
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#ifndef SRC_BEGIN_H
#define SRC_BEGIN_H

#include <vector>
#include <list>

#include "../cross/core/src/runner.h"

#include "progress.h"

#define CX 12
#define CY 8
#define FR 4

namespace main
{
    class Begin : public core::Runner {
    private:
        enum CellType: unsigned char
        {
            Empty,
            Food,
            Head,
            Tail,
        };
        int frame_;
        int column_;
        int cell_size_;
        int margin_x_;
        int margin_y_;
        CellType board_[CY][CX];
        std::vector<unsigned char> pattern_base_;
        std::vector<unsigned char> pattern_current_;
        std::vector<unsigned char> pattern_target_;

        int score_;
        int level_;
        int lives_;
        int food_[2];
        int head_[2];
        std::list<unsigned char> snake_;
        unsigned char side_;

    public:
        Begin();
        ~Begin();

        void Escape() override;
        void Initial() override;
        void Step(__uint32_t* pixels) override;
        void TouchBegin(const float x, const float y) override;
        void TouchMove(const float x, const float y) override;
        void TouchEnd(const float x, const float y) override;

    private:

        void MoveCell(const int part, int & y, int & x);
        void MoveHead(const int part, int & y, int & x);
        void Play();
        void DrawBoard();
        void DrawSnake();
        void DrawFood();
        void ApplyBoard(__uint32_t* pixels);
        void DrawSquare(__uint32_t* pixels, const int y, const int x, const __uint32_t color);
    };
}

#endif //SRC_BEGIN_H
