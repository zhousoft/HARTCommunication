#include "dampingtimewidget.h"
#include "ui_dampingtimewidget.h"
#include <QMessageBox>
#include <QDebug>
DampingTimeWidget::DampingTimeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DampingTimeWidget)
{
    ui->setupUi(this);
}

DampingTimeWidget::~DampingTimeWidget()
{
    delete ui;
}

void DampingTimeWidget::handleData(int cmd, QByteArray data)
{
    switch (cmd) {
    case 15:
        {
            union
            {
                unsigned char cvalue[4];
                float fvalue;
            }tempdata;
            //11-14为阻尼值 单位秒
            tempdata.cvalue[3] = (unsigned char)data.at(11);//大端模式 高位在前
            tempdata.cvalue[2] = (unsigned char)data.at(12);
            tempdata.cvalue[1] = (unsigned char)data.at(13);
            tempdata.cvalue[0] = (unsigned char)data.at(14);
            ui->lineEdit->setText(QString::number(tempdata.fvalue));
        }
        break;
    case 34://写阻尼
        ui->lineEdit->clear();//设置失败 清空
        break;
    default:
        break;
    }
}

void DampingTimeWidget::on_queryButton_clicked()
{
    emit setCmdPage(1);
    emit sendCmd(15);
}

void DampingTimeWidget::on_setButton_clicked()
{
    bool ok;
    float value = ui->lineEdit->text().toFloat(&ok);
    if(ok)
    {
        emit sendCmd(34,value);
    }
    else
    {
        QMessageBox::critical(this,tr("无效设置"),tr("请输入有效的阻尼时间！"));
    }
}
