#pragma once
#include"CountData.h"
class CSVLoader {
public:

	//从csv文件中读取基因表达数据
	CountData loadFromCSV(const string& filename);

};