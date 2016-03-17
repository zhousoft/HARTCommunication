#include "twolinewindow.h"
#include "ui_twolinewindow.h"
#include "titlewidget.h"
TwoLineWindow::TwoLineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TwoLineWindow)
{
    ui->setupUi(this);
}

TwoLineWindow::~TwoLineWindow()
{
    delete ui;
}
