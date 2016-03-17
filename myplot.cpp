#include "myplot.h"
#include <qwt_plot_canvas.h>

MyPlot::MyPlot(QWidget *parent)
    :QwtPlot(parent)
{
    this->setAutoReplot(false);

}

