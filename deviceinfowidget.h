#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include <QWidget>

namespace Ui {
class DeviceInfoWidget;
}

class DeviceInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceInfoWidget(QWidget *parent = 0);
    ~DeviceInfoWidget();

signals:
    void sendCmd(int cmd);
    void sendCmd(int cmd,QByteArray data);

private slots:
    void handleData(int cmd,QByteArray data);
    void on_readTagButton_clicked();


    void on_readMsgButton_clicked();

private:
    Ui::DeviceInfoWidget *ui;
};

#endif // DEVICEINFOWIDGET_H
