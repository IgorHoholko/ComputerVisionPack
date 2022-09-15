//
// Created by Игорь Хохолко on 13.09.22.
//

#include "video_viewer.h"


namespace cvp::gui {
    VideoViewer::VideoViewer(QWidget* parent)
        : QMainWindow(parent)
        , _capturer(nullptr) {
        _initUI();
        _data_lock = new QMutex();
    }


    VideoViewer::~VideoViewer() {}

    void VideoViewer::_initUI() {
        this->resize(1000, 800);
        // setup menubar
        _file_menu = menuBar()->addMenu("&File");


        // Create Video Area
        QGridLayout* main_layout = new QGridLayout();
        _image_scene             = new QGraphicsScene(this);
        _image_view              = new QGraphicsView(_image_scene);
        main_layout->addWidget(_image_view, 0, 0, 12, 1);

        // Create "Video Player" buttons
        QGridLayout* tools_layout = new QGridLayout();
        main_layout->addLayout(tools_layout, 12, 0, 1, 1);

        _monitor_check_box = new QCheckBox(this);
        _monitor_check_box->setText("Monitor On/Off");
        tools_layout->addWidget(_monitor_check_box, 0, 0);

        _record_button = new QPushButton(this);
        _record_button->setText("Record");
        tools_layout->addWidget(_record_button, 0, 1, Qt::AlignHCenter);
        tools_layout->addWidget(new QLabel(this), 0, 2);// empty placeholder

        // Create List of saved videos
        _saved_list = new QListView(this);
        main_layout->addWidget(_saved_list, 13, 0, 4, 1);

        // Setup main window layout
        QWidget* widget = new QWidget();
        widget->setLayout(main_layout);
        setCentralWidget(widget);

        // Setup status bar
        _main_status_bar   = statusBar();
        _main_status_label = new QLabel(_main_status_bar);
        _main_status_bar->addPermanentWidget(_main_status_label);
        _main_status_label->setText("Gazer is Ready");

        _createActions();
    }

    void VideoViewer::_createActions() {

        _camera_info_action = new QAction("Show Camera Info", this);
        _file_menu->addAction(_camera_info_action);
        connect(_camera_info_action, SIGNAL(triggered(bool)), this, SLOT(_showCameraInfo()));

        _open_camera_action = new QAction("Open Camera", this);
        _file_menu->addAction(_open_camera_action);
        connect(_open_camera_action, SIGNAL(triggered(bool)), this, SLOT(_openCamera()));

        _setUpShortcuts();
    }


    void VideoViewer::_setUpShortcuts() {
    }


    void VideoViewer::_setNewPixmap(const QPixmap& pixmap) {
        _image_scene->clear();
        _image_view->resetTransform();

        _image_scene->addPixmap(pixmap);

        _image_scene->update();
        _image_view->setSceneRect(_image_scene->sceneRect());
        _image_view->fitInView(_image_scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);
    }
    void VideoViewer::_showCameraInfo() {

        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        QString                    info    = QString("Available Cameras: \n");
        for (const QCameraDevice& cameraDevice : cameras) {
            info += " - " + cameraDevice.description() + ": ";
            //            if (cameraDevice.description() == "mycamera")
            //                camera = new QCamera(cameraDevice);
        }

        //        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();


        //        foreach (const QCameraInfo &cameraInfo, cameras) {
        //            info += " - " + cameraInfo.deviceName() + ": ";
        //            info += cameraInfo.description() + "\n";
        //        }
        QMessageBox::information(this, "Cameras", info);
    }

    void VideoViewer::_openCamera() {
        if (_capturer != nullptr) {
            // if a thread is already running, stop it
            _capturer->setRunning(false);
            disconnect(_capturer, &CaptureThread::frameCaptured, this, &VideoViewer::_updateFrame);
            connect(_capturer, &CaptureThread::finished, _capturer, &CaptureThread::deleteLater);
        }
        int camera_id = 0;
        _capturer     = new CaptureThread(camera_id, _data_lock);
        connect(_capturer, &CaptureThread::frameCaptured, this, &VideoViewer::_updateFrame);
        _capturer->start();
        _main_status_label->setText(QString("Capturing Camera %1").arg(camera_id));
    }

    void VideoViewer::_updateFrame(cv::Mat* mat) {
        _data_lock->lock();
        _current_frame = *mat;
        _data_lock->unlock();

        _setNewPixmap(cvMatToQPixmap(_current_frame));
    }


}// namespace cvp::gui