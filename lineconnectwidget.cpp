#include "lineconnectwidget.h"
#include "ui_lineconnectwidget.h"

LineConnectWidget::LineConnectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineConnectWidget)
{
    ui->setupUi(this);
}

LineConnectWidget::~LineConnectWidget()
{
    delete ui;
}
