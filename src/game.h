//
//  begin.h
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2020 Shaidin. All rights reserved.
//

#ifndef SRC_GAME_H
#define SRC_GAME_H

#include <array>
#include <vector>

#include "../cross/core/src/runner.h"

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
        int frame_;
        int pattern_frame_;
        int column_;
        int cell_;
        int border_;
        int margin_[2];
        float touch_[2];
        std::vector<unsigned char> pattern_base_;
        std::vector<unsigned char> pattern_current_;
        std::vector<unsigned char> pattern_target_;

    public:
        Game();
        ~Game();

        void Escape() override;
        void Initial() override;
        void Step(__uint32_t* pixels) override;
        void TouchBegin(const float x, const float y) override;
        void TouchMove(const float x, const float y) override;
        void TouchEnd(const float x, const float y) override;

    private:

        int Move();
        void Play(bool turn);
        void ApplyBoard(__uint32_t* pixels);
        void DrawBorder(__uint32_t* pixels, const __uint32_t color);
        void DrawFood(__uint32_t* pixels);
        void DrawHead(__uint32_t* pixels);
        void DrawTail(__uint32_t* pixels, const std::array<int, 2> & tail, const int side);
    };
}

#endif //SRC_GAME_H
