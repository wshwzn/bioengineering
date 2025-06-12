#include <QtCharts>
#include"DESeq2Result.h"
#ifndef PAINTER_H
#define PAINTER_H

class Painter{
public:
    QChart* createVolcanoPlotQt(const std::vector<DESeq2Result>& data, int width=800, int height=600);
};

#endif // PAINTER_H
