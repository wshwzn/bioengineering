#ifndef ALIGNMENTRESULT_H
#define ALIGNMENTRESULT_H

#include<string>
#include<vector>
using namespace std;

struct AlignmentResult
{
public:
    string seq1header;  //进行比对序列信息
    int seq1lenth;  //进行比对序列长度
    string seq2header;
    int seq2lenth;
    string aligned_seq1;  //比对后序列1
    string aligned_seq2;  //比对后序列2
    string match_line;    //匹配指示行
};

#endif // ALIGNMENTRESULT_H
