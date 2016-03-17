#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSignalMapper>
#include "mytoolbutton.h"
#include "mypushbutton.h"

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);

signals:
    void showMin();
    void closeWidget();
    void turnPage(int currentPage);

public slots:
    void turnPage(QString currentPage);

private:
    QPoint pressPoint;//鼠标按下去的点
    bool isMove;
    QLabel *programLabel;//标题
    MyPushButton *minButton;//最小化
    MyPushButton *closeButton;//关闭

    QLabel *nameLabel;
    QLabel *companyLabel;

    QList<MyToolButton *>buttonList;
};

#endif // TITLEWIDGET_H
