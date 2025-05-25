#pragma once
#include<iostream>
using namespace std;

struct DESeq2Result
{
	string gene_name;//基因名称
	//double base_mean;//平均标准化表达量
	double log2_fold_change;//对数2倍变化值
	double p_value;//原始p值
	double padj;//校正后p值

};