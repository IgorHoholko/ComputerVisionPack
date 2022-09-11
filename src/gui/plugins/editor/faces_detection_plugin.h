//
// Created by Игорь Хохолко on 11.09.22.
//

#ifndef COMPUTERVISIONPACK_FACES_DETECTOR_PLUGIN_H
#define COMPUTERVISIONPACK_FACES_DETECTOR_PLUGIN_H



#include <QObject>
#include <QtPlugin>

#include "editor_plugin_interface.h"
#include "dl_models/models_factory.h"


namespace cvp::gui {

    class FacesDetectionPlugin: public QObject, public EditorPluginInterface
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID EDIT_PLUGIN_INTERFACE_IID);
        Q_INTERFACES(EditorPluginInterface);
    public:
        explicit FacesDetectionPlugin(QObject *parent = nullptr);
        QString name() override;
        void edit(const cv::Mat &input, cv::Mat &output, const cvp::ParamsDict& params) override;
        EditPanel* createEditPanel(QWidget* parent = 0) override;

    private:

        std::shared_ptr<BaseDetector> _model_ptr;
        BaseDetector* _model;
        ModelsFactoryUPtr _models_factory;
    };
};// namespace cvp::gui

#endif//COMPUTERVISIONPACK_FACES_DETECTOR_PLUGIN_H
