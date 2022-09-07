#ifndef EDITOR_PLUGIN_INTERFACE_H
#define EDITOR_PLUGIN_INTERFACE_H

#include "opencv2/opencv.hpp"
#include <QDialog>
#include <QObject>
#include <QString>

#include "common/common.h"
#include "gui/edit_panel.h"


class EditorPluginInterface {
public:
    virtual ~EditorPluginInterface(){};
    virtual QString              name()                                                                    = 0;
    virtual void                 edit(const cv::Mat& input, cv::Mat& output, const cvp::ParamsDict& params) = 0;
    virtual cvp::gui::EditPanel* createEditPanel(QWidget* parent = 0)                                      = 0;
};


#define EDIT_PLUGIN_INTERFACE_IID "com.kdr2.editorplugininterface"
Q_DECLARE_INTERFACE(EditorPluginInterface, EDIT_PLUGIN_INTERFACE_IID);


#endif