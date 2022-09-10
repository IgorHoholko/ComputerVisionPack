//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_BASE_MODEL_H
#define COMPUTERVISIONPACK_BASE_MODEL_H

#include <memory>
#include <string>
#include <filesystem>

#include "nn/inference_wrapper.h"


namespace cvp {


    template<class InputType, class OutputType>
    class BaseModel {
    public:
        BaseModel(const std::string& resource_path, const std::string& graph_name, ModelBackend backend, const std::string& model_directory);

        virtual void init(size_t batch_size = 1);

        void forward(const InputType& input, std::vector<OutputType>& outputs);

        virtual void    forward(const std::vector<InputType>& input, std::vector<OutputType>& outputs)                             = 0;
        virtual cv::Mat preprocess(const InputType& input)                                                                         = 0;
        virtual void    postprocess(std::vector<Floats>& network_outputs, std::vector<OutputType>& outputs, size_t batch_size = 1) = 0;


        std::vector<cv::Mat> preprocess(const std::vector<InputType>& inputs);

        void run(const std::vector<InputType>& inputs, std::vector<OutputType>& outputs, TensorFormatType format, const Floats& lower = {}, const Floats& upper = {}, const std::vector<const char*>& output_names = {});
        void run(const InputType& input, std::vector<OutputType>& outputs, TensorFormatType format, const Floats& lower = {}, const Floats& upper = {}, const std::vector<const char*>& output_names = {});
        void run(const std::vector<InputType>& inputs, std::vector<OutputType>& outputs, const std::vector<const char*>& output_names = {});
        void run(const InputType& input, std::vector<OutputType>& outputs, const std::vector<const char*>& output_names = {});


        ModelBackend _backend;
        std::string  _resource_path;
        std::string  _graph_name;

        std::unique_ptr<InferenceWrapper> _network;
    };

    template<class InputType, class OutputType>
    BaseModel<InputType, OutputType>::BaseModel(const std::string& resource_path, const std::string& graph_name, ModelBackend backend, const std::string& model_directory)
        : _graph_name(graph_name)
        , _backend(backend) {
        using std::filesystem::path;
        this->_resource_path = path(resource_path) / "graphs" / path(model_directory);
    }

    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::init(size_t batch_size) {
        _network = std::make_unique<InferenceWrapper>(this->_resource_path, this->_graph_name, this->_backend);
        _network->init(batch_size);
    }


    template<class InputType, class OutputType>
    std::vector<cv::Mat> BaseModel<InputType, OutputType>::preprocess(const std::vector<InputType>& inputs) {
        std::vector<cv::Mat> output;
        output.reserve(inputs.size());
        for (const auto& input : inputs) {
            output.push_back(this->preprocess(input));
        }
        return output;
    };


    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::forward(const InputType& input, std::vector<OutputType>& outputs) {
        this->run(std::vector<InputType>{input}, outputs);
    }


    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::run(const std::vector<InputType>& inputs, std::vector<OutputType>& outputs, TensorFormatType format,
                                               const Floats& lower, const Floats& upper, const std::vector<const char*>& output_names) {
        std::vector<cv::Mat> model_input = this->preprocess(inputs);

        std::vector<Floats> network_outputs = this->_network->run(model_input, format, lower, upper, output_names);

        this->postprocess(network_outputs, outputs, inputs.size());
    }

    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::run(const InputType& input, std::vector<OutputType>& outputs, TensorFormatType format,
                                               const Floats& lower, const Floats& upper, const std::vector<const char*>& output_names) {
        this->run(std::vector<InputType>({input}), outputs, format, lower, upper, output_names);
    }

    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::run(const std::vector<InputType>& inputs, std::vector<OutputType>& outputs, const std::vector<const char*>& output_names) {
        std::vector<cv::Mat> model_input = this->preprocess(inputs);

        std::vector<Floats> network_outputs = this->_network->run(model_input, output_names);

        this->postprocess(network_outputs, outputs, inputs.size());
    }

    template<class InputType, class OutputType>
    void BaseModel<InputType, OutputType>::run(const InputType& input, std::vector<OutputType>& outputs, const std::vector<const char*>& output_names) {
        this->run(std::vector<InputType>({input}), outputs, output_names);
    }

}// namespace cvp

#endif//COMPUTERVISIONPACK_BASE_MODEL_H
