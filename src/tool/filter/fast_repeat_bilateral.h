//
// Created by Игорь Хохолко on 21.08.22.
//

#ifndef COMPUTERVISIONPACK_FAST_REPEAT_BILATERAL_H
#define COMPUTERVISIONPACK_FAST_REPEAT_BILATERAL_H

#include "opencv2/opencv.hpp"


namespace CVP::filter{

    /// Bilateral Filter.
    /// \param [in] inp input image in BGR format
    /// \param [out] dst output image in BGR format
    /// \param [in] rescale_factor how much we compress the input image for further processing
    /// \param [in] kernel_size diameter of each pixel neighborhood that is used during filtering.
    ///                         If it is non-positive, it is computed from sigmaSpace.
    /// \param [in] sigma_color filter sigma in the color space. A larger value of the parameter means
    ///                         that farther colors within the pixel neighborhood (see sigmaSpace) will
    ///                         be mixed together, resulting in larger areas of semi-equal color.
    /// \param [in] sigma_space filter sigma in the coordinate space. A larger value of the parameter means
    ///                         that farther pixels will influence each other as long as their colors are close enough (see sigmaColor ).
    ///                         When d>0, it specifies the neighborhood size regardless of sigmaSpace. Otherwise, d is proportional to sigmaSpace
    /// \param [in] repetitions How many times apply the filter. Can lead to strong cartoon effect.
    void fastRepeatBilateralFilter(const cv::Mat& inp, cv::Mat* dst,  size_t rescale_factor = 2,  int kernel_size = 9,  double sigma_color = 9.,
                                   double sigma_space = 7., size_t repetitions = 7) {

        using namespace cv;

        Size size = inp.size();
        Size small_size(size.width / rescale_factor, size.height / rescale_factor);

        // get small image copy for further processing
        Mat small_img = Mat(small_size, CV_8UC3);
        resize(inp, small_img, small_size, 0, 0, INTER_LINEAR);

        // apply filter
        Mat tmp         = Mat(small_size, CV_8UC3);
        for (int i = 0; i < repetitions; i++) {
            bilateralFilter(small_img, tmp, kernel_size, sigma_color, sigma_space);
            bilateralFilter(tmp, small_img, kernel_size, sigma_color, sigma_space);
        }

        // transform result to original size
        resize(small_img, *dst, size, 0,0, INTER_LINEAR);
    }
}


#endif//COMPUTERVISIONPACK_FAST_REPEAT_BILATERAL_H
