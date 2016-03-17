#include "leidawuwei.h"
#include "ui_leidawuwei.h"
#include <QMessageBox>
#include <QDebug>
LeiDaWuWei::LeiDaWuWei(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeiDaWuWei)
{
    ui->setupUi(this);
}

LeiDaWuWei::~LeiDaWuWei()
{
    delete ui;
}

void LeiDaWuWei::handleData(int cmd, QByteArray data)
{
    qDebug()<<"leida handle";
    union
    {
        unsigned char cvalue[4];
        float fvalue;
    }tempdata;
    switch (cmd)
    {
    case 14://读主变量传感器信息-测量上限、下限、精度
        tempdata.cvalue[3] = (unsigned char)data.at(4);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(5);
        tempdata.cvalue[1] = (unsigned char)data.at(6);
        tempdata.cvalue[0] = (unsigned char)data.at(7);
        ui->upLimitLineEdit->setText(QString::number(tempdata.fvalue));
        tempdata.cvalue[3] = (unsigned char)data.at(8);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(9);
        tempdata.cvalue[1] = (unsigned char)data.at(10);
        tempdata.cvalue[0] = (unsigned char)data.at(11);
        ui->lowLimitLineEdit->setText(QString::number(tempdata.fvalue));
        tempdata.cvalue[3] = (unsigned char)data.at(12);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(13);
        tempdata.cvalue[1] = (unsigned char)data.at(14);
        tempdata.cvalue[0] = (unsigned char)data.at(15);
        ui->spanlineEdit->setText(QString::number(tempdata.fvalue));
        break;
    case 15://读主变量输出信息
        tempdata.cvalue[3] = (unsigned char)data.at(3);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(4);
        tempdata.cvalue[1] = (unsigned char)data.at(5);
        tempdata.cvalue[0] = (unsigned char)data.at(6);
        ui->upRangeLineEdit->setText(QString::number(tempdata.fvalue));//量程上限
        tempdata.cvalue[3] = (unsigned char)data.at(7);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(8);
        tempdata.cvalue[1] = (unsigned char)data.at(9);
        tempdata.cvalue[0] = (unsigned char)data.at(10);
        ui->lowRangeLineEdit->setText(QString::number(tempdata.fvalue));//量程下限
        break;
    default:
        break;
    }

}

void LeiDaWuWei::on_queryLimitButton_clicked()
{
    emit setCmdPage(0);
    emit sendCmd(14);
}

void LeiDaWuWei::on_queryRangeButton_clicked()
{
    emit setCmdPage(0);
    emit sendCmd(15);
}


void LeiDaWuWei::on_setRangeButton_clicked()
{
    float temfloat1,temfloat2;
    bool ok;
    temfloat1 = ui->upRangeLineEdit->text().toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this,tr("无效操作"),tr("请输入有效的量程上限!"));
        return;
     }
     temfloat2 = ui->lowRangeLineEdit->text().toFloat(&ok);
     if(!ok)
     {
         QMessageBox::critical(this,tr("无效操作"),tr("请输入有效的量程下限!"));
         return;
     }
     emit sendCmd(35,temfloat1,temfloat2);
}
