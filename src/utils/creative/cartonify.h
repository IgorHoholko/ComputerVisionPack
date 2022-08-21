
#ifndef IMAGE_PROCESSING_CARTONIFY_CPP
#define IMAGE_PROCESSING_CARTONIFY_CPP


#include "opencv2/opencv.hpp"

#include "tool/filter/fast_repeat_bilateral.h"

namespace CVP::creative {

    /**
         * Get an BGR image for input and transform it to cartoon image
         * @param [in] inp input image in BGR format
         * @param [out] dst output image in BGR format
         * @param [out] dst_edges output edges image in BGR format
         * @param [out] dst_cartoon output cartoon image in BGR format
         * @param [in] median_filter_size initial smoothing param
         * @param [in] laplacian_filter_size edges detection param
         * @param [in] edges_threshold apply threshold to edges after Laplacian Filter
         */
    inline void cartonify(const cv::Mat& inp, cv::Mat* dst, cv::Mat* dst_edges = nullptr, cv::Mat* dst_cartoon = nullptr,
                          size_t median_filter_size = 7, size_t laplacian_filter_size = 5, size_t edges_threshold = 80) {

        cv::Mat gray;
        cv::cvtColor(inp, gray, cv::COLOR_BGR2GRAY);

        cv::medianBlur(gray, gray, (int) median_filter_size);

        cv::Mat edges;
        cv::Laplacian(gray, edges, CV_8U, (int) laplacian_filter_size);

        cv::Mat mask;
        cv::threshold(edges, mask, edges_threshold, 255, cv::THRESH_BINARY_INV);

        cv::Mat cartoon;
        filter::fastRepeatBilateralFilter(inp, &cartoon);

        // form output
        if (dst->size != inp.size) {
            *dst = cv::Mat::zeros(inp.rows, inp.cols, inp.type());
        } else {
            dst->setTo(0);
        }
        cartoon.copyTo(*dst, mask);

        if (dst_edges != nullptr) { *dst_edges = std::move(edges); }
        if (dst_cartoon != nullptr) { *dst_cartoon = std::move(cartoon); }
    }
}// namespace CVP::creative

#endif//IMAGE_PROCESSING_CARTONIFY_CPP