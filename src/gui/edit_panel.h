//
// Created by Игорь Хохолко on 7.09.22.
//

#ifndef COMPUTERVISIONPACK_EDIT_PANEL_H
#define COMPUTERVISIONPACK_EDIT_PANEL_H


#include <QCheckBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QToolTip>

#include "common/common.h"


namespace cvp::gui {

    class EditPanel : public QDialog {
        Q_OBJECT

    public:
        explicit EditPanel(QWidget* parent = 0);

        QPushButton* addApplyButton();
        //        QPushButton* getAppplyButton(){ return _apply_button; };

        void addSlider(const std::string& key, Qt::Orientation orientation, int min, int max, const QString& desc);
        void addDoubleSpinBox(const std::string& key, double min, double max, double step, const QString& desc);
        void addIntSpinBox(const std::string& key, int min, int max, int step, const QString& desc);
        void addCheckBox(const std::string& key, bool checked, const QString& desc);
        //
        int    getSliderValue(const std::string& key);
        double getDoubleSpinBoxValue(const std::string& key);
        int    getIntSpinBoxValue(const std::string& key);
        bool   getCheckBoxValue(const std::string& key);

        template<class Type>
        Type getParam(const std::string& key) {
            return std::any_cast<Type>(_params[key]);
        };

        ParamsDict getCurrentParams() const { return _params; };

    private:
        QVBoxLayout* _main_layout;
        QFormLayout* _form_layout;
        QPushButton* _apply_button;

        ParamsDict _params;
    };


};// namespace cvp::gui


#endif//COMPUTERVISIONPACK_EDIT_PANEL_H
