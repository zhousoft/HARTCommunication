#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

class MyPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MyPushButton(QWidget *parent = 0);
    ~MyPushButton();
    void setPicName(QString picName);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    //枚举按钮的几种状态
    enum ButtonStatus
    {
        NORMAL,
        ENTER,
        PRESS,
        NOSTATUS
    };
    ButtonStatus status;
    QString picName;
    int btnWidth;
    int btnHeight;
    bool isPress;
};

#endif // MYPUSHBUTTON_H
