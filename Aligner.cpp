#include"Aligner.h"
#include<algorithm>
AlignmentResult Aligner::NeedlemanWunch(const DNAsequence& dnaseq1, const DNAsequence& dnaseq2,
	int match_score,
	int mismatch_penalty,
	int gap_penalty) {
	string seq1 = dnaseq1.sequence;
	string seq2 = dnaseq2.sequence;
	//�����÷־���
	auto score_matrix = createScoringMatrix(seq1, seq2, gap_penalty);
	//����������
	for (int i = 1; i <= seq1.size(); i++) {
		for (int j = 1; j <= seq2.size(); j++) {
			//�����������ܵ÷�
			int s1 = score_matrix[i - 1][j] + gap_penalty;
			int s2 = score_matrix[i][j - 1] + gap_penalty;
			int s3 = score_matrix[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? match_score : mismatch_penalty);
			//ȡ���ֵ��Ϊ��ǰ�÷�
			score_matrix[i][j] = max({s1,s2,s3});
		}
	}
	//���ɱȶԺ�����
	auto aligned_seqs = traceback(score_matrix, seq1, seq2, match_score, mismatch_penalty, gap_penalty);
	string aligned_seq1 = aligned_seqs.first;
	string aligned_seq2 = aligned_seqs.second;
	//����ƥ��ָʾ��
	string match_line = get_match_line(aligned_seqs);

	return{ dnaseq1.header,dnaseq1.length(),dnaseq2.header,dnaseq2.length(),aligned_seq1,aligned_seq2,match_line};
}

vector<vector<int>> Aligner::createScoringMatrix(const string& seq1, const string& seq2, int gap_penalty) {
	//��������Ϊseq1.size+1 ����Ϊseq2.size+1�ľ��󣬲�����ֵΪ0
	vector<vector<int>>matrix(seq1.size() + 1, vector<int>(seq2.size() + 1, 0));
	//��ʼ����0�е�0��
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
		//���ر߽����
		if (i > 0 && j > 0) {
			//ƥ��
			if (seq1[i - 1] == seq2[j - 1]) {
				aligned_seq1 = seq1[i - 1] + aligned_seq1;
				aligned_seq2 = seq2[j - 1] + aligned_seq2;
				i--;   j--; 
				continue;
			}
			//��ƥ��
			else {
				s1 = score_matrix[i - 1][j]+gap_penalty;
				s2 = score_matrix[i][j-1]+gap_penalty;
				s3 = score_matrix[i - 1][j-1]+mismatch_penalty;
				//����
				if (score_matrix[i][j] == s1) {
					aligned_seq1 = seq1[i - 1] + aligned_seq1;
					aligned_seq2 = '-' + aligned_seq2;
					i--; j;
					continue;
				}
				//����
				else if (score_matrix[i][j] == s2) {
					aligned_seq1 = '-' + aligned_seq1;
					aligned_seq2 = seq2[j - 1] + aligned_seq2;
					i; j--;
					continue;
				}
				//�Խ����ƶ�
				else {
					aligned_seq1 = seq1[i - 1] + aligned_seq1;
					aligned_seq2 = seq2[j - 1] + aligned_seq2;
					i--;   j--; 
					continue;
				}
			}
		}
		//�ر߽����
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