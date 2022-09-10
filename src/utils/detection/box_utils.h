//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_BOX_UTILS_H
#define COMPUTERVISIONPACK_BOX_UTILS_H

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include <math.h>
#include <tuple>
#include <vector>

namespace cvp {
    namespace BoxUtils {
        /*
		 * Decode bounding box coordinates predicted by the model into center-size form.
		 * The encoding algorithm is the following:
		 *   1. For the center coordinates, find the offset with respect to the prior box, and scale by the size of the prior box
		 *   2. For the size coordinates, scale by the size of the prior box, and convert to the log-space
		 */
        inline void gcxgcy_to_cxcy(const float* gcxgcy, const float* prior, float* cxcy, float var_center = 0.1f, float var_size = 0.2f) {
            cxcy[0] = gcxgcy[0] * prior[2] * var_center + prior[0];
            cxcy[1] = gcxgcy[1] * prior[3] * var_center + prior[1];
            cxcy[2] = expf(gcxgcy[2] * var_size) * prior[2];
            cxcy[3] = expf(gcxgcy[3] * var_size) * prior[3];
        }

        inline void gcxgcy_to_cxcy(const float* gcxgcy, const float* priors, size_t nPriors, float* cxcy, float var_center = 0.1f, float var_size = 0.2f) {
            for (size_t i = 0; i < nPriors * 4; i += 4)
                gcxgcy_to_cxcy(gcxgcy + i, priors + i, cxcy + i);
        }

        inline void decode_landmarks(const float* predicted, const float* priors, float* landmarks, float var_center = 0.1f) {
            for (size_t i = 0; i < 5; ++i) {
                landmarks[2 * i]     = predicted[2 * i] * var_center * priors[2] + priors[0];
                landmarks[2 * i + 1] = predicted[2 * i + 1] * var_center * priors[3] + priors[1];
            }
        }

        /*
		 * Decode landm from predictions using priors to undo the encoding we did for offset regression at train time.
		 *   - predicted, landmarks: [nPriors, 10] - x,y for 5 points
		 *   - priors: [nPriors, 4]
		 */
        inline void decode_landmarks(const float* predicted, const float* priors, size_t nPriors, float* landmarks, float var_center = 0.1f) {
            for (size_t i = 0; i < nPriors; ++i)
                decode_landmarks(predicted + 10 * i, priors + 4 * i, landmarks + 10 * i, var_center);
        }

        /*
		 * Convert bounding boxes from center-size coordinates (c_x, c_y, w, h) to boundary coordinates (x_min, y_min, x_max, y_max)
		 */
        inline void cxcy_to_xy(const float* cxcy, float* xy) {
            xy[0] = cxcy[0] - cxcy[2] / 2;
            xy[1] = cxcy[1] - cxcy[3] / 2;
            xy[2] = cxcy[0] + cxcy[2] / 2;
            xy[3] = cxcy[1] + cxcy[3] / 2;
        }

        inline void cxcy_to_xy(const float* cxcy, size_t size, float* xy) {
            for (size_t i = 0; i < size * 4; i += 4)
                cxcy_to_xy(cxcy + i, xy + i);
        }

        template<class T>
        inline void clip(T& val, T min, T max) {
            val = val < min ? min : val;
            val = val > max ? max : val;
        }


        template<class T>
        inline bool validate_bbox(T* xy, T w, T h) {
            clip(xy[0], (T)0, w);
            clip(xy[2], (T)0, w);
            clip(xy[1], (T)0, h);
            clip(xy[3], (T)0, h);
            return (xy[0] < xy[2]) && (xy[1] < xy[3]);
        }

        template<class T>
        inline bool validate_bbox(T* xy){
            T min, max;
            min = std::min(xy[0], xy[2]);
            max = std::max(xy[0], xy[2]);
            xy[0] = min;
            xy[2] = max;

            min = std::min(xy[1], xy[3]);
            max = std::max(xy[1], xy[3]);
            xy[1] = min;
            xy[3] = max;
            return true;
        }

        /*
		 * Apply Affine Transform <A> [2x3] to <points>. Points must be passed in [x1, y1, x2, y2, ....] format.
		 */
        template<class T>
        inline void transform(std::vector<T>& points, const cv::Mat& A) {
            for (size_t i = 0; i < points.size() / 2; i++) {
                T x = points[2 * i], y = points[2 * i + 1];
                points[2 * i]     = (T)(A.at<double>(0, 0) * x + A.at<double>(0, 1) * y + A.at<double>(0, 2));
                points[2 * i + 1] = (T)(A.at<double>(1, 0) * x + A.at<double>(1, 1) * y + A.at<double>(1, 2));
            }
        }


        template<class T>
        inline std::tuple<T,T> rotate_point(T x, T y, T cos_, T sin_, T cx = 0, T cy = 0) {
            T x_ = cos_ * (x-cx) - sin_ * (y-cy) + cx;
            T y_ = sin_ * (x-cx) + cos_ * (y-cy) + cy;
            return std::tie(x_, y_);
        }

        template<class T>
        inline std::tuple<T,T> rotate_point(T x, T y, T theta, T cx = 0, T cy = 0) {
            T x_ = std::cos(theta) * (x-cx) - std::sin(theta) * (y-cy) + cx;
            T y_ = std::sin(theta) * (x-cx) + std::cos(theta) * (y-cy) + cy;
            return std::tie(x_, y_);
        }


        /*
		 * Clip x1, y1, x2, y2 coordinates.
		 */
        inline void clip_rect(cv::Rect& rect, size_t min_x, size_t min_y, size_t max_x, size_t max_y, bool keep_even = false) {
            cv::Rect r(cv::Point(std::max<ptrdiff_t>(min_x, rect.x), std::max<ptrdiff_t>(min_y, rect.y)),
                       cv::Point(std::min<ptrdiff_t>(max_x, rect.x + rect.width), std::min<ptrdiff_t>(max_y, rect.y + rect.height)));
            if (keep_even) {
                r.width -= r.width % 2;
                r.height -= r.height % 2;
            }
            rect = r;
        }

        /*
		 * Clip x1, y1 coordinates and keep width and height if possible (Move rect to be in the range).
		 */
        inline void clip_move_rect(cv::Rect& rect, size_t min_x, size_t min_y, size_t max_x, size_t max_y, bool keep_even = false) {
            // move from left side
            rect.x = std::max<ptrdiff_t>(min_x, rect.x);
            rect.y = std::max<ptrdiff_t>(min_y, rect.y);
            // move from right side
            if (rect.x + rect.width > max_x)
                rect.x = max_x - rect.width;
            if (rect.y + rect.height > max_y)
                rect.y = max_y - rect.height;

            BoxUtils::clip_rect(rect, min_x, min_y, max_x, max_y, keep_even);
        }



    }// namespace BoxUtils
}// namespace fr

#endif//COMPUTERVISIONPACK_BOX_UTILS_H
