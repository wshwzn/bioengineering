#pragma once
#include"CountData.h"
#include<vector>
#include<unordered_map>
#include"DESeq2Result.h"
using namespace std;

class DESeq2Analyzer
{
public:
	vector<DESeq2Result> analyze(const CountData& data,const string&control_group,const string&treat_group);
	
private:
	//计算几何均值
	vector<double>calculateGeometricMeans(const vector<vector<int>>& counts);

	//计算大小因子
	vector<double> calculateSizeFactors(const vector<vector<int>>& counts);

	//大小因子归一化
	vector<vector<double>> normalizeCounts(const vector<vector<int>>& counts, const vector<double>& size_factors);

	//离散度估计
	vector<double> estimateGeneDispersions(const vector<vector<double>>& normalized,
		const vector<string>& sample_names,
		const unordered_map<string,string>& sample_to_group,
		const string& control_group,const string& treatment_group);

	//拟合离散度趋势
	void fitDispersionTrend(const vector<double>& means,
		const vector<double>& dispersions);

	vector<double>trend_means; //用于拟合的趋势点
	vector<double> trend_disps;//对应的拟合离散度
	double trend_slope;//趋势线斜率
	double trend_intercept;//趋势线截距

	//根据趋势预测离散度
	double predictDispersion(double mean)const;

	
	//Wald检验
	vector<DESeq2Result> performWaldTest(const vector<vector<double>>normalized,
		const vector<double>& dispersions,
		const CountData& data,
		const string& control_group, const string& treat_group);

	//BH方法
	void performBH(vector<DESeq2Result>& results);
};
