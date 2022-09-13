//
// Created by Игорь Хохолко on 10.09.22.
//

#ifndef COMPUTERVISIONPACK_ANCHOR_SSD_H
#define COMPUTERVISIONPACK_ANCHOR_SSD_H

#include "base_detector.h"
#include "common.h"
#include "tool/detection/box_utils.h"


namespace cvp {


    class AnchorSSD : public BaseDetector {
    public:
        AnchorSSD(const std::string& resource_path, const std::string& model_folder, ModelBackend backend,
                  const std::string& graph_name, DetectorSettingsUPtr t_settings);

        void init(size_t batch_size = 1) override;

    protected:
        void postprocess(std::vector<Floats>& network_outputs, BoxesItem* outputs, size_t batch_size) override;


    private:

        PriorBox _priors;
    };


}


#endif//COMPUTERVISIONPACK_ANCHOR_SSD_H
