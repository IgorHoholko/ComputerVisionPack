//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_PRIOR_BOX_H
#define COMPUTERVISIONPACK_PRIOR_BOX_H

#include <math.h>
#include <vector>

namespace cvp {
    using namespace std;


    struct PriorBoxParams {
        vector<vector<size_t>> min_sizes;
        vector<size_t>         steps;
        bool                   clip      = false;
        pair<float, float>     variances = {0.1f, 0.2f};
    };


    class PriorBox {
    public:
        PriorBox()
            : _nPriors(0){};

        PriorBox(const PriorBoxParams& params, size_t imageH, size_t imageW)
            : PriorBox(params.min_sizes, params.steps, params.clip, imageH, imageW){};

        PriorBox(const vector<vector<size_t>>& min_sizes_, const vector<size_t>& steps, bool clip, size_t imageH, size_t imageW) {
            vector<pair<size_t, size_t>> feature_maps;
            for (const auto& step : steps) {
                feature_maps.push_back({ceilf((float) imageH / step), ceilf((float) imageW / step)});
            }

            for (size_t k = 0; k < feature_maps.size(); ++k) {
                const auto& f         = feature_maps[k];
                const auto& min_sizes = min_sizes_[k];
                for (size_t i = 0; i < f.first; ++i) {
                    for (size_t j = 0; j < f.second; ++j) {
                        float dense_cx = (j + 0.5f) * steps[k] / imageW;
                        float dense_cy = (i + 0.5f) * steps[k] / imageH;

                        for (const auto& min_size : min_sizes) {
                            float s_kx = (float) min_size / imageW;
                            float s_ky = (float) min_size / imageH;

                            _data.push_back(dense_cx);
                            _data.push_back(dense_cy);
                            _data.push_back(s_kx);
                            _data.push_back(s_ky);
                        }
                    }
                }
            }
            if (clip) {
                for (auto& x : _data)
                    x = std::min(1.f, std::max(0.f, x));
            }

            _nPriors = _data.size() / 4;
        }

        size_t getNPriors() const {
            return _nPriors;
        }

        // array size is getNPriors() * 4
        const float* operator()() const {
            return _data.data();
        }


    private:
        size_t        _nPriors;
        vector<float> _data;
    };
}// namespace cvp


#endif//COMPUTERVISIONPACK_PRIOR_BOX_H
