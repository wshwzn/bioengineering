#pragma once
#include"DNAsequence.h"
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

class FASTAparser
{
public:
	
	//�ӵ���FASTA�ļ���ȡ������Ϣ
	DNAsequence  singleParse(const string& filename);

};