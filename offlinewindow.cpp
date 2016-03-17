#include "offlinewindow.h"
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
OfflineWindow::OfflineWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(900,600);
    setWindowIcon(QIcon(":/myImage/logo"));

    stackedWidget = new QStackedWidget();
    titleWidget = new TitleWidget();

    QPalette palette;
    palette.setBrush(QPalette::Window,Qt::white);
    stackedWidget->setPalette(palette);
    stackedWidget->setAutoFillBackground(true);


    DeviceInfoWidget *devieceInfoWidget = new DeviceInfoWidget();
    ProcessMonitorWidget *processMonitorWidget = new ProcessMonitorWidget();
    CurrentWidget *currentWidget = new CurrentWidget();
    OtherWidget *otherWidget = new OtherWidget();
    LineConnectWidget *lineConnectWidget = new LineConnectWidget();

    LeiDaWuWei *leiDaWuWei = new LeiDaWuWei();
    TwoLineWuWei *twoLineWuWei = new TwoLineWuWei();

    switch (GlobalValue::deviceType) {
    case GlobalValue::PULSERADAR:
        stackedWidget->addWidget(leiDaWuWei);
        break;
    case GlobalValue::TOWLINESOUND:
        stackedWidget->addWidget(twoLineWuWei);
        break;
    default:
        break;
    }

    stackedWidget->addWidget(currentWidget);

    DampingTimeWidget *dampingTimeWidget = new DampingTimeWidget();
    SampleCountWeidget *sampleCountWidget = new SampleCountWeidget();

    switch (GlobalValue::deviceType) {
    case GlobalValue::PULSERADAR:
        stackedWidget->addWidget(dampingTimeWidget);
        break;
    case GlobalValue::TOWLINESOUND:
        stackedWidget->addWidget(sampleCountWidget);
        break;
    default:
        break;
    }

    stackedWidget->addWidget(processMonitorWidget);
    stackedWidget->addWidget(devieceInfoWidget);
    stackedWidget->addWidget(otherWidget);
    stackedWidget->addWidget(lineConnectWidget);


    RightInfoWidget *rightInfoWidget = new RightInfoWidget();
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(stackedWidget);
    centerLayout->setSpacing(0);
    //centerLayout->addWidget(infoWidget);
    centerLayout->addWidget(rightInfoWidget);
    centerLayout->setStretch(0,3);
    centerLayout->setStretch(1,1);
    centerLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleWidget);
    mainLayout->addLayout(centerLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    //QMainWindow默认已有布局管理器，不能直接setLayout
    QWidget *mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);


    this->statusBar()->showMessage(tr("欢迎使用格莱普Hart仪表调试工具"),10000);

    connect(titleWidget,SIGNAL(turnPage(int)),this,SLOT(turnPage(int)));
    connect(rightInfoWidget,SIGNAL(reConnect()),this,SLOT(reConnect()));

    titleWidget->turnPage("0");
}

OfflineWindow::~OfflineWindow()
{

}

void OfflineWindow::paintEvent(QPaintEvent *)
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

void OfflineWindow::showWindow()
{
    this->showNormal();
    this->raise();
    this->activateWindow();
}

void OfflineWindow::turnPage(int currentPage)
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

void OfflineWindow::reConnect()
{
    this->close();
    Widget *widget = new Widget();
    widget->show();
}

