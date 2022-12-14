//
// Created by Игорь Хохолко on 10.09.22.
//

#include "anchor_ssd.h"


namespace cvp {

    AnchorSSD::AnchorSSD(const std::string& resource_path, const std::string& model_folder, ModelBackend backend,
                         const std::string& graph_name, DetectorSettingsUPtr t_settings)
        : BaseDetector(resource_path, model_folder, backend, graph_name, std::move(t_settings)) {}

    void AnchorSSD::init(size_t batch_size) {
        this->_priors = PriorBox(settings->prior_box_params, settings->input_height, settings->input_width);
        BaseDetector::init(batch_size);
    }

    //FIXME: adopt for multiclass
    void AnchorSSD::postprocess(std::vector<Floats>& network_outputs, BoxesItem* outputs, size_t batch_size) {
        using namespace std;
        using namespace BoxUtils;

        vector<float> dummy     = {-1};
        auto          variances = settings->prior_box_params.variances;

        float* loc   = network_outputs[0].data();                                                      // [batch_size, num_anchors, 4].
        float* score = network_outputs[1].data() + 1;                                                  // [batch_size, num_anchors, 2].
        float* lndm  = (settings->landmarks_presented) ? network_outputs[2].data() : dummy.data();// [batch_size, num_anchors, num_landmarks].

        float         cxcy[4];
        vector<float> xy(4);
        vector<float> landmark(settings->num_landmarks);

        for (size_t batch_idx = 0; batch_idx < batch_size; batch_idx++) {

            const float* prior = this->_priors();

            for (size_t box_idx = 0; box_idx < this->_priors.getNPriors(); ++box_idx, loc += 4, score += 2, prior += 4, lndm += settings->num_landmarks * settings->landmarks_presented) {
                if (*score < settings->threshold)
                    continue;

                gcxgcy_to_cxcy(loc, prior, cxcy, variances.first, variances.second);
                cxcy_to_xy(cxcy, xy.data());

                // validate xy
                if (!validate_bbox(xy.data(), 1.f, 1.f))
                    continue;

                xy[0] *= settings->input_width;
                xy[1] *= settings->input_height;
                xy[2] *= settings->input_width;
                xy[3] *= settings->input_height;

                tie(xy[0], xy[1]) = this->_mapPointBackward[batch_idx](xy[0], xy[1]);
                tie(xy[2], xy[3]) = this->_mapPointBackward[batch_idx](xy[2], xy[3]);

                outputs[batch_idx].boxes.push_back(xy);
                outputs[batch_idx].scores.push_back(*score);

                if (settings->landmarks_presented) {
                    decode_landmarks(lndm, prior, landmark.data(), variances.first);
                    for (size_t j = 0; j < settings->num_landmarks / 2; j += 1) {
                        clip(landmark[2 * j], 0.f, 1.f);
                        clip(landmark[2 * j + 1], 0.f, 1.f);
                        landmark[2 * j] *= settings->input_width;
                        landmark[2 * j + 1] *= settings->input_height;
                        tie(landmark[2 * j], landmark[2 * j + 1]) = this->_mapPointBackward[batch_idx](landmark[2 * j], landmark[2 * j + 1]);
                    }
                    outputs[batch_idx].landmarks.push_back(landmark);
                }
            }
        }
    }

}// namespace cvp