//
// Created by Игорь Хохолко on 11.09.22.
//

#ifndef COMPUTERVISIONPACK_MODELS_FACTORY_H
#define COMPUTERVISIONPACK_MODELS_FACTORY_H

#include "dl_models/detection/base_detector.h"


namespace cvp {

    class ModelsFactory {
    public:
        BaseDetector* getDetector(const std::string& graph_name, const std::string& resources_path, ModelBackend backend, float conf_threshold = -1.f,
                                     bool smart_pad_resize = false, size_t smart_pad_resize_padding = 0, bool allow_upscale = true);
    };

    using ModelsFactoryPtr = std::shared_ptr<ModelsFactory>;
    using ModelsFactoryUPtr = std::unique_ptr<ModelsFactory>;
}// namespace cvp


#endif//COMPUTERVISIONPACK_MODELS_FACTORY_H
