//
// Created by Игорь Хохолко on 15.09.22.
//

#ifndef COMPUTERVISIONPACK_CAPTURE_THREAD_H
#define COMPUTERVISIONPACK_CAPTURE_THREAD_H

#include <string>

#include "opencv2/opencv.hpp"
#include <QDateTime>
#include <QMutex>
#include <QThread>
#include <filesystem>

namespace cvp::gui {

    constexpr size_t DEFAULT_FPS = 25;

    enum VideoSavingStatus {
        STARTING,
        STARTED,
        STOPPING,
        STOPPED
    };

    class CaptureThread : public QThread {
        Q_OBJECT
    public:
        CaptureThread(int camera, QMutex* lock, const std::string& save_path);
        CaptureThread(const std::string& video_path, QMutex* lock, const std::string& save_path);
        ~CaptureThread();
        void setRunning(bool run) { _running = run; };

        void              setVideoSavingStatus(VideoSavingStatus status) { _video_saving_status = status; };
        VideoSavingStatus getVideoSavingStatus() const { return _video_saving_status; }

    protected:
        void run() override;

    signals:
        void _frameCaptured(cv::Mat* data);
        void _videoSaved(std::string);


    private:
        void _startSavingVideo(const cv::Mat& first_frame);
        void _stopSavingVideo();

        bool        _running;
        int         _camera_id;
        std::string _video_path;
        QMutex*     _data_lock;
        cv::Mat     _frame;

        // video saving
        int                   _frame_width;
        int                   _frame_height;
        double                _fps;
        VideoSavingStatus     _video_saving_status;
        std::filesystem::path _save_path;
        std::string           _saved_video_name;
        cv::VideoWriter*      _video_writer;
    };
}// namespace cvp::gui


#endif//COMPUTERVISIONPACK_CAPTURE_THREAD_H
