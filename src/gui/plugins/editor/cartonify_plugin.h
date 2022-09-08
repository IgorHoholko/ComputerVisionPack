//
// Created by Игорь Хохолко on 8.09.22.
//

#ifndef COMPUTERVISIONPACK_CARTONIFY_PLUGIN_H
#define COMPUTERVISIONPACK_CARTONIFY_PLUGIN_H


#include <QObject>
#include <QtPlugin>

#include "editor_plugin_interface.h"
#include "utils/creative/cartonify.h"


namespace cvp::gui {


    class CartonifyPlugin : public QObject, public EditorPluginInterface
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID EDIT_PLUGIN_INTERFACE_IID);
        Q_INTERFACES(EditorPluginInterface);
    public:

        QString name() override;
        void edit(const cv::Mat &input, cv::Mat &output, const cvp::ParamsDict& params) override;
        EditPanel* createEditPanel(QWidget* parent = 0) override;
    };
};// namespace cvp::gui


#endif//COMPUTERVISIONPACK_CARTONIFY_PLUGIN_H
