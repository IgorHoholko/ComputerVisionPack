//
// Created by Игорь Хохолко on 13.09.22.
//

#ifndef COMPUTERVISIONPACK_BOX_H
#define COMPUTERVISIONPACK_BOX_H

#include "opencv2/core/types.hpp"

namespace cvp {

    class Box {
    public:
        Box() = default;
        Box(const cv::Rect2i& rect, const std::vector<cv::Point2f>& landmarks, float score, float rotation_angle = 0, const std::string& label = "");
        Box(const std::vector<float>& box_vec, const std::vector<float>& landmarks, float score, float rotation_angle = 0, const std::string& label = "");

        void setScore(float score) {_score = score;};
        void setRotationAngle(float rotation_angle) { _rotation_angle = rotation_angle;};
        void setRect(const cv::Rect2i& rect);
        void setLandmarks(const std::vector<cv::Point2f>& landmarks) {_landmarks = landmarks; };
        void addLandmark(const cv::Point2f& landmark) { _landmarks.push_back(landmark); };
        void setLabel(const std::string& label){ _label = label; };


        float getScore() const {return _score;};
        float getRotationAngle() const {return _rotation_angle;};
        const cv::Rect2i& getRect() const {return _rect;};
        const cv::Point2i& getCenter() const {return _center;};
        const std::vector<cv::Point2f>& getLandmarks() const {return _landmarks; };
        const std::string& getLabel() const {return _label; }

        static cv::Rect2i               vector2rect(const std::vector<float>& box);
        static std::vector<cv::Point2f> vector2points(const std::vector<float>& landmarks);

        size_t width() const { return this->_rect.width; }
        size_t height() const { return this->_rect.height; }
        int    x1() const { return this->_rect.x; }
        int    x2() const { return this->_rect.x + this->_rect.width; }
        int    y1() const { return this->_rect.y; }
        int    y2() const { return this->_rect.y + this->_rect.height; }

        bool operator==(const Box& other) const;

    private:
        cv::Rect2i               _rect;
        cv::Point2i              _center;
        std::vector<cv::Point2f> _landmarks;
        float                    _score;

        float       _rotation_angle;
        std::string _label;
    };

    using Boxes = std::vector<Box>;

}// namespace cvp


#endif//COMPUTERVISIONPACK_BOX_H
