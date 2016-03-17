#include "twolinemainwidget.h"
#include "globalvalue.h"
#include <QDebug>
TwoLineMainWidget::TwoLineMainWidget(QWidget *parent)
    :DropEnableWidget(parent)
{
    setMinimumSize(900,600);
    setWindowIcon(QIcon("/myImage/logo"));

    stackeWidget = new QStackedWidget();
    titleWidget = new TitleWidget();
    infoWidget = new InfoWidget();

    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(Qt::white));
    stackeWidget->setPalette(palette);
    stackeWidget->setAutoFillBackground(true);
    infoWidget->setPalette(palette);
    infoWidget->setAutoFillBackground(true);

    QGridLayout *layout1 = new QGridLayout();//临时创建两个widget测试用
    QLabel *label1 = new QLabel("page 1");
    layout1->addWidget(label1);
    QWidget *page1 = new QWidget();
    page1->setLayout(layout1);

    QGridLayout *layout2 = new QGridLayout();
    QLabel *label2 = new QLabel("page 2");
    layout2->addWidget(label2);
    QWidget *page2 = new QWidget();
    page2->setLayout(layout2);

    stackeWidget->addWidget(page1);
    stackeWidget->addWidget(page2);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(stackeWidget);
    centerLayout->setSpacing(0);
    centerLayout->addWidget(infoWidget);
    centerLayout->setStretch(0,3);
    centerLayout->setStretch(1,1);
    centerLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleWidget);
    mainLayout->addLayout(centerLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(5,5,5,5);

    setLayout(mainLayout);

    connect(titleWidget,SIGNAL(showMin()),this,SLOT(showMinimized()));
    connect(titleWidget,SIGNAL(closeWidget()),this,SLOT(close()));
    connect(titleWidget,SIGNAL(turnPage(int)),this,SLOT(turnPage(int)));

    titleWidget->turnPage("0");


    qDebug()<<GlobalValue::address;

}

TwoLineMainWidget::~TwoLineMainWidget()
{

}

bool TwoLineMainWidget::isRunning()
{
    return isRun;
}

void TwoLineMainWidget::paintEvent(QPaintEvent *event)
{
    DropEnableWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawPixmap(QRect(5,5,this->width()-2*5,this->height()-2*5),QPixmap(":/skin/14_big"));

}

void TwoLineMainWidget::showWidget()
{
    this->showNormal();
    this->raise();
    this->activateWindow();
}

void TwoLineMainWidget::turnPage(int currentPage)
{
    switch (currentPage) {
    case 0:
        stackeWidget->setCurrentIndex(0);
        break;
    default:
        stackeWidget->setCurrentIndex(1);
        break;
    }
}




