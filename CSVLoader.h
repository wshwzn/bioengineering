#pragma once
#include"GeneExpressionData.h"
class CSVLoader {
public:

	//��csv�ļ��ж�ȡ����������
	GeneExpressionData loadFromCSV(const string& filename);

};