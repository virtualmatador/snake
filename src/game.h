//
//  begin.h
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#ifndef SRC_BEGIN_H
#define SRC_BEGIN_H

#include <array>
#include <list>
#include <vector>

#include "../cross/core/src/runner.h"

#include "progress.h"

#define CX 20
#define CY 10
#define FR 4

namespace main
{
    class Game : public core::Runner {
    private:
        static const uint32_t color_empty_ = 0 << 24 | 0 << 16 | 0 << 8 | 0 << 0;
        static const uint32_t color_food_ = 80 << 24 | 80 << 16 | 80 << 8 | 80 << 0;
        static const uint32_t color_head_ = 64 << 24 | 64 << 16 | 64 << 8 | 64 << 0;
        static const uint32_t color_tail_ = 48 << 24 | 48 << 16 | 48 << 8 | 48 << 0;
        static const uint32_t color_border_ = 32 << 24 | 32 << 16 | 32 << 8 | 32 << 0;
        bool pause_;
        int pattern_frame_;
        int game_frame_;
        int column_;
        int cell_;
        int border_;
        int margin_[2];
        float touch_[2];
        std::vector<unsigned char> pattern_base_;
        std::vector<unsigned char> pattern_current_;
        std::vector<unsigned char> pattern_target_;

        int score_;
        int level_;
        int lives_;
        std::array<int, 2> food_;
        std::list<std::array<int, 2>> parts_;
        int side_;

    public:
        Game();
        ~Game();

        void Escape() override;
        void Initial() override;
        void Step(__uint32_t* pixels) override;
        void ResetSnake();
        void ResetFood();
        void TouchBegin(const float x, const float y) override;
        void TouchMove(const float x, const float y) override;
        void TouchEnd(const float x, const float y) override;

    private:

        bool Move();
        void Play();
        void ApplyBoard(__uint32_t* pixels);
        void DrawBorder(__uint32_t* pixels, const __uint32_t color);
        void DrawSquare(__uint32_t* pixels, const std::array<int, 2> & target, const __uint32_t color);
    };
}

#endif //SRC_BEGIN_H
