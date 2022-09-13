//
// Created by Игорь Хохолко on 11.09.22.
//

#include "faces_detection_plugin.h"

#include <memory>


namespace cvp::gui {

    FacesDetectionPlugin::FacesDetectionPlugin(QObject* parent)
        : QObject(parent)
        , _prev_backend_str("")
        , _prev_graph_name("")
        , _detection_alg(nullptr)
        , _drawer(1, {"Face"}) {//FIXME: Get Number of classes and classes from detector itself.
        _models_factory = std::make_unique<ModelsFactory>();
        _resources_path = (QApplication::instance()->applicationDirPath() + "/../../data").toStdString();
    }

    QString FacesDetectionPlugin::name() {
        return "FacesDetection";
    }

    EditPanel* FacesDetectionPlugin::createEditPanel(QWidget* parent) {

        auto* edit_panel = new EditPanel(parent);

        edit_panel->addComboBox("graph_name", {"retina_resnet50_sml_480x640"}, 0, "Model Name: ");
        edit_panel->addComboBox("backend", {"onnxruntime"}, 0, "Models Backend: ");
        edit_panel->addDoubleSpinBox("threshold_conf", 0, 1, 0.9, 0.01, "Confidence Threshold: ");
        edit_panel->addDoubleSpinBox("threshold_nms", 0, 1, 0.9, 0.01, "IoU Threshold (NMS): ");
        edit_panel->addCheckBox("smart_pad_resize", true, "Use SmartPadResize: ");

        return edit_panel;
    }

    void FacesDetectionPlugin::edit(const cv::Mat& input, cv::Mat& output, const cvp::ParamsDict& params) {

        // Parse params
        std::string graph_name       = std::any_cast<QString>(params.at("graph_name")).toStdString();
        std::string backend_str      = std::any_cast<QString>(params.at("backend")).toStdString();
        float       threshold_conf   = (float) std::any_cast<double>(params.at("threshold_conf"));
        float       threshold_nms    = (float) std::any_cast<double>(params.at("threshold_nms"));
        bool        smart_pad_resize = std::any_cast<bool>(params.at("smart_pad_resize"));

        ModelBackend backend = ModelBackend::ONNX_RUNTIME;
        if (backend_str == "onnxruntime") {
            backend = ModelBackend::ONNX_RUNTIME;
        }

        // Set Up model and params
        bool core_changes_happened = (graph_name != _prev_graph_name || backend_str != _prev_backend_str);

        if (_detection_alg && core_changes_happened) {
            _detection_alg->reset(graph_name, _resources_path, backend, threshold_conf, threshold_nms, smart_pad_resize);
        }
        if (_detection_alg == nullptr) {
            _detection_alg = std::make_unique<DetectionAlg>(_models_factory, graph_name, _resources_path, backend, threshold_conf, threshold_nms, smart_pad_resize);
            _detection_alg->init();
        }

        _detection_alg->setConfThreshold(threshold_conf);
        _detection_alg->setSmartPadResize(smart_pad_resize);
        _detection_alg->setNmsIouThreshold(threshold_nms);

        // Infer model
        Boxes boxes = _detection_alg->process(input);
        for (auto& box: boxes){
            box.setLabel("Face"); // FIXME: get info from detector.
        }

        // Show
        output = input;
        _drawer.draw(output, boxes);
    }

}// namespace cvp::gui
