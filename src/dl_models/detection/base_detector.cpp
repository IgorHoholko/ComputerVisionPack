//
// Created by Игорь Хохолко on 10.09.22.
//

#include "base_detector.h"

namespace cvp {

    using namespace std;


    BaseDetector::BaseDetector(const std::string& resource_path, const std::string& model_folder, ModelBackend backend, const std::string& graph_name, DetectorSettingsUPtr t_settings)
        : BaseModel(resource_path, model_folder, backend, graph_name)
        , settings(std::move(t_settings))
        , _input_size(settings->input_width, settings->input_height){};

    void BaseDetector::init(size_t batch_size) {
        BaseModel::init(batch_size);
    }

    cv::Mat BaseDetector::preprocess(const cv::Mat* input) {
        auto w   = (float) input->cols;
        auto h   = (float) input->rows;
        auto w_i = (float) settings->input_width;
        auto h_i = (float) settings->input_height;

        if (!this->settings->smart_pad_resize) {
            cv::resize(*input, this->_input, _input_size, settings->interpolation);
            _mapPointBackward.emplace_back([w, h, w_i, h_i](float x, float y) { return tuple<float, float>{x * w / w_i, y * h / h_i}; });
        } else {
            _mapPointBackward.emplace_back(SmartPadResize::resize(*input, this->_input, _input_size, cv::BorderTypes::BORDER_CONSTANT,
                                                                  settings->interpolation, settings->smart_pad_resize_padding, 0));
        }

        return _input;
    }

    void BaseDetector::forward(const cv::Mat* input, size_t size, BoxesItem* outputs) {
        _mapPointBackward.clear();
        _mapPointBackward.reserve(size);

        BaseDetector::run(input, size, outputs, this->settings->format, this->settings->mean, this->settings->std, this->settings->output_names);
    }
}// namespace cvp
