//
// Created by Игорь Хохолко on 10.09.22.
//

#include "smart_pad_resize.h"


namespace cvp{

    std::function<std::tuple<float, float>(float, float)> SmartPadResize::_getBackwardPointsMapping(size_t x0_dash, size_t y0_dash, float c) {
        /**
         * Initially we have an image I1 [0, 0, x1, y1]. We fit it into I2 [0, 0, w2, h2] image center with aspect ratio keeping.
         * Fitted image I_dash after all has coordinates [left, top, w2 - right, h2 - bottom].
         * We want to estimate transformation P: I_dash -> I1.
         * The transformation matrix will be the composition of translation and scaling: P = A = T*S.
         * Because of we keep aspect ratio - scaling factor will be the same for both x and y coordinates.
         * So we have:
         *     [c, 0, Tx]
         * A = [0, c, Ty]
         *     [0, 0, 1 ]
         *
         * Take two points from I1 P1:(0,0), (x1, y1) and two from I_dash P_dash: (left, top), (w2 - right, h2 - bottom).
         * Estimate A from solving the equation: P1 = A*P_dash.
         * Beacause of c we already know from forward step - we don't need to solve eq. for other two points.
         * Find Tx and Ty.
         */

        float tx = -c * x0_dash;
        float ty = -c * y0_dash;

        return [c, tx, ty](float x, float y) {
            return std::tuple<float, float>{c * x + tx, c * y + ty};
        };
    }


    std::tuple<size_t, size_t, size_t, size_t, size_t, size_t, float> SmartPadResize::_baseResize(const cv::Size& input_size, const cv::Size& target_size, size_t pad) {

        size_t h1 = input_size.height,
               w1 = input_size.width,
               h2 = target_size.height,
               w2 = target_size.width;

        if (w1 == 0 || h1 == 0) {
            std::stringstream msg;
            msg << "Src image with Height or Width equals 0 has been passed into smartPadResize function!";
            throw std::invalid_argument(msg.str().c_str());
        }
        if (w2 == 0 || h2 == 0) {
            std::stringstream msg;
            msg << "Dst image with Height or Width equals 0 has been passed into smartPadResize function!";
            throw std::invalid_argument(msg.str().c_str());
        }

        float c1 = (float) h1 / (float) h2;
        float c2 = (float) w1 / (float) w2;
        float c  = std::max(c1, c2);

        std::size_t h_dash = (float) h1 / c;
        std::size_t w_dash = (float) w1 / c;

        h_dash = std::min(h_dash, h2);
        w_dash = std::min(w_dash, w2);

        //paddings
        size_t top    = (h2 - h_dash) / 2;
        size_t left   = (w2 - w_dash) / 2;
        size_t bottom = (h2 - h_dash) - top;
        size_t right  = (w2 - w_dash) - left;

        // Create vars for additional padding: if pad == 0 - nothing will change.
        size_t w_dash_dash = w_dash;
        size_t h_dash_dash = h_dash;
        float  c_dash      = 1.f;
        if (pad) {
            if (w_dash - w2 == 0) {//If resized touches sides
                w_dash_dash = w_dash - 2 * pad;
                h_dash_dash = h_dash - 2 * pad * h_dash / w_dash;
                size_t q    = (h_dash - h_dash_dash) / 2;

                left += pad;
                top += q;
            } else if (h_dash - h2 == 0) {//If resized touches top and bott.
                h_dash_dash = h_dash - 2 * pad;
                w_dash_dash = w_dash - 2 * pad * w_dash / h_dash;
                size_t q    = (w_dash - w_dash_dash) / 2;

                left += q;
                top += pad;
            }
            bottom = (h2 - h_dash_dash) - top;
            right  = (w2 - w_dash_dash) - left;

            c_dash = (float) w_dash / (float) w_dash_dash;
        }

        assert(h_dash_dash + top + bottom == h2);
        assert(w_dash_dash + left + right == w2);

        float cum_c = c * c_dash;

        return {w_dash_dash, h_dash_dash, top, bottom, left, right, cum_c};
    }


    std::function<std::tuple<float, float>(float, float)> SmartPadResize::resize(const cv::Mat& src, cv::Mat& dst, const cv::Size& target_size, int border_type,
                                                                                 int interpolation, size_t pad, const cv::Scalar& value) {

        size_t w_dash_dash, h_dash_dash, top, bottom, left, right;
        float  c;

        std::tie(w_dash_dash, h_dash_dash, top, bottom, left, right, c) = SmartPadResize::_baseResize({src.cols, src.rows}, {target_size.width, target_size.height}, pad);

        cv::Mat target_bar;
        cv::resize(src, target_bar, cv::Size(w_dash_dash, h_dash_dash), interpolation);

        cv::copyMakeBorder(target_bar, dst, top, bottom, left, right, border_type, value);

        return _getBackwardPointsMapping(left, top, c);
    }


}