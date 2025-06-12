#include "ResultLoader.h"
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;

vector<DESeq2Result> ResultLoader::readDESeq2Results(const string& filename) {

    vector<DESeq2Result> data;
    ifstream file(filename);
    if (!file.is_open())
        throw runtime_error("无法打开文件\"" + filename + "\"");

    string line;
    // 跳过标题行
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        DESeq2Result gene;
        string token;

        // CSV格式为: genename,log2_fold_change,pvalue,padj
        getline(iss, gene.gene_name, ',');
        //genmename
        getline(iss, token, ',');
        gene.log2_fold_change = stod(token);
        //log2fc
        getline(iss, token, ',');
        gene.p_value = stod(token);
        //pvalue
        getline(iss, token, ',');
        gene.padj = stod(token) + (2.2e-16);
        //padj
        data.push_back(gene);
    }

    file.close();
    return data;
}
