//
// Created by Игорь Хохолко on 9.09.22.
//

#include "onnx_runtime_wrapper.h"

#ifdef WITH_GPU
#include "onnxruntime/core/providers/cuda/cuda_provider_factory.h"
#ifdef WITH_TENSORRT
#include "onnxruntime/core/providers/tensorrt/tensorrt_provider_factory.h"
#endif
#endif

namespace cvp {


    OnnxRuntimeWrapper::OnnxRuntimeWrapper(bool use_gpu, int log_level)
        : _use_gpu(use_gpu)
        , _memory_info(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault))
        , _batch_size(1)
        , _log_level(log_level) {
    }

    void OnnxRuntimeWrapper::init(const std::string& model_path, int batch_size) {
        std::string full_model_path = model_path + ".onnx";
        if (!std::filesystem::exists(full_model_path)) {
            std::string error = "Trying to create OnnxRuntimeWrapper on non-existent path " + full_model_path;
            throw std::runtime_error(error.c_str());
        }
        _batch_size = batch_size;

        Ort::SessionOptions session_options;
        session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
#ifdef WITH_GPU
        if (_use_gpu) {
#ifdef WITH_TENSORRT
            if (OrtSessionOptionsAppendExecutionProvider_tensorrt(sessionOptions, 0) != nullptr)
                throw std::runtime_error("Cannot Initialize ONNXRT TensorRT Session");
#else
            if (OrtSessionOptionsAppendExecutionProvider_CUDA(sessionOptions, _GPUindex) != nullptr)
                throw std::runtime_error("Cannot Initialize ONNXRT CUDA Session");
#endif
        } else
#endif
        {
            session_options.SetInterOpNumThreads(1);
            session_options.SetIntraOpNumThreads(1);
            if (OrtSessionOptionsAppendExecutionProvider_CPU(session_options, 0) != nullptr) {
                throw std::runtime_error("Cannot Initialize ONNXRT CPU Session");
            }
        }
//        std::stringstream log_name;
//        log_name << "log_";
//        log_name << std::hex << std::this_thread::get_id();
//        log_name << std::endl;
//        session_options.SetLogId(log_name.str().c_str());
        session_options.SetLogSeverityLevel(_log_level);

        Ort::Env env;
        _session.reset(new Ort::Session(env, full_model_path.c_str(), session_options));

        Ort::AllocatorWithDefaultOptions allocator;
        assert(_session->GetInputCount() == 1);
        for (size_t i = 0, n = _session->GetInputCount(); i < n; i++) {
            _input_names.push_back(_session->GetInputName(i, allocator));
            _input_dim = _session->GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
        }

        if (_input_dim[0] == -1) {
//            std::stringstream message;
//            message << full_model_path << " has dynamic input[0] : { ";
//            for (long long & j : _input_dim) {
//                message << j << " ";
//            }
//            message << "}. Try to set batch " << _batch_size << ".";

            _input_dim[0] = _batch_size;
        }

        for (size_t i = 0, n = _session->GetOutputCount(); i < n; i++) {
            _output_names.push_back(_session->GetOutputName(i, allocator));
            _output_dims.push_back(_session->GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape());
        }
    }

    std::vector<Ort::Value>& OnnxRuntimeWrapper::run(float* imgs, size_t size, size_t batch_size) {
        std::vector<std::int64_t> shape = _input_dim;
        shape[0]                        = batch_size;
        Ort::Value inputValue           = Ort::Value::CreateTensor<float>(_memory_info, imgs, size, shape.data(), shape.size());

        _output_values.clear();
        for (size_t i = 0; i < _output_names.size(); i++) {
            _output_values.emplace_back(nullptr);
        }

        _session->Run(Ort::RunOptions{nullptr}, _input_names.data(), &inputValue, _input_names.size(),
                      _output_names.data(), _output_values.data(), _output_names.size());

        return _output_values;
    }

    void OnnxRuntimeWrapper::setOutputNames(const std::vector<const char*>& new_names) {
        assert(_output_names.size() == new_names.size());

        Dims new_output_dims;
        new_output_dims.resize(new_names.size());

        for (size_t i = 0; i < new_names.size(); i++){
            const auto *new_name = new_names[i];
            auto it = std::find_if(_output_names.begin(), _output_names.end(), [new_name](const char* name){return (strcmp(name, new_name) == 0);});
            size_t idx = std::distance(_output_names.begin(), it);
            new_output_dims[idx] = _output_dims[i];
        }
        _output_dims = new_output_dims;
        _output_names = new_names;
    }
}// namespace cvp