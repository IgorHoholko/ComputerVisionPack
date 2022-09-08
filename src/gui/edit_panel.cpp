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


    void EditPanel::addButtons() {
        _apply_button = new QPushButton("Apply", this);
        _save_button = new QPushButton("Save", this);

        _form_layout->addRow(_apply_button, _save_button);
    }

    void EditPanel::addSlider(const std::string& key, int min, int max, int value, const QString& desc, Qt::Orientation orientation) {
        auto* slider = new QSlider(orientation, this);

        int value_half = min + (max - min) / 2;
        value          = (value >= min && value <= max) ? value : value_half;

        slider->setRange(min, max);
        slider->setValue(value);
        slider->setMinimumWidth(250);
        connect(slider, &QSlider::sliderMoved, [&](int value) { QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr); });

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, slider);

        _params[key] = value;
        connect(slider, &QSlider::valueChanged, [this, key](int value) { _params[key] = value; });
    }

    void EditPanel::addDoubleSpinBox(const std::string& key, double min, double max, double value, double step, const QString& desc) {
        auto   spin_box = new QDoubleSpinBox(this);

        double value_half = min + (max - min) / 2.;
        value          = (value >= min && value <= max) ? value : value_half;

        spin_box->setRange(min, max);
        spin_box->setValue(value);
        spin_box->setSingleStep(step);

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, spin_box);

        _params[key] = value;
        connect(spin_box, &QDoubleSpinBox::valueChanged, [this, key](int value) { _params[key] = value; });
    }

    void EditPanel::addIntSpinBox(const std::string& key, int min, int max, int value, int step, const QString& desc) {
        auto spin_box = new QSpinBox(this);

        int value_half = min + (max - min) / 2;
        value          = (value >= min && value <= max) ? value : value_half;

        spin_box->setRange(min, max);
        spin_box->setValue(value);
        spin_box->setSingleStep(step);

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, spin_box);

        _params[key] = value;
        connect(spin_box, &QSpinBox::valueChanged, [this, key](int value) { _params[key] = value; });
    }

    void EditPanel::addCheckBox(const std::string& key, bool checked, const QString& desc) {
        auto* check_box = new QCheckBox("Box Check", this);

        check_box->setChecked(checked);

        _form_layout->addRow(check_box);

        _params[key] = check_box->isChecked();
        connect(check_box, &QCheckBox::stateChanged, [this, key, check_box](bool value) { _params[key] = check_box->isChecked(); });
    }

    void EditPanel::addComboBox(const std::string& key, const QList<QString>& items, const QString& value, const QString& desc) {
        auto* combo_box = new QComboBox(this);
        combo_box->addItems(items);
        combo_box->setCurrentText(value);

        _form_layout->addRow(desc, combo_box);

        _params[key] = combo_box->currentText();
        connect(combo_box, &QComboBox::currentTextChanged, [this, key](const QString& txt) { _params[key] = txt; });
    }

}// namespace cvp::gui