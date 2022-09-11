//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_BASE_DETECTOR_H
#define COMPUTERVISIONPACK_BASE_DETECTOR_H


#include "opencv2/imgproc.hpp"

#include "common.h"
#include "dl_models/base_model.h"
#include "tool/image/smart_pad_resize.h"
#include "tool/detection/prior_box.h"


namespace cvp {

    using namespace std;


    struct DetectorSettings {
        size_t                   input_width;
        size_t                   input_height;
        float                    threshold;
        vector<float>            mean;
        vector<float>            std;
        TensorFormatType         format;
        cv::InterpolationFlags   interpolation            = cv::INTER_LINEAR;
        bool                     smart_pad_resize         = false;
        size_t                   smart_pad_resize_padding = 0;
        bool                     allow_upscale            = true;
        std::vector<const char*> output_names             = {};


        bool   landmarks_presented = true;
        size_t num_landmarks       = 10;

        PriorBoxParams prior_box_params = {{}, {8, 16, 32}};
    };

    struct BoxesItem {
        vector<vector<float>> boxes;
        vector<float>         scores;
        vector<vector<float>> landmarks;
    };


    class BaseDetector : public BaseModel<cv::Mat, BoxesItem> {

    public:
        BaseDetector(const std::string& resource_path, const std::string& graph_name, ModelBackend backend, const DetectorSettings& settings);

        void init(size_t batch_size = 1) override;

        void forward(const vector<cv::Mat>& input, vector<BoxesItem>& outputs) override;

    protected:
        cv::Mat preprocess(const cv::Mat& input) override;

        void postprocess(vector<Floats>& network_outputs, vector<BoxesItem>& outputs, size_t batch_size = 1) override = 0;


    protected:
        DetectorSettings _settings;

        vector<std::function<tuple<float, float>(float, float)>> _mapPointBackward;

        cv::Mat  _input;
        cv::Size _input_size;
    };

    using BaseDetectorUPtr = unique_ptr<BaseDetector>;


}// namespace cvp


#endif//COMPUTERVISIONPACK_BASE_DETECTOR_H
