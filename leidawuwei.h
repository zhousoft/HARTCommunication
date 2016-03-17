#ifndef LEIDAWUWEI_H
#define LEIDAWUWEI_H

#include <QWidget>

namespace Ui {
class LeiDaWuWei;
}

class LeiDaWuWei : public QWidget
{
    Q_OBJECT

public:
    explicit LeiDaWuWei(QWidget *parent = 0);
    ~LeiDaWuWei();

signals:
    void setCmdPage(int cmdPage);
    void sendCmd(int cmd);
    void sendCmd(int cmd,float farg1,float farg2);

private slots:
    void handleData(int cmd,QByteArray data);

    void on_queryLimitButton_clicked();

    void on_queryRangeButton_clicked();

    void on_setRangeButton_clicked();

private:
    Ui::LeiDaWuWei *ui;
};

#endif // LEIDAWUWEI_H
