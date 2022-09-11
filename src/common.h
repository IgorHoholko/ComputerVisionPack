//
// Created by Игорь Хохолко on 21.08.22.
//

#ifndef COMPUTERVISIONPACK_COMMON_H
#define COMPUTERVISIONPACK_COMMON_H

#include <any>
#include "map"
#include "string"
#include "stack"

namespace cvp{


    using ParamsDict = std::map<std::string, std::any>;
    using Floats = std::vector<float>;

    enum ModelBackend{
        ONNX_RUNTIME
    };

    enum TensorFormatType{
        NCHW,
        NHWC
    };


    inline void make_odd(int& val){
        if (val % 2 == 0){
            val++;
        }
    };

}



#endif//COMPUTERVISIONPACK_COMMON_H
