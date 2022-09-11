//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_SMART_PAD_RESIZE_H
#define COMPUTERVISIONPACK_SMART_PAD_RESIZE_H

#include <functional>

#include "opencv2/imgproc.hpp"


namespace cvp {


    class SmartPadResize {
    public:
        /*!
    * \fn  smartPadResize(const cv::Mat& src, cv::Mat& dst, const cv::Size& target_size, int border_type, int interpolation = cv::INTER_LINEAR,
    *                          size_t pad_height = 0, size_t pad_width = 0, const cv::Scalar& value = cv::Scalar())
    *
    * \brief Resize image with proportions saving
    *
    *
    * \param [in] src - input image.
    * \param [out] dst - output image.
    * \param [in] target_size - target_size.
    * \param [in] border_type - cv border type.
    * \param [in] interpolation - cv interpolation.
    * \param [in] pad - carefully resized image will be additionally padded from side with yet no paddin by (pad_height/2).
    *                   And then proportionally from another side to keep aspect ratio.
    * \param [in] value - Padding value if border_type==BORDER_CONSTANT.
    *
    * \return function for mapping points from resized image to original one. Arguments have to be past in format (float x, float y), where x,y are scaled to be in image (px) coordinates.
    */
        static std::function<std::tuple<float, float>(float, float)> resize(const cv::Mat& src, cv::Mat& dst, const cv::Size& target_size, int border_type,
                                                                            int interpolation = cv::INTER_LINEAR, size_t pad = 0, const cv::Scalar& value = cv::Scalar());


        static std::tuple<size_t, size_t, size_t, size_t, size_t, size_t, float> _baseResize(const cv::Size& input_size, const cv::Size& target_size, size_t pad);

        static std::function<std::tuple<float, float>(float, float)> _getBackwardPointsMapping(size_t x0_dash, size_t y0_dash, float c);
    };


}


#endif//COMPUTERVISIONPACK_SMART_PAD_RESIZE_H
