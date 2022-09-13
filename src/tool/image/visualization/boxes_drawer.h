//
// Created by Игорь Хохолко on 12.09.22.
//

#ifndef COMPUTERVISIONPACK_BOXES_DRAWER_H
#define COMPUTERVISIONPACK_BOXES_DRAWER_H

#include "string"
#include <iomanip>
#include <sstream>
#include <tuple>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "color_map.h"
#include "common.h"
#include "entities/box.h"


namespace cvp {


    const float FONT_SCALE = 0.9;
    const int   FONT_FACE  = cv::FONT_HERSHEY_SIMPLEX;
    const int   THICKNESS  = 1;
    const cv::Scalar WHITE(255, 255, 255);


    class BoxesDrawer {
    public:
        BoxesDrawer(int num_classes, const std::vector<std::string>& labels);

        void draw(cv::Mat& img, const std::vector<Floats>& boxes, Floats confidences, std::vector<std::string> labels, std::vector<Floats> landmarks_list) const;
        void draw(cv::Mat& img, const Boxes& boxes) const;

        static void draw_item(cv::Mat& img, const cv::Rect& rect, float conf, const std::string& label, const std::vector<cv::Point2f>& landmarks, const Color& color);

        static void drawLabel(cv::Mat& input_image, const std::string& label, const Color& color, int left, int top);

    private:

        ColorMap _colors;
    };
}// namespace cvp


#endif//COMPUTERVISIONPACK_BOXES_DRAWER_H
