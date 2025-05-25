#pragma once
#include<vector>
#include<map>
#include<unordered_map>
#include<iostream>
#include<string>
using namespace std;

struct CountData {
	vector<string> gene_names; 
	vector<string> sample_names;
	vector<vector<int>> count_matrix;
	unordered_map <string, string> sample_to_group;
};

