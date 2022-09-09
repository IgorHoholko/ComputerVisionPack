//
// Created by Игорь Хохолко on 7.09.22.
//

#ifndef COMPUTERVISIONPACK_EDIT_PANEL_H
#define COMPUTERVISIONPACK_EDIT_PANEL_H


#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QToolTip>
#include <QButtonGroup>
#include <QRadioButton>
#include <QGroupBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QFormLayout>

#include "common/common.h"


namespace cvp::gui {

    constexpr int EDIT_PANEL_SLIDER_MIN_WIDTH = 200;
    constexpr int EDIT_PANEL_GROUPBOX_MIN_WIDTH = 200;
    constexpr int EDIT_PANEL_WINDOW_WIDTH = 400;


    class EditPanel : public QDialog {
        Q_OBJECT

    public:
        explicit EditPanel(QWidget* parent = 0);

        void finalize();
        QPushButton* getAppplyButton() const { return _apply_button; };
        QPushButton* getSaveButton() const { return _save_button; };

        void addSlider(const std::string& key, int min, int max, int value, const QString& desc, Qt::Orientation orientation = Qt::Orientation::Horizontal);
        void addDoubleSpinBox(const std::string& key, double min, double max, double value, double step, const QString& desc);
        void addIntSpinBox(const std::string& key, int min, int max, int value, int step, const QString& desc);
        void addCheckBox(const std::string& key, bool checked, const QString& desc);
        void addComboBox(const std::string& key, const QList<QString>& items, const QString& value, const QString& desc);
        void addRadioButtonsGroup(const std::string& key, const QList<QString>& items, int checked_id, const QString& desc);

        template<class Type>
        Type getParam(const std::string& key) const {
            return std::any_cast<Type>(_params.at(key));
        };

        int     getSliderValue(const std::string& key) const { return getParam<int>(key); }
        double  getDoubleSpinBoxValue(const std::string& key) const { return getParam<double>(key); }
        int     getIntSpinBoxValue(const std::string& key) const { return getParam<int>(key); }
        bool    getCheckBoxValue(const std::string& key) const { return getParam<bool>(key); }
        QString getComboBoxValue(const std::string& key) const { return getParam<QString>(key); }

        ParamsDict getCurrentParams() const { return _params; };

    private:
        QVBoxLayout* _layout;
        QFormLayout* _form_layout;
        int _current_layout_row_idx;

        QPushButton* _apply_button;
        QPushButton* _save_button;

        ParamsDict _params;
    };


};// namespace cvp::gui


#endif//COMPUTERVISIONPACK_EDIT_PANEL_H
