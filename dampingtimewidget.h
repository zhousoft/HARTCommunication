#ifndef DAMPINGTIMEWIDGET_H
#define DAMPINGTIMEWIDGET_H

#include <QWidget>

namespace Ui {
class DampingTimeWidget;
}

class DampingTimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DampingTimeWidget(QWidget *parent = 0);
    ~DampingTimeWidget();

signals:
    void sendCmd(int cmd);
    void sendCmd(int cmd,float farg);
    void setCmdPage(int cmdPage);

private slots:
    void handleData(int cmd,QByteArray data);
    void on_queryButton_clicked();

    void on_setButton_clicked();

private:
    Ui::DampingTimeWidget *ui;
};

#endif // DAMPINGTIMEWIDGET_H
