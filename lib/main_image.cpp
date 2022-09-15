#include <QApplication>

#include "gui/image_viewer.h"

using namespace cvp;
using namespace cvp::gui;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ImageViewer window;
    window.setWindowTitle("image_viewer");
    window.show();
    return app.exec();
}
