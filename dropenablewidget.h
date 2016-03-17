#ifndef DROPENABLEWIDGET_H
#define DROPENABLEWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>

class DropEnableWidget : public QDialog
{
    Q_OBJECT

public:
    explicit DropEnableWidget(QWidget *parent = 0);
    ~DropEnableWidget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private:
    QPoint movePoint;//移动的距离
    bool isPress;
};

#endif // DROPENABLEWIDGET_H
