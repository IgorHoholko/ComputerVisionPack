#include "erode_plugin.h"


namespace cvp::gui {

    QString ErodePlugin::name() {
        return "Erode";
    }

    void ErodePlugin::edit(const cv::Mat& input, cv::Mat& output, const cvp::ParamsDict& params) {
        erode(input, output, cv::Mat());
    }

    EditPanel* ErodePlugin::createEditPanel(QWidget* parent){
        return nullptr;
    }
}// namespace cvp::gui
