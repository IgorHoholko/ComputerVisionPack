//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_BASE_DETECTOR_H
#define COMPUTERVISIONPACK_BASE_DETECTOR_H


#include "opencv2/imgproc.hpp"

#include "common.h"
#include "dl_models/base_model.h"
#include "tool/detection/prior_box.h"
#include "tool/image/smart_pad_resize.h"


namespace cvp {

    using namespace std;


    struct DetectorSettings {
        const size_t                   input_width;
        const size_t                   input_height;
        float                          threshold;
        const vector<float>            mean;
        const vector<float>            std;
        const TensorFormatType         format;
        const cv::InterpolationFlags   interpolation            = cv::INTER_LINEAR;
        bool                           smart_pad_resize         = false;
        size_t                         smart_pad_resize_padding = 0;
        bool                           allow_upscale            = true;
        const std::vector<const char*> output_names             = {};


        const bool   landmarks_presented = true;
        const size_t num_landmarks       = 10;

        const PriorBoxParams prior_box_params = {{}, {8, 16, 32}};
    };

    using DetectorSettingsUPtr = std::unique_ptr<DetectorSettings>;

    struct BoxesItem {
        vector<vector<float>> boxes;
        vector<float>         scores;
        vector<vector<float>> landmarks;
    };


    class BaseDetector : public BaseModel<cv::Mat, BoxesItem> {

    public:
        BaseDetector(const std::string& resource_path, const std::string& model_folder, ModelBackend backend, const std::string& graph_name, DetectorSettingsUPtr t_settings);

        void init(size_t batch_size = 1) override;

        void forward(const cv::Mat* input, size_t size, BoxesItem* outputs) override;

        const DetectorSettingsUPtr settings;

    protected:
        cv::Mat preprocess(const cv::Mat* input) override;

        void postprocess(vector<Floats>& network_outputs, BoxesItem* outputs, size_t batch_size = 1) override = 0;

        vector<std::function<tuple<float, float>(float, float)>> _mapPointBackward;

        cv::Mat  _input;
        cv::Size _input_size;
    };

    using BaseDetectorUPtr = unique_ptr<BaseDetector>;


}// namespace cvp


#endif//COMPUTERVISIONPACK_BASE_DETECTOR_H
