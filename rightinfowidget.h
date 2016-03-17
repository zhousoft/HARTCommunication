#ifndef RIGHTINFOWIDGET_H
#define RIGHTINFOWIDGET_H

#include <QWidget>

namespace Ui {
class RightInfoWidget;
}

class RightInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RightInfoWidget(QWidget *parent = 0);
    ~RightInfoWidget();

signals:
    void reConnect();
    void sendCmd(int cmd);
private slots:


    void on_reConnectButton_clicked();

    void on_updateButton_clicked();

private slots:
    void handleData(int cmd,QByteArray data);

private:
    Ui::RightInfoWidget *ui;
};

#endif // RIGHTINFOWIDGET_H
