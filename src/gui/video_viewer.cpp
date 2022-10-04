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

        QGridLayout* main_layout = new QGridLayout();

        // Create Video Area
        _image_scene             = new QGraphicsScene(this);
        _image_view              = new QGraphicsView(_image_scene);
        main_layout->addWidget(_image_view, 0, 0, 12, 1);

        // Create "Video Player" buttons
        QGridLayout* tools_layout = new QGridLayout();
        main_layout->addLayout(tools_layout, 12, 0, 1, 1);

        _record_button = new QPushButton(this);
        _record_button->setText("Record");
        tools_layout->addWidget(_record_button, 0, 1, Qt::AlignHCenter);
        tools_layout->addWidget(new QLabel(this), 0, 2);// empty placeholder
        connect(_record_button, &QPushButton::clicked, this, &VideoViewer::_recordingStartStop);

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

        _chooseSaveFolder();
    }

    void VideoViewer::_createActions() {

        _open_camera_action = new QAction("Open Camera", this);
        _file_menu->addAction(_open_camera_action);
        connect(_open_camera_action, SIGNAL(triggered(bool)), this, SLOT(_openCamera()));

        _open_video_action = new QAction("Open Video", this);
        _file_menu->addAction(_open_video_action);
        connect(_open_video_action, SIGNAL(triggered(bool)), this, SLOT(_openVideo()));

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

    void VideoViewer::_openCamera() {
        if (_capturer != nullptr) {
            // if a thread is already running, stop it
            _capturer->setRunning(false);
            disconnect(_capturer, &CaptureThread::_frameCaptured, this, &VideoViewer::_updateFrame);
            connect(_capturer, &CaptureThread::finished, _capturer, &CaptureThread::deleteLater);
        }
        int camera_id = 0;
        _capturer     = new CaptureThread(camera_id, _data_lock, _save_path);
        connect(_capturer, &CaptureThread::_frameCaptured, this, &VideoViewer::_updateFrame);
        _capturer->start();
        _main_status_label->setText(QString("Capturing Camera %1").arg(camera_id));
    }

    void VideoViewer::_openVideo() {
        QFileDialog dialog(this);
        dialog.setWindowTitle("Open Video");
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilter(tr("Images (*.avi *.mp4)"));
        QStringList filePaths;
        if (dialog.exec()) {
            filePaths = dialog.selectedFiles();
            _capturer     = new CaptureThread(filePaths.at(0).toStdString(), _data_lock, _save_path);
            connect(_capturer, &CaptureThread::_frameCaptured, this, &VideoViewer::_updateFrame);
            _capturer->start();
            _main_status_label->setText(QString("Capturing Video %1").arg(filePaths.at(0)));
        }
    }

    void VideoViewer::_updateFrame(cv::Mat* mat) {
        _data_lock->lock();
        _current_frame = *mat;
        _data_lock->unlock();

        _setNewPixmap(cvMatToQPixmap(_current_frame));
    }

    void VideoViewer::_chooseSaveFolder() {
        if (!_save_path.empty()) return ;
        _save_path = QFileDialog::getExistingDirectory(this, tr("Choose Save Folder")).toStdString();
    }

    void VideoViewer::_recordingStartStop() {
        QString text = _record_button->text();
        if(text == "Record" && _capturer) {
            _capturer->setVideoSavingStatus(VideoSavingStatus::STARTING);
            _record_button->setText("Stop Recording");
        } else if(text == "Stop Recording" && _capturer != nullptr) {
            _capturer->setVideoSavingStatus(VideoSavingStatus::STOPPING);
            _record_button->setText("Record");
        }
    }

}// namespace cvp::gui