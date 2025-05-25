#pragma once
#include<iostream>
#include"DNAsequence.h"
#include"AlignmentResult.h"
#include"DESeq2Result.h"
using namespace std;

class FileSaver
{
public:
	//Ϊ�ļ���������ֺ�׺�����ظ�
	void namePlus(string &filename);

	//����DNA���з������
	void saveAnalysis(const string& filename,const DNAsequence& seq);

	//����DNA���бȶԽ��
	void saveAlignment(const string& filename, const AlignmentResult& alignmentresult);

	///���������������
	void saveDESeq2(const string& filename, const vector<DESeq2Result>& results);

	//�����ɽͼ
};

