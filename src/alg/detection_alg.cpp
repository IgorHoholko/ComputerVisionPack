//
// Created by Игорь Хохолко on 13.09.22.
//

#include "detection_alg.h"
namespace cvp {
    DetectionAlg::DetectionAlg(ModelsFactoryPtr models_factory, const std::string& graph_name, const std::string& resources_path,
                               ModelBackend backend, float conf_threshold, float nms_iou_threshold, bool smart_pad_resize)
        : _models_factory(models_factory)
        , _graph_name(graph_name)
        , _resources_path(resources_path)
        , _backend(backend)
        , _conf_threshold(conf_threshold)
        , _nms_iou_threshold(nms_iou_threshold)
        , _smart_pad_resize(smart_pad_resize)
        , _inited(false) {
    }
    DetectionAlg::DetectionAlg(ModelsFactoryPtr models_factory)
        : _models_factory(models_factory)
        , _inited(false) {
    }


    void DetectionAlg::init() {
        if (_graph_name.empty()){
            throw std::invalid_argument("Call DetectionAlg::reset first before initialization!");
        }
        _detector.reset(_models_factory->getDetector(_graph_name, _resources_path, _backend, _conf_threshold, _smart_pad_resize));
        _detector->init(); //FIXME: add batch size param
        _inited = true;
    }

    void DetectionAlg::reset(const std::string& graph_name, const std::string& resources_path,
                             ModelBackend backend, float conf_threshold, float nms_iou_threshold, bool smart_pad_resize) {
        _graph_name        = graph_name;
        _resources_path    = resources_path;
        _backend           = backend;
        _conf_threshold    = conf_threshold;
        _nms_iou_threshold = nms_iou_threshold;
        _smart_pad_resize  = smart_pad_resize;

        init();
    }

    Boxes DetectionAlg::process(const cv::Mat& input) {
        return process(&input, 1)[0];
    }

    std::vector<Boxes> DetectionAlg::process(const std::vector<cv::Mat>& input) {
        return process(input.data(), input.size());
    }

    std::vector<Boxes> DetectionAlg::process(const cv::Mat* input, size_t size) {
        if (!_inited){
            throw std::invalid_argument("Init DetectionAlg before calling process!");
        }

        // Infer model
        vector<BoxesItem> boxes_item_batch;
        boxes_item_batch.resize(size);
        _detector->forward(input, size, boxes_item_batch.data());

        // Do Non-Maximum Suppression and transform to Boxes
        std::vector<Boxes> boxes_batch;
        boxes_batch.resize(size);
        for (size_t batch_idx = 0; batch_idx < size; batch_idx++) {
            boxes_batch[batch_idx] = std::move(this->_filter(boxes_item_batch[batch_idx]));
        }

        return boxes_batch;
    }

    Boxes DetectionAlg::_filter(const BoxesItem& item) const {
        Boxes boxes;

        vector<int> keep = std::move(NonMaximumSuppression::process(item.boxes, item.scores, _nms_iou_threshold));
        boxes.resize(keep.size());
        std::transform(keep.cbegin(), keep.cend(), boxes.begin(), [item](int idx) {
            return Box{item.boxes[idx], item.landmarks[idx], item.scores[idx], 0};
        });

        // sort by scores (remove_if is unstable, so scores are unordered again)
        std::sort(boxes.begin(), boxes.end(), [](Box& b1, Box& b2) { return b1.getScore() > b2.getScore(); });

        return boxes;
    }
    void DetectionAlg::setConfThreshold(float t) {
        _conf_threshold                = t;
        _detector->settings->threshold = t;
    }
    void DetectionAlg::getNmsIouThreshold(float t) {
        _nms_iou_threshold = t;
    }
    void DetectionAlg::setSmartPadResize(bool allowed) {
        _smart_pad_resize                     = allowed;
        _detector->settings->smart_pad_resize = allowed;
    }

}// namespace cvp
