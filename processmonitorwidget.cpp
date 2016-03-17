#include "processmonitorwidget.h"
#include "ui_processmonitorwidget.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <QDebug>
#include <QTime>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <QMessageBox>
#include <globalvalue.h>

ProcessMonitorWidget::ProcessMonitorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessMonitorWidget)
{
    ui->setupUi(this);

    upOpen = false;
    downOpen = false;

    upCurve = new QwtPlotCurve();
    downCurve = new QwtPlotCurve();
    upCurve->setPen(QColor(Qt::red),0,Qt::DashLine);
    downCurve->setPen(QColor(Qt::magenta),0,Qt::DashLine);
    for(int i =0;i<WARNINGLENGTH;i++)
    {
        upData[i] = 0;
        downData[i] = 0;
        warnPoint[i] = 100-i;
    }
    upCurve->attach(ui->measurePlot);
    upCurve->setRawSamples(warnPoint,upData,WARNINGLENGTH);
    downCurve->attach(ui->measurePlot);
    downCurve->setRawSamples(warnPoint,downData,WARNINGLENGTH);
    upCurve->setVisible(false);
    downCurve->setVisible(false);


    mearureCurve = new QwtPlotCurve();
    currentCurve = new QwtPlotCurve();
    mearureCurve->setPen(QColor(Qt::green),0,Qt::SolidLine);
    currentCurve->setPen(QColor(Qt::yellow),0,Qt::SolidLine);

    for(int i =0;i<SHOWDATALENGTH;i++)
    {
        measureData[i] = 0;
        currentData[i] = 0;
        showPoint[i] = 100-i;// 点1-100显示曲线，新数据放在数据数组开头
    }
    showCount = 0;
    mearureCurve->attach(ui->measurePlot);
    mearureCurve->setRawSamples(showPoint,measureData,showCount);
    currentCurve->attach(ui->currentPlot);
    currentCurve->setSamples(showPoint,currentData,showCount);
    mearureCurve->setVisible(false);
    currentCurve->setVisible(false);
    newMearure = 0;
    newCurrent = 0;


    QwtPlotCanvas *canvas_1 = new QwtPlotCanvas();//新建画布
    QwtPlotCanvas *canvas_2 = new QwtPlotCanvas();
   // canvas_1->setBorderRadius(5);//圆角矩形

    ui->measurePlot->setCanvas(canvas_1);
    ui->currentPlot->setCanvas(canvas_2);

    ui->measurePlot->setCanvasBackground(Qt::black);
    ui->currentPlot->setCanvasBackground(Qt::black);

    ui->measurePlot->plotLayout()->setAlignCanvasToScales(true);//画布与坐标轴对齐
    ui->currentPlot->plotLayout()->setAlignCanvasToScales(true);

    QwtPlotGrid *grid_1 = new QwtPlotGrid();//网格
    grid_1->setMajorPen(Qt::darkGreen,0,Qt::DotLine);//设置网格主线颜色，宽度，形状
    //grid_1->setMinorPen(Qt::darkGreen,0,Qt::DashDotDotLine);
    QwtPlotGrid *grid_2 = new QwtPlotGrid();
    grid_2->setMajorPen(Qt::darkGreen,0,Qt::DotLine);

    grid_1->attach(ui->measurePlot);
    grid_2->attach(ui->currentPlot);

    ui->measurePlot->setAxisScale(QwtPlot::xBottom,0,100);//x轴坐标范围
    ui->measurePlot->setAxisScale(QwtPlot::yLeft,0,30);
    ui->currentPlot->setAxisScale(QwtPlot::xBottom,0,100);
    ui->currentPlot->setAxisScale(QwtPlot::yLeft,0,24);

    ui->measurePlot->setAxisTitle(QwtPlot::xBottom,QString(tr("物（液）位曲线图")));
    ui->currentPlot->setAxisTitle(QwtPlot::xBottom,QString(tr("环路电流曲线图")));


    //设置选择，注意选择与下面的设置缩放的按键冲突问题，（选择是用左键选择，缩放时默认用选择指定范围缩放）
   /* QwtPlotPicker *picker = new QwtPlotPicker(QwtPlot::xBottom,QwtPlot::yLeft,ui->measurePlot->canvas());
    picker->setRubberBand(QwtPlotPicker::CrossRubberBand);//设置选择圈样式为矩形-下一句相关
    picker->setStateMachine(new QwtPickerDragLineMachine());//设置状态机为矩形状态机
    picker->setRubberBandPen(QColor(Qt::white));//设置选择边框颜色
    picker->setTrackerMode(QwtPicker::ActiveOnly);///选中时才显示坐标数字
    picker->setTrackerPen(QColor(Qt::magenta));//设置显示坐标数字颜色*/

    QwtPlotZoomer *zoomer = new QwtPlotZoomer(ui->measurePlot->canvas());
    zoomer->setRubberBandPen(QColor(Qt::white));
    zoomer->setTrackerPen(QColor(Qt::yellow));
    zoomer->setTrackerPen(QColor(Qt::yellow));//提示数字颜色
    zoomer->setMousePattern(QwtEventPattern::MouseSelect1,Qt::LeftButton);//左键放大，默认是左键放大
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton);//crtl+右键恢复全屏，默认是右键恢复全屏
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton,Qt::AltModifier);//alt+右键单步恢复*/


    timer = new QTimer(this);
    showTimeScale = SECOND;//默认坐标间隔为秒
    timerCount = 1000;//1000毫秒
    connect(timer,SIGNAL(timeout()),this,SLOT(updatePlot()));

    isRuning = false;

    ui->measureNumber->display(0.0);
    ui->percentNumber->display(0.0);
    ui->currentNumber->display(0.0);

    upAlarmSound = new QSound(QString(":/sound/upAlarm"),this);
    lowAlarmSound = new QSound(QString(":/sound/lowAlarm"),this);
    upAlarmSound->setLoops(QSound::Infinite);//无限循环
    lowAlarmSound->setLoops(QSound::Infinite);

    //ui->measurePlot->setFixedHeight(300);
   // ui->currentPlot->setFixedHeight(300);
}

