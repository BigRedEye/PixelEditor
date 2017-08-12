#include "pixeleditor.h"
#include "ui_pixeleditor.h"
#include <QToolBar>
#include <QKeySequence>
#include <QFileDialog>
#include <QDebug>
#include <QScrollArea>
#include <QPalette>
#include <QShortcut>
#include <QStandardPaths>
#include <QSettings>
#include <QFileInfo>
#include <QWidget>
#include <QKeySequence>
#include <QStyleFactory>

#include "image.h"

PixelEditor::PixelEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PixelEditor)
{
    ui->setupUi(this);
    
    /*
     * close tabs with Ctrl+F4
     */
    QKeySequence seq("Ctrl+F4");
    QShortcut* closeTab = new QShortcut(seq, ui->centralWidget);
    connect(closeTab, SIGNAL(activated()), this, SLOT(on_tabWidget_currentTabCloseRequested()));
    
    scaleFactorBox = new QComboBox();
    scaleFactorLabel = new QLabel("zoom: ");
    ui->statusBar->addPermanentWidget(scaleFactorLabel);
    ui->statusBar->addPermanentWidget(scaleFactorBox);
    for (int i = 0; i<Image::zoomFactors; ++i)
        scaleFactorBox->addItem(QString::number(int(100 * Image::zoomFactor[i])) + "%");
    scaleFactorBox->hide();
    scaleFactorLabel->hide();
    connect(scaleFactorBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_scaleFactorBox_currentIndexChanged(int)));\
    
    QKeySequence zoomInSeq = QKeySequence("Ctrl++");
    QKeySequence zoomOutSeq = QKeySequence("Ctrl+-");
    QShortcut* zoomIn = new QShortcut(zoomInSeq, ui->centralWidget);
    QShortcut* zoomOut = new QShortcut(zoomOutSeq, ui->centralWidget);
    connect(zoomIn, SIGNAL(activated()), this, SLOT(on_zoomIn()));
    connect(zoomOut, SIGNAL(activated()), this, SLOT(on_zoomOut()));
    
    QSettings appSettings;
    int theme = appSettings.value("APP_THEME").toInt();
    setAppTheme(static_cast<Theme>(theme));
    
    updateStatusBar();    
    
}

PixelEditor::~PixelEditor()
{
    delete ui;
    delete scaleFactorBox;
    delete scaleFactorLabel;
}

void PixelEditor::undo()
{
    
}

void PixelEditor::redo()
{
    
}

void PixelEditor::saveAs()
{
    
}

void PixelEditor::save()
{    
    
}

void PixelEditor::open()
{
    /*
     * load a file and open it in a new tab 
     */
    
    const QString DEFAULT_DIR_KEY = "Default picture dir";
    QSettings appSettings;
    if (appSettings.value(DEFAULT_DIR_KEY).isNull())
        appSettings.setValue(DEFAULT_DIR_KEY, QStandardPaths::standardLocations(QStandardPaths::PicturesLocation));
        
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    appSettings.value(DEFAULT_DIR_KEY).toString(),
                                                    tr("Images (*.png *.jpeg *.jpg *.bmp *.gif)"));
    if (!fileName.isEmpty())
    {
        QDir currentDir;
        appSettings.setValue(DEFAULT_DIR_KEY, currentDir.absoluteFilePath(fileName));
    }
    else
        return;
    
    Image *img = new Image();
    if (img->loadImage(fileName))
    {
        delete img;
        return;
    }

    QFileInfo fileInfo(fileName);
    addTab(img, fileInfo.fileName());
}

void PixelEditor::create()
{
    
}

void PixelEditor::on_actionOpen_triggered()
{
    this->open();
}

void PixelEditor::on_actionSettings_triggered()
{
    ConfigDialog *config = new ConfigDialog();
    if (config->exec() == QDialog::Accepted)
        setAppTheme(static_cast<Theme>(config->getTheme()));
    delete config;
}


void PixelEditor::on_tabWidget_tabCloseRequested(int index)
{
    if (!ui->tabWidget->widget(index))
        return;
    QWidget *tab = ui->tabWidget->widget(index);
    for (auto it : tab->children())
        delete it;
    imageByTab.erase(imageByTab.find(tab));
    ui->tabWidget->removeTab(index);
    delete tab;
}

void PixelEditor::on_tabWidget_currentTabCloseRequested()
{
    on_tabWidget_tabCloseRequested(ui->tabWidget->currentIndex());
    ui->statusBar->clearMessage();
}

void PixelEditor::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    if (currentImage())
        ui->statusBar->showMessage("zoom: " + QString::number((double)currentImage()->getScale() * 100, 'f', 3) + "%");
}

void PixelEditor::on_scaleFactorBox_currentIndexChanged(int index)
{
    currentImage()->setScale(Image::zoomFactor[index]);
}

void PixelEditor::on_mouseMoved(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    updateStatusBar();
}

void PixelEditor::addTab(Image *img, const QString &tabName)
{
    QScrollArea *area = new QScrollArea();
    int index = ui->tabWidget->addTab(area, tabName);
    
    area->setWidget(img);
    area->setAlignment(Qt::AlignCenter);
    area->setBackgroundRole(QPalette::Base);
    img->setScrollArea(area);
    
    ui->tabWidget->setCurrentIndex(index);
    imageByTab[ui->tabWidget->currentWidget()] = img;
    on_tabWidget_currentChanged(index);
    
    connect(img, SIGNAL(mouseMoved(int,int)), this, SLOT(on_mouseMoved(int,int)));
}

void PixelEditor::on_zoomIn()
{
    Image *img = currentImage();
    if (img)
        img->zoomIn();
    updateStatusBar();
}

void PixelEditor::on_zoomOut()
{
    Image *img = currentImage();
    if (img)
        img->zoomOut();
    updateStatusBar();
}

Image* PixelEditor::currentImage()
{
    QMap<QWidget*,Image*>::iterator it;
    if ((it = imageByTab.find(ui->tabWidget->currentWidget())) != imageByTab.end())
        return *it;
    else
        return NULL;
}

void PixelEditor::updateStatusBar()
{
    ui->statusBar->clearMessage();
    QPoint mousePos;
    long double zoom;
    Image *img;
    if ( (img = currentImage()) )
    {
        img->getMouseAndZoom(mousePos, zoom);
        ui->statusBar->showMessage("x: " + QString::number(mousePos.x()) + 
                                   ", y: " + QString::number(mousePos.y()) + 
                                   ", zoom: " + QString::number(int(zoom * 100)) + "%");
    }
}

void PixelEditor::setAppTheme(PixelEditor::Theme theme)
{
    QSettings appSettings;
    appSettings.setValue("APP_THEME", static_cast<int>(theme));
    if (theme == DarkTheme)
    {            
        QPalette darkPalette = QApplication::palette();
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        
        qApp->setPalette(darkPalette);
        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }
    else if (theme == LightTheme)
    {       
        qApp->setPalette(this->style()->standardPalette());
        qApp->setStyleSheet("");
    }
}
