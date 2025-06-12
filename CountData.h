#ifndef COUNTDATA_H
#define COUNTDATA_H

#include<vector>
//#include<map>
#include<unordered_map>
#include<string>
using namespace std;

struct CountData {
    vector<string> gene_names;
    vector<string> sample_names;
    vector<vector<int>> count_matrix;
    unordered_map <string, string> sample_to_group;
};

#endif // COUNTDATA_H
