#include "currentwidget.h"
#include "ui_currentwidget.h"
#include <QMessageBox>
#include <QDebug>
CurrentWidget::CurrentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentWidget)
{
    ui->setupUi(this);
}

CurrentWidget::~CurrentWidget()
{
    delete ui;
}

void CurrentWidget::handleData(int cmd, QByteArray data)
{
    qDebug()<<cmd<<"current handle: "<<data.count()<<":"<<data.toHex();
    union
    {
        unsigned char cvalue[4];
        float fvalue;
    }tempData;
    unsigned char tempChar;
    unsigned char receiveData[25];
    for(int i=0;i<data.count();i++)
    {
        receiveData[i] = (unsigned char)data.at(i);
    }
    switch (cmd) {
    case 2:
        tempData.cvalue[3] = receiveData[0];//大端模式，高位在前
        tempData.cvalue[2] = receiveData[1];
        tempData.cvalue[1] = receiveData[2];
        tempData.cvalue[0] = receiveData[3];
        ui->currentlineEdit->setText(QString::number(tempData.fvalue));
        tempData.cvalue[3] = receiveData[4];//大端模式，高位在前
        tempData.cvalue[2] = receiveData[5];
        tempData.cvalue[1] = receiveData[6];
        tempData.cvalue[0] = receiveData[7];
        tempChar = tempData.fvalue*100;
        ui->currentPercentlineEdit->setText(QString::number(tempChar)+"%");
        break;
        break;
    default:
        break;
    }

}

//查询电流及百分比
void CurrentWidget::on_queryCurrentButton_clicked()
{
    currentCmd = 2;
    emit sendCmd(currentCmd);
}

//选择框内设置固定电流
void CurrentWidget::on_currentChoiceButton_clicked()
{
    float tempfloat;
    currentCmd = 40;
    tempfloat = ui->currentChoicecomboBox->currentIndex()*4+4;
    emit sendCmd(currentCmd,tempfloat);
}
//自定义固定电流设置
void CurrentWidget::on_currentSetButton_clicked()
{
    float tempflaot;
    bool ok;
    currentCmd = 40;
    tempflaot = ui->currentSetlineEdit->text().toFloat(&ok);
    if(ok)
    {
        if(tempflaot>=4 && tempflaot <=20)
        {
            emit sendCmd(currentCmd,tempflaot);
        }
        else
        {
            QMessageBox::critical(this,tr("无效设置"),tr("电流设置范围为4-20mA"));
        }

    }
    else
    {
        QMessageBox::critical(this,tr("无效设置"),tr("请输入有效的电流数字！"));
    }

}
//退出固定电流模式-设置电流值为0时退出
void CurrentWidget::on_currentQuiteButton_clicked()
{
    float tempfloat = 0;
    currentCmd = 40;
    emit sendCmd(currentCmd,tempfloat);
}
//电流输出下限校验
void CurrentWidget::on_currentLowSetButton_clicked()
{
    float tempfloat;
    bool ok;
    currentCmd = 45;
    tempfloat = ui->currentLowSetlineEdit->text().toFloat(&ok);
    if(ok)
    {
        if(tempfloat>=2 && tempfloat<=6)
        {
            emit sendCmd(currentCmd,tempfloat);
        }
        else
        {
            QMessageBox::critical(this,tr("无效设置"),tr("电流设置范围为2-6！"));
        }
    }
    else
    {
        QMessageBox::critical(this,tr("无效设置"),tr("请输入有效的电流数字"));
    }
}

//电流输出上限校验
void CurrentWidget::on_currentUpSetButton_clicked()
{
    float tempfloat;
    bool ok;
    currentCmd = 46;
    tempfloat = ui->currentUpSetlineEdit->text().toFloat(&ok);
    if(ok)
    {
        if(tempfloat>=18 && tempfloat<=22)
        {
            emit sendCmd(currentCmd,tempfloat);
        }
        else
        {
            QMessageBox::critical(this,tr("无效设置"),tr("电流设置范围为18-22"));
        }
    }
    else
    {
        QMessageBox::critical(this,tr("无效设置"),tr("请输入正确的电流值"));
    }
}
