#ifndef SAMPLECOUNTWEIDGET_H
#define SAMPLECOUNTWEIDGET_H

#include <QWidget>

namespace Ui {
class SampleCountWeidget;
}

class SampleCountWeidget : public QWidget
{
    Q_OBJECT

public:
    explicit SampleCountWeidget(QWidget *parent = 0);
    ~SampleCountWeidget();

signals:
    void sendCmd(int cmd);
    void sendCmd(int cmd,float farg);
    void setCmdPage(int cmdPage);

private slots:
    void handleData(int cmd,QByteArray data);
    void on_queryButton_clicked();

    void on_setButton_clicked();

private:
    Ui::SampleCountWeidget *ui;
};

#endif // SAMPLECOUNTWEIDGET_H
