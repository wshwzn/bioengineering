#include"Aligner.h"
#include<algorithm>
AlignmentResult Aligner::NeedlemanWunch(const DNAsequence& dnaseq1, const DNAsequence& dnaseq2,
	int match_score,
	int mismatch_penalty,
	int gap_penalty) {
	string seq1 = dnaseq1.sequence;
	string seq2 = dnaseq2.sequence;
	//创建得分矩阵
	auto score_matrix = createScoringMatrix(seq1, seq2, gap_penalty);
	//填充矩阵主体
	for (int i = 1; i <= seq1.size(); i++) {
		for (int j = 1; j <= seq2.size(); j++) {
			//计算三个可能得分
			int s1 = score_matrix[i - 1][j] + gap_penalty;
			int s2 = score_matrix[i][j - 1] + gap_penalty;
			int s3 = score_matrix[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? match_score : mismatch_penalty);
			//取最大值作为当前得分
			score_matrix[i][j] = max({s1,s2,s3});
		}
	}
	//生成比对后序列
	auto aligned_seqs = traceback(score_matrix, seq1, seq2, match_score, mismatch_penalty, gap_penalty);
	string aligned_seq1 = aligned_seqs.first;
	string aligned_seq2 = aligned_seqs.second;
	//生成匹配指示行
	string match_line = get_match_line(aligned_seqs);

	return{ dnaseq1.header,dnaseq1.length(),dnaseq2.header,dnaseq2.length(),aligned_seq1,aligned_seq2,match_line};
}

vector<vector<int>> Aligner::createScoringMatrix(const string& seq1, const string& seq2, int gap_penalty) {
	//创建行数为seq1.size+1 列数为seq2.size+1的矩阵，并赋初值为0
	vector<vector<int>>matrix(seq1.size() + 1, vector<int>(seq2.size() + 1, 0));
	//初始化第0行第0列
	for (int i = 1; i <= seq1.size(); i++) {
		matrix[i][0] = matrix[i - 1][0] + gap_penalty;
	}
	for (int i = 1; i <= seq2.size(); i++) {
		matrix[0][i] = matrix[0][i-1] + gap_penalty;
	}
	return matrix;
}

pair<string, string> Aligner::traceback(
	const vector<vector<int>>& score_matrix,
	const string& seq1,
	const string& seq2,
	int match_score,
	int mismatch_penalty,
	int gap_penalty) 
{
	string aligned_seq1;
	string aligned_seq2;
	int i = seq1.size();
	int j = seq2.size();
	int s1, s2, s3;
	while (i > 0 || j > 0) {
		//不沿边界情况
		if (i > 0 && j > 0) {
			//匹配
			if (seq1[i - 1] == seq2[j - 1]) {
				aligned_seq1 = seq1[i - 1] + aligned_seq1;
				aligned_seq2 = seq2[j - 1] + aligned_seq2;
				i--;   j--; 
				continue;
			}
			//不匹配
			else {
				s1 = score_matrix[i - 1][j]+gap_penalty;
				s2 = score_matrix[i][j-1]+gap_penalty;
				s3 = score_matrix[i - 1][j-1]+mismatch_penalty;
				//上移
				if (score_matrix[i][j] == s1) {
					aligned_seq1 = seq1[i - 1] + aligned_seq1;
					aligned_seq2 = '-' + aligned_seq2;
					i--; j;
					continue;
				}
				//左移
				else if (score_matrix[i][j] == s2) {
					aligned_seq1 = '-' + aligned_seq1;
					aligned_seq2 = seq2[j - 1] + aligned_seq2;
					i; j--;
					continue;
				}
				//对角线移动
				else {
					aligned_seq1 = seq1[i - 1] + aligned_seq1;
					aligned_seq2 = seq2[j - 1] + aligned_seq2;
					i--;   j--; 
					continue;
				}
			}
		}
		//沿边界情况
		else if (i == 0 && j > 0) {
			aligned_seq1 = '-' + aligned_seq1;
			aligned_seq2 = seq2[j - 1] + aligned_seq2;
			i; j--;
			continue;
		}
		else {
			aligned_seq1 = seq1[i - 1] + aligned_seq1;
			aligned_seq2 = '-' + aligned_seq2;
			i--; j;
			continue;
		}
	}
	return make_pair(aligned_seq1, aligned_seq2);
}

string Aligner::get_match_line(const pair<string, string>&seqs) {
	string seq1 = seqs.first;
	string seq2 = seqs.second;
	string match_line="";
	for (int i = 0; i < seq1.size(); i++) {
		if (seq1.at(i) == seq2.at(i)) {
			match_line = match_line+ "|" ;
		}
		else {
			match_line = match_line+" ";
		}
	}
	return match_line;
}