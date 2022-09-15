//
// Created by Игорь Хохолко on 15.09.22.
//

#include "capture_thread.h"

namespace cvp::gui {

    CaptureThread::CaptureThread(int camera, QMutex* lock)
        : _running(false)
        , _camera_id(camera)
        , _video_path("")
        , _data_lock(lock) {
    }

    CaptureThread::CaptureThread(QString video_path, QMutex* lock)
        : _running(false)
        , _camera_id(-1)
        , _video_path(video_path)
        , _data_lock(lock) {
    }

    CaptureThread::~CaptureThread() {
    }

    void CaptureThread::run() {
        _running = true;
        cv::VideoCapture cap(_camera_id);
        cv::Mat          tmp_frame;
        while (_running) {
            cap >> tmp_frame;
            if (tmp_frame.empty()) {
                break;
            }

            _data_lock->lock();
            _frame = tmp_frame;
            _data_lock->unlock();
            emit frameCaptured(&_frame);
        }
        cap.release();
        _running = false;
    }

}// namespace cvp::gui