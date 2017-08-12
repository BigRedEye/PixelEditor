#include "pixeleditor.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    a.setStyle(QStyleFactory::create("Fusion"));
    a.setApplicationName("PixelEditor");
    PixelEditor pixelEditor;
    pixelEditor.show();
    
    return a.exec();
}
