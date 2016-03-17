#include "otherwidget.h"
#include "ui_otherwidget.h"
#include <QMessageBox>
OtherWidget::OtherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherWidget)
{
    ui->setupUi(this);
}

OtherWidget::~OtherWidget()
{
    delete ui;
}

void OtherWidget::handleData(int cmd, QByteArray data)
{
    switch (cmd)
    {
    case 16:
        {
            union
            {
                unsigned char cvalue[4];
                qint32 ivalue;

            }tempdata;
            tempdata.cvalue[3] = 0;//大端模式 高位在前
            tempdata.cvalue[2] = (unsigned char)data.at(0);
            tempdata.cvalue[1] = (unsigned char)data.at(1);
            tempdata.cvalue[0] = (unsigned char)data.at(2);
            ui->numberEdit->setText(QString::number(tempdata.ivalue));
        }
        break;
    default:
        break;
    }
}

void OtherWidget::on_queryButton_clicked()
{
    emit sendCmd(16);
}

void OtherWidget::on_setButton_clicked()
{
    bool ok;
    qint32 value = ui->numberEdit->text().toInt(&ok);
    if(ok)
    {
        if(value > 0x00FFFFFF)//工位号为3个字节
        {
            QMessageBox::critical(this,tr("无效操作"),tr("不支持的工位号!"));
            return;
        }
    }
    else
    {
        QMessageBox::critical(this,tr("无效操作"),tr("请输入有效的工位号!"));
        return;
     }
     emit sendCmd(19,value);

}
