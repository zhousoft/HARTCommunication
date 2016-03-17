#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"
#include "hartasciihandle.h"
#include <QDebug>

DeviceInfoWidget::DeviceInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceInfoWidget)
{
    ui->setupUi(this);
}

DeviceInfoWidget::~DeviceInfoWidget()
{
    delete ui;
}

void DeviceInfoWidget::handleData(int cmd, QByteArray data)
{
    qDebug()<<"device:"<<data.count()<<" "<<data.toHex();
    switch (cmd)
    {
    case 12://读消息
        {
            QByteArray result;
            HartASCIIHandle::DecompressASCString(data,result);
            qDebug()<<result.count()<<" "<<result;
            ui->MsglineEdit->setText(QString(result));
            ui->MsglineEdit->setCursorPosition(0);//最开头开始显示
        }
        break;
    case 13://读tag，描述符和日期
        {
            QByteArray tempdata,result;
            unsigned char temChar;
            for(int i=0;i<=5;i++)//字节0-5 TAG
            {
                tempdata.append(data.at(i));
            }
            HartASCIIHandle::DecompressASCString(tempdata,result);
            ui->tagLineEdit->setText(QString(result));
            ui->tagLineEdit->setCursorPosition(0);
            tempdata.clear();
            result.clear();
            for(int i=6;i<=17;i++)
            {
                tempdata.append(data.at(i));
            }
            HartASCIIHandle::DecompressASCString(tempdata,result);
            ui->descriLineEdit->setText(QString(result));
            ui->descriLineEdit->setCursorPosition(0);
            temChar = (unsigned char)data.at(18);
            ui->dateDayLineEdit->setText(QString::number(temChar));
            temChar = (unsigned char)data.at(19);
            ui->DateMoonLineEdit->setText(QString::number(temChar));
            temChar = (unsigned char)data.at(20);
            ui->dateYearLineEdit->setText(QString::number(temChar+1900));
        }
        break;
    default:
        break;
    }
}

void DeviceInfoWidget::on_readTagButton_clicked()
{
    emit sendCmd(13);
}

void DeviceInfoWidget::on_readMsgButton_clicked()
{
    emit sendCmd(12);
}
