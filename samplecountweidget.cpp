#include "samplecountweidget.h"
#include "ui_samplecountweidget.h"
#include <QMessageBox>
SampleCountWeidget::SampleCountWeidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SampleCountWeidget)
{
    ui->setupUi(this);
}

SampleCountWeidget::~SampleCountWeidget()
{
    delete ui;
}

void SampleCountWeidget::handleData(int cmd, QByteArray data)
{
    switch (cmd) {
    case 15:
        {
            union
            {
                unsigned char cvalue[4];
                float fvalue;
            }tempdata;
            //11-14为采样个数 （原阻尼值）
            tempdata.cvalue[3] = (unsigned char)data.at(11);//大端模式 高位在前
            tempdata.cvalue[2] = (unsigned char)data.at(12);
            tempdata.cvalue[1] = (unsigned char)data.at(13);
            tempdata.cvalue[0] = (unsigned char)data.at(14);
            ui->lineEdit->setText(QString::number(tempdata.fvalue));
        }
        break;
    case 34://写采样时间
        ui->lineEdit->clear();//设置失败 清空
        break;
    default:
        break;
    }
}

void SampleCountWeidget::on_queryButton_clicked()
{
    emit setCmdPage(1);
    emit sendCmd(15);
}

void SampleCountWeidget::on_setButton_clicked()
{
    bool ok;
    float value;
    value = ui->lineEdit->text().toFloat(&ok);
    if(ok)
    {
        if((qint32)value%2 == 0)
        {
           QMessageBox::critical(this,tr("无效操作"),tr("采样数值请输入奇数!"));
        }
        else
        {
            emit sendCmd(34,value);
        }
    }
    else
    {
        QMessageBox::critical(this,tr("无效操作"),tr("请输入有效的采样数值!"));
    }
}
