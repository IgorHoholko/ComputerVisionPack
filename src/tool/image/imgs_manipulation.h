//
// Created by Игорь Хохолко on 21.08.22.
//

#ifndef COMPUTERVISIONPACK_IMGS_MANIPULATION_H
#define COMPUTERVISIONPACK_IMGS_MANIPULATION_H

#include "opencv2/imgproc.hpp"

namespace cvp {

    inline cv::Mat stackImages(std::vector<cv::Mat> imgs, cv::Size cell_size) {

        int n           = imgs.size();
        int imgs_in_row = ceil(sqrt((float) n));
        int imgs_in_col = ceil((float) n / (float) imgs_in_row);

        int result_img_w = cell_size.width * imgs_in_row;
        int result_img_h = cell_size.height * imgs_in_col;

        cv::Mat result_img = cv::Mat::zeros(result_img_h, result_img_w, CV_8UC3);

        for (int idx = 0; idx < imgs.size(); idx++) {
            int i = idx / imgs_in_col;
            int j = idx % imgs_in_row;

            cv::Mat img;
            if (imgs[idx].channels() == 3) {
                img = imgs[idx];
            } else if (imgs[idx].channels() == 1) {
                cv::cvtColor(imgs[idx], img, cv::COLOR_GRAY2BGR);
            }

            int cell_row = i * cell_size.height;
            int cell_col = j * cell_size.width;

            img.copyTo(result_img(cv::Range(cell_row, cell_row + img.rows),
                                  cv::Range(cell_col, cell_col + img.cols)));
        }
        return result_img;
    }
}// namespace cvp


#endif//COMPUTERVISIONPACK_IMGS_MANIPULATION_H
