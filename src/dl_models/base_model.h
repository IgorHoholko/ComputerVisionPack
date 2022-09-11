//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_BASE_MODEL_H
#define COMPUTERVISIONPACK_BASE_MODEL_H

#include <filesystem>
#include <memory>
#include <string>

#include "nn/inference_wrapper.h"


namespace cvp {

    // forward = user pre-steps + run
    // run = preprocess + graph inference + postprocess
    template<class InputType, class OutputType>
    class BaseModel {
    public:
        BaseModel(const std::string& resource_path, const std::string& model_folder, ModelBackend backend, const std::string& graph_name);

        virtual void init(size_t batch_size = 1);

        virtual void forward(const cv::Mat* input, size_t size, OutputType* outputs) = 0;

    protected:
        virtual cv::Mat preprocess(const InputType* input)                                                            = 0;
        virtual void    postprocess(std::vector<Floats>& network_outputs, OutputType* outputs, size_t batch_size = 1) = 0;

        std::vector<cv::Mat> preprocess(const InputType* inputs, size_t size);

        void run(const InputType* inputs, size_t size, OutputType* outputs, TensorFormatType format, const Floats& lower = {}, const Floats& upper = {}, const std::vector<const char*>& output_names = {});
        void run(const InputType* inputs, size_t size, OutputType* outputs, const std::vector<const char*>& output_names = {});

        ModelBackend _backend;
        std::string  _graph_folder;
        std::string  _graph_name;

        std::unique_ptr<InferenceWrapper> _network;
    };

    template<class InputType, class OutputType>
    BaseModel<InputType, OutputType>::BaseModel(const std::string& resource_path, const std::string& model_folder, ModelBackend backend, const std::string& graph_name)
        : _graph_name(graph_name)
        , _backend(backend) {
        using std::filesystem::path;
        this->_graph_folder = path(resource_path) / path(model_folder);
    }

    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::init(size_t batch_size) {
        _network = std::make_unique<InferenceWrapper>(this->_graph_folder, this->_backend, this->_graph_name);
        _network->init(batch_size);
    }

    template<class InputType, class OutputType>
    std::vector<cv::Mat> BaseModel<InputType, OutputType>::preprocess(const InputType* inputs, size_t size) {
        std::vector<cv::Mat> outputs;
        outputs.reserve(size);

        for (int i = 0; i < size; i++){
            outputs.push_back( this->preprocess(inputs+i) );
        }
        return outputs;
    };


    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::run(const InputType* inputs, size_t size, OutputType* outputs, TensorFormatType format,
                                               const Floats& lower, const Floats& upper, const std::vector<const char*>& output_names) {
        std::vector<cv::Mat> network_input = this->preprocess(inputs, size);

        std::vector<Floats> network_outputs = this->_network->run(network_input.data(), size, format, lower, upper, output_names);

        this->postprocess(network_outputs, outputs, size);
    }

    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::run(const InputType* inputs, size_t size, OutputType* outputs, const std::vector<const char*>& output_names) {
        std::vector<cv::Mat> network_input = this->preprocess(inputs, size);

        std::vector<Floats> network_outputs = this->_network->run(network_input, size, output_names);

        this->postprocess(network_outputs, outputs, size);
    }


}// namespace cvp

#endif//COMPUTERVISIONPACK_BASE_MODEL_H
