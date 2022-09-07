
#ifndef COMPUTERVISIONPACK_ERODE_PLUGIN_H
#define COMPUTERVISIONPACK_ERODE_PLUGIN_H


#include <QObject>
#include <QtPlugin>

#include "editor_plugin_interface.h"


namespace cvp::gui {



    class ErodePlugin: public QObject, public EditorPluginInterface
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


#endif//COMPUTERVISIONPACK_ERODE_PLUGIN_H
