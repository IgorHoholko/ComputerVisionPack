//
// Created by Игорь Хохолко on 7.09.22.
//

#include "edit_panel.h"


namespace cvp::gui {

    EditPanel::EditPanel(QWidget* parent)
        : QDialog(parent) {

        //        this->resize(300, 200);
        _layout = new QVBoxLayout(this);
        this->setLayout(_layout);

        _form_layout = new QFormLayout(this);
        _form_layout->setLabelAlignment(Qt::AlignLeft);

        this->setMinimumWidth(EDIT_PANEL_WINDOW_WIDTH);
        this->setMaximumWidth(EDIT_PANEL_WINDOW_WIDTH);
    }


    void EditPanel::finalize() {
        _apply_button = new QPushButton("Apply");
        _save_button = new QPushButton("Save");

        _layout->addLayout(_form_layout);

        _layout->addWidget(_apply_button);
        _layout->addWidget(_save_button);
    }

    void EditPanel::addSlider(const std::string& key, int min, int max, int value, const QString& desc, Qt::Orientation orientation) {
        auto* slider = new QSlider(orientation);

        int value_half = min + (max - min) / 2;
        value          = (value >= min && value <= max) ? value : value_half;

        slider->setRange(min, max);
        slider->setValue(value);
        slider->setMinimumWidth(EDIT_PANEL_SLIDER_MIN_WIDTH);
        connect(slider, &QSlider::sliderMoved, [&](int value) { QToolTip::showText(QCursor::pos(), QString("%1").arg(value), nullptr); });

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, slider);

        _params[key] = value;
        connect(slider, &QSlider::valueChanged, [this, key](int value) { _params[key] = value; });
        _current_layout_row_idx++;
    }

    void EditPanel::addDoubleSpinBox(const std::string& key, double min, double max, double value, double step, const QString& desc) {
        auto*   spin_box = new QDoubleSpinBox;

        double value_half = min + (max - min) / 2.;
        value          = (value >= min && value <= max) ? value : value_half;

        spin_box->setRange(min, max);
        spin_box->setValue(value);
        spin_box->setSingleStep(step);

        auto* label_desc = new QLabel(desc);
        label_desc->setWordWrap(true);

        _form_layout->addRow(label_desc, spin_box);

        _params[key] = value;
        connect(spin_box, &QDoubleSpinBox::valueChanged, [this, key](double value) { _params[key] = value; });
        _current_layout_row_idx++;
    }

    void EditPanel::addIntSpinBox(const std::string& key, int min, int max, int value, int step, const QString& desc) {
        auto* spin_box = new QSpinBox;

        int value_half = min + (max - min) / 2;
        value          = (value >= min && value <= max) ? value : value_half;

        spin_box->setRange(min, max);
        spin_box->setValue(value);
        spin_box->setSingleStep(step);

        auto* label_desc = new QLabel(desc);
        label_desc->setWordWrap(true);

        _form_layout->addRow(label_desc, spin_box);

        _params[key] = value;
        connect(spin_box, &QSpinBox::valueChanged, [this, key](int value) { _params[key] = value; });
        _current_layout_row_idx++;
    }

    void EditPanel::addCheckBox(const std::string& key, bool checked, const QString& desc) {
        auto* check_box = new QCheckBox(desc);

        check_box->setChecked(checked);

        _form_layout->addRow(check_box);

        _params[key] = check_box->isChecked();
        connect(check_box, &QCheckBox::stateChanged, [this, key, check_box](bool value) { _params[key] = check_box->isChecked(); });
        _current_layout_row_idx++;
    }

    void EditPanel::addComboBox(const std::string& key, const QList<QString>& items, const QString& value, const QString& desc) {
        auto* combo_box = new QComboBox;
        combo_box->addItems(items);
        combo_box->setCurrentText(value);

        auto* label_desc = new QLabel(desc);
        label_desc->setWordWrap(true);

        _form_layout->addRow(label_desc, combo_box);

        _params[key] = combo_box->currentText();
        connect(combo_box, &QComboBox::currentTextChanged, [this, key](const QString& txt) { _params[key] = txt; });
        _current_layout_row_idx++;
    }
    void EditPanel::addRadioButtonsGroup(const std::string& key, const QList<QString>& items, int checked_id, const QString& desc) {
        if (items.empty()){ return ;}

        auto *button_group = new QButtonGroup;
        auto *group_box = new QGroupBox;

        QVBoxLayout *vbox = new QVBoxLayout;

        for (size_t i = 0; i < items.size(); i++){
            QRadioButton *radio = new QRadioButton(items[i]);
            radio->setChecked( checked_id == i );
            button_group->addButton(radio);
            vbox->addWidget(radio);
        }

        vbox->addStretch(1);
        group_box->setLayout(vbox);
        group_box->setMinimumWidth(EDIT_PANEL_GROUPBOX_MIN_WIDTH);

        auto* label_desc = new QLabel(desc);

        _form_layout->addRow(label_desc, group_box);

        _params[key] = items[checked_id];
        connect(button_group, &QButtonGroup::buttonClicked, [this, key](QAbstractButton * button) { _params[key] = button->text();} );
        _current_layout_row_idx++;
    }

}// namespace cvp::gui