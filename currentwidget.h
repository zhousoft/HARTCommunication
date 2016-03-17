#ifndef CURRENTWIDGET_H
#define CURRENTWIDGET_H

#include <QWidget>
#include <QByteArray>
namespace Ui {
class CurrentWidget;
}

class CurrentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentWidget(QWidget *parent = 0);
    ~CurrentWidget();

signals:
    void sendCmd(int cmd);
    void sendCmd(int cmd,float farg);

private slots:
    void handleData(int cmd,QByteArray data);
    void on_queryCurrentButton_clicked();

    void on_currentChoiceButton_clicked();

    void on_currentSetButton_clicked();

    void on_currentQuiteButton_clicked();

    void on_currentLowSetButton_clicked();

    void on_currentUpSetButton_clicked();

private:
    Ui::CurrentWidget *ui;
    int currentCmd;
};

#endif // CURRENTWIDGET_H
