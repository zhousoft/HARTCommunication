#ifndef TWOLINEMAINWIDGET_H
#define TWOLINEMAINWIDGET_H


#include <QWidget>
#include <QDesktopWidget>
#include <QStackedWidget>

#include "dropenablewidget.h"
#include "titlewidget.h"

class TwoLineMainWidget : public DropEnableWidget
{
    Q_OBJECT

public:
    TwoLineMainWidget(QWidget *parent = 0);
    ~TwoLineMainWidget();
    bool isRunning();

protected:
    virtual void paintEvent(QPaintEvent *event);

public slots:
    void showWidget();

private slots:
    void turnPage(int currentPage);

private:
    QStackedWidget *stackeWidget;
    TitleWidget *titleWidget;

    bool isRun;
};

#endif // TWOLINEMAINWIDGET_H
