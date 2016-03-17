#include "rightinfowidget.h"
#include "ui_rightinfowidget.h"
#include "globalvalue.h"
RightInfoWidget::RightInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightInfoWidget)
{
    ui->setupUi(this);
    switch (GlobalValue::deviceType)
    {
    case GlobalValue::TOWLINESOUND:
        ui->devicNameLabel->setText(tr("二线制超声波物（液）位计"));
        ui->argLabel_1->setText(tr("液位(米)："));
        ui->argLabel_2->setText(tr("空距(米)："));
        ui->argLabel_3->setText(tr("量程(米)："));
        ui->argLabel_4->setText(tr("安装高度(米)："));
        break;
    case GlobalValue::PULSERADAR:
        ui->devicNameLabel->setText(tr("超声波脉冲雷达物（液）位计"));
        break;
    default:
        break;
    }
    switch (GlobalValue::hostType)
    {
    case GlobalValue::FIRTST_HOST:
        ui->hostTypeLineEdit->setText(tr("第一主机"));
        break;
    case GlobalValue::SECOND_HOST:
        ui->hostTypeLineEdit->setText(tr("第二主机"));
        break;
    default:
        break;
    }
    switch (GlobalValue::connectStatus)
    {
    case GlobalValue::LOCAL:
        ui->connectLineEdit->setText(tr("本地连接"));
        break;
    case GlobalValue::OFFLINE:
        ui->connectLineEdit->setText(tr("离线连接"));
        break;
    default:
        break;
    }
}

RightInfoWidget::~RightInfoWidget()
{
    delete ui;
}



void RightInfoWidget::on_reConnectButton_clicked()
{
    emit reConnect();
}

void RightInfoWidget::on_updateButton_clicked()
{
    emit sendCmd(3);
}

void RightInfoWidget::handleData(int cmd, QByteArray data)
{
    union
    {
        unsigned char cvalue[4];
        float fvalue;
    }tempdata;
    switch (cmd)
    {
    case 3:
        tempdata.cvalue[3] = (unsigned char)data.at(5);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(6);
        tempdata.cvalue[1] = (unsigned char)data.at(7);
        tempdata.cvalue[0] = (unsigned char)data.at(8);
        ui->argLineEdit_1->setText(QString::number(tempdata.fvalue));//主变量
        tempdata.cvalue[3] = (unsigned char)data.at(10);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(11);
        tempdata.cvalue[1] = (unsigned char)data.at(12);
        tempdata.cvalue[0] = (unsigned char)data.at(13);
        ui->argLineEdit_2->setText(QString::number(tempdata.fvalue));//第二变量
        tempdata.cvalue[3] = (unsigned char)data.at(15);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(16);
        tempdata.cvalue[1] = (unsigned char)data.at(17);
        tempdata.cvalue[0] = (unsigned char)data.at(18);
        ui->argLineEdit_3->setText(QString::number(tempdata.fvalue));//第三变量
        tempdata.cvalue[3] = (unsigned char)data.at(20);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(21);
        tempdata.cvalue[1] = (unsigned char)data.at(22);
        tempdata.cvalue[0] = (unsigned char)data.at(23);
        ui->argLineEdit_4->setText(QString::number(tempdata.fvalue));//第四变量
        break;
    default:
        break;
    }
}
