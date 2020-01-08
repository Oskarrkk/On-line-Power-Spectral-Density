
#include "xyseriesiodevice.h"
#include <iostream>
#include <QVector>
#include <QPointF>
#include <math.h>

#define PI 3.14159265

XYSeriesIODevice::XYSeriesIODevice(QCustomPlot *tmp_1, QCustomPlot *tmp_2, QObject *parent) :
    QIODevice(parent),
    upper_chart(tmp_1),
    lower_chart(tmp_2)
{
  //Giving the initial size of the charts
  vec_Y.resize(chart_size);
  vec_prob.resize(chart_size);
  vec_SP.resize(chart_size);
  vec_U.resize(chart_size);
  //Filling samples before starting the simulation with zeros
  vec_Y.fill(0, chart_size);
  vec_SP.fill(0, chart_size);
  vec_X.fill(0,chart_size);
  vec_U.fill(0, chart_size);
  //
  for (int i=0; i<chart_size; ++i) vec_prob[i] = i-chart_size+1;

  upper_chart->xAxis->setLabel("probka");

  //Początkowe nastawy do wykresów
  upper_chart->legend->setVisible(true);
  upper_chart->addGraph()->setName("Wyskokość [m]");
  upper_chart->addGraph()->setName("Set Point [m]");
  lower_chart->addGraph()->setName("GWM");
  upper_chart->graph(0)->setPen(QPen(Qt::blue));
  upper_chart->graph(0)->setData(vec_prob, vec_Y);
  upper_chart->graph(1)->setPen(QPen(Qt::red));
  upper_chart->graph(1)->setData(vec_prob, vec_SP);
  lower_chart->graph(0)->setPen(QPen(Qt::red));
  lower_chart->graph(0)->setData(vec_X, vec_U);
  upper_chart->yAxis->setRange(-1, 1);
  upper_chart->xAxis->setRange(vec_prob.first(), vec_prob.last());
  lower_chart->yAxis->setRange(-25, 100);
  lower_chart->xAxis->setRange(vec_X.first(), vec_X.last());
  // now we move the legend from the inset layout of the axis rect into the main grid layout.
  // We create a sub layout so we can generate a small gap between the plot layout cell border
  // and the legend border:
  QCPLayoutGrid *subLayout = new QCPLayoutGrid;
  upper_chart->plotLayout()->addElement(1, 0, subLayout);
  subLayout->setMargins(QMargins(5, 0, 5, 5));
  subLayout->addElement(0, 0, upper_chart->legend);
  // change the fill order of the legend, so it's filled left to right in columns:
  upper_chart->legend->setFillOrder(QCPLegend::foColumnsFirst);
  // set legend's row stretch factor very small so it ends up with minimum height:
  upper_chart->plotLayout()->setRowStretchFactor(1, 0.001);

  upper_chart->replot();
  lower_chart->replot();
}

qint64 XYSeriesIODevice::readData(char * data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 XYSeriesIODevice::writeData(const char * data, qint64 maxSize)
{

    int resolution = 1;

  //  Put the samples taken from the microphone into the vector. When the vector is full, new samples replace old ones.
    for (int k = 0; k < maxSize/resolution; k++)
    {
        vec_Y.push_back(((quint8)data[resolution * k] - 128)/128.0);
        vec_Y.pop_front();
    }


    int n = vec_Y.count() ;

    fftw_complex *in, *out;
    fftw_plan p;

    in = (fftw_complex*) fftw_malloc(n*sizeof(fftw_complex));
    out = (fftw_complex*) fftw_malloc(n*sizeof(fftw_complex));

  //  We create a plan, i.e. an object containing all the data needed to calculate the FFT.
    p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  //  We save samples from the vector into an array, which is one of the arguments of the function that creates the plan.
  //  Signal samples are multiplied by the Blackman window function.
    for (int i = 0; i<vec_Y.size()  ; ++i){
      in[i][0]=vec_Y[i]*0.5*(1 - cos(2*PI*i/chart_size));
      in[i][1]=0;
    }

  //  Calculate FFT
     fftw_execute(p);

  //  Calculating the GWM and inserting it into the vector that will be displayed on the chart.
    for (int i = (chart_size/resolution)/2; i< (chart_size/resolution)  ; ++i){
        vec_U.push_back(20*log10(pow(sqrt(out[i][0]*out[i][0]+out[i][1]*out[i][1]),2)));
        vec_U.pop_front();
    }

    for (int i=44100; i>0; i=i-44100/(vec_U.size())){
        vec_X.push_back(i);
        vec_X.pop_front();
    }


    //Data update in charts
    upper_chart->graph(0)->setData(vec_prob, vec_Y);
    upper_chart->yAxis->setRange(-1, 1);
    upper_chart->xAxis->setRange(vec_prob.first(), vec_prob.last());
    lower_chart->graph(0)->setData(vec_X, vec_U);
    lower_chart->yAxis->setRange(-50, 100);
    lower_chart->xAxis->setRange(vec_X.last(), vec_X.last()+22000);
    //Chart update
    upper_chart->replot();
    lower_chart->replot();
    return maxSize;
}
