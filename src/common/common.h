//
// Created by Игорь Хохолко on 21.08.22.
//

#ifndef COMPUTERVISIONPACK_COMMON_H
#define COMPUTERVISIONPACK_COMMON_H

#include <any>
#include "map"
#include "string"

namespace cvp{


    using ParamsDict = std::map<std::string, std::any>;


    inline void make_odd(int& val){
        if (val % 2 == 0){
            val++;
        }
    }

}



#endif//COMPUTERVISIONPACK_COMMON_H
