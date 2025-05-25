#pragma once
#include<iostream>
#include"DNAsequence.h"
#include"AlignmentResult.h"
#include"DESeq2Result.h"
using namespace std;

class FileSaver
{
public:
	//为文件名添加数字后缀以免重复
	void namePlus(string &filename);

	//保存DNA序列分析结果
	void saveAnalysis(const string& filename,const DNAsequence& seq);

	//保存DNA序列比对结果
	void saveAlignment(const string& filename, const AlignmentResult& alignmentresult);

	///保存差异表达分析结果
	void saveDESeq2(const string& filename, const vector<DESeq2Result>& results);

	//保存火山图
};

