#pragma once
#include<vector>
#include<map>
#include<iostream>
#include<string>
using namespace std;

struct GeneExpressionData {
	vector<string> gene_names; 
	vector<string> sample_names;
	vector<vector<double>> expression_matrix;
	//map<string, string> metadata;
};