#include "titlewidget.h"
#include "globalvalue.h"
TitleWidget::TitleWidget(QWidget *parent) : QWidget(parent)
{
    programLabel = new QLabel();
    minButton = new MyPushButton();
    closeButton = new MyPushButton();
    programLabel->setObjectName("whiteLabel");
    programLabel->setText(tr("HART仪表调试工具"));

    //设置图片
    minButton->setPicName(QString(":/sysButton/min"));
    closeButton->setPicName(QString(":/sysButton/close"));

    connect(minButton,SIGNAL(clicked()),this,SIGNAL(showMin()));
    connect(closeButton,SIGNAL(clicked()),this,SIGNAL(closeWidget()));

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(programLabel,0,Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(minButton,0,Qt::AlignTop);
    titleLayout->addWidget(closeButton,0,Qt::AlignTop);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0,0,5,0);
    programLabel->setContentsMargins(15,0,0,0);

    QStringList strList;
    strList<<":/toolWidget/wuwei"<<":/toolWidget/current"<<":/toolWidget/sample"<<":/toolWidget/process"
        <<":/toolWidget/device"<<":/toolWidget/other"<<":/toolWidget/connect";
    QHBoxLayout *buttonLayput = new QHBoxLayout();
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i=0;i<strList.size();i++)
    {
        MyToolButton *toolButton = new MyToolButton(strList.at(i));     
        buttonList.append(toolButton);
        connect(toolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(toolButton,QString::number(i,10));
        buttonLayput->addWidget(toolButton,0,Qt::AlignBottom);
    }
    buttonList.at(0)->setText(tr("物位调整"));
    buttonList.at(1)->setText(tr("输出电流"));
    switch (GlobalValue::deviceType) {
    case GlobalValue::PULSERADAR:
         buttonList.at(2)->setText(tr("阻尼时间"));
        break;
    case GlobalValue::TOWLINESOUND:
         buttonList.at(2)->setText(tr("采样个数"));
        break;
    default:
        break;
    }
    buttonList.at(3)->setText(tr("过程监测"));
    buttonList.at(4)->setText(tr("仪表信息"));
    buttonList.at(5)->setText(tr("其他设置"));
    buttonList.at(6)->setText(tr("仪表接法"));

    connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(turnPage(QString)));

    nameLabel = new QLabel();
    companyLabel = new QLabel();
    nameLabel->setText(tr("格莱普"));
    companyLabel->setText(tr("高新技术有限公司"));
    QVBoxLayout *nameLayout = new QVBoxLayout();
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(companyLabel);
    nameLayout->setSpacing(5);
    nameLayout->setContentsMargins(0,0,0,0);

    QLabel *logoLabel = new QLabel();
    QPixmap pixmap(QString(":/myImage/logo"));
    logoLabel->setPixmap(pixmap);
    logoLabel->setFixedSize(pixmap.size());

    nameLabel->setObjectName("whiteLabel");
    companyLabel->setObjectName("whiteLabel");

    QFont nameFont = nameLabel->font();
    nameFont.setPointSize(18);
    nameFont.setBold(true);

    nameLabel->setFont(nameFont);
    companyLabel->setFont(nameFont);

    nameLabel->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    companyLabel->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    buttonLayput->addStretch();
    buttonLayput->addLayout(nameLayout);
    buttonLayput->addWidget(logoLabel);
    buttonLayput->setSpacing(8);
    buttonLayput->setContentsMargins(15,0,15,0);

   /* QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(buttonLayput);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);*/

    this->setLayout(buttonLayput);
    setFixedHeight(100);
    isMove = false;

}

void TitleWidget::turnPage(QString currentPage)
{
    bool ok;
    int currentIndex = currentPage.toInt(&ok,10);
    for(int i =0;i<buttonList.size();i++)
    {
        MyToolButton *toolButton =buttonList.at(i);
        if(currentIndex == i)
        {
            toolButton->setMousePress(true);
        }
        else
        {
            toolButton->setMousePress(false);
        }
    }
    emit turnPage(currentIndex);
}

