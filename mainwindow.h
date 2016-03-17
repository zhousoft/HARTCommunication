#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "titlewidget.h"
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QTimer>
#define TIME_OUT 5000
#define RECEVE_END_TIME 100

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QString portName,QWidget *parent = 0);
    ~MainWindow();
    void createHartCmd(QByteArray &str,int cmd);
    void createHartCmd(QByteArray &str, int cmd,float farg);
    void createHartCmd(QByteArray &str, int cmd,qint32 iarg);
    void createHartCmd(QByteArray &str, int cmd, float farg1,float farg2);

    unsigned char ParityCheck(QList<unsigned char> &list);
    void receiveDataHandle();


protected:
    void paintEvent(QPaintEvent *event);

signals:
    void sendTwoLineWuWeiData(int cmd,const QByteArray data);

    void sendleiDaWuWeiData(int cmd,const QByteArray data);

    void sendCurrentData(int cmd,const QByteArray data);

    void sendDampingTimeData(int cmd,const QByteArray data);

    void sendSampleCountData(int cmd,const QByteArray data);

    void sendProcessMonitorData(int cmd,const QByteArray data);

    void sendDeviceInfoData(int cmd,const QByteArray data);

    void sendOtherSetData(int cmd,const QByteArray data);

    void sendRightInfoData(int cmd,const QByteArray data);




private slots:
    void showWindow();
    void turnPage(int currentPage);
    void reConnect();

    void readCom();//读取串口
    void receiveEnd();//串口一帧数据接收结束
    void communicateTimeOut();//通讯超时

    void sendCmd(int cmd);//接收各页面传来的hart命令，转发给下位机
    void sendCmd(int cmd,qint32 iarg);
    void sendCmd(int cmd,float farg);
    void sendCmd(int cmd, float farg1,float farg2);
    void setCmdPage(int cmdPage);//不同页面发送相同命令时判断来自哪个页面

private:
    QStackedWidget *stackedWidget;
    TitleWidget *titleWidget;
    QSerialPort *seriaport;
    QTimer *receiveTimer;//判断一帧是否接受完毕
    QTimer *outTimer;//判断通讯是否超时
    bool isReceving;//是否在等待接收数据
    int currentCmd;//当前命令号，供重发使用
    int preCount;//从机请求最小前导符个数
    QByteArray receivebuf;//接收数据的缓冲区
    QByteArray sendbuf;//发送缓冲
    unsigned char createID;//制造商ID
    unsigned char sensorType;//传感器类型
    unsigned char deveiceNumber[3];//传感器序列号（设备标识号）
    unsigned char deveiceTag[6];//设备标记符（tag）
    unsigned char deveiDecription[12];//设备描述符
    float sendfData1;//发送的第一个浮点数 -供判断数据修改是否成功
    float sendfData2;//发送的第二个浮点数
    qint32 sendint;//发送的整数
    int cmdPage;
    bool isFirstRun;//启动时要发送0号命令来获取设备信息
};

#endif // MAINWINDOW_H
