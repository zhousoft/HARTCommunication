#include "dropenablewidget.h"

DropEnableWidget::DropEnableWidget(QWidget *parent)
    :QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);

    isPress = false;
}

DropEnableWidget::~DropEnableWidget()
{

}

void DropEnableWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPress = true;
    }
    //窗口移动距离的参考值
    movePoint = event->globalPos() - pos();
}

void DropEnableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    isPress = false;
}

void DropEnableWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isPress)//移动窗口
    {
        QPoint movePos = event->globalPos();
        move(movePos - movePoint);
    }
}

void DropEnableWidget::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10,10,this->width()-20,this->height()-20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillPath(path,QBrush(Qt::white));

    QColor color(0,0,0,50);
    for(int i=0;i<10;i++)//绘制界面边框
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

