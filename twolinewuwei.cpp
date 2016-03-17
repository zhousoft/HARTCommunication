#include "twolinewuwei.h"
#include "ui_twolinewuwei.h"
#include <QMessageBox>

TwoLineWuWei::TwoLineWuWei(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoLineWuWei)
{
    ui->setupUi(this);
}

TwoLineWuWei::~TwoLineWuWei()
{
    delete ui;
}

void TwoLineWuWei::handleData(int cmd, QByteArray data)
{
    union
    {
        unsigned char cvalue[4];
        float fvalue;
    }tempdata;
    switch (cmd)
    {
    case 14://读主变量传感器信息-测量上限、盲区
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
        break;
    case 15://读主变量输出信息
        tempdata.cvalue[3] = (unsigned char)data.at(3);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(4);
        tempdata.cvalue[1] = (unsigned char)data.at(5);
        tempdata.cvalue[0] = (unsigned char)data.at(6);
        ui->dheightLineEdit->setText(QString::number(tempdata.fvalue));//量程上限
        tempdata.cvalue[3] = (unsigned char)data.at(7);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(8);
        tempdata.cvalue[1] = (unsigned char)data.at(9);
        tempdata.cvalue[0] = (unsigned char)data.at(10);
        ui->gheightLineEdit->setText(QString::number(tempdata.fvalue));
        break;
    default:
        break;
    }
}

void TwoLineWuWei::on_queryLimitButton_clicked()
{
    emit sendCmd(14);
}

void TwoLineWuWei::on_queryRangeButton_clicked()
{
    emit setCmdPage(0);//多个页面发送同意命令时判断发送页面
    emit sendCmd(15);
}



void TwoLineWuWei::on_setRangeButton_clicked()
{
    float temfloat1,temfloat2;
    bool ok;
    temfloat1 = ui->dheightLineEdit->text().toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this,tr("无效操作"),tr("请输入有效的量程数值!"));
        return;
     }
     temfloat2 = ui->gheightLineEdit->text().toFloat(&ok);
     if(!ok)
     {
         QMessageBox::critical(this,tr("无效操作"),tr("请输入有效的安装高度!"));
         return;
     }
     emit sendCmd(35,temfloat1,temfloat2);


}
