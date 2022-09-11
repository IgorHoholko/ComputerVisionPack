//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_NON_MAXIMUM_SUPRESSION_H
#define COMPUTERVISIONPACK_NON_MAXIMUM_SUPRESSION_H

#include <vector>

namespace cvp {
    using namespace std;

    class NonMaximumSuppression {
    public:
        static void sort(const std::vector<float>& x, std::vector<int>& indices);

        static vector<int> process(const vector<vector<float>>& detections, const vector<float>& scores, float overlapThreshold);
    };
}// namespace cvp


#endif//COMPUTERVISIONPACK_NON_MAXIMUM_SUPRESSION_H
