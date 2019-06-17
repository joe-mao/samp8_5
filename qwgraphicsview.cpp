#include "qwgraphicsview.h"
#include <QMouseEvent>

QWGraphicsView::QWGraphicsView(QWidget * parent)
    :QGraphicsView (parent)
{

}

void QWGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标移动事件
    QPoint point = event->pos();//GraphicsView的坐标
    emit mouseMovePoint(point);//发射信号
    QGraphicsView::mouseMoveEvent(event);
}

void QWGraphicsView::mousePressEvent(QMouseEvent *event)
{
    //鼠标按下事件
    if(event->button() == Qt::LeftButton){
        QPoint point = event->pos();//QGraphicsView的坐标
        emit mouseClicked(point);//发射信号
    }
    QGraphicsView::mousePressEvent(event);
}

void QWGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    //鼠标双击事件
    if(event->button() == Qt::LeftButton){
        QPoint point = event->pos();//QGraphicsView的坐标
        emit mouseDoubleClick(point);//发射信号
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void QWGraphicsView::keyPressEvent(QKeyEvent *event)
{
    //按键事件
    emit keyPress(event);//发射信号
    QGraphicsView::keyPressEvent(event);
}
