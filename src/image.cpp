#include "image.h"
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QScrollBar>
#include <QCursor>

//long double Image::zoomByScrollCoeff;

constexpr long double Image::zoomFactor[];
const int Image::zoomFactors;

Image::Image(QWidget *parent)
    : QWidget(parent), currentScaleIdx(9)
{
    m_pixLabel = new QLabel(this);
    
    m_pixLabel->installEventFilter(this);    
    m_pixLabel->setMouseTracking(true); 
    m_pixLabel->setStyleSheet("background-image: url(:/res/alpha_background.png);\
                               background-repeat: repeat-xy;\
                               background-attachment: fixed;");
}

Image::~Image()
{
    delete m_pixLabel;
}

bool Image::eventFilter(QObject *watched, QEvent *event)
{
        if (watched == m_pixLabel && event->type() == QEvent::MouseMove)
        {
            QMouseEvent* ev = static_cast<QMouseEvent*>(event);
            this->mouseMoveEvent(ev);
            ev->accept();
            return true;
        }
        else if ((watched == m_area || watched == m_pixLabel) && event->type() == QEvent::Wheel)
        {
            QWheelEvent* ev = static_cast<QWheelEvent*>(event);
            if (ev->modifiers() & Qt::CTRL)
            {
                this->wheelEvent(ev);
                ev->accept();
                return true;
            }
            else
            {
                this->redraw();
            }
        }
        else if ((watched == m_area) && event->type() == QEvent::Resize)
        {
            this->redraw();
        }
    return QWidget::eventFilter(watched, event);
}

void Image::setImage(const QImage &img)
{
    m_pix = QPixmap::fromImage(img, Qt::AutoColor);
    m_scale = 1;
    this->setSize(m_pix.size() * m_scale);
    m_pixLabel->setPixmap(m_pix);
}

bool Image::loadImage(const QString &path)
{
    QImage img(path);
    if (img.isNull())
        return 1;
    setImage(img);
    return 0;
}

void Image::zoom(long double coeff)
{
    if (m_scale * coeff < 129 && m_scale * coeff > 1. / 129.)
        m_scale *= coeff;
    else
        return;
    this->setSize(m_pix.size() * m_scale);
    adjustScrollBar(m_area->verticalScrollBar(), coeff);
    adjustScrollBar(m_area->horizontalScrollBar(), coeff);
    redraw();
}

void Image::zoomIn()
{
    if (currentScaleIdx + 1 < Image::zoomFactors)
        setScale(zoomFactor[++currentScaleIdx]);
}

void Image::zoomOut()
{
    if (currentScaleIdx - 1 >= 0)
        setScale(zoomFactor[--currentScaleIdx]);
}

void Image::zoomAdd(long double add)
{
    if (m_scale + add < 129 && m_scale + add > 1. / 129.)
        m_scale += add;
    
    long double coeff = (m_scale) / (m_scale - add);
    this->setSize(m_pix.size() * m_scale);
    adjustScrollBar(m_area->verticalScrollBar(), coeff);
    adjustScrollBar(m_area->horizontalScrollBar(), coeff);
    redraw();
}

void Image::mouseMoveEvent(QMouseEvent *ev)
{
    emit mouseMoved(ev->x(), ev->y());
    ev->accept();
}

void Image::wheelEvent(QWheelEvent *ev)
{
    static int delta = 0;
    if (ev->modifiers() & Qt::CTRL)
    {
        delta += ev->angleDelta().y() / 8;
        if (delta >= 60)
        {
            if (currentScaleIdx + 1 < Image::zoomFactors)
                setScale(zoomFactor[++currentScaleIdx]);
            delta %= 60;
        }
        if (delta <= -60)
        {
            if (currentScaleIdx - 1 >= 0)
                setScale(zoomFactor[--currentScaleIdx]);
            delta %= 60;
        }
        emit mouseMoved(ev->x(), ev->y());
    }
}

void Image::setSize(const QSize &size)
{
    this->resize(size);
    m_alphaPix = QPixmap(size);
}

void Image::setScale(long double scale)
{
    long double coeff = scale / m_scale;
    zoom(coeff);
}

void Image::getMouseAndZoom(QPoint &mousePos, long double &zoom)
{
    mousePos = this->mapFromGlobal(QCursor::pos());
    zoom = m_scale;
}

long double Image::getScale() const
{
    return m_scale;
}

void Image::setScrollArea(QScrollArea *area)
{
    m_area = area;
    area->setMouseTracking(true);
    area->installEventFilter(this);
    connect(m_area->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(redraw(int)));
    connect(m_area->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(redraw(int)));
}

void Image::redraw(int)
{
    Qt::TransformationMode transform = m_scale < 1.67 ? Qt::SmoothTransformation : Qt::FastTransformation;
    
    int x = m_area->horizontalScrollBar()->value();
    int y = m_area->verticalScrollBar()->value();
    
    QPixmap pixCropped = m_pix.copy(x / m_scale, y / m_scale, m_area->width()/m_scale + 10, m_area->height()/m_scale + 10);
    m_pixLabel->setPixmap(pixCropped.scaled(pixCropped.size() * m_scale, Qt::KeepAspectRatio, transform));
    m_pixLabel->adjustSize();
    m_pixLabel->move(x,y);
}

void Image::adjustScrollBar(QScrollBar *scrollBar, long double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                               + ((factor - 1) * scrollBar->pageStep()/2)));
}
