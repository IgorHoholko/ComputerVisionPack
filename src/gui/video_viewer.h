//
// Created by Игорь Хохолко on 13.09.22.
//

#ifndef COMPUTERVISIONPACK_VIDEO_VIEWER_H
#define COMPUTERVISIONPACK_VIDEO_VIEWER_H


#include <vector>

#include "QPluginLoader"
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QMainWindow>
#include <QMap>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegExp>
#include <QStatusBar>
#include <QToolBar>
#include <QCheckBox>
#include <QPushButton>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QMediaDevices>

#include "capture_thread.h"
#include "tool/image/imgs_conversion.h"


namespace cvp::gui {
    class VideoViewer : public QMainWindow {
        Q_OBJECT

    public:
        explicit VideoViewer(QWidget* parent = nullptr);
        ~VideoViewer();

    private:
        void _initUI();
        void _createActions();

        void _setUpShortcuts();

        void _setNewPixmap(const QPixmap& pixmap);


        // UI
        QMenu* _file_menu;

        QGraphicsScene* _image_scene;
        QGraphicsView*  _image_view;

        QCheckBox*   _monitor_check_box;
        QPushButton* _record_button;

        QListView* _saved_list;

        QStatusBar* _main_status_bar;
        QLabel*     _main_status_label;

        // Actions
        QAction* _camera_info_action;
        QAction* _open_camera_action;
        QAction* _exit_action;

        // Core
        cv::Mat _current_frame;

        // for capture thread
        QMutex *_data_lock;
        CaptureThread *_capturer;


    private slots:
        void _showCameraInfo();
        void _openCamera();
        void _updateFrame(cv::Mat*);
    };

}// namespace cvp::gui


#endif//COMPUTERVISIONPACK_VIDEO_VIEWER_H
