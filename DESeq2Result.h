#pragma once
#include<iostream>
using namespace std;

struct DESeq2Result
{
	string gene_name;//��������
	//double base_mean;//ƽ����׼�������
	double log2_fold_change;//����2���仯ֵ
	double p_value;//ԭʼpֵ
	double padj;//У����pֵ

};