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
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

#include "stage.h"

namespace main
{
    class Game : public core::Stage
    {
    private:
        static inline const uint32_t color_empty_ = 0 << 24 | 0 << 16 | 0 << 8 | 0 << 0;
        static inline const uint32_t color_food_ = 80 << 24 | 80 << 16 | 80 << 8 | 80 << 0;
        static inline const uint32_t color_head_ = 64 << 24 | 64 << 16 | 64 << 8 | 64 << 0;
        static inline const uint32_t color_tail_ = 48 << 24 | 48 << 16 | 48 << 8 | 48 << 0;
        static inline const uint32_t color_border_ = 32 << 24 | 32 << 16 | 32 << 8 | 32 << 0;
        static inline const int32_t request_width_ = 480;
        static inline const int32_t columns_ = 10;
        static inline const int32_t animation_frames_ = 4;
    
    private:
        std::thread worker_;
        bool run_;
        std::chrono::milliseconds frame_lenght_;
        std::int32_t width_;
        std::int32_t height_;
        std::uint32_t order_rgba_;
        bool pause_;
        int frame_;
        int pattern_frame_;
        int cell_;
        int border_;
        int margin_[2];
        std::vector<unsigned char> pattern_base_;
        std::vector<unsigned char> pattern_current_;
        std::vector<unsigned char> pattern_target_;
        std::vector<uint32_t> pixels_;
        std::mutex pixels_lock_;

    public:
        Game();
        ~Game();

    private:
        void Escape() override;
        void Image();
        void Run(const char* dimensions);
        void Resize(const char* size);
        void Tick();
        void Initial(int32_t width, int32_t height);
        void Step();
        void SetHandlers();
        void Click();
        void Drag(const char* side);
        int Move();
        void Play(bool turn);
        void ApplyBoard();
        void DrawBorder(const std::uint32_t color);
        void DrawFood();
        void DrawHead();
        void DrawTail(const std::array<int, 2> & tail, const int side);
        void PlayAudio(const char* audio);
    };
}

#endif // SRC_GAME_H
