//
// Created by Игорь Хохолко on 11.09.22.
//

#ifndef COMPUTERVISIONPACK_FACES_DETECTOR_PLUGIN_H
#define COMPUTERVISIONPACK_FACES_DETECTOR_PLUGIN_H


#include <QApplication>
#include <QObject>
#include <QtPlugin>

#include "alg/detection_alg.h"
#include "dl_models/models_factory.h"
#include "editor_plugin_interface.h"
#include "tool/image/visualization/boxes_drawer.h"


namespace cvp::gui {

    class FacesDetectionPlugin : public QObject, public EditorPluginInterface {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID EDIT_PLUGIN_INTERFACE_IID);
        Q_INTERFACES(EditorPluginInterface);

    public:
        explicit FacesDetectionPlugin(QObject* parent = nullptr);
        QString    name() override;
        void       edit(const cv::Mat& input, cv::Mat& output, const cvp::ParamsDict& params) override;
        EditPanel* createEditPanel(QWidget* parent = 0) override;

    private:
        std::string _prev_graph_name;
        std::string _prev_backend_str;

        std::string _resources_path;

        std::unique_ptr<DetectionAlg> _detection_alg;
        ModelsFactoryPtr              _models_factory;

        BoxesDrawer _drawer;
    };
};// namespace cvp::gui

#endif//COMPUTERVISIONPACK_FACES_DETECTOR_PLUGIN_H
