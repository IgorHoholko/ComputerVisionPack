//
// Created by Игорь Хохолко on 13.09.22.
//

#include "box.h"

namespace cvp {


    Box::Box(const cv::Rect2i& rect, const std::vector<cv::Point2f>& landmarks, float score, float rotation_angle, const std::string& label)
        : _rect(rect)
        , _landmarks(landmarks)
        , _score(score)
        , _rotation_angle(rotation_angle)
        , _label(label)
        , _center(rect.x + rect.width / 2, rect.y + rect.height / 2) {
    }

    Box::Box(const std::vector<float>& box_vec, const std::vector<float>& landmarks, float score, float rotation_angle, const std::string& label)
        : _score(score)
        , _rotation_angle(rotation_angle)
        , _label(label) {
        _landmarks = Box::vector2points(landmarks);
        this->setRect(Box::vector2rect(box_vec));
    }

    void Box::setRect(const cv::Rect2i& rect) {
        this->_rect   = rect;
        this->_center = cv::Point2i(rect.x + rect.width / 2, rect.y + rect.height / 2);
    }

    cv::Rect2i Box::vector2rect(const std::vector<float>& box) {
        return {(int) box[0], (int) box[1],
                (int) (box[2] - box[0]),
                (int) (box[3] - box[1])};
    }

    std::vector<cv::Point2f> Box::vector2points(const std::vector<float>& landmarks) {
        std::vector<cv::Point2f> out;
        out.reserve(landmarks.size() / 2);
        for (size_t idx = 0; idx < landmarks.size() / 2; idx += 1) {
            out.emplace_back(landmarks[2 * idx], landmarks[2 * idx + 1]);
        }
        return out;
    }

    bool Box::operator==(const Box& other) const {
        return this->getCenter() == other.getCenter() &&
               this->getLandmarks() == other.getLandmarks() &&
               this->getRect() == other.getRect() &&
               this->getRotationAngle() == other.getRotationAngle() &&
               this->getScore() == other.getScore();
    };
}// namespace cvp
