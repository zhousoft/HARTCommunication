#ifndef MYPLOT_H
#define MYPLOT_H
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
class MyPlot : public QwtPlot
{
public:
    MyPlot(QWidget *parent = 0);

    QwtPlotCurve *curve;
};

#endif // MYPLOT_H
