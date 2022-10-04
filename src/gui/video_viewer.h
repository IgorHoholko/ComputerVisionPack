//
// Created by Игорь Хохолко on 13.09.22.
//

#ifndef COMPUTERVISIONPACK_VIDEO_VIEWER_H
#define COMPUTERVISIONPACK_VIDEO_VIEWER_H


#include <vector>

#include "QPluginLoader"
#include <QAction>
#include <QApplication>
#include <QCheckBox>
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
#include <QPushButton>
#include <QRegExp>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QToolBar>


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

        void _chooseSaveFolder();


        // UI
        QMenu* _file_menu;

        QGraphicsScene* _image_scene;
        QGraphicsView*  _image_view;

        QPushButton* _record_button;

        QStatusBar* _main_status_bar;
        QLabel*     _main_status_label;

        // Actions
        QAction* _open_camera_action;
        QAction* _open_video_action;
        QAction* _exit_action;

        // Core
        cv::Mat _current_frame;

        // for capture thread
        QMutex*        _data_lock;
        CaptureThread* _capturer;
        std::string    _save_path;


    private slots:
        void _openCamera();
        void _openVideo();
        void _updateFrame(cv::Mat*);
        void _recordingStartStop();
    };

}// namespace cvp::gui


#endif//COMPUTERVISIONPACK_VIDEO_VIEWER_H
