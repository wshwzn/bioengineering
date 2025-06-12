#include"Painter.h"
#include <vector>
#include <cmath>


QChart* Painter::createVolcanoPlotQt(const std::vector<DESeq2Result>& data, int width, int height) {
    // 创建图表
    QChart *chart = new QChart();
    chart->setTitle("Volcano Plot");
    chart->setMinimumSize(width, height);

    // 创建散点系列
    QScatterSeries *sigUpSeries = new QScatterSeries();    // 显著上调(红色)
    QScatterSeries *sigDownSeries = new QScatterSeries();  // 显著下调(蓝色)
    QScatterSeries *fcOnlySeries = new QScatterSeries();   // 仅FC显著(橙色)
    QScatterSeries *nsSeries = new QScatterSeries();       // 不显著(灰色)

    // 设置点样式
    sigUpSeries->setName("Significant up-regulated");
    sigUpSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    sigUpSeries->setColor(Qt::red);
    sigUpSeries->setMarkerSize(3.5);

    sigDownSeries->setName("Significant down-regulated");
    sigDownSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    sigDownSeries->setColor(Qt::blue);
    sigDownSeries->setMarkerSize(3.5);

    fcOnlySeries->setName("Only |FC|>1");
    fcOnlySeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    fcOnlySeries->setColor(QColor(255, 165, 0)); // 橙色
    fcOnlySeries->setMarkerSize(3.5);

    nsSeries->setName("Not significant");
    nsSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    nsSeries->setColor(Qt::gray);
    nsSeries->setMarkerSize(3.5);

    // 确定数据范围
    double maxAbsLog2FC = 0;
    double maxNegLog10P = 0;

    for (const auto& gene : data) {
        double absLog2FC = std::abs(gene.log2_fold_change);
        if (absLog2FC > maxAbsLog2FC) maxAbsLog2FC = absLog2FC;

        double negLog10P = -std::log10(gene.padj);
        if (negLog10P > maxNegLog10P) maxNegLog10P = negLog10P;
    }

    // 设置合理的轴范围
    maxAbsLog2FC = std::max(3.0, maxAbsLog2FC * 1.1); // 至少显示到±3
    maxNegLog10P = std::max(5.0, maxNegLog10P * 1.1); // 至少显示到5

    // 添加数据点
    for (const auto& gene : data) {
        double x = gene.log2_fold_change;
        double y = -std::log10(gene.padj);

        if (gene.padj < 0.05 && gene.log2_fold_change > 1) {
            sigUpSeries->append(x, y);
        } else if (gene.padj < 0.05 && gene.log2_fold_change < -1) {
            sigDownSeries->append(x, y);
        } else if (std::abs(gene.log2_fold_change) > 1) {
            fcOnlySeries->append(x, y);
        } else {
            nsSeries->append(x, y);
        }
    }

    // 添加系列到图表
    chart->addSeries(sigUpSeries);
    chart->addSeries(sigDownSeries);
    chart->addSeries(fcOnlySeries);
    chart->addSeries(nsSeries);

    // 创建坐标轴
    QValueAxis *xAxis = new QValueAxis();
    xAxis->setTitleText("log2(Fold Change)");
    xAxis->setRange(-maxAbsLog2FC, maxAbsLog2FC);
    xAxis->setLabelFormat("%.1f");

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setTitleText("-log10(p-adj)");
    yAxis->setRange(0, maxNegLog10P);
    yAxis->setLabelFormat("%.1f");

    chart->setAxisX(xAxis);
    chart->setAxisY(yAxis);

    // 连接系列到坐标轴
    sigUpSeries->attachAxis(xAxis);
    sigUpSeries->attachAxis(yAxis);
    sigDownSeries->attachAxis(xAxis);
    sigDownSeries->attachAxis(yAxis);
    fcOnlySeries->attachAxis(xAxis);
    fcOnlySeries->attachAxis(yAxis);
    nsSeries->attachAxis(xAxis);
    nsSeries->attachAxis(yAxis);

    // 添加阈值线
    // p-value阈值线 (p=0.05)
    double pThresh = -std::log10(0.05);
    QLineSeries *pThresholdLine = new QLineSeries();
    pThresholdLine->append(-maxAbsLog2FC, pThresh);
    pThresholdLine->append(maxAbsLog2FC, pThresh);
    pThresholdLine->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    chart->addSeries(pThresholdLine);
    pThresholdLine->attachAxis(xAxis);
    pThresholdLine->attachAxis(yAxis);

    // log2FC阈值线 (±1)
    QLineSeries *fcPosThreshold = new QLineSeries();
    fcPosThreshold->append(1, 0);
    fcPosThreshold->append(1, maxNegLog10P);
    fcPosThreshold->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    chart->addSeries(fcPosThreshold);
    fcPosThreshold->attachAxis(xAxis);
    fcPosThreshold->attachAxis(yAxis);

    QLineSeries *fcNegThreshold = new QLineSeries();
    fcNegThreshold->append(-1, 0);
    fcNegThreshold->append(-1, maxNegLog10P);
    fcNegThreshold->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    chart->addSeries(fcNegThreshold);
    fcNegThreshold->attachAxis(xAxis);
    fcNegThreshold->attachAxis(yAxis);

    // 设置图例
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    //隐藏阈值线的图例
    for (QLegendMarker* marker : chart->legend()->markers()) {
        if (qobject_cast<QLineSeries*>(marker->series())) {
            marker->setVisible(false);  // 隐藏所有线系列的图例
        }
    }

    return chart;
}
