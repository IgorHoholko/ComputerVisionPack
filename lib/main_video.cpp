#include <QApplication>

#include "gui/video_viewer.h"

using namespace cvp;
using namespace cvp::gui;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    VideoViewer window;
    window.setWindowTitle("video_viewer");
    window.show();
    return app.exec();
}
