#include "mypushbutton.h"
#include <QDebug>
MyPushButton::MyPushButton(QWidget *parent)
    :QPushButton(parent)
{
    status = NORMAL;
    isPress = false;
}

MyPushButton::~MyPushButton()
{

}

void MyPushButton::setPicName(QString picName)
{
    this->picName = picName;
    setFixedSize(QPixmap(picName).size());
}

void MyPushButton::enterEvent(QEvent *)
{
    status = ENTER;
    qDebug("entry");
    update();
}

void MyPushButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}

void MyPushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPress = true;
        status = PRESS;
        qDebug("press");
        update();
    }
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(isPress && this->rect().contains(event->pos()))
    {
        isPress = false;
        status = ENTER;
        update();
        emit clicked();
    }
}

void MyPushButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap;
    switch (status)
    {
    case NORMAL:
        {
            pixmap.load(picName);
            break;
        }
    case ENTER:
        {
            pixmap.load(picName+QString("_hover"));
            break;
        }
    case PRESS:
        {
            pixmap.load(picName+QString("_pressed"));
            break;
        }
    case NOSTATUS:
        {
            pixmap.load(picName);
            break;
        }
    default:
        pixmap.load(picName);
        break;
    }
    painter.drawPixmap(this->rect(),pixmap);
}


