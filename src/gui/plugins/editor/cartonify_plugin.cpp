//
// Created by Игорь Хохолко on 8.09.22.
//

#include "cartonify_plugin.h"

namespace cvp::gui {

    QString CartonifyPlugin::name() {
        return "Cartonify";
    }

    EditPanel* CartonifyPlugin::createEditPanel(QWidget* parent) {
        using namespace cvp::creative;

        auto* edit_panel = new EditPanel(parent);

        edit_panel->addIntSpinBox("median_fs", CARTONIFY_MEDIAN_FILTER_SIZE_MIN, CARTONIFY_MEDIAN_FILTER_SIZE_MAX, CARTONIFY_MEDIAN_FILTER_SIZE_DEFAULT, 2, "Median Filter Kernel Size: ");
        edit_panel->addIntSpinBox("laplacian_fs", CARTONIFY_LAPLACIAN_FILTER_SIZE_MIN, CARTONIFY_LAPLACIAN_FILTER_SIZE_MAX, CARTONIFY_LAPLACIAN_FILTER_SIZE_DEFAULT, 2, "Laplacian Filter Kernel Size: ");
        edit_panel->addSlider("edges_t", CARTONIFY_EDGES_THRESHOLD_MIN, CARTONIFY_EDGES_THRESHOLD_MAX, CARTONIFY_EDGES_THRESHOLD_DEFAULT, "Edges Threshold: ");
        edit_panel->addIntSpinBox("rescale", CARTONIFY_RESCALE_FACTOR_MIN, CARTONIFY_RESCALE_FACTOR_MAX, CARTONIFY_RESCALE_FACTOR_DEFAULT, 1, "Rescale Factor: ");
        edit_panel->addIntSpinBox("bilateral_fs", CARTONIFY_BILATERAL_KETNEL_SIZE_MIN, CARTONIFY_BILATERAL_KETNEL_SIZE_MAX, CARTONIFY_BILATERAL_KETNEL_SIZE_DEFAULT, 2, "Bilateral Filter Kernel Size: ");
        edit_panel->addSlider("repetitions", CARTONIFY_REPETITIONS_MIN, CARTONIFY_REPETITIONS_MAX, CARTONIFY_REPETITIONS_DEFAULT, "Repetitions: ");
        edit_panel->addDoubleSpinBox("sigma_space", CARTONIFY_SIGMA_SPACE_MIN, CARTONIFY_SIGMA_SPACE_MAX, CARTONIFY_SIGMA_SPACE_DEFAULT, 0.25, "Sigma Space: ");
        edit_panel->addDoubleSpinBox("sigma_color", CARTONIFY_SIGMA_COLOR_MIN, CARTONIFY_SIGMA_COLOR_MAX, CARTONIFY_SIGMA_COLOR_DEFAULT, 0.25, "Sigma Color: ");
        edit_panel->addRadioButtonsGroup("output_image_type", {"Cartoon", "Edges", "Raw Cartoon"}, 0, "Return Image");

        return edit_panel;
    }

    void CartonifyPlugin::edit(const cv::Mat& input, cv::Mat& output, const cvp::ParamsDict& params) {
        using namespace cvp::creative;

        CartoonSettings settings;
        settings.median_filter_size    = std::any_cast<int>(params.at("median_fs"));
        settings.laplacian_filter_size = std::any_cast<int>(params.at("laplacian_fs"));
        settings.edges_threshold       = (float)std::any_cast<int>(params.at("edges_t"));
        settings.rescale_factor        = std::any_cast<int>(params.at("rescale"));
        settings.bilateral_kernel_size = std::any_cast<int>(params.at("bilateral_fs"));
        settings.repetitions           = std::any_cast<int>(params.at("repetitions"));
        settings.sigma_space           = std::any_cast<double>(params.at("sigma_space"));
        settings.sigma_color           = std::any_cast<double>(params.at("sigma_color"));

        QString output_image_type =      std::any_cast<QString>(params.at("output_image_type"));

        cv::Mat dst_edges;
        cv::Mat dst_cartoon_raw;
        cartonify(input, &output, settings, &dst_edges, &dst_cartoon_raw);

        if (output_image_type == "Edges"){
            output = dst_edges;
        }else if (output_image_type == "Raw Cartoon"){
            output = dst_cartoon_raw;
        }


    }

}// namespace cvp::gui
