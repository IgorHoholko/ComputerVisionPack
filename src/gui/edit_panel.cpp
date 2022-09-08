//
// Created by Игорь Хохолко on 7.09.22.
//

#include "edit_panel.h"


namespace cvp::gui {

    EditPanel::EditPanel(QWidget* parent)
        : QDialog(parent)
    , _current_layout_idx(0){

        //        this->resize(300, 200);
        _layout = new QGridLayout(this);
        this->setMinimumWidth(400);
    }


    QPushButton* EditPanel::addApplyButton() {
        _apply_button = new QPushButton("Apply", this);
        _layout->addWidget(_apply_button, _current_layout_idx, 1, 1, 3);
        return _apply_button;
    }

    void EditPanel::addSlider(Qt::Orientation orientation, int min, int max, const QString& desc, const std::string& key) {
        auto* slider = new QSlider(orientation, this);
        int   value  = min + (max - min) / 2;

        slider->setRange(min, max);
        slider->setValue(value);
        connect(slider, &QSlider::sliderMoved, [&](int value) {QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr); } );

        auto* label_desc = new QLabel(desc );
        label_desc->setStyleSheet("font-weight: bold");
        auto* label_min = new QLabel( QString("Min: %1").arg(min) );
        auto* label_max = new QLabel(QString("Max: %1").arg(max) );

        _layout->addWidget(label_desc, _current_layout_idx++, 0, 1, 5);

        _layout->addWidget(label_min, _current_layout_idx, 0, 1, 1);
        _layout->addWidget(slider, _current_layout_idx, 1, 1, 3);
        _layout->addWidget(label_max, _current_layout_idx, 4, 1, 1);


        _params[key] = value;
        connect(slider, &QSlider::valueChanged, [this, key](int value) { _params[key] = value; });

        _current_layout_idx++;
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