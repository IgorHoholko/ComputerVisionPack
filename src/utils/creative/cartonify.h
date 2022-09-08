
#ifndef IMAGE_PROCESSING_CARTONIFY_CPP
#define IMAGE_PROCESSING_CARTONIFY_CPP


#include "opencv2/opencv.hpp"

#include "tool/filter/fast_repeat_bilateral.h"

namespace cvp::creative {

    int constexpr CARTONIFY_REPETITIONS_MIN     = 1;
    int constexpr CARTONIFY_REPETITIONS_MAX     = 15;
    int constexpr CARTONIFY_REPETITIONS_DEFAULT = 3;

    int constexpr CARTONIFY_RESCALE_FACTOR_MIN     = 1;
    int constexpr CARTONIFY_RESCALE_FACTOR_MAX     = 6;
    int constexpr CARTONIFY_RESCALE_FACTOR_DEFAULT = 2;

    int constexpr CARTONIFY_MEDIAN_FILTER_SIZE_MIN     = 3;
    int constexpr CARTONIFY_MEDIAN_FILTER_SIZE_MAX     = 21;
    int constexpr CARTONIFY_MEDIAN_FILTER_SIZE_DEFAULT = 7;

    int constexpr CARTONIFY_LAPLACIAN_FILTER_SIZE_MIN     = 3;
    int constexpr CARTONIFY_LAPLACIAN_FILTER_SIZE_MAX     = 21;
    int constexpr CARTONIFY_LAPLACIAN_FILTER_SIZE_DEFAULT = 5;

    int constexpr CARTONIFY_BILATERAL_KETNEL_SIZE_MIN     = 3;
    int constexpr CARTONIFY_BILATERAL_KETNEL_SIZE_MAX     = 33;
    int constexpr CARTONIFY_BILATERAL_KETNEL_SIZE_DEFAULT = 9;

    int constexpr CARTONIFY_EDGES_THRESHOLD_MIN     = 5;
    int constexpr CARTONIFY_EDGES_THRESHOLD_MAX     = 255;
    int constexpr CARTONIFY_EDGES_THRESHOLD_DEFAULT = 100;

    double constexpr CARTONIFY_SIGMA_SPACE_MIN     = 3.;
    double constexpr CARTONIFY_SIGMA_SPACE_MAX     = 35.;
    double constexpr CARTONIFY_SIGMA_SPACE_DEFAULT = 7.;

    double constexpr CARTONIFY_SIGMA_COLOR_MIN     = 3.;
    double constexpr CARTONIFY_SIGMA_COLOR_MAX     = 100.;
    double constexpr CARTONIFY_SIGMA_COLOR_DEFAULT = 9.;


    struct CartoonSettings {
        // initial smoothing param
        int median_filter_size = CARTONIFY_MEDIAN_FILTER_SIZE_DEFAULT;

        // edges detection param
        int laplacian_filter_size = CARTONIFY_LAPLACIAN_FILTER_SIZE_DEFAULT;

        // apply threshold to edges after Laplacian Filter
        float edges_threshold = CARTONIFY_EDGES_THRESHOLD_DEFAULT;

        // how much we compress the input image for further processing
        int rescale_factor = CARTONIFY_RESCALE_FACTOR_DEFAULT;

        // diameter of each pixel neighborhood that is used during filtering.
        // If it is non-positive, it is computed from sigmaSpace.
        int bilateral_kernel_size = CARTONIFY_BILATERAL_KETNEL_SIZE_DEFAULT;

        // filter sigma in the color space. A larger value of the parameter means
        // that farther colors within the pixel neighborhood (see sigmaSpace) will
        // be mixed together, resulting in larger areas of semi-equal color.
        double sigma_color = CARTONIFY_SIGMA_COLOR_DEFAULT;

        // filter sigma in the coordinate space. A larger value of the parameter means
        // that farther pixels will influence each other as long as their colors are close enough (see sigmaColor ).
        // When d>0, it specifies the neighborhood size regardless of sigmaSpace. Otherwise, d is proportional to sigmaSpace
        double sigma_space = CARTONIFY_SIGMA_SPACE_DEFAULT;

        // how many times apply the filter. Can lead to strong cartoon effect.
        int repetitions = CARTONIFY_REPETITIONS_DEFAULT;
    };

    /**
         * Get an BGR image for input and transform it to cartoon image
         * @param [in] inp input image in BGR format
         * @param [out] dst output image in BGR format
         * @param [out] dst_edges output edges image in BGR format
         * @param [out] dst_cartoon_raw output cartoon image in BGR format
         */
    inline void cartonify(const cv::Mat& inp, cv::Mat* dst, const CartoonSettings& settings,
                          cv::Mat* dst_edges = nullptr, cv::Mat* dst_cartoon_raw = nullptr) {

        cv::Mat gray;
        cv::cvtColor(inp, gray, cv::COLOR_BGR2GRAY);

        cv::medianBlur(gray, gray, (int) settings.median_filter_size);

        cv::Mat edges;
        cv::Laplacian(gray, edges, CV_8U, (int) settings.laplacian_filter_size);

        cv::Mat mask;
        cv::threshold(edges, mask, settings.edges_threshold, 255, cv::THRESH_BINARY_INV);

        cv::Mat cartoon_raw;
        filter::fastRepeatBilateralFilter(inp, &cartoon_raw, settings.rescale_factor, settings.bilateral_kernel_size,
                                          settings.sigma_color, settings.sigma_space, settings.repetitions);

        // form output
        if (dst->size != inp.size) {
            *dst = cv::Mat::zeros(inp.rows, inp.cols, inp.type());
        } else {
            dst->setTo(0);
        }
        cartoon_raw.copyTo(*dst, mask);

        if (dst_edges != nullptr) { *dst_edges = std::move(edges); }
        if (dst_cartoon_raw != nullptr) { *dst_cartoon_raw = std::move(cartoon_raw); }
    }
}// namespace cvp::creative

#endif//IMAGE_PROCESSING_CARTONIFY_CPP