ProcessMonitorWidget::~ProcessMonitorWidget()
{
    delete ui;
}

void ProcessMonitorWidget::on_upCheckBox_clicked(bool checked)
{
    ui->upKnob->setEnabled(checked);
    ui->upKnob->repaint();
    upOpen = checked;
    upCurve->setVisible(checked);
    upLimt = ui->upKnob->value();
    ui->measurePlot->replot();
    if(!checked)
    {
        if(!upAlarmSound->isFinished())
        {
            upAlarmSound->stop();
        }
    }
    qDebug()<<"报警上限:"<<upLimt;
}

void ProcessMonitorWidget::on_downCheckBox_clicked(bool checked)
{
    ui->downKnob->setEnabled(checked);
    ui->downKnob->repaint();
    downOpen = checked;
    downCurve->setVisible(checked);
    downLimt = ui->downKnob->value();
    ui->measurePlot->replot();
    if(!checked)
    {
        if(!lowAlarmSound->isFinished())
        {
            lowAlarmSound->stop();
        }
    }
    qDebug()<<"报警下限"<<downLimt;
}

void ProcessMonitorWidget::on_limtKnob_valueChanged(double value)
{
    emit ui->upCheckBox->clicked(false);
    emit ui->downCheckBox->clicked(false);
    ui->upCheckBox->setChecked(false);
    ui->downCheckBox->setChecked(false);
    ui->upKnob->setUpperBound(value);
    ui->downKnob->setUpperBound(value);

    ui->measurePlot->setAxisScale(QwtPlot::yLeft,0,value);
    ui->measurePlot->replot();

}

void ProcessMonitorWidget::on_limtKnob_sliderReleased()
{
   /* double value = ui->limtKnob->value();
    emit ui->upCheckBox->clicked(false);
    emit ui->downCheckBox->clicked(false);
    ui->upCheckBox->setChecked(false);
    ui->downCheckBox->setChecked(false);
    ui->upKnob->setUpperBound(value);
    ui->downKnob->setUpperBound(value);*/
}

