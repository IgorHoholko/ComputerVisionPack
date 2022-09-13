//
// Created by Игорь Хохолко on 12.09.22.
//

#ifndef COMPUTERVISIONPACK_COLOR_MAP_H
#define COMPUTERVISIONPACK_COLOR_MAP_H


#include "unordered_map"
#include <any>
#include <string>
#include <tuple>
#include <vector>

#include <opencv2/imgproc.hpp>

namespace cvp {

    using Color = cv::Scalar;

    class ColorMap {

    public:
        ColorMap(int num_classes, const std::vector<std::string>& keys = {});

        void setUpKeys(const std::vector<std::string>& keys);

        Color at(int index) const;
        Color operator[] (int index) const;

        Color at(const std::string& key) const;
        Color operator[](const std::string& key) const;

    private:
        void _generateColors(size_t num);

        std::vector<Color>                   _colors_list{};
        std::unordered_map<std::string, int> _keys_to_idxs_map;
    };

}// namespace cvp


#endif//COMPUTERVISIONPACK_COLOR_MAP_H
