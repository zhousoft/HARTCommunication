#ifndef PROCESSMONITORWIDGET_H
#define PROCESSMONITORWIDGET_H

#include <QWidget>
#include <qwt_plot_curve.h>
#include <QTimer>
#include <QSound>
#define WARNINGLENGTH 100 //报警曲线长度
#define SHOWDATALENGTH 100 //显示数据长度

namespace Ui {
class ProcessMonitorWidget;
}

class ProcessMonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessMonitorWidget(QWidget *parent = 0);
    ~ProcessMonitorWidget();

    enum ShowTimeScale
    {
        SECOND,
        MINUTE,
        HOUR
    };

signals:
    void sendCmd(int cmd);

private slots:

    void on_upCheckBox_clicked(bool checked);

    void on_downCheckBox_clicked(bool checked);

    void on_limtKnob_valueChanged(double value);

    void on_limtKnob_sliderReleased();

    void on_upKnob_valueChanged(double value);

    void on_downKnob_valueChanged(double value);

    void on_secondRadioButton_clicked();

    void on_muniteRadioButton_clicked();

    void on_hourRadioButton_clicked();

    void on_startButton_clicked();

    void handleData(int cmd, QByteArray data);

    void updatePlot();

private:
    Ui::ProcessMonitorWidget *ui;
    double upLimt;//报警上限
    double downLimt;//报警下限
    bool upOpen;//上限报警打开
    bool downOpen;//下限报警打开
    QwtPlotCurve *upCurve;
    QwtPlotCurve *downCurve;
    QwtPlotCurve *mearureCurve;
    QwtPlotCurve *currentCurve;
    double upData[WARNINGLENGTH];
    double downData[WARNINGLENGTH];
    double warnPoint[WARNINGLENGTH];
    double measureData[SHOWDATALENGTH];//监控的变量
    double currentData[SHOWDATALENGTH];//电流数据
    double showPoint[SHOWDATALENGTH];//横坐标值
    double newMearure;//物位
    double newCurrent;//电流
    int showCount;//初始显示个数，慢慢递增至SHOWDATALENGTH来显示整个坐标，避免一开始显示全部0
    QTimer *timer;//定时器，定时刷新
    int timerCount;//定时时间
    ShowTimeScale showTimeScale;//物位曲线图横坐标间隔- 秒 分 时
    bool isRuning;//是否正在执行监控
    QSound *upAlarmSound;//上限报警音效
    QSound *lowAlarmSound;//下限报警音效
};

#endif // PROCESSMONITORWIDGET_H
