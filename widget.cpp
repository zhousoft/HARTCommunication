#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
#include "twolinewindow.h"
#include "titlewidget.h"
#include "twolinemainwidget.h"
#include "globalvalue.h"
#include "mainwindow.h"
#include "offlinewindow.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    serial = new QSerialPort(this);
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())//自动检测可用串口
    {
        ui->portNameComboBox->addItem(info.portName());
    }
    if(ui->portNameComboBox->count() == 0)//没有可用串口
    {
       ui->portNameComboBox->addItem(tr("未检测到有效串口"));
    }
    ui->companyInfoLabel->setOpenExternalLinks(true);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connectButton_clicked()
{

       //判断设备类型
    switch (ui->deviceTypeComboBox->currentIndex())
    {
    case 0:
        GlobalValue::deviceType = GlobalValue::PULSERADAR;
        GlobalValue::address = ui->addressComboBox->currentIndex();
        break;
    case 1:
        GlobalValue::deviceType = GlobalValue::TOWLINESOUND;
        GlobalValue::address = 0;
        break;
    default:
        break;
    }
    //判断主机类型
    switch (ui->hostTyoeComboBox->currentIndex())
    {
    case 0:
        GlobalValue::hostType = GlobalValue::FIRTST_HOST;//0- 第一主机
        break;
    case 1:
        GlobalValue::hostType = GlobalValue::SECOND_HOST;//1-第二主机
    default:
        break;
    }

    //判断连接类型
    switch (ui->connectTypeComboBox->currentIndex())
    {
        case 0:
        {
            GlobalValue::connectStatus = GlobalValue::LOCAL;
            serial->setPortName(ui->portNameComboBox->currentText());
            serial->setBaudRate(QSerialPort::Baud1200);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::OddParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            if(serial->open(QIODevice::ReadWrite))//打开串口
            {

                serial->close();
                MainWindow *mainWindow = new MainWindow(serial->portName());
                serial = NULL;
                mainWindow->show();
                this->close();
            }
            else
            {
               QMessageBox::critical(this,tr("连接失败！"),QString(tr("串口打开失败，错误原因：\n"))+serial->errorString());
            }
        }
        break;
        case 1:
        {
            GlobalValue::connectStatus = GlobalValue::OFFLINE;
            OfflineWindow *offlineWindow = new OfflineWindow();
            this->close();
            offlineWindow->show();
        }
        break;
        default:
        break;
    }  
}


void Widget::on_deviceTypeComboBox_currentIndexChanged(int index)
{
    ui->protocolComboBox->setCurrentIndex(index);
}

void Widget::on_protocolComboBox_currentIndexChanged(int index)
{
    ui->deviceTypeComboBox->setCurrentIndex(index);
}

void Widget::on_quitButton_clicked()
{
    this->close();
}
