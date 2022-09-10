//
// Created by Игорь Хохолко on 9.09.22.
//

#include "inference_wrapper.h"

#include <memory>


namespace cvp {

    void InferenceWrapper::init(size_t batch_size) {
        _batch_size = batch_size;

        switch (_backend) {
            case ModelBackend::ONNX_RUNTIME: {
                this->_onnx_model = std::make_unique<OnnxRuntimeWrapper>();
                std::filesystem::path graph_path = this->_resource_path / "onnx" / this->_graph_name;
                this->_onnx_model->init(graph_path, batch_size);
                break;
            }

        }
    }

    std::vector<Floats> InferenceWrapper::run(const cv::Mat& img, TensorFormatType format, Floats mean, Floats std, const std::vector<const char*>& output_names) {
        return this->run(std::vector<cv::Mat>({img}), format, mean, std, output_names);
    }
    std::vector<Floats> cvp::InferenceWrapper::run(const std::vector<cv::Mat>& imgs, TensorFormatType format, Floats mean, Floats std, const std::vector<const char*>& output_names) {
        std::vector<Floats> output;

        if (mean.empty() || std.empty()) {
            mean.clear();
            std.clear();
            for (size_t i = 0; i < imgs[0].channels(); i++){
                mean.push_back(0.F);
                std.push_back(1.F);
            }
        }

        switch (this->_backend) {
            case ModelBackend::ONNX_RUNTIME: {
                try {
                    size_t item_size  = imgs[0].cols * imgs[0].rows * imgs[0].channels();
                    size_t batch_size = imgs.size();

                    std::vector<float> input;
                    input.resize(batch_size * item_size);

                    if (output_names.size()){
                        this->_onnx_model->setOutputNames( output_names );
                    }

                    preprocessImage(imgs, format, mean, std, input.data());

                    auto& res = this->_onnx_model->run(input, batch_size);
                    for (auto& re : res) {
                        auto size = re.GetTensorTypeAndShapeInfo().GetElementCount();
                        output.emplace_back(re.GetTensorMutableData<float>(), re.GetTensorMutableData<float>() + size);
                    }
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    throw e;
                }
            }

        }
        return output;
    }

    std::vector<Floats> cvp::InferenceWrapper::run(const std::vector<cv::Mat>& inputs, const std::vector<const char*>& output_names) {
        return std::vector<Floats>();
    }
    std::vector<Floats> cvp::InferenceWrapper::run(const cv::Mat& input, const std::vector<const char*>& output_names) {
        return std::vector<Floats>();
    }
}// namespace cvp
