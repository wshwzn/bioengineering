#pragma once
#include"DNAsequence.h"
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

class FASTAparser
{
public:
	
	//从单个FASTA文件读取序列信息
	DNAsequence  singleParse(const string& filename);

};