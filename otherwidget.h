#ifndef OTHERWIDGET_H
#define OTHERWIDGET_H

#include <QWidget>

namespace Ui {
class OtherWidget;
}

class OtherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OtherWidget(QWidget *parent = 0);
    ~OtherWidget();

signals:
    void sendCmd(int cmd);
    void sendCmd(int cmd,qint32 ivalue);

private slots:
    void handleData(int cmd,QByteArray data);
    void on_queryButton_clicked();

    void on_setButton_clicked();

private:
    Ui::OtherWidget *ui;
};

#endif // OTHERWIDGET_H
