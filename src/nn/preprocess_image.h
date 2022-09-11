//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_PREPROCESS_IMAGE_H
#define COMPUTERVISIONPACK_PREPROCESS_IMAGE_H

#include <functional>

#include "opencv2/imgproc.hpp"

#include "common.h"

namespace cvp {

    inline void preprocessImage(const cv::Mat* images, size_t size, TensorFormatType format, const Floats& mean, const Floats& std, float* output) {
        using namespace cv;

        int C = images[0].channels(),
            H = images[0].rows,
            W = images[0].cols;

        std::function<int(int, int, int, int)> offset;


        switch (format) {
            case TensorFormatType::NCHW: {
                offset = [C, H, W](int n, int c, int h, int w) { return n * (C * H * W) + c * (H * W) + h * W + w; };
                break;
            }
            case TensorFormatType::NHWC: {
                offset = [H, W, C](int n, int c, int h, int w) { return n * (H * W * C) + h * (W * C) + w * C + c; };
                break;
            }
            default:
                throw std::invalid_argument("Not supported <TensorFormatType> passed!");
        }

        for (int n = 0; n < size; n++) {
            const cv::Mat& I = *(images+n);
            // accept only char type matrices
            CV_Assert(I.depth() == CV_8U);

            const int channels = I.channels();
            switch (channels) {
                case 1: {
                    for (int i = 0; i < I.rows; ++i) {
                        for (int j = 0; j < I.cols; ++j) {
                            *(output + offset(n, 0, i, j)) = ((float) I.at<uchar>(i, j) - mean[0]) / std[0];
                        }
                    }
                    break;
                }
                case 3: {
                    Mat_<Vec3b> _I = I;
                    for (int i = 0; i < I.rows; ++i) {
                        for (int j = 0; j < I.cols; ++j) {
                            *(output + offset(n, 0, i, j)) = ((float) _I(i, j)[0] - mean[0]) / std[0];
                            *(output + offset(n, 1, i, j)) = ((float) _I(i, j)[1] - mean[1]) / std[1];
                            *(output + offset(n, 2, i, j)) = ((float) _I(i, j)[2] - mean[2]) / std[2];
                        }
                    }
                }
            }
        }
    }


    inline void preprocessImage(const std::vector<cv::Mat>& images, TensorFormatType format, const Floats& mean, const Floats& std, float* output) {
        preprocessImage(images.data(), images.size(), format, mean, std, output);
    }


}// namespace cvp

#endif//COMPUTERVISIONPACK_PREPROCESS_IMAGE_H
