//
// Created by Игорь Хохолко on 10.09.22.
//

#include "base_detector.h"

namespace cvp{

    using namespace std;


    BaseDetector::BaseDetector(const std::string& resource_path, const std::string& graph_name, ModelBackend backend, const DetectorSettings& _settings)
        : BaseModel(resource_path, graph_name, backend, "detection/face")
        , _settings(_settings){};

    void BaseDetector::init(size_t batch_size) {
        BaseModel::init(batch_size);
        _input_size = cv::Size(_settings.input_width, _settings.input_height);
    }

    cv::Mat BaseDetector::preprocess(const cv::Mat& input) {
        auto w   = (float) input.cols;
        auto h   = (float) input.rows;
        auto w_i = (float) _settings.input_width;
        auto h_i = (float) _settings.input_height;

        if (!this->_settings.smart_pad_resize) {
            cv::resize(input, this->_input, _input_size, _settings.interpolation);
            this->_mapPointBackward.emplace_back([w, h, w_i, h_i](float x, float y) { return tuple<float, float>{x * w / w_i, y * h / h_i}; });
        } else {
            this->_mapPointBackward.emplace_back(SmartPadResize::resize(input, this->_input, _input_size, cv::BorderTypes::BORDER_CONSTANT,
                                                                        _settings.interpolation, _settings.smart_pad_resize_padding, 0));
        }

        return _input;
    }

    void BaseDetector::forward(const std::vector<cv::Mat>& input, std::vector<BoxesItem>& outputs) {
        this->_mapPointBackward.clear();
        this->_mapPointBackward.reserve(input.size());

        this->BaseDetector::run(input, outputs, this->_settings.format, this->_settings.mean, this->_settings.std, this->_settings.output_names);
    }
}
