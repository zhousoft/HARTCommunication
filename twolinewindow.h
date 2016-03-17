#ifndef TWOLINEWINDOW_H
#define TWOLINEWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
namespace Ui {
class TwoLineWindow;
}

class TwoLineWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TwoLineWindow(QWidget *parent = 0);
    ~TwoLineWindow();

private:
    Ui::TwoLineWindow *ui;
    QSerialPort *serial;
    int deviceAdres;
};

#endif // TWOLINEWINDOW_H
