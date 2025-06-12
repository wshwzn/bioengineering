#ifndef DNASEQUENCE_H
#define DNASEQUENCE_H

#include<string>
using namespace std;

class DNAsequence {
public:
    string header;//序列标识
    string sequence;//实际序列数据



    //构造函数
    DNAsequence():header(""),sequence(""){}

    //计算序列长度
    int length()const;

    //计算A含量
    double aAmount()const;
    double aContent()const;

    //计算T含量
    double tAmount()const;
    double tContent()const;

    //计算C含量
    double cAmount()const;
    double cContent()const;

    //计算G含量
    double gAmount()const;
    double gContent()const;

    //计算N含量
    double nAmount()const;
    double nContent()const;

    //计算GC含量
    double gcContent()const;


};

#endif // DNASEQUENCE_H
