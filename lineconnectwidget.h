#ifndef LINECONNECTWIDGET_H
#define LINECONNECTWIDGET_H

#include <QWidget>

namespace Ui {
class LineConnectWidget;
}

class LineConnectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LineConnectWidget(QWidget *parent = 0);
    ~LineConnectWidget();

private:
    Ui::LineConnectWidget *ui;
};

#endif // LINECONNECTWIDGET_H