void ProcessMonitorWidget::on_upKnob_valueChanged(double value)
{ 
        for(int i=0;i<WARNINGLENGTH;i++)
        {
            upData[i] = value;
        }
        upCurve->setSamples(warnPoint,upData,WARNINGLENGTH);
        ui->measurePlot->replot();
        upLimt = value;
}

void ProcessMonitorWidget::on_downKnob_valueChanged(double value)
{ 
        for(int i=0;i<WARNINGLENGTH;i++)
        {
            downData[i] = value;
        }
        downCurve->setSamples(warnPoint,downData,WARNINGLENGTH);
        ui->measurePlot->replot();
        downLimt = value;
}

void ProcessMonitorWidget::on_secondRadioButton_clicked()
{
    if(showTimeScale == SECOND)
    {
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this,tr("修改坐标时间"),
                                                 tr("是否将监测坐标时间间隔修改为秒，重新绘制曲线？")))
    {
        showTimeScale = SECOND;
        timer->start(1000);
        showCount = 0;//刷新曲线图-清除旧曲线
        mearureCurve->setSamples(showPoint,measureData,showCount);
        currentCurve->setSamples(showPoint,currentData,showCount);
        ui->measurePlot->replot();
        ui->currentPlot->replot();
        if(upOpen)
        {
            if(!upAlarmSound->isFinished())
            {
                upAlarmSound->stop();
            }
        }
        if(downOpen)
        {
            if(!lowAlarmSound->isFinished())
            {
                lowAlarmSound->stop();
            }
        }
        qDebug()<<" sencond checked";
    }
    else
    {
        ui->secondRadioButton->setChecked(false);
        switch (showTimeScale)
        {
        case SECOND:
            ui->secondRadioButton->setChecked(true);
            break;
        case MINUTE:
            ui->muniteRadioButton->setChecked(true);
            break;
        case HOUR:
            ui->hourRadioButton->setChecked(true);
            break;
        default:
            break;
        }
    }
}

void ProcessMonitorWidget::on_muniteRadioButton_clicked()
{
    if(showTimeScale == MINUTE)
    {
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this,tr("修改坐标时间"),
                                                 tr("是否将监测坐标时间间隔修改为分，重新绘制曲线？")))
    {
        ui->muniteRadioButton->setChecked(true);
        showTimeScale = MINUTE;
        timer->start(60000);
        showCount = 0;
        mearureCurve->setSamples(showPoint,measureData,showCount);
        currentCurve->setSamples(showPoint,currentData,showCount);
        ui->measurePlot->replot();
        ui->currentPlot->replot();
        if(upOpen)
        {
            if(!upAlarmSound->isFinished())
            {
                upAlarmSound->stop();
            }
        }
        if(downOpen)
        {
            if(!lowAlarmSound->isFinished())
            {
                lowAlarmSound->stop();
            }
        }
        qDebug()<<" minute checked";

    }
    else
    {
        ui->muniteRadioButton->setChecked(false);
        switch (showTimeScale)
        {
        case SECOND:
            ui->secondRadioButton->setChecked(true);
            break;
        case MINUTE:
            ui->muniteRadioButton->setChecked(true);
            break;
        case HOUR:
            ui->hourRadioButton->setChecked(true);
            break;
        default:
            break;
        }
    }
}

void ProcessMonitorWidget::on_hourRadioButton_clicked()
{
    if(showTimeScale == HOUR)
    {
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this,tr("修改坐标时间"),
                                                 tr("是否将监测坐标时间间隔修改为小时，重新绘制曲线？")))
    {
        showTimeScale = HOUR;
        timer->start(3600000);
        showCount = 0;
        mearureCurve->setSamples(showPoint,measureData,showCount);
        currentCurve->setSamples(showPoint,currentData,showCount);
        ui->measurePlot->replot();
        ui->currentPlot->replot();
        if(upOpen)
        {
            if(!upAlarmSound->isFinished())
            {
                upAlarmSound->stop();
            }
        }
        if(downOpen)
        {
            if(!lowAlarmSound->isFinished())
            {
                lowAlarmSound->stop();
            }
        }
        qDebug()<<"hour checked";
    }
    else
    {
        ui->hourRadioButton->setChecked(true);
        switch (showTimeScale)
        {
        case SECOND:
            ui->secondRadioButton->setChecked(true);
            break;
        case MINUTE:
            ui->muniteRadioButton->setChecked(true);
            break;
        case HOUR:
            ui->hourRadioButton->setChecked(true);
            break;
        default:
            break;
        }
    }
}

