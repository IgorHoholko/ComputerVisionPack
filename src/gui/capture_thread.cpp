//
// Created by Игорь Хохолко on 15.09.22.
//

#include "capture_thread.h"

namespace cvp::gui {

    CaptureThread::CaptureThread(int camera, QMutex* lock, const std::string&  save_path)
        : _running(false)
        , _camera_id(camera)
        , _video_path("")
        , _data_lock(lock)
        , _frame_width(0)
        , _frame_height(0)
        , _video_saving_status(VideoSavingStatus::STOPPED)
        , _video_writer(nullptr)
        , _save_path(save_path){
    }

    CaptureThread::CaptureThread(const std::string& video_path, QMutex* lock, const std::string&  save_path)
        : _running(false)
        , _camera_id(-1)
        , _video_path(video_path)
        , _data_lock(lock)
        , _frame_width(0)
        , _frame_height(0)
        , _video_saving_status(VideoSavingStatus::STOPPED)
        , _video_writer(nullptr)
        , _save_path(save_path){
    }

    CaptureThread::~CaptureThread() {
    }

    void CaptureThread::run() {
        _running = true;

        cv::VideoCapture cap;
        if (_video_path.empty()){
            cap.open(_camera_id);
        }else{
            cap.open(_video_path);
        }

        _fps = cap.get(cv::CAP_PROP_FPS);
        _frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        _frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

        cv::Mat          tmp_frame;
        while (_running) {
            cap >> tmp_frame;
            if (tmp_frame.empty()) {
                break;
            }

            if(_video_saving_status == STARTING) {
                _startSavingVideo(tmp_frame);
            }
            if(_video_saving_status == STARTED) {
                _video_writer->write(tmp_frame);
            }
            if(_video_saving_status == STOPPING) {
                _stopSavingVideo();
            }

            _data_lock->lock();
            _frame = tmp_frame;
            _data_lock->unlock();
            emit _frameCaptured(&_frame);
        }
        cap.release();
        _running = false;
    }


     void CaptureThread::_startSavingVideo(const cv::Mat &first_frame)
    {
         _saved_video_name = QDateTime::currentDateTime().toString("yyyy-MM-dd+HH:mm:ss").toStdString();

        auto cover_folder = _save_path / "covers";
        auto video_folder =  _save_path / "videos";

        std::filesystem::create_directories(cover_folder);
        std::filesystem::create_directories(video_folder);

        auto cover_img_path = cover_folder / (_saved_video_name + ".jpg");
        auto video_path = video_folder / (_saved_video_name + ".avi");

        cv::imwrite(cover_img_path.string(), first_frame);

        _video_writer = new cv::VideoWriter(video_path.string(),
                                            cv::CAP_FFMPEG,
                                            cv::VideoWriter::fourcc('M','J','P','G'),
                                            _fps && !_video_path.empty() ? _fps : DEFAULT_FPS,
                                            cv::Size(_frame_width,_frame_height));

        _video_saving_status = STARTED;
    }


    void CaptureThread::_stopSavingVideo() {
        _video_saving_status = STOPPED;
        _video_writer->release();
        delete _video_writer;
        _video_writer = nullptr;
        emit _videoSaved(_saved_video_name);
    }


}// namespace cvp::gui