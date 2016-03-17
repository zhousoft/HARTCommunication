#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QToolButton>
#include <QMouseEvent>
#include <QPainter>

class MyToolButton : public QToolButton
{
public:
    explicit MyToolButton(QString picName,QWidget *parent = 0);
    ~MyToolButton();
    void setMousePress(bool isPress);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void painterInfo(int topColor,int middleColor,int bottomColor);

public:
    bool isOver;//鼠标是否移过
    bool isPress;//鼠标是否按下
};

#endif // MYTOOLBUTTON_H
