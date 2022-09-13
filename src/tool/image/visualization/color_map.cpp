//
// Created by Игорь Хохолко on 13.09.22.
//

#include "color_map.h"


namespace cvp {

    ColorMap::ColorMap(int num_classes, const std::vector<std::string>& keys) {
        this->_generateColors(num_classes);
        if (!keys.empty()){
            this->setUpKeys(keys);
        }
    }

    Color ColorMap::at(int index) const { return _colors_list[index]; }
    Color ColorMap::operator[](int index) const { return _colors_list[index]; }

    Color ColorMap::at(const std::string& key) const {
        if (_keys_to_idxs_map.size() != _colors_list.size()){
            throw std::invalid_argument("Before getting colors by keys, initialize them!");
        }
        return _colors_list[_keys_to_idxs_map.at(key)];
    }
    Color ColorMap::operator[](const std::string& key) const { return this->at(key); }

    void ColorMap::setUpKeys(const std::vector<std::string>& keys) {
        if (keys.size() != _colors_list.size()){
            throw std::invalid_argument("Size of keys must match size of colors!");
        }
        for (int i = 0; i < keys.size(); i++){
            _keys_to_idxs_map[keys[i]] = i;
        }
    }

    void ColorMap::_generateColors(size_t num) {
        std::vector<int> color;
        int              j;
        int              lab;
        for (size_t i = 0; i < num; i++) {
            color = {0, 0, 0};
            j     = 0;
            lab   = i;
            while (lab != 0) {
                color[0] |= (((lab >> 0) & 1) << (7 - j));
                color[1] |= (((lab >> 1) & 1) << (7 - j));
                color[2] |= (((lab >> 2) & 1) << (7 - j));
                j++;
                lab >>= 3;
            }
            _colors_list.emplace_back(color[0], color[1], color[2]);
        }
    };


}// namespace cvp