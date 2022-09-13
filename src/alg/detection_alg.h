//
// Created by Игорь Хохолко on 13.09.22.
//

#ifndef COMPUTERVISIONPACK_DETECTIONALG_H
#define COMPUTERVISIONPACK_DETECTIONALG_H


#include "dl_models/models_factory.h"
#include "entities/box.h"
#include "i_alg.h"
#include "tool/detection/non_maximum_supression.h"


namespace cvp {

    class DetectionAlg : IAlg {
    public:

        DetectionAlg(ModelsFactoryPtr models_factory, const std::string& graph_name, const std::string& resources_path,
                     ModelBackend backend, float conf_treshold = -1.f, float nms_iou_treshold = 0.5f, bool smart_pad_resize = true);
        DetectionAlg(ModelsFactoryPtr models_factory);


        void init() override;
        void reset(const std::string& graph_name, const std::string& resources_path,
                   ModelBackend backend, float conf_treshold = -1.f, float nms_iou_treshold = 0.5f, bool smart_pad_resize = true);

        Boxes              process(const cv::Mat& input);
        std::vector<Boxes> process(const vector<cv::Mat>& input);
        std::vector<Boxes> process(const cv::Mat* input, size_t size);


        const std::string& getGraphName() const { return _graph_name; };
        const std::string& getResourcesPath() const { return _resources_path; };
        ModelBackend       getBackend() const { return _backend; };
        float              getConfThreshold() const { return _conf_threshold; };
        float              getNmsIouThreshold() const { return _nms_iou_threshold; };
        bool               getSmartPadResize() const { return _smart_pad_resize; }

        void setConfThreshold(float t);
        void getNmsIouThreshold(float t);
        void setSmartPadResize(bool allowed);


    private:
        bool _inited;

        Boxes _filter(const BoxesItem& boxes) const;

        std::string  _graph_name;
        std::string  _resources_path;
        ModelBackend _backend;
        float        _conf_threshold;
        float        _nms_iou_threshold;
        bool         _smart_pad_resize;

        ModelsFactoryPtr _models_factory;

        std::unique_ptr<BaseDetector> _detector;
    };

    using DetectionAlgPtr = std::shared_ptr<DetectionAlg>;
}// namespace cvp


#endif//COMPUTERVISIONPACK_DETECTIONALG_H
