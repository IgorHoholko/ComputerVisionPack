#include "erode_plugin.h"


namespace cvp::gui {

    QString ErodePlugin::name() {
        return "Erode";
    }

    void ErodePlugin::edit(const cv::Mat& input, cv::Mat& output, const cvp::ParamsDict& params) {
        erode(input, output, cv::Mat());
    }

    EditPanel* ErodePlugin::createEditPanel(QWidget* parent){
        auto* edit_panel = new EditPanel(parent);
        edit_panel->addSlider(Qt::Orientation::Horizontal, 0, 255, tr("My Desc"), "s");


        return edit_panel;
    }
}// namespace cvp::gui
