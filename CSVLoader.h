#pragma once
#include"GeneExpressionData.h"
class CSVLoader {
public:

	//从csv文件中读取基因表达数据
	GeneExpressionData loadFromCSV(const string& filename);

};