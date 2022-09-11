//
// Created by Игорь Хохолко on 9.09.22.
//

#ifndef COMPUTERVISIONPACK_INFERENCE_WRAPPER_H
#define COMPUTERVISIONPACK_INFERENCE_WRAPPER_H


#include <iostream>
#include <string>

#include "opencv2/imgproc.hpp"
#include <opencv2/dnn/dnn.hpp>

#include "common.h"
#include "nn/onnx_runtime_wrapper.h"
#include "nn/preprocess_image.h"

namespace cvp {

    class InferenceWrapper {
    public:
        InferenceWrapper(const std::string& models_folder, ModelBackend backend, const std::string& graph_name)
            : _models_folder(models_folder)
            , _backend(backend)
            , _graph_name(graph_name)
            , _batch_size(1) {}

        void init(size_t batch_size = 1);

        std::vector<Floats> run(const cv::Mat* imgs, size_t size, TensorFormatType format, Floats mean = {}, Floats std = {}, const std::vector<const char*>& output_names = {});

        std::vector<Floats> run(const std::vector<cv::Mat>& inputs, const std::vector<const char*>& output_names = {});
        std::vector<Floats> run(const cv::Mat& input, const std::vector<const char*>& output_names = {});

    protected:
        std::filesystem::path _models_folder;
        ModelBackend          _backend;
        std::filesystem::path _graph_name;
        size_t                _batch_size;

        std::unique_ptr<OnnxRuntimeWrapper> _onnx_model;
    };

};// namespace cvp


#endif//COMPUTERVISIONPACK_INFERENCE_WRAPPER_H
