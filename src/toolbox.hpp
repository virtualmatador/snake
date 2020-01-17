//
//  toolbox.h
//  snake
//
//  Created by Ali Asadpoor on 7/25/19.
//  Copyright © 2019 Shaidin. All rights reserved.
//

#ifndef SRC_TOOLBOX_H
#define SRC_TOOLBOX_H

namespace toolbox
{
    template<typename T>
    void Load(std::istream & parser, T & value, const T & begin, const T & end)
    {
        parser >> value;
        if (parser.fail())
            throw "";
        if (value < begin || value > end)
            throw "";
    };

    template<typename T>
    void Save(std::ostream & composer, const T & value)
    {
        composer << value << ' ';
        if (composer.fail())
            throw "";
    }
}

#endif //SRC_TOOLBOX_H
