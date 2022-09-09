
#include "image_viewer.h"

namespace cvp::gui {
    ImageViewer::ImageViewer(QWidget* parent)
        : QMainWindow(parent)
        , _file_menu(nullptr)
        , _view_menu(nullptr)
        , _edit_panel(nullptr)
        , _current_plugin(nullptr) {
        _initUI();
        _loadPlugins();
    }


    ImageViewer::~ImageViewer() {}

    void ImageViewer::_initUI() {
        this->resize(800, 600);

        // setup menubar
        _file_menu = menuBar()->addMenu("&File");
        _view_menu = menuBar()->addMenu("&View");
        _edit_menu = menuBar()->addMenu("&Edit");

        // setup toolbar
        _file_tool_bar = addToolBar("File");
        _view_tool_bar = addToolBar("View");
        _edit_tool_bar = addToolBar("Edit");

        // main area for image display
        _image_scene = new QGraphicsScene(this);
        _image_view  = new QGraphicsView(_image_scene);
        setCentralWidget(_image_view);

        // setup status bar
        _main_status_bar   = statusBar();
        _main_status_label = new QLabel(_main_status_bar);
        _main_status_bar->addPermanentWidget(_main_status_label);
        _main_status_label->setText("Image Information will be here!");

        _createActions();
    }

    void ImageViewer::_createActions() {

        _open_action = new QAction("&Open", this);
        _file_menu->addAction(_open_action);
        _file_tool_bar->addAction(_open_action);
        connect(_open_action, SIGNAL(triggered(bool)), this, SLOT(_openImage()));

        _save_as_action = new QAction("&Save as", this);
        _file_menu->addAction(_save_as_action);
        connect(_save_as_action, SIGNAL(triggered(bool)), this, SLOT(_saveAs()));

        _exit_action = new QAction("&Exit", this);
        _file_menu->addAction(_exit_action);
        connect(_exit_action, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));


        _zoom_in_action = new QAction("Zoom in", this);
        _view_menu->addAction(_zoom_in_action);
        _view_tool_bar->addAction(_zoom_in_action);
        connect(_zoom_in_action, SIGNAL(triggered(bool)), this, SLOT(_zoomIn()));

        _zoom_out_action = new QAction("Zoom out", this);
        _view_menu->addAction(_zoom_out_action);
        _view_tool_bar->addAction(_zoom_out_action);
        connect(_zoom_out_action, SIGNAL(triggered(bool)), this, SLOT(_zoomOut()));

        _previous_image_action = new QAction("Previous image", this);
        _view_menu->addAction(_previous_image_action);
        _view_tool_bar->addAction(_previous_image_action);
        connect(_previous_image_action, SIGNAL(triggered(bool)), this, SLOT(_prevImage()));

        _next_image_action = new QAction("Next image", this);
        _view_menu->addAction(_next_image_action);
        _view_tool_bar->addAction(_next_image_action);
        connect(_next_image_action, SIGNAL(triggered(bool)), this, SLOT(_nextImage()));

        _cancel_action = new QAction("&Cancel", this);
        _view_menu->addAction(_cancel_action);
        _view_tool_bar->addAction(_cancel_action);
        connect(_cancel_action, SIGNAL(triggered(bool)), this, SLOT(_cancelEdit()));

        _reset_action = new QAction("&Reset", this);
        _view_menu->addAction(_reset_action);
        _view_tool_bar->addAction(_reset_action);
        connect(_reset_action, SIGNAL(triggered(bool)), this, SLOT(_resetImage()));

