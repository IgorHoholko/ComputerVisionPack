

#ifndef COMPUTERVISIONPACK_IMAGE_VIEWER_H
#define COMPUTERVISIONPACK_IMAGE_VIEWER_H

#include <vector>

#include "QPluginLoader"
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegExp>
#include <QStatusBar>
#include <QToolBar>


#include "gui/edit_panel.h"
#include "gui/plugins/editor/editor_plugin_interface.h"
#include "tool/image/imgs_conversion.h"


namespace cvp::gui {
    class ImageViewer : public QMainWindow {
        Q_OBJECT

    public:
        explicit ImageViewer(QWidget* parent = nullptr);
        ~ImageViewer();

    private:
        void _initUI();
        void _createActions();
        void _showImage(const QString& path);
        void _setUpShortcuts();

        void _loadPlugins();

        void _setNewPixmap(const QPixmap& pixmap);

        int counter = 0;

        EditPanel*             _edit_panel;
        EditorPluginInterface* _current_plugin;

        QMap<QString, EditorPluginInterface*> _edit_plugins;

        QGraphicsPixmapItem* _current_image;
        QString              _current_image_path;

        std::vector<QPixmap> _imgs_stack_past;
        std::vector<QPixmap> _imgs_stack_next;


        // UI
        QMenu* _file_menu;
        QMenu* _view_menu;
        QMenu* _edit_menu;

        QToolBar* _file_tool_bar;
        QToolBar* _view_tool_bar;
        QToolBar* _edit_tool_bar;

        QGraphicsScene* _image_scene;
        QGraphicsView*  _image_view;

        QStatusBar* _main_status_bar;
        QLabel*     _main_status_label;

        // Actions
        QAction* _open_action;
        QAction* _save_as_action;
        QAction* _exit_action;
        QAction* _zoom_in_action;
        QAction* _zoom_out_action;
        QAction* _previous_image_action;
        QAction* _next_image_action;
        QAction* _back_edit_action;
        QAction* _next_edit_action;
        QAction* _reset_action;

    private slots:
        void _openImage();
        void _zoomIn();
        void _zoomOut();
        void _saveAs();
        void _prevImage();
        void _nextImage();
        void _pluginOpen();
        void _pluginPerform();
        void _pluginClose();

        void _saveCurrentEditedImage();
        void _resetImage();
        void _backEdit();
        void _nextEdit();

    };

}// namespace cvp::gui

#endif//COMPUTERVISIONPACK_IMAGE_VIEWER_H
