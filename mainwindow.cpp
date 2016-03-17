#include "mainwindow.h"
#include <QDebug>
#include <QStatusBar>
#include "deviceinfowidget.h"
#include "processmonitorwidget.h"
#include "currentwidget.h"
#include "twolinewuwei.h"
#include "samplecountweidget.h"
#include "otherwidget.h"
#include "lineconnectwidget.h"
#include "rightinfowidget.h"
#include "leidawuwei.h"
#include "dampingtimewidget.h"
#include "globalvalue.h"
#include "widget.h"
#include <QMessageBox>
#include <QSettings>

//#define DEBUG
MainWindow::MainWindow(QString portName,QWidget *parent) : QMainWindow(parent)
{

    setMinimumSize(1000,600);
    setWindowTitle(tr("HART仪表调试软件"));
    setAttribute(Qt::WA_DeleteOnClose);//关闭窗口时删除窗口
   //----------串口相关初始化---
    cmdPage = 0;
    preCount = 5;
    createID = 0x01;//制造商ID
    sensorType =0x02;//传感器类型
    deveiceNumber[0] = 0x08;//传感器序列号（设备标识号）
    deveiceNumber[1] = 0x09;
    deveiceNumber[2] = 0x75;
    deveiceTag[0] = 0X1D;//设备标记符（tag）
    deveiceTag[1] = 0X9C;
    deveiceTag[2] = 0XA0;
    deveiceTag[3] = 0X82;
    deveiceTag[4] = 0X08;
    deveiceTag[5] = 0X20;
    isReceving = false;
    seriaport = NULL;
    seriaport = new QSerialPort();
    seriaport->setPortName(portName);
    seriaport->setBaudRate(QSerialPort::Baud1200);
    seriaport->setDataBits(QSerialPort::Data8);
    seriaport->setParity(QSerialPort::OddParity);
    seriaport->setStopBits(QSerialPort::OneStop);
    seriaport->setFlowControl(QSerialPort::NoFlowControl);
    if(seriaport->open(QIODevice::ReadWrite))//打开串口
    {
#ifdef DEBUG
        qDebug()<<"succuss";
#endif
    }
    else
    {
        this->statusBar()->showMessage(tr("串口打开失败,请重新连接！"));
    }
    connect(seriaport,SIGNAL(readyRead()),this,SLOT(readCom()));

    outTimer = new QTimer(this);
    receiveTimer = new QTimer(this);
    receiveTimer->setSingleShot(true);//只运行一次
    outTimer->setSingleShot(true);
    connect(outTimer,SIGNAL(timeout()),this,SLOT(communicateTimeOut()));
    connect(receiveTimer,SIGNAL(timeout()),this,SLOT(receiveEnd()));

    //------------------------------------

    stackedWidget = new QStackedWidget();
    titleWidget = new TitleWidget();


    QPalette palette;
    palette.setBrush(QPalette::Window,Qt::white);
    stackedWidget->setPalette(palette);
    stackedWidget->setAutoFillBackground(true);



    DeviceInfoWidget *deviceInfoWidget = new DeviceInfoWidget();
    ProcessMonitorWidget *processMonitorWidget = new ProcessMonitorWidget();
    CurrentWidget *currentWidget = new CurrentWidget();
    OtherWidget *otherWidget = new OtherWidget();
    LineConnectWidget *lineConnectWidget = new LineConnectWidget();



    //判断设备类型
    switch (GlobalValue::deviceType)
    {
    case GlobalValue::PULSERADAR:
        {
            //雷达物位页面
            LeiDaWuWei *leiDaWuWei = new LeiDaWuWei();
            stackedWidget->addWidget(leiDaWuWei);
            connect(leiDaWuWei,SIGNAL(setCmdPage(int)),this,SLOT(setCmdPage(int)));
            connect(leiDaWuWei,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
            connect(leiDaWuWei,SIGNAL(sendCmd(int,float,float)),this,SLOT(sendCmd(int,float,float)));
            connect(this,SIGNAL(sendleiDaWuWeiData(int,QByteArray)),leiDaWuWei,SLOT(handleData(int,QByteArray)));
        }
        break;
    case GlobalValue::TOWLINESOUND:
        {
            //二线制物位页面---------
            TwoLineWuWei *twoLineWuWei = new TwoLineWuWei();
            stackedWidget->addWidget(twoLineWuWei);
            connect(twoLineWuWei,SIGNAL(setCmdPage(int)),this,SLOT(setCmdPage(int)));
            connect(twoLineWuWei,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
            connect(twoLineWuWei,SIGNAL(sendCmd(int,float,float)),this,SLOT(sendCmd(int,float,float)));
            connect(this,SIGNAL(sendTwoLineWuWeiData(int,QByteArray)),twoLineWuWei,SLOT(handleData(int,QByteArray)));
        }
        break;
    default:
        break;
    }

    //输出电流页面---------------
    stackedWidget->addWidget(currentWidget);
    connect(currentWidget,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
    connect(currentWidget,SIGNAL(sendCmd(int,float)),this,SLOT(sendCmd(int,float)));
    connect(this,SIGNAL(sendCurrentData(int,QByteArray)),currentWidget,SLOT(handleData(int,QByteArray)));





    //判断设备类型
    switch (GlobalValue::deviceType)
    {
    case GlobalValue::PULSERADAR://脉冲雷达
        {
            //阻尼时间页面-----
            DampingTimeWidget *dampingTimeWidget = new DampingTimeWidget();
            stackedWidget->addWidget(dampingTimeWidget);
            connect(dampingTimeWidget,SIGNAL(setCmdPage(int)),this,SLOT(setCmdPage(int)));
            connect(dampingTimeWidget,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
            connect(dampingTimeWidget,SIGNAL(sendCmd(int,float)),this,SLOT(sendCmd(int,float)));
            connect(this,SIGNAL(sendDampingTimeData(int,QByteArray)),dampingTimeWidget,SLOT(handleData(int,QByteArray)));
        }
        break;
    case GlobalValue::TOWLINESOUND:
        {
            //采样次数页面------
            SampleCountWeidget *sampleCountWidget = new SampleCountWeidget();
            stackedWidget->addWidget(sampleCountWidget);
            connect(sampleCountWidget,SIGNAL(setCmdPage(int)),this,SLOT(setCmdPage(int)));
            connect(sampleCountWidget,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
            connect(sampleCountWidget,SIGNAL(sendCmd(int,float)),this,SLOT(sendCmd(int,float)));
            connect(this,SIGNAL(sendSampleCountData(int,QByteArray)),sampleCountWidget,SLOT(handleData(int,QByteArray)));
        }
        break;
    default:
        break;
    }
    //过程监测页面
    stackedWidget->addWidget(processMonitorWidget);
    connect(processMonitorWidget,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
    connect(this,SIGNAL(sendProcessMonitorData(int,QByteArray)),processMonitorWidget,SLOT(handleData(int,QByteArray)));

    //设备信息页面--------------
    stackedWidget->addWidget(deviceInfoWidget);
    connect(deviceInfoWidget,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
    connect(this,SIGNAL(sendDeviceInfoData(int,QByteArray)),deviceInfoWidget,SLOT(handleData(int,QByteArray)));

    //其他设置页面--------------
    stackedWidget->addWidget(otherWidget);
    connect(otherWidget,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
    connect(otherWidget,SIGNAL(sendCmd(int,qint32)),this,SLOT(sendCmd(int,qint32)));
    connect(this,SIGNAL(sendOtherSetData(int,QByteArray)),otherWidget,SLOT(handleData(int,QByteArray)));

    stackedWidget->addWidget(lineConnectWidget);

    //右侧信息界面---------
    RightInfoWidget *rightInfoWidget = new RightInfoWidget();
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(stackedWidget);
    centerLayout->setSpacing(0);
    centerLayout->addWidget(rightInfoWidget);
    centerLayout->setStretch(0,3);
    centerLayout->setStretch(1,1);
    centerLayout->setContentsMargins(0,0,0,0);

    connect(rightInfoWidget,SIGNAL(sendCmd(int)),this,SLOT(sendCmd(int)));
    connect(this,SIGNAL(sendRightInfoData(int,QByteArray)),rightInfoWidget,SLOT(handleData(int,QByteArray)));




    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleWidget);
    mainLayout->addLayout(centerLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    //QMainWindow默认已有布局管理器，不能直接setLayout
    QWidget *mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    this->statusBar()->showMessage("欢迎使用",10000);

    connect(titleWidget,SIGNAL(turnPage(int)),this,SLOT(turnPage(int)));
    connect(rightInfoWidget,SIGNAL(reConnect()),this,SLOT(reConnect()));
    titleWidget->turnPage("0");
    isFirstRun = true;
    sendCmd(0);

}

MainWindow::~MainWindow()
{
#ifdef DEBUG
    qDebug()<<"delete";
#endif
    if(seriaport != NULL)
    {
        if(seriaport->isOpen())
        {
            seriaport->close();
        }
        seriaport = NULL;
    }
}
//生成校验值
unsigned char MainWindow::ParityCheck(QList<unsigned char> &list)
{
  unsigned char result = 0;
  QList<unsigned char>::const_iterator itetor;//只读迭代器
  for(itetor = list.constBegin();itetor!=list.constEnd();++itetor)
  {
      result ^= *itetor;
  }
  return result;
}
//对收到的数据帧进行处理
void MainWindow::receiveDataHandle()
{
     unsigned char tempChar,startChar,startCount,cmd,datalen,status1,status2,checkChar;
     unsigned char receiveData[50];
     QList<unsigned char>checkList;
     QByteArray temarray,dataArray;
     receivebuf = seriaport->readAll();//读取串口缓冲区全部内容
#ifdef DEBUG
     qDebug()<<"handle buf:"<<receivebuf.count();
#endif
     seriaport->clear();
     union
     {
        unsigned char cvalue[4];
        float         fvalue;
        qint32           ivalue;
     }tempData,tempData2;
     int count;
     if(outTimer->isActive())//通讯延时计时器激活
     {
         outTimer->stop();
         if(!receivebuf.isEmpty())
         {
            for(count=0;count<receivebuf.count();count++)//前导符
            {
                 if((unsigned char)receivebuf.at(count) != 0xFF)
                 {
#ifdef DEBUG
                     qDebug()<<receivebuf.toHex();
#endif
                     break;
                 }
            }
            if(count>1 && count<receivebuf.count())
            {
                startCount = count;
                startChar = (unsigned char)receivebuf.at(count++);//得到起始字节
                switch (startChar)//判断短帧长帧结构
                {
                case 0x06://短帧
                    tempChar = (unsigned char)receivebuf.at(count);//获得地址字节
                    //地址待处理
                    count++;
                    break;
                case 0x86://长帧
                    count+=5;//跳过地址字节-暂不分析地址
                    break;
                default:
                    break;
                }
                if(receivebuf.count()<count+4)//不满足最小长度帧要求
                {
#ifdef DEBUG
                    qDebug()<<tr("通讯失败：不满足最小长度帧要求");
#else
                    qDebug()<<tr("通讯失败");
#endif
                    receivebuf.clear();
                    isReceving = false;
                    return;
                }

                cmd = (unsigned char)receivebuf.at(count++);//获得命令

                datalen = (unsigned char)receivebuf.at(count++);//获得数据长度
                if(datalen < 2)//从机返回数据帧数据长度最小为2（两个状态码）
                {
#ifdef DEBUG
                    qDebug()<<tr("通讯失败：数据长度小于2");
#else
                    qDebug()<<tr("通讯失败");
#endif
                    receivebuf.clear();
                    isReceving = false;
                    return;
                }
                status1 = (unsigned char)receivebuf.at(count++);
                status2 = (unsigned char)receivebuf.at(count++);
                if(receivebuf.count()>datalen+2+5)
                {
                    //防止QByteArray越界，原因待查，可能跟线程有关
                    //发送频率太快接受时中间会丢数据，原因待查
                }
                else
                {
#ifdef DEBUG
                    qDebug()<<tr("通讯失败：丢失尾部数据");
#else
                    qDebug()<<tr("通讯失败,丢失尾部数据");
#endif
                    receivebuf.clear();
                    isReceving = false;
                    return;
                }
                for(int i=0;i<datalen-2;i++)
                {
                    receiveData[i] = (unsigned char)receivebuf.at(count);//获取数据
                    dataArray.append(receivebuf.at(count));
                    count++;
                }
                checkChar = (unsigned char)receivebuf.at(count);//获取校验字节
                for(int i=0;i<count-startCount;i++)
                {
                    checkList <<  (unsigned char)receivebuf.at(startCount+i);
                }
#ifdef DEBUG
                tempChar = count - startCount;
                qDebug()<<tempChar;
#endif
                tempChar = ParityCheck(checkList);
#ifdef DEBUG
                qDebug()<<tempChar;
#endif
                //校验
                if(checkChar != tempChar)//校验错误
                {
                    qDebug()<<tr("校验错误，通讯失败");
                    receivebuf.clear();
                    isReceving = false;
                    return;
                }
                switch(status1)//检查状态字节
                {
                case 0x00://通讯正常
                    break;
                case 0x88://从机校验错误，请求重发消息
                    receivebuf.clear();
                    isReceving = false;
                    switch(cmd)
                    {
                    case 0:
                    case 2:
                    case 3:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                        sendCmd(cmd);
                        break;
                    case 19:
                        sendCmd(cmd,sendint);
                        break;
                    case 34:
                    case 40:
                    case 45:
                    case 46:
                      //  sendMsg(cmd,sendfData1);
                        break;
                    case 35:
                      //  sendMsg(cmd,sendfData1,sendfData2);
                        break;
                    }
                    return;
                    break;
                case 0x09://未进入固定电流
                    receivebuf.clear();
                    isReceving = false;
                    if(cmd == 45)
                    {                      
                        this->statusBar()->showMessage(tr("校准失败，进行输出低电流校准前必需先设置固定4mA输出"));
                        return;
                    }
                    if(cmd == 46)
                    {
                        this->statusBar()->showMessage(tr("校准失败，进行输出高电流校准前必须先设置固定20mA输出"));
                        return;
                    }
                    break;
                }
                switch (cmd)//判断命令进行处理
                {
                case 0:

                    createID = (unsigned char)receiveData[1];//制造商ID
                    sensorType = (unsigned char)receiveData[2];//传感器类型
                    preCount = receiveData[3];//前导符个数
                    for(int i=0;i<3;i++)
                    {
                        deveiceNumber[i] = receiveData[9+i];//获得传感器序列号（设备标识）
                    }
                    if(isFirstRun)
                    {
                        this->statusBar()->showMessage(tr("查询设备成功..."),5000);
                        isFirstRun = false;
                    }
                    break;
                case 2://读主变量电流和百分比
                    if(dataArray.count() < 8)
                    {
                        this->statusBar()->showMessage(tr("通讯错误，回传数据帧不足"));
                    }
                    else
                    {
                        emit sendCurrentData(2,dataArray);//转发给电流页面处理
                        this->statusBar()->showMessage(tr("通讯成功"),1000);
                    }
                    break;
                case 3://读动态变量和主变量电流
                    if(dataArray.count() < 24)
                    {
                        this->statusBar()->showMessage(tr("通讯错误，回传数据帧不足"));
                    }
                    else
                    {
                        emit sendProcessMonitorData(cmd,dataArray);
                        emit sendRightInfoData(cmd,dataArray);
                        this->statusBar()->showMessage(tr("通讯成功"),1000);
                    }
                    break;
                case 12://读消息
                    emit sendDeviceInfoData(cmd,dataArray);
                    this->statusBar()->showMessage(tr("通讯成功"),1000);
                    break;
                case 13://读TAG....
                    emit sendDeviceInfoData(cmd,dataArray);
                    this->statusBar()->showMessage(tr("通讯成功"),1000);
                    break;
                case 14://读主变量传感器信息
                    if(dataArray.count() < 16)
                    {
                        this->statusBar()->showMessage(tr("通讯错误，回传数据帧不足"));
                    }
                    else
                    {
                        switch (GlobalValue::deviceType)
                        {
                        case GlobalValue::PULSERADAR://雷达
                            emit sendleiDaWuWeiData(cmd,dataArray);
                            break;
                        case GlobalValue::TOWLINESOUND://二线
                            emit sendTwoLineWuWeiData(cmd,dataArray);
                            break;
                        default:
                            break;
                        }
                        this->statusBar()->showMessage(tr("通讯成功"),1000);
                    }
                    break;
                case 15://主变量输出信息 -(区分设备)
                    if(dataArray.count() < 17)
                    {
                        this->statusBar()->showMessage(tr("通讯错误，回传数据帧不足"));
                    }
                    else
                    {
                        switch (GlobalValue::deviceType)
                        {
                        case GlobalValue::PULSERADAR:
                            switch (cmdPage)
                            {
                            case 0://物位页面
                                emit sendleiDaWuWeiData(cmd,dataArray);
                                break;
                            case 1://阻尼页面
                                emit sendDampingTimeData(cmd,dataArray);
                                break;
                            default:
                                break;
                            }
                            break;
                        case GlobalValue::TOWLINESOUND:
                            switch (cmdPage) //判断哪个页面
                            {
                            case 0:
                                emit sendTwoLineWuWeiData(cmd,dataArray);
                                break;
                            case 1:
                                 emit sendSampleCountData(cmd,dataArray);
                                break;
                            default:
                                break;
                            }

                            break;
                        default:
                            break;
                        }
                        this->statusBar()->showMessage(tr("通讯成功"),1000);
                    }
                    break;
                case 16://读工位号
                    if(dataArray.count() < 3)
                    {
                        this->statusBar()->showMessage(tr("通讯错误，回传数据帧不足"));
                    }
                    else
                    {
                        emit sendOtherSetData(cmd,dataArray);
                        this->statusBar()->showMessage(tr("通讯成功"),1000);
                    }
                    break;
                case 19://写最后装配号-工位号
                    tempData.cvalue[3] = 0;//大端模式，高位在前
                    tempData.cvalue[2] = receiveData[0];
                    tempData.cvalue[1] = receiveData[1];
                    tempData.cvalue[0] = receiveData[2];
                    if(tempData.ivalue == sendint)
                    {
                        this->statusBar()->showMessage(tr("设置成功"),1000);
                    }
                    else
                    {
                       this->statusBar()->showMessage(tr("设置失败"));
                    }
                    break;
                case 34://写阻尼
                    tempData.cvalue[3] = receiveData[0];//大端模式，高位在前
                    tempData.cvalue[2] = receiveData[1];
                    tempData.cvalue[1] = receiveData[2];
                    tempData.cvalue[0] = receiveData[3];
                    if(tempData.fvalue == sendfData1)
                    {
                        this->statusBar()->showMessage(tr("设置成功"),1000);
                    }
                    else
                    {
                        this->statusBar()->showMessage(tr("设置失败"));
                        switch (GlobalValue::deviceType)
                        {
                        case GlobalValue::PULSERADAR:
                            emit sendDampingTimeData(cmd,dataArray);
                            break;
                        case GlobalValue::TOWLINESOUND:
                            emit sendSampleCountData(cmd,dataArray);
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                case 35://写量程上下限
                    tempData.cvalue[3] = receiveData[1];//大端模式，高位在前
                    tempData.cvalue[2] = receiveData[2];
                    tempData.cvalue[1] = receiveData[3];
                    tempData.cvalue[0] = receiveData[4];
                    tempData2.cvalue[3] = receiveData[5];//大端模式，高位在前
                    tempData2.cvalue[2] = receiveData[6];
                    tempData2.cvalue[1] = receiveData[7];
                    tempData2.cvalue[0] = receiveData[8];
                    if(tempData.fvalue == sendfData1 && tempData2.fvalue == sendfData2)
                    {
                        switch (GlobalValue::deviceType)
                        {
                        case GlobalValue::PULSERADAR:
                            emit sendleiDaWuWeiData(cmd,dataArray);
                            break;
                        case GlobalValue::TOWLINESOUND:
                            emit sendTwoLineWuWeiData(cmd,dataArray);
                            break;
                        default:
                            break;
                        }
                        this->statusBar()->showMessage(tr("设置成功"),1000);
                    }
                    else
                    {
                        this->statusBar()->showMessage(tr("设置成功"));
                    }
                    break;
                case 40://设置固定电流
                    tempData.cvalue[3] = receiveData[0];//大端模式，高位在前
                    tempData.cvalue[2] = receiveData[1];
                    tempData.cvalue[1] = receiveData[2];
                    tempData.cvalue[0] = receiveData[3];
                    if(tempData.fvalue == sendfData1)
                    {
                       sendCurrentData(cmd,dataArray);
                       this->statusBar()->showMessage(tr("通讯成功"),1000);
                    }
                    else
                    {
                       this->statusBar()->showMessage(tr("设置固定电流失败"));
                    }
                    break;
                case 45://电流输出下限校准
                    tempData.cvalue[3] = receiveData[0];//大端模式，高位在前
                    tempData.cvalue[2] = receiveData[1];
                    tempData.cvalue[1] = receiveData[2];
                    tempData.cvalue[0] = receiveData[3];
#ifdef DEBUG
                    qDebug()<<tempData.fvalue;
#endif
                    if(tempData.fvalue == 4)
                    {
                        this->statusBar()->showMessage(tr("校准完成"),1000);
                    }
                    else
                    {
                        this->statusBar()->showMessage(tr("校准失败"));
                    }
                    break;
                case 46://电流输出上限校准
                    tempData.cvalue[3] = receiveData[0];//大端模式，高位在前
                    tempData.cvalue[2] = receiveData[1];
                    tempData.cvalue[1] = receiveData[2];
                    tempData.cvalue[0] = receiveData[3];
#ifdef DEBUG
                    qDebug()<<tempData.fvalue;
#endif
                    if(tempData.fvalue == 20)
                    {
                        this->statusBar()->showMessage(tr("校准完成"),1000);
                    }
                    else
                    {
                        this->statusBar()->showMessage(tr("校准失败"));
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
#ifdef DEBUG
                qDebug()<<tr("通讯失败：前导符后数据帧长度不足");
#else
                qDebug()<<tr("通讯失败");
#endif
            }
         }
     }
     receivebuf.clear();
     isReceving = false;
}
//根据命令号生成对应的Hart命令帧-无数据
void MainWindow::createHartCmd(QByteArray &str, int cmd)
{
    unsigned char hostChar,temChar;
    QList<unsigned char> checklist;
    str.resize(0);
    if(preCount<5)//发送前导符不小于5个
    {
        preCount = 5;
    }
    if(GlobalValue::hostType == GlobalValue::FIRTST_HOST)
    {
        hostChar = 0x80;//第一主机
    }
    else
    {
        hostChar = 0x00;//第二主机
    }
    for(int i=0;i<preCount;i++)//添加前导符
    {
        str.append((unsigned char)0xFF);
    }
    switch(cmd)
    {
    case 0://读标识码-仪表信息-使用短帧格式
        str.append((char)0x02);
        temChar = GlobalValue::address;
        temChar &= 0x0F;
        str.append((char)hostChar|temChar);
        str.append((char)0x00);
        str.append((char)0x00);
        str.append((char)hostChar|0x02);
      //str = "FF FF FF FF FF 02 80 00 00 82";//通讯测试命令
        break;
    case 2://读取主变量电流和百分比-长帧格式
    case 3://读动态变量和主变量电流
    case 12://读取设备信息
    case 13://读标签，描述符和日期
    case 14://读主变量传感器信息
    case 15://读主变量输出信息
    case 16://读工位号    
        str.append((unsigned char)0x82);//主机至从机-长帧格式
        str.append(hostChar|createID);//主机类型|制造商ID
        str.append(sensorType);//添加传感器类型
        for(int i=0;i<3;i++)//3个字节设备序列号
        {
            str.append(deveiceNumber[i]);
        }      
        str.append((char)cmd);//命令号
        str.append((char)0x00);//数据长度
        for(int i=0;i<str.count()-preCount;i++)//提取准备校验的数据
        {

            checklist << str.at(i+preCount);
        }
        temChar =ParityCheck(checklist);//生成校验码
        str.append(temChar);
        break;
    }
}
//根据命令号生成对应的HART命令帧-数据为浮点数
void MainWindow::createHartCmd(QByteArray &str, int cmd, float farg)
{
    unsigned char hostChar,temChar;
    QList<unsigned char> checkList;
    union
    {
        unsigned char cvalue[4];
        float fvalue;
    }tempData;
    tempData.fvalue = farg;
    if(preCount<5)//发送前导符不小于5个
    {
        preCount = 5;
    }
    if(GlobalValue::hostType == GlobalValue::FIRTST_HOST)
    {
        hostChar = 0x80;//第一主机
    }
    else
    {
        hostChar = 0x00;//第二主机
    }
    for(int i=0;i<preCount;i++)//添加前导符
    {
        str.append((unsigned char)0xFF);
    }
    switch (cmd)
    {
    case 34://写阻尼-不同仪表不同
    case 40://进入退出固定电流 写固定电流0时退出固定电流模式
    case 45://电流下限调整
    case 46://电流上限调整
        str.append((unsigned char)0x82);//主机至从机-长帧格式
        str.append(hostChar|createID);//主机类型|制造商ID
        str.append(sensorType);//添加传感器类型
        for(int i=0;i<3;i++)//3个字节设备序列号
        {
            str.append(deveiceNumber[i]);
        }
        str.append((unsigned char)cmd);//命令号
        str.append((unsigned char)0x04);//数据长度
        for(int i=0;i<4;i++)
        {
            str.append(tempData.cvalue[3-i]);//添加数据
        }
        for(int i=0;i<str.count()-preCount;i++)//提取准备校验的数据
        {
           checkList << str.at(i+preCount);
        }
        temChar = ParityCheck(checkList);
        str.append(temChar);
        break;
    default:
        break;
    }
}
//根据命令号生成对应的Hart命令帧-数据为整型
void MainWindow::createHartCmd(QByteArray &str, int cmd, qint32 iarg)
{
    unsigned char hostChar,temChar;
    QList<unsigned char> checkList;
    union
    {
        unsigned char cvalue[4];
        qint32 ivalue;
    }tempData;
    tempData.ivalue = iarg;
    if(preCount<5)//发送前导符不小于5个
    {
        preCount = 5;
    }
    if(GlobalValue::hostType == GlobalValue::FIRTST_HOST)
    {
        hostChar = 0x80;//第一主机
    }
    else
    {
        hostChar = 0x00;//第二主机
    }
    for(int i=0;i<preCount;i++)//添加前导符
    {
        str.append((unsigned char)0xFF);
    }
    switch(cmd)
    {
    case 19://写工位号
        str.append((unsigned char)0x82);//主机至从机-长帧格式
        str.append(hostChar|createID);//主机类型|制造商ID
        str.append(sensorType);//添加传感器类型
        for(int i=0;i<3;i++)//3个字节设备序列号
        {
            str.append(deveiceNumber[i]);
        }
        str.append((unsigned char)cmd);//命令号
        str.append((unsigned char)0x03);//数据长度
        for(int i=0;i<3;i++)
        {
            str.append(tempData.cvalue[2-i]);//添加数据
        }
        for(int i=0;i<str.count()-preCount;i++)//提取准备校验的数据
        {
            checkList << str.at(i+preCount);
        }
        temChar = ParityCheck(checkList);
        str.append(temChar);
        break;
    }
}
//根据命令号生成对应的Hart命令-数据为两个浮点数
void MainWindow::createHartCmd(QByteArray &str, int cmd, float farg1, float farg2)
{
    unsigned char hostChar,temChar;
    QList<unsigned char> checkList;
    union
    {
        unsigned char cvalue[4];
        float fvalue;
    }tempData;
    if(preCount<5)//发送前导符不小于5个
    {
        preCount = 5;
    }
    if(GlobalValue::hostType == GlobalValue::FIRTST_HOST)
    {
        hostChar = 0x80;//第一主机
    }
    else
    {
        hostChar = 0x00;//第二主机
    }
    for(int i=0;i<preCount;i++)//添加前导符
    {
        str.append((unsigned char)0xFF);
    }
    str.append((unsigned char)0x82);//主机至从机-长帧格式
    str.append(hostChar|createID);//主机类型|制造商ID
    str.append(sensorType);//添加传感器类型
    for(int i=0;i<3;i++)//3个字节设备序列号
    {
        str.append(deveiceNumber[i]);
    }
    switch(cmd)
    {
    case 35://写主变量量程值
        str.append((unsigned char)cmd);//命令号
        str.append((unsigned char)0x09);//数据长度
        str.append((unsigned char)0x2D);//量程单位代码-米
        tempData.fvalue = farg1;
        for(int i=0;i<4;i++)
        {
            str.append(tempData.cvalue[3-i]);//添加量程
        }
        tempData.fvalue = farg2;
        for(int i=0;i<4;i++)
        {
            str.append(tempData.cvalue[3-i]);//添加安装高度
        }
        for(int i=0;i<str.count()-preCount;i++)//提取准备校验的数据
        {
            checkList << str.at(i+preCount);
        }
        temChar = ParityCheck(checkList);
        str.append(temChar);
        break;
    default:
        break;
    }
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿
    painter.drawPixmap(QRect(0,0,this->width(),this->height()),QPixmap(":/skin/14_big"));

    //绘制状态栏
    QLinearGradient linearGradient(QPointF(0,this->height()-22),QPointF(0,this->height()));//线性渐变
    linearGradient.setColorAt(0,QColor(170,200,245));
    linearGradient.setColorAt(0.1,Qt::white);
    linearGradient.setColorAt(0.5,QColor(190,211,245));
    linearGradient.setColorAt(1,Qt::white);
    linearGradient.setSpread(QGradient::RepeatSpread);
    painter.setBrush(linearGradient);
    painter.drawRect(0,this->height()-22,this->width(),22);
}

void MainWindow::showWindow()
{
    this->showNormal();
    this->raise();
    this->activateWindow();
}

void MainWindow::turnPage(int currentPage)
{
    switch (currentPage)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        stackedWidget->setCurrentIndex(currentPage);
        break;
    default:
        stackedWidget->setCurrentIndex(1);
        break;
    }
}
//重新连接
void MainWindow::reConnect()
{
    this->close();
    Widget *widget = new Widget();
    widget->show();
}
//串口有数据到来
void MainWindow::readCom()
{
    if(isReceving)
    {
        receiveTimer->start(RECEVE_END_TIME);//重启定时器
#ifdef DEBUG
       qDebug()<<"receive";
#endif
    }
    else
    {
        seriaport->clear();
#ifdef DEBUG
        qDebug()<<"port clear";
#endif
    }
}

//帧接受完毕，准备对数据进行处理
void MainWindow::receiveEnd()
{
    receiveDataHandle();
}

//通讯超时
void MainWindow::communicateTimeOut()
{
    if(isFirstRun)
    {
        this->statusBar()->showMessage(tr("查询设备失败..."));
        QMessageBox::critical(this,tr("查询设备失败"),tr("串口打开正常，但未查询到有效设备，\n请重新选择连接选项或者离线启动"));
        this->close();
        Widget *widget = new Widget();
        widget->show();

    }
    else
    {
        outTimer->stop();
        isReceving = false;
        this->statusBar()->showMessage(tr("通讯超时，请检查连接状态"));
    }

}

void MainWindow::sendCmd(int cmd)
{
    if(!isReceving)
    {
        currentCmd = cmd;
        QByteArray cmdArray;
        createHartCmd(cmdArray,cmd);
        if(isFirstRun)
        {
           this->statusBar()->showMessage(tr("正在查询设备，请稍后..."));
        }
        else
        {
           this->statusBar()->showMessage(tr("通信中..."));
        }

        seriaport->write(cmdArray);
        outTimer->start(TIME_OUT);//一段时间内没收到数据认为通讯超时
        isReceving = true;
    }
}

void MainWindow::sendCmd(int cmd, qint32 iarg)
{
    if(!isReceving)
    {
        currentCmd = cmd;
        sendint = iarg;
        QByteArray cmdArray;
        createHartCmd(cmdArray,cmd,iarg);
        this->statusBar()->showMessage(tr("通信中..."));
        seriaport->write(cmdArray);
        outTimer->start(TIME_OUT);//一段时间内没收到数据认为通讯超时
        isReceving = true;
    }
}

void MainWindow::sendCmd(int cmd, float farg)
{
    if(!isReceving)
    {
        currentCmd = cmd;
        sendfData1 = farg;
        QByteArray cmdArray;
        createHartCmd(cmdArray,cmd,farg);
        this->statusBar()->showMessage(tr("通信中..."));
        seriaport->write(cmdArray);
        outTimer->start(TIME_OUT);//一段时间内没收到数据认为通讯超时
        isReceving = true;
    }
}

void MainWindow::sendCmd(int cmd, float farg1, float farg2)
{
    if(!isReceving)
    {
        currentCmd = cmd;
        sendfData1 = farg1;
        sendfData2 = farg2;
        QByteArray cmdArray;
        createHartCmd(cmdArray,cmd,farg1,farg2);
        this->statusBar()->showMessage(tr("通信中..."));
        seriaport->write(cmdArray);
        outTimer->start(TIME_OUT);//一段时间内没收到数据认为通讯超时
        isReceving = true;
    }
}

void MainWindow::setCmdPage(int cmdPage)
{

    this->cmdPage = cmdPage;
}

