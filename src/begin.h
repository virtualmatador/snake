//
//  begin.h
//  snake-3D
//
//  Created by Ali Asadpoor on 4/11/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#ifndef SRC_BEGIN_H
#define SRC_BEGIN_H

#include "../cross/core/runner.h"

#include "progress.h"

namespace main
{
    class Begin : public core::Runner {
    private:
        int column_;
        std::vector<unsigned char> pattern_;

        int data_x_;
        int data_y_;

    public:
        Begin();
        ~Begin();
        void Initial() override;
        void Step(__int32_t* pixels) override;
        void TouchBegin(float x, float y) override;
        void TouchMove(float x, float y) override;
        void TouchEnd(float x, float y) override;
    };
}

#endif //SRC_BEGIN_H
