#ifndef TWOLINEWUWEI_H
#define TWOLINEWUWEI_H

#include <QWidget>

namespace Ui {
class TwoLineWuWei;
}

class TwoLineWuWei : public QWidget
{
    Q_OBJECT

public:
    explicit TwoLineWuWei(QWidget *parent = 0);
    ~TwoLineWuWei();

signals:
    void sendCmd(int cmd);
    void sendCmd(int cmd,float farg1,float farg2);
    void setCmdPage(int cmdPage);

private slots:
    void handleData(int cmd,QByteArray data);

    void on_queryLimitButton_clicked();

    void on_queryRangeButton_clicked();

    void on_setRangeButton_clicked();

private:
    Ui::TwoLineWuWei *ui;
};

#endif // TWOLINEWUWEI_H
