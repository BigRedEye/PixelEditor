#ifndef IMAGE_H
#define IMAGE_H

#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QUndoView>
#include <QUndoStack>

class Image : public QWidget
{
    Q_OBJECT
    
public:
    Image(QWidget *parent = NULL);
    ~Image();
    
    void setImage(const QImage &img);
    bool loadImage(const QString &path);
    void setSize(const QSize &size);
    void getMouseAndZoom(QPoint &mousePos, long double &zoom);
    QPoint mouseFromGlobal();
    
    void setScale(long double scale);
    void zoomIn();
    void zoomOut();
    void zoom(long double coeff);
    void zoomAdd(long double add);
    
    void setScrollArea(QScrollArea *m_area);
    void adjustScrollBar(QScrollBar *scrollBar, long double factor);
    long double getScale() const;
    
    constexpr static long double zoomFactor[] = {0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.4, 0.6, 0.8, 1, 1.5, 2, 3, 4, 6, 8, 12, 16, 32, 64, 128};
    const static int zoomFactors = 21;
    
private:
    long double m_scale;
    QPixmap m_pix;
    QPixmap m_alphaPix;
    QLabel *m_pixLabel;
    QScrollArea *m_area;
    int currentScaleIdx;
    QUndoStack *m_undoStack;
    
    static constexpr long double zoomByScrollCoeff = 1.25;
    
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void mouseMoved(int x, int y);
public slots:
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);
    void redraw(int value = 0);
};

#endif // IMAGE_H
