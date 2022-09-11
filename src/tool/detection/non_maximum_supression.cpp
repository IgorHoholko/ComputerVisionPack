//
// Created by Игорь Хохолко on 10.09.22.
//

#include "non_maximum_supression.h"


namespace cvp {


    void NonMaximumSuppression::sort(const vector<float>& x, vector<int>& indices) {
        for (unsigned int i = 0; i < x.size(); i++) {
            for (unsigned int j = i + 1; j < x.size(); j++) {
                if (x[indices[j]] < x[indices[i]]) {
                    int tmp    = indices[i];
                    indices[i] = indices[j];
                    indices[j] = tmp;
                }
            }
        }
    }

    vector<int> NonMaximumSuppression::process(const vector<vector<float>>& detections, const vector<float>& scores, float thresh_iou) {
        int numBoxes = (int) detections.size();

        if (numBoxes <= 0)
            return {};

        vector<float> area(numBoxes);
        vector<int>   indices(numBoxes);

        for (int i = 0; i < numBoxes; i++) {
            indices[i] = i;
            float x1   = detections[i][0];
            float y1   = detections[i][1];
            float x2   = detections[i][2];
            float y2   = detections[i][3];
            area[i]    = (x2 - x1) * (y2 - y1);
        }

        // sort boxes by score
        NonMaximumSuppression::sort(scores, indices);
        vector<int> keep;
        vector<int> suppress;

        while (indices.size() > 0) {

            // The index of largest confidence score
            int  index             = indices.back();
            auto indices_place_ptr = indices.begin();

            keep.push_back(index);

            // Compute coordinates of intersection-over-union(IOU)
            for (int k = 0; k < indices.size() - 1; k++) {
                int   i   = indices[k];
                float xx1 = max(detections[index][0], detections[i][0]);
                float yy1 = max(detections[index][1], detections[i][1]);
                float xx2 = min(detections[index][2], detections[i][2]);
                float yy2 = min(detections[index][3], detections[i][3]);

                float w = std::max(0.f, xx2 - xx1);
                float h = std::max(0.f, yy2 - yy1);

                float intersection = w * h;
                float ratio        = intersection / (area[index] + area[i] - intersection + 10e-9);

                if (ratio > thresh_iou) {
                    suppress.push_back(i);
                } else {
                    *indices_place_ptr++ = i;
                }
            }

            indices.erase(indices_place_ptr, indices.end());
        }

        return keep;
    }

}// namespace cvp