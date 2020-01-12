//
//  progress.h
//  snake
//
//  Created by Ali Asadpoor on 7/13/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#ifndef PROGRESS_H
#define PROGRESS_H

#include <stdint.h>


enum class PROGRESS : __int32_t
{
    MENU,
    GAME,
};

extern PROGRESS progress_;

#endif //PROGRESS_H
