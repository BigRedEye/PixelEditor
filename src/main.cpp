#include "pixeleditor.h"
#include <QApplication>
#include <QStyleFactory>
#include <QImage>
#include <QColor>
/*
 * E.g.: Icon made by Freepik from www.flaticon.com 
 */
#include <QImage>

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    a.setStyle(QStyleFactory::create("Fusion"));
    //a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    a.setApplicationName("PixelEditor");
    PixelEditor pixelEditor;
    pixelEditor.show();
    
    return a.exec();
}
