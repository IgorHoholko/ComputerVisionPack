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
                this->_onnx_model                = std::make_unique<OnnxRuntimeWrapper>();
                std::filesystem::path graph_path = this->_models_folder / "onnx" / this->_graph_name;
                this->_onnx_model->init(graph_path, batch_size);
                break;
            }
        }
    }


    std::vector<Floats> InferenceWrapper::run(const cv::Mat* imgs, size_t size, TensorFormatType format, Floats mean, Floats std, const std::vector<const char*>& output_names) {
        std::vector<Floats> output;

        if (mean.empty() || std.empty()) {
            mean.clear();
            std.clear();
            for (size_t i = 0; i < imgs[0].channels(); i++) {
                mean.push_back(0.F);
                std.push_back(1.F);
            }
        }

        switch (this->_backend) {
            case ModelBackend::ONNX_RUNTIME: {
                //                try {
                size_t item_size  = imgs[0].cols * imgs[0].rows * imgs[0].channels();
                size_t batch_size = size;// TODO: fix

                std::vector<float> input;
                input.resize(batch_size * item_size);

                if (output_names.size()) {
                    this->_onnx_model->setOutputNames(output_names);
                }

                //                    preprocessImage(imgs, size, format, mean, std, input.data());
                cv::Mat resizedImageBGR, resizedImage, preprocessedImage;
                cv::resize(*imgs, resizedImageBGR, cv::Size(640, 480), cv::InterpolationFlags::INTER_LINEAR);
                resizedImageBGR.convertTo(resizedImage, CV_32F, 1.0 / 255);

                cv::Mat channels[3];
                cv::split(resizedImage, channels);
                // Normalization per channel
                // Normalization parameters obtained from
                // https://github.com/onnx/models/tree/master/vision/classification/squeezenet
                channels[0] = (channels[0] - 0.485) / 0.229;
                channels[1] = (channels[1] - 0.456) / 0.224;
                channels[2] = (channels[2] - 0.406) / 0.225;
                cv::merge(channels, 3, resizedImage);
                // HWC to CHW
                cv::dnn::blobFromImage(resizedImage, preprocessedImage);

                size_t             inputTensorSize = item_size;
                std::vector<float> inputTensorValues(inputTensorSize);
                // Make copies of the same image input.
                for (int64_t i = 0; i < 1; ++i) {
                    std::copy(preprocessedImage.begin<float>(),
                              preprocessedImage.end<float>(),
                              inputTensorValues.begin() + i * inputTensorSize / 1);
                }


                auto& res = this->_onnx_model->run(inputTensorValues.data(), batch_size);
                for (auto& re : res) {
                    auto size = re.GetTensorTypeAndShapeInfo().GetElementCount();
                    output.emplace_back(re.GetTensorMutableData<float>(), re.GetTensorMutableData<float>() + size);
                }
                //                } catch (const std::exception& e) {
                //                    std::cerr << e.what() << std::endl;
                //                    throw e;
                //                }
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
