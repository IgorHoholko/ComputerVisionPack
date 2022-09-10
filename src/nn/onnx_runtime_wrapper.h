//
// Created by Игорь Хохолко on 9.09.22.
//

#ifndef COMPUTERVISIONPACK_ONNXRUNTIME_WRAPPER_H
#define COMPUTERVISIONPACK_ONNXRUNTIME_WRAPPER_H

#include <filesystem>
#include <vector>

#include "onnxruntime/core/providers/cpu/cpu_provider_factory.h"
#include "onnxruntime_c_api.h"
#include "onnxruntime_cxx_api.h"


namespace cvp {
    class OnnxRuntimeWrapper {
        using Dim  = std::vector<int64_t>;
        using Dims = std::vector<Dim>;

    public:
        OnnxRuntimeWrapper(bool use_gpu = false, int log_level = 4);

        void init(const std::string& modelPath, int batchSize);

        std::vector<Ort::Value>& run(const std::vector<float>& imgs, size_t batchSize);

        void setOutputNames(const std::vector<const char*>& new_names);

        inline int64_t     height() const { return _input_dim[2]; }
        inline int64_t     width() const { return _input_dim[3]; }
        inline const Dims& outputDims() const { return _output_dims; }

    private:
        bool _use_gpu;

        Ort::MemoryInfo _memory_info;

        std::vector<const char*> _input_names;
        Dim                      _input_dim;

        std::vector<const char*> _output_names;
        std::vector<Ort::Value>  _output_values;
        Dims                     _output_dims;

        std::shared_ptr<Ort::Session> _session;
        int                           _batch_size;

        int _log_level;
    };

}// namespace cvp


#endif//COMPUTERVISIONPACK_ONNXRUNTIME_WRAPPER_H
