//
// Created by Игорь Хохолко on 7.09.22.
//

#include "edit_panel.h"


namespace cvp::gui {

    EditPanel::EditPanel(QWidget* parent)
        : QDialog(parent) {
        _layout = new QVBoxLayout;
    }


    QPushButton* EditPanel::addApplyButton(){
        _apply_button = new QPushButton("Apply", this);
        _layout->addWidget(_apply_button);
        return _apply_button;
    }

    void EditPanel::addSlider(Qt::Orientation orientation, int min, int max, const QString& desc, const std::string& key) {
        auto* slider = new QSlider(orientation, this);
        slider->setRange(min, max);
        slider->setAccessibleDescription(desc);
        _layout->addWidget(slider);

        _params[key] = value;
        connect(slider, &QSlider::valueChanged, [this, key](int value){_params[key] = value; } );
    }

//    void EditPanel::addDoubleSpinBox(double min, double max, double step, const QString& desc, const std::string& key) {
//    }
//
//    void EditPanel::addIntSpinBox(int min, int max, int step, const QString& desc, const std::string& key) {
//    }

//
//    int EditPanel::getSliderValue(const std::string& key) {
//        return 0;
//    }
//    double EditPanel::getDoubleSpinBoxValue(const std::string& key) {
//        return 0;
//    }
//    int EditPanel::getIntSpinBoxValue(const std::string& key) {
//        return 0;
//    }


}// namespace cvp::gui