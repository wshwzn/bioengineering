#include "Painter.h"
using namespace cv;

Mat Painter::createVolcanoPlot(const std::vector<DESeq2Result>& data, int width, int height) {
    //�ر�����ʱ��־����
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    // �����հ�ͼ��
    Mat plot(height, width, CV_8UC3, Scalar(255, 255, 255));

    // ���ñ߾�
    const int margin = 50;
    const int plotWidth = width - 2 * margin;
    const int plotHeight = height - 2 * margin;
    const int bottomY = height - margin;
    const int leftX = margin;

    // ȷ�����ݷ�Χ
    double maxAbsLog2FC = 0;
    double maxNegLog10P = 0;

    for (const auto& gene : data) {
        double absLog2FC = std::abs(gene.log2_fold_change);
        if (absLog2FC > maxAbsLog2FC) maxAbsLog2FC = absLog2FC;

        double negLog10P = -std::log10(gene.padj);
        if (negLog10P > maxNegLog10P) maxNegLog10P = negLog10P;
    }

    // ���ú�����᷶Χ
    maxAbsLog2FC = std::max(3.0, maxAbsLog2FC * 1.1); // ������ʾ����3

    maxNegLog10P = std::max(5.0, maxNegLog10P * 1.1); // ������ʾ��5
    //std::cout << "���pֵ��" << maxNegLog10P;


    // ������������
    const double xScale = plotWidth / (2 * maxAbsLog2FC);
    const double yScale = plotHeight / maxNegLog10P;

    // ����������
    line(plot, Point(leftX, bottomY), Point(width - margin, bottomY), Scalar(0, 0, 0), 2);
    line(plot, Point(leftX + plotWidth / 2, margin), Point(leftX + plotWidth / 2, bottomY), Scalar(0, 0, 0), 2);

    // ������ǩ
    putText(plot, "log2(Fold Change)", Point(width / 2 - 80, height - 10),
        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));

    // ��ת�������Y���ǩ
    Mat rotated;
    transpose(plot, rotated);
    flip(rotated, rotated, 0);
    putText(rotated, "-log10(p-adj)", Point(20, 30),
        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
    transpose(rotated, plot);
    flip(plot, plot, 1);

    // �������ݵ�
    for (const auto& gene : data) {
        // ����x���� (������Ϊ0)
        int x = leftX + plotWidth / 2 + static_cast<int>(gene.log2_fold_change * xScale);

        // ����y���� (�ӵײ�����)
        double negLog10P = -std::log10(gene.padj);
        int y = bottomY - static_cast<int>(negLog10P * yScale);

        // ȷ�����ڻ�ͼ������
        x = std::max(leftX, std::min(width - margin, x));
        y = std::max(margin, std::min(bottomY, y));

        // ������ɫ
        Scalar color;
        if (gene.padj < 0.05 && gene.log2_fold_change > 1) {
            color = Scalar(0, 0, 255); // �����ϵ������ú�ɫ
        }
        else if (gene.padj < 0.05 && gene.log2_fold_change < -1) {
            color = Scalar(255, 0, 0); // �����µ�����ɫ
        }
        else if (std::abs(gene.log2_fold_change) > 1) {
            color = Scalar(0, 165, 255); // ��FC�����ó�ɫ
        }
        else {
            color = Scalar(150, 150, 150); // �������û�ɫ
        }

        circle(plot, Point(x, y), 1, color, -1);
    }

    // �����ֵ��
    // log2FC��ֵ�� (��1)
    int threshX1 = leftX + plotWidth / 2 + static_cast<int>(1.0 * xScale);
    int threshX2 = leftX + plotWidth / 2 + static_cast<int>(-1.0 * xScale);
    line(plot, Point(threshX1, margin), Point(threshX1, bottomY), Scalar(200, 200, 200), 1);
    line(plot, Point(threshX2, margin), Point(threshX2, bottomY), Scalar(200, 200, 200), 1);

    // p-value��ֵ�� (��Ӧp=0.05)
    double pThresh = -std::log10(0.05);
    int threshY = bottomY - static_cast<int>(pThresh * yScale);
    line(plot, Point(leftX, threshY), Point(width - margin, threshY), Scalar(200, 200, 200), 1);

    // ���ͼ��
    putText(plot, "Significant (FDR<0.05 & FC>1)", Point(width - 250, 30),
        FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255));
    putText(plot, "Significant (FDR<0.05 & FC<-1)", Point(width - 250, 50),
        FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0));
    putText(plot, "Only |FC|>1", Point(width - 250, 70),
        FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 165, 255));
    putText(plot, "Not significant", Point(width - 250, 90),
        FONT_HERSHEY_SIMPLEX, 0.4, Scalar(150, 150, 150));

    return plot;
}
