#pragma once
#include<iostream>
using namespace std;
struct DifferentialResult
{
	string gene_name;
	double log2_fold_change;
	double p_value;
	double adjusted_p;
	double mean_group1;
	double mean_group2;
};