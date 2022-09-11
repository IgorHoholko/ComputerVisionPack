//
// Created by Игорь Хохолко on 11.09.22.
//

#include "faces_detection_plugin.h"


namespace cvp::gui {

    FacesDetectionPlugin::FacesDetectionPlugin(QObject* parent)
        : QObject(parent) {
        _models_factory = std::make_unique<ModelsFactory>();
    }

    QString FacesDetectionPlugin::name() {
        return "FacesDetection";
    }

    EditPanel* FacesDetectionPlugin::createEditPanel(QWidget* parent) {

        auto* edit_panel = new EditPanel(parent);

        edit_panel->addComboBox("graph_name", {"retina_resnet50_sml_480x640"}, 0, "Model Name: ");
        edit_panel->addComboBox("backend", {"onnxruntime"}, 0, "Models Backend: ");

        return edit_panel;
    }

    void FacesDetectionPlugin::edit(const cv::Mat& input, cv::Mat& output, const cvp::ParamsDict& params) {

        std::string graph_name     = std::any_cast<QString>(params.at("graph_name")).toStdString();
        std::string backend_str    = std::any_cast<QString>(params.at("backend")).toStdString();
        std::string resources_path = "/Users/iharkhakholka/Documents/Projects/ComputerVisionPack/data";

        ModelBackend backend = ModelBackend::ONNX_RUNTIME;
        if (backend_str == "onnxruntime") {
            backend = ModelBackend::ONNX_RUNTIME;
        }

        //        _model.reset( _models_factory->getDetector(graph_name, resources_path, backend) );
        _model = _models_factory->getDetector(graph_name, resources_path, backend);
        _model->init();

        BoxesItem output_boxes;
        _model->forward(&input, 1, &output_boxes);

        std::cout << "SOmething happened!";
    }

}// namespace cvp::gui
