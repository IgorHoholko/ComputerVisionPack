//
// Created by Игорь Хохолко on 12.09.22.
//

#include "boxes_drawer.h"

#include <opencv2/core/core.hpp>

// Drawing shapes


namespace cvp {

    BoxesDrawer::BoxesDrawer(int num_classes, const std::vector<std::string>& labels)
        : _colors(num_classes, labels) {
    }

    void BoxesDrawer::draw(cv::Mat& img, const Boxes& boxes) const{
        for (const Box& box : boxes) {
            Color color = _colors[box.getLabel()];
            draw_item(img, box.getRect(), box.getScore(), box.getLabel(), box.getLandmarks(), color);
        }
    }

    void BoxesDrawer::draw(cv::Mat& img, const std::vector<Floats>& boxes, Floats confidences, std::vector<std::string> labels, std::vector<Floats> landmarks_list) const {
        for (int i = 0; i < boxes.size(); i++) {
            std::string   label     = labels[i];
            double        conf      = confidences[i];
            const Floats& box       = boxes[i];
            const Floats& landmarks = landmarks_list[i];
            Color         color     = _colors[label];

            int xmin, ymin, xmax, ymax;
            std::tie(xmin, ymin, xmax, ymax) = std::tie(box[0], box[1], box[2], box[3]);
            cv::Rect rect(xmin, ymin, xmax - xmin, ymax - ymin);

            draw_item(img, rect, conf, label, Box::vector2points(landmarks), color);
        }
    }

    void BoxesDrawer::draw_item(cv::Mat& img, const cv::Rect& rect, float conf, const std::string& label, const std::vector<cv::Point2f>& landmarks, const Color& color) {

        std::string text = cv::format("%.2f", conf);
        text             = label + " : " + text;

        cv::rectangle(img, rect.tl(), rect.br(), color, 3 * THICKNESS);
        BoxesDrawer::drawLabel(img, text, color, rect.x, rect.y);
    }

    void BoxesDrawer::drawLabel(cv::Mat& input_image, const std::string& label, const Color& color, int left, int top) {
        // Display the label at the top of the bounding box.
        int      base_line;
        cv::Size label_size = cv::getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &base_line);

        top           = std::max(top, label_size.height);
        cv::Point tlc = cv::Point(left, top);
        cv::Point brc = cv::Point(left + label_size.width, top + label_size.height + base_line);

        cv::rectangle(input_image, tlc, brc, color, cv::FILLED);
        putText(input_image, label, cv::Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, WHITE, THICKNESS);
    }

}// namespace cvp