//
// Created by Игорь Хохолко on 9.09.22.
//

#ifndef COMPUTERVISIONPACK_INFERENCE_WRAPPER_H
#define COMPUTERVISIONPACK_INFERENCE_WRAPPER_H


#include <string>
#include <iostream>

#include "opencv2/imgproc.hpp"
#include <opencv2/dnn/dnn.hpp>

#include "common.h"
#include "nn/onnx_runtime_wrapper.h"
#include "nn/preprocess_image.h"

namespace cvp {

    class InferenceWrapper {
    public:
        InferenceWrapper(const std::string& resource_path, const std::string& graph_name, ModelBackend backend)
            : _resource_path(resource_path)
            , _graph_name(graph_name)
            , _backend(backend) {}

        void init(size_t batch_size = 1);

        std::vector<Floats> run(const std::vector<cv::Mat>& imgs, TensorFormatType format, Floats mean = {}, Floats std = {}, const std::vector<const char*>& output_names = {});
        std::vector<Floats> run(const cv::Mat& img, TensorFormatType format, Floats mean = {}, Floats std = {}, const std::vector<const char*>& output_names = {});

        std::vector<Floats> run(const std::vector<cv::Mat>& inputs, const std::vector<const char*>& output_names = {});
        std::vector<Floats> run(const cv::Mat& input, const std::vector<const char*>& output_names = {});

    protected:
        ModelBackend _backend;
        std::filesystem::path  _resource_path;
        std::filesystem::path _graph_name;
        size_t       _batch_size;

        std::unique_ptr<OnnxRuntimeWrapper> _onnx_model;
    };

};// namespace cvp


#endif//COMPUTERVISIONPACK_INFERENCE_WRAPPER_H
