#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_connectButton_clicked();

    void on_deviceTypeComboBox_currentIndexChanged(int index);

    void on_protocolComboBox_currentIndexChanged(int index);

    void on_quitButton_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *serial;

};

#endif // WIDGET_H
