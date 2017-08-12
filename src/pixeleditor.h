#ifndef PIXELEDITOR_H
#define PIXELEDITOR_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QComboBox>
#include <QAction>

#include "image.h"
#include "configdialog.h"

namespace Ui {
class PixelEditor;
}

class PixelEditor : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PixelEditor(QWidget *parent = 0);
    ~PixelEditor();
    void addTab(Image *img, const QString &tabName);
    Image* currentImage();
    void updateStatusBar();
    
    enum Theme {
        DarkTheme = 0,
        LightTheme
    };
    
    void setAppTheme(Theme);
    
public slots:
    void undo();    
    void redo();    
    void save();
    void open();   
    void create();    
    void saveAs();
    
    void on_actionOpen_triggered();
    void on_scaleFactorBox_currentIndexChanged(int index);
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentTabCloseRequested();
    void on_tabWidget_currentChanged(int index);
    void on_mouseMoved(int x, int y);
    void on_zoomIn();
    void on_zoomOut();
private slots:
    void on_actionSettings_triggered();
    
private:
    Ui::PixelEditor *ui;
    QMap<QWidget*, Image*> imageByTab;
    QComboBox *scaleFactorBox;
    QLabel *scaleFactorLabel;
    QPalette darkThemePalette, lightThemePalette;
};

#endif // PIXELEDITOR_H
