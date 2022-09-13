//
// Created by Игорь Хохолко on 12.09.22.
//

#ifndef COMPUTERVISIONPACK_GEOMETRY_H
#define COMPUTERVISIONPACK_GEOMETRY_H

#include <vector>
#include <numeric>

namespace cvp {

    template<typename T>
    inline T vectorProduct(const std::vector<T>& v) {
        return std::accumulate(v.begin(), v.end(), 1, std::multiplies<T>());
    }

}// namespace cvp


#endif//COMPUTERVISIONPACK_GEOMETRY_H