        _setUpShortcuts();
    }
    void ImageViewer::_openImage() {
        QFileDialog dialog(this);
        dialog.setWindowTitle("Open Image");
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg *.jpeg)"));
        QStringList filePaths;
        if (dialog.exec()) {
            filePaths = dialog.selectedFiles();
            _showImage(filePaths.at(0));
        }
    }
    void ImageViewer::_showImage(const QString& path) {
        _current_image_path = path;

        QPixmap pixmap(path);
        _setNewPixmap(pixmap);

        // Clear History Stack: remove all except 1 and then remove last 1 :)
        _resetHistory();
        _imgs_history.push( std::move(_current_image->pixmap().copy()) );

        QString status = QString("%1, %2x%3, %4 Bytes").arg(path).arg(pixmap.width()).arg(pixmap.height()).arg(QFile(path).size());
        _main_status_label->setText(status);
    }

    void ImageViewer::_zoomIn() {
        _image_view->scale(1.2, 1.2);
    }

    void ImageViewer::_zoomOut() {
        _image_view->scale(0.8, 0.8);
    }

    void ImageViewer::_saveAs() {
        if (_current_image == nullptr) {
            QMessageBox::information(this, "Information", "Nothing to save.");
            return;
        }
        QFileDialog dialog(this);
        dialog.setWindowTitle("Save Image As ...");
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg *.jpeg)"));
        QStringList fileNames;
        if (dialog.exec()) {
            fileNames = dialog.selectedFiles();
            if (QRegExp(".+\\.(png|bmp|jpg|jpeg)").exactMatch(fileNames.at(0))) {
                _imgs_history.top().save(fileNames.at(0));
            } else {
                QMessageBox::information(this, "Information", "Save error: bad format or filename.");
            }
        }
    }

    void ImageViewer::_prevImage() {
        QFileInfo current(_current_image_path);
        QDir      dir = current.absoluteDir();

        QStringList nameFilters;
        nameFilters << "*.png"
                    << "*.bmp"
                    << "*.jpg"
                    << "*.jpeg";
        QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
        int         idx       = fileNames.indexOf(current.fileName());
        if (idx > 0) {
            _showImage(dir.absoluteFilePath(fileNames.at(idx - 1)));
        } else {
            QMessageBox::information(this, "Information", "Current image is the first one.");
        }
    }

    void ImageViewer::_nextImage() {
        QFileInfo current(_current_image_path);
        QDir      dir = current.absoluteDir();

        QStringList nameFilters;
        nameFilters << "*.png"
                    << "*.bmp"
                    << "*.jpg"
                    << "*.jpeg";
        QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
        int         idx       = fileNames.indexOf(current.fileName());
        if (idx < fileNames.size() - 1) {
            _showImage(dir.absoluteFilePath(fileNames.at(idx + 1)));
        } else {
            QMessageBox::information(this, "Information", "Current image is the last one.");
        }
    }
    void ImageViewer::_setUpShortcuts() {
        QList<QKeySequence> shortcuts;
        shortcuts << Qt::Key_Plus << Qt::Key_Equal;
        _zoom_in_action->setShortcuts(shortcuts);

        shortcuts.clear();
        shortcuts << Qt::Key_Minus << Qt::Key_Underscore ;
        _zoom_out_action->setShortcuts(shortcuts);

        shortcuts.clear();
        shortcuts << Qt::Key_Up << Qt::Key_Left;
        _previous_image_action->setShortcuts(shortcuts);

        shortcuts.clear();
        shortcuts << Qt::Key_Down << Qt::Key_Right;
        _next_image_action->setShortcuts(shortcuts);
    }

    void ImageViewer::_loadPlugins() {
        QDir        pluginsDir(QApplication::instance()->applicationDirPath() + "/../lib");
        QStringList nameFilters;
        nameFilters << "*.so"
                    << "*.dylib"
                    << "*.dll";

        QFileInfoList plugins = pluginsDir.entryInfoList(
                nameFilters, QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
        foreach (QFileInfo plugin, plugins) {
            QPluginLoader          pluginLoader(plugin.absoluteFilePath(), this);
            EditorPluginInterface* plugin_ptr = dynamic_cast<EditorPluginInterface*>(pluginLoader.instance());
            if (plugin_ptr) {
                QAction* action = new QAction(plugin_ptr->name());
                _edit_menu->addAction(action);
                _edit_tool_bar->addAction(action);
                _edit_plugins[plugin_ptr->name()] = plugin_ptr;
                connect(action, SIGNAL(triggered(bool)), this, SLOT(_pluginOpen()));
                // pluginLoader.unload();
            } else {
                qDebug() << "bad plugin: " << plugin.absoluteFilePath();
            }
        }
    }

    void ImageViewer::_pluginOpen() {
        if (_current_image == nullptr) {
            QMessageBox::information(this, "Information", "No image to edit.");
            return;
        }

        if (_edit_panel != nullptr) {
            QMessageBox::information(this, "Information", "Close Previous Edit Menu!");
            return;
        }

        // Get out plugin by sender's button name
        QAction*               active_action = qobject_cast<QAction*>(sender());
        _current_plugin    = _edit_plugins[active_action->text()];
        if (!_current_plugin) {
            QMessageBox::information(this, "Information", "No plugin is found.");
            return;
        }

        // Create Edit Panel to change params for editor (Optional for each plugin)
        _edit_panel = _current_plugin->createEditPanel(this);

        if (_edit_panel != nullptr) {
            _edit_panel->addButtons();
            connect(_edit_panel, &EditPanel::rejected, this, &ImageViewer::_pluginClose);
            connect(_edit_panel->getAppplyButton(), &QPushButton::clicked, this, &ImageViewer::_pluginPerform);
            connect(_edit_panel->getSaveButton(), &QPushButton::clicked, this, &ImageViewer::_saveCurrentEditedImage);
            _edit_panel->show();
        } else {
            _pluginPerform();
            _saveCurrentEditedImage();
        }
    }

    void ImageViewer::_pluginPerform() {
        cv::Mat mat = qPixmapToCvMat(_imgs_history.top());
        cv::Mat mat_edited;

        if (_edit_panel != nullptr){
            _current_plugin->edit(mat, mat_edited, _edit_panel->getCurrentParams());
        }else{
            _current_plugin->edit(mat, mat_edited, {});
        }


        QPixmap pixmap_edited = cvMatToQPixmap(mat_edited);

        _setNewPixmap(pixmap_edited);

        QString status = QString("(editted image), %1x%2").arg(pixmap_edited.width()).arg(pixmap_edited.height());
        _main_status_label->setText(status);
    }


    void ImageViewer::_setNewPixmap(const QPixmap& pixmap) {
        _image_scene->clear();
        _image_view->resetTransform();

        _current_image = _image_scene->addPixmap(pixmap);

        _image_scene->update();
        _image_view->setSceneRect(_image_scene->sceneRect());
        _image_view->fitInView(_image_scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);
    }

    void ImageViewer::_pluginClose() {
        _edit_panel = nullptr;
    }

    void ImageViewer::_saveCurrentEditedImage() {
        _imgs_history.push( std::move(_current_image->pixmap().copy()) );
    }

    void ImageViewer::_resetImage() {
        if (_imgs_history.size() == 1){
            return ;
        }
        while (_imgs_history.size() != 1){
            _imgs_history.pop();
        }
        _setNewPixmap(_imgs_history.top());
    }

    void ImageViewer::_cancelEdit() {
        if (_imgs_history.size() == 1){
            return ;
        }
        _imgs_history.pop();
        _setNewPixmap(_imgs_history.top());
    }

    void ImageViewer::_resetHistory() {
        while (_imgs_history.size()){
            _imgs_history.pop();
        }
    }

}// namespace cvp::gui