void ProcessMonitorWidget::on_startButton_clicked()
{
    if(isRuning)//已经在监测中
    {
        isRuning = false;
        timer->stop();
      //  mearureCurve->setVisible(false);
      //  currentCurve->setVisible(false);
        ui->startButton->setText(tr("开始监测"));
        if(!upAlarmSound->isFinished())
        {
            upAlarmSound->stop();
        }
        if(!lowAlarmSound->isFinished())
        {
            lowAlarmSound->stop();
        }

    }
    else//开启监测
    {
        isRuning = true;
        ui->startButton->setText(tr("停止监测"));
        switch (showTimeScale)
        {
        case SECOND:
            timerCount = 1000;
            break;
        case MINUTE:
            timerCount = 60000;
            break;
        case HOUR:
            timerCount = 3600000;
            break;
        default:
            timerCount = 1000;
            break;
        }
        mearureCurve->setVisible(true);
        currentCurve->setVisible(true);
        timer->start(timerCount);//开启定时器
    }

}

void ProcessMonitorWidget::handleData(int cmd, QByteArray data)
{
    union
    {
        unsigned char cvalue[4];
        float fvalue;
    }tempdata;
    switch (cmd)
    {
    case 3:
        tempdata.cvalue[3] = (unsigned char)data.at(0);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(1);
        tempdata.cvalue[1] = (unsigned char)data.at(2);
        tempdata.cvalue[0] = (unsigned char)data.at(3);
        newCurrent = tempdata.fvalue;
        tempdata.cvalue[3] = (unsigned char)data.at(5);//大端模式 高位在前
        tempdata.cvalue[2] = (unsigned char)data.at(6);
        tempdata.cvalue[1] = (unsigned char)data.at(7);
        tempdata.cvalue[0] = (unsigned char)data.at(8);
        newMearure = tempdata.fvalue;
        break;
    default:
        break;
    }
}

//定时更新曲线
void ProcessMonitorWidget::updatePlot()
{
    emit sendCmd(3);
    if(GlobalValue::connectStatus == GlobalValue::OFFLINE)//离线模式-生成随机曲线
    {
        qsrand(QTime::currentTime().second()*1000+QTime::currentTime().msec());
        newMearure = qrand()%15+5;
        newCurrent = qrand()%10+6;//生成4-20随机数
    }
    if(showCount < SHOWDATALENGTH)//曲线慢慢增长至整个坐标
    {
        showCount++;
    }
    for(int i = SHOWDATALENGTH-1;i>0;i--)
    {
        measureData[i] = measureData[i-1];
        currentData[i] = currentData[i-1];
    }
    measureData[0] = newMearure;
    currentData[0] = newCurrent;
    mearureCurve->setSamples(showPoint,measureData,showCount);
    currentCurve->setSamples(showPoint,currentData,showCount);
    ui->measurePlot->replot();
    ui->currentPlot->replot();
    ui->measureNumber->display(newMearure);
    ui->currentNumber->display(newCurrent);
    //根据电流计算百分比
    double percent = (newCurrent-4)/16*100;
    percent = percent<0 ? 0 : percent;
    percent = percent>100 ? 100 : percent;
    ui->percentNumber->display(percent);

    if(upOpen)
    {
        if(newMearure > upLimt)
        {
            upAlarmSound->play();
        }
        else
        {
            if(!upAlarmSound->isFinished())
            {
                upAlarmSound->stop();
            }

        }
    }
    else
    {
        if(!upAlarmSound->isFinished())
        {
            upAlarmSound->stop();
        }
    }
    if(downOpen)
    {
        if(newMearure < downLimt)
        {
            lowAlarmSound->play();
        }
        else
        {
            if(!lowAlarmSound->isFinished())
            {
               lowAlarmSound->stop();
            }

        }
    }
    else
    {
        if(!lowAlarmSound->isFinished())
        {
            lowAlarmSound->stop();
        }
    }
}
