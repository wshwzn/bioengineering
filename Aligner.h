#pragma once
#include<iostream>
#include<string>
#include"AlignmentResult.h"
#include"DNAsequence.h"
using namespace std;

class Aligner {
public:
	//NeedlemanWunch算法
	AlignmentResult NeedlemanWunch(const DNAsequence& dnaseq1,const DNAsequence& dnaseq2,
		int match_score=1,
	    int mismatch_penalty=-1,
		int gap_penalty=-1);

private:
	
	//创建得分矩阵
	vector<vector<int>> createScoringMatrix(const string& seq1, const string& seq2, int gap_penalty);
	
	//回溯找出最优比对路径
	pair<string, string> traceback(
		const vector<vector<int>>& score_matrix,
		const string& seq1,
		const string& seq2,
		int match_score,
		int mismatch_penalty,
		int gap_penalty
	);

	//获取匹配指示行
	string get_match_line(const pair<string, string>&seqs);

};