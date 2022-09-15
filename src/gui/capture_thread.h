//
// Created by Игорь Хохолко on 15.09.22.
//

#ifndef COMPUTERVISIONPACK_CAPTURE_THREAD_H
#define COMPUTERVISIONPACK_CAPTURE_THREAD_H

#include "opencv2/opencv.hpp"
#include <QMutex>
#include <QThread>

namespace cvp::gui {

    class CaptureThread : public QThread {
        Q_OBJECT
    public:
        CaptureThread(int camera, QMutex* lock);
        CaptureThread(QString videoPath, QMutex* lock);
        ~CaptureThread();
        void setRunning(bool run) { _running = run; };

    protected:
        void run() override;

    signals:
        void frameCaptured(cv::Mat* data);

    private:
        bool    _running;
        int     _camera_id;
        QString _video_path;
        QMutex* _data_lock;
        cv::Mat _frame;
    };
}// namespace cvp::gui


#endif//COMPUTERVISIONPACK_CAPTURE_THREAD_H
