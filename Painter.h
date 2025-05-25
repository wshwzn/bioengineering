#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include"DESeq2Result.h"



class Painter
{
public:
	cv::Mat createVolcanoPlot(const std::vector<DESeq2Result>& data, int width = 800, int height = 600);
};

