#pragma once
#include<string>
#include<vector>
using namespace std;

struct AlignmentResult
{
public:
	string seq1header;  //���бȶ�������Ϣ
	int seq1lenth;  //���бȶ����г���
	string seq2header;
	int seq2lenth;
	string aligned_seq1;  //�ȶԺ�����1
	string aligned_seq2;  //�ȶԺ�����2
	string match_line;    //ƥ��ָʾ��
};
