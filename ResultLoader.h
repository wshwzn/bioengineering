#pragma once
#include"DESeq2Result.h"
#include<vector>

class ResultLoader
{
public:
	//��CSV�ļ��ж�ȡDESeq2�������
	vector<DESeq2Result> readDESeq2Results(const std::string& filename);
};

