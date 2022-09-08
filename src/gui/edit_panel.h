//
// Created by Игорь Хохолко on 7.09.22.
//

#ifndef COMPUTERVISIONPACK_EDIT_PANEL_H
#define COMPUTERVISIONPACK_EDIT_PANEL_H


#include <QDialog>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolTip>

#include "common/common.h"


namespace cvp::gui {

    class EditPanel : public QDialog {
        Q_OBJECT

    public:
        explicit EditPanel(QWidget* parent = 0);

        QPushButton* addApplyButton();
//        QPushButton* getAppplyButton(){ return _apply_button; };

        void addSlider(Qt::Orientation orientation, int min, int max, const QString& desc, const std::string& key);
//        void addDoubleSpinBox(double min, double max, double step, const QString& desc, const std::string& key);
//        void addIntSpinBox(int min, int max, int step, const QString& desc, const std::string& key);
//
//        int getSliderValue(const std::string& key);
//        double getDoubleSpinBoxValue(const std::string& key);
//        int getIntSpinBoxValue(const std::string& key);

        ParamsDict getCurrentParams() const { return _params; };

    private:

        QGridLayout* _layout;
        QPushButton* _apply_button;

        ParamsDict _params;

        int _current_layout_idx;


    };


};// namespace CVP::gui


#endif//COMPUTERVISIONPACK_EDIT_PANEL_H
