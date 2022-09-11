//
// Created by Игорь Хохолко on 11.09.22.
//

#include "models_factory.h"

#include "dl_models/detection/anchor_ssd.h"

namespace cvp{

    BaseDetector* ModelsFactory::getDetector(const std::string & graph_name, const std::string& resources_path, ModelBackend backend,
                                               float conf_threshold, bool smart_pad_resize, size_t smart_pad_resize_padding, bool allow_upscale) {

        if (graph_name == "retina_resnet50_sml_480x640") {
            auto prior_box_params = PriorBoxParams{  {{16,32},{64,128},{256,512}},  {8,16,32}  };
            std::vector<const char*> output_names = {"bbox_regressions", "classifications", "ldm_regressions"};
            conf_threshold = (conf_threshold < 0.f) ? 0.95f : conf_threshold;
            auto settings = DetectorSettings { 640, 480, conf_threshold, {-2.1179, -2.0357, -1.8044}, {2.2489, 2.4286, 2.6400},
                                             TensorFormatType::NCHW, cv::InterpolationFlags::INTER_LINEAR, smart_pad_resize,
                                             smart_pad_resize_padding, allow_upscale, output_names};
            settings.prior_box_params = std::move( prior_box_params ) ;
//            LOG(Debug, "\tModelsFactory::getDetector: " << "Loading " << graph_name << " detector with <conf_threshold> = " << conf_threshold);
            return new AnchorSSD(resources_path, "graph/detection/face", backend, graph_name, settings);
        }

        std::stringstream msg;
        msg << "No such Detector!"
            << " (" << graph_name << ")";
        throw std::invalid_argument(msg.str().c_str());
    }

}