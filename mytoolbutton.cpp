#include "mytoolbutton.h"
MyToolButton::MyToolButton(QString picName, QWidget *parent)
    :QToolButton(parent)
{
    //设置文本颜色
    QPalette txtPalette = this->palette();
    txtPalette.setColor(QPalette::ButtonText,QColor(230,230,230));
    setPalette(txtPalette);

    //设置文本粗体
    QFont &txtFont = const_cast<QFont &>(this->font());
    txtFont.setWeight(QFont::Bold);
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//设置文本在图标下方

    //设置图标
    QPixmap pixmap(picName);
    this->setIcon(pixmap);
    this->setIconSize(pixmap.size());

    //设置大小
    setFixedSize(pixmap.width()+25,pixmap.height()+25);
    setAutoRaise(true);
    setObjectName("transparentToolButton");

    this->setStyleSheet("background:transparent");

    isOver = false;
    isPress = false;

}

MyToolButton::~MyToolButton()
{

}

void MyToolButton::setMousePress(bool isPress)
{
    this->isPress = isPress;
    update();
}

void MyToolButton::enterEvent(QEvent *)
{
    isOver = true;
}

void MyToolButton::leaveEvent(QEvent *)
{
    isOver = false;
}

void MyToolButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)//左键按下
    {
        emit clicked();
    }
}

void MyToolButton::paintEvent(QPaintEvent *event)
{
    if(isOver)//鼠标滑过
    {
        painterInfo(10,100,150);
    }
    else
    {
        if (isPress)
        {
            painterInfo(10,100,150);
        }
    }
    QToolButton::paintEvent(event);//重写后要调用父类的绘制事件
}

void MyToolButton::painterInfo(int topColor, int middleColor, int bottomColor)
{
    QPainter painter(this);
    QPen pen(Qt::NoBrush,1);
    painter.setPen(pen);
    QLinearGradient linear(rect().topLeft(),rect().bottomLeft());//线性渐变
    linear.setColorAt(0, QColor(230, 230, 230, topColor));
    linear.setColorAt(0.5, QColor(230, 230, 230, middleColor));
    linear.setColorAt(1, QColor(230, 230, 230, bottomColor));
    painter.setBrush(linear);
    painter.drawRect(rect());

}

