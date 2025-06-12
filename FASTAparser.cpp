#include "FASTAparser.h"
#include <QDebug>


DNAsequence  FASTAparser::singleParse(const string& filename) {

    ifstream fin(filename);
    qDebug()<<filename;
    if(fin.is_open()){
        qDebug()<<"打开成功";
    }
    else{
        qDebug()<<"打开失败";
    }
    DNAsequence currentSeq;
    string line;

    while (getline(fin, line)) {
        //跳过空行
        if (line.empty()) {
            continue;
        }
        //头行写入当前序列的序列标识
        else if (line[0] == '>') {
            currentSeq.header = line;
        }
        //数据行写入当前序列的序列数据
        else {
            currentSeq.sequence += line;
        }
    }
    fin.close();
    return currentSeq;
}
