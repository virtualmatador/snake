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
    void Load(const char* key, T & value, const T & begin, const T & end)
    {
        std::istringstream parser(bridge::GetPreference(key));
        parser >> value;
        if (parser.fail())
            throw "";
        if (value < begin || value > end)
            throw "";
    };

    template<typename T>
    void Save(const char* key, const T & value)
    {
        std::ostringstream composer;
        composer << value;
        bridge::SetPreference(key, composer.str().c_str());
    }
}

#endif //SRC_TOOLBOX_H
