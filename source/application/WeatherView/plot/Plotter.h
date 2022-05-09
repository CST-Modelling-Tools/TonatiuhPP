#pragma once

class QCustomPlot;

#include "TMY/ModelTMY.h"



class Plotter
{
public:
    Plotter(QCustomPlot* customPlot);

    void makePlot(ModelTMY* model);
    void resetPlot();
    void addSun(bool on);

    double tMin();
    double tMax();
    double tStep() {return m_tStep;}

private:
    QCustomPlot* m_customPlot;
    ModelTMY* m_model;
    double m_tStep;
    double m_tOrigin;
    double m_x0;
};

