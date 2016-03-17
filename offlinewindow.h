#ifndef OFFLINEWINDOW_H
#define OFFLINEWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "titlewidget.h"



class OfflineWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit OfflineWindow(QWidget *parent = 0);
    ~OfflineWindow();
protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void showWindow();
    void turnPage(int currentPage);
    void reConnect();
private:
    QStackedWidget *stackedWidget;
    TitleWidget *titleWidget;

};

#endif // OFFLINEWINDOW_H
