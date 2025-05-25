#pragma once
#include"DESeq2Result.h"
#include<vector>

class ResultLoader
{
public:
	//从CSV文件中读取DESeq2分析结果
	vector<DESeq2Result> readDESeq2Results(const std::string& filename);
};

