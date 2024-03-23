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
        static inline const int32_t bpp_ = 4;
        using PIXEL = std::array<unsigned char, bpp_>;
        static inline const PIXEL color_empty_ = {16, 16, 16, 255};
        static inline const PIXEL color_food_ = {64, 64, 64, 255};
        static inline const PIXEL color_head_ = {48, 48, 48, 255};
        static inline const PIXEL color_tail_ = {32, 32, 32, 255};
        static inline const PIXEL color_border_ = {0, 0, 0, 255};
        static inline const int32_t request_width_ = 480;
        static inline const int32_t columns_ = 10;
        static inline const int32_t animation_frames_ = 4;
    
    private:
        std::thread worker_;
        bool run_;
        std::chrono::milliseconds frame_length_;
        std::mutex pixels_lock_;
        std::condition_variable waiter_;
        std::int32_t width_;
        std::int32_t height_;
        std::uint32_t order_rgba_;
        int drag_;
        bool pause_;
        int frame_;
        int pattern_frame_;
        int cell_;
        int border_;
        int margin_[2];
        std::vector<unsigned char> pattern_base_;
        std::vector<unsigned char> pattern_current_;
        std::vector<unsigned char> pattern_target_;
        std::vector<unsigned char> pixels_;
        std::vector<unsigned char> pixels_copy_;
        uint32_t image_index_;

    public:
        Game();
        ~Game();

    private:
        void Escape() override;
        void FeedUri(const char* uri, std::function<void(
            const std::vector<unsigned char>&)>&& consume) override;
        void Setup();
        void Run(const char* dimensions);
        void Resize(const char* size);
        void Tick();
        void Initial(int32_t width, int32_t height);
        void Step();
        void SetHandlers();
        void Drag(const char* side);
        void Turn();
        int Move();
        void Play(bool turn);
        void ApplyBoard(PIXEL* pixels);
        void DrawBorder(PIXEL* pixels);
        void DrawFood(PIXEL* pixels);
        void DrawHead(PIXEL* pixels);
        void DrawTail(PIXEL* pixels,
            const std::array<int, 2> & tail, const int side);
        void UpdatePause(bool state, bool async);
        void ApplyBack();
        void PlayAudio(const char* audio);
    };
}

#endif // SRC_GAME_H
