//
// Created by Игорь Хохолко on 7.09.22.
//

#include "edit_panel.h"


namespace cvp::gui {

    EditPanel::EditPanel(QWidget* parent)
        : QDialog(parent) {

        //        this->resize(300, 200);
        _main_layout = new QVBoxLayout(this);

        _form_layout = new QFormLayout;
        _form_layout->setLabelAlignment(Qt::AlignLeft);

        _main_layout->addLayout(_form_layout);

        this->setMinimumWidth(400);
        this->setMaximumWidth(400);
    }


    QPushButton* EditPanel::addApplyButton() {
        _apply_button = new QPushButton("Apply", this);
        _main_layout->addWidget(_apply_button);
        return _apply_button;
    }

    void EditPanel::addSlider(const std::string& key, Qt::Orientation orientation, int min, int max, const QString& desc) {
        auto* slider = new QSlider(orientation, this);
        int   value  = min + (max - min) / 2;

        slider->setRange(min, max);
        slider->setValue(value);
        slider->setMinimumWidth(250);
        connect(slider, &QSlider::sliderMoved, [&](int value) { QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr); });

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, slider);

        _params[key] = value;
        connect(slider, &QSlider::valueChanged, [this, key](int value) { _params[key] = value; });
    }

    void EditPanel::addDoubleSpinBox(const std::string& key, double min, double max, double step, const QString& desc) {
        auto   spin_box = new QDoubleSpinBox(this);
        double value    = min + (max - min) / 2.;

        spin_box->setRange(min, max);
        spin_box->setValue(value);
        spin_box->setSingleStep(step);

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, spin_box);

        _params[key] = value;
        connect(spin_box, &QDoubleSpinBox::valueChanged, [this, key](int value) { _params[key] = value; });
    }

    void EditPanel::addIntSpinBox(const std::string& key, int min, int max, int step, const QString& desc) {
        auto   spin_box = new QSpinBox(this);
        int value    = min + (max - min) / 2;

        spin_box->setRange(min, max);
        spin_box->setValue(value);
        spin_box->setSingleStep(step);

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, spin_box);

        _params[key] = value;
        connect(spin_box, &QSpinBox::valueChanged, [this, key](int value) { _params[key] = value; });
    }

    void EditPanel::addCheckBox(const std::string& key, bool checked, const QString& desc){
        auto* check_box = new QCheckBox("Box Check", this);

        _form_layout->addRow(check_box);

        _params[key] = check_box->isChecked();
        connect(check_box, &QCheckBox::stateChanged, [this, key, check_box](bool value) { _params[key] = check_box->isChecked(); });
    }


    int EditPanel::getSliderValue(const std::string& key) {
        return getParam<int>(key);
    }
    double EditPanel::getDoubleSpinBoxValue(const std::string& key) {
        return getParam<double>(key);
    }
    int EditPanel::getIntSpinBoxValue(const std::string& key) {
        return getParam<int>(key);
    }
    bool EditPanel::getCheckBoxValue(const std::string& key) {
        return getParam<bool>(key);
    }


}// namespace cvp::gui