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
	//���㼸�ξ�ֵ
	vector<double>calculateGeometricMeans(const vector<vector<int>>& counts);

	//�����С����
	vector<double> calculateSizeFactors(const vector<vector<int>>& counts);

	//��С���ӹ�һ��
	vector<vector<double>> normalizeCounts(const vector<vector<int>>& counts, const vector<double>& size_factors);

	//��ɢ�ȹ���
	vector<double> estimateGeneDispersions(const vector<vector<double>>& normalized,
		const vector<string>& sample_names,
		const unordered_map<string,string>& sample_to_group,
		const string& control_group,const string& treatment_group);

	//�����ɢ������
	void fitDispersionTrend(const vector<double>& means,
		const vector<double>& dispersions);

	vector<double>trend_means; //������ϵ����Ƶ�
	vector<double> trend_disps;//��Ӧ�������ɢ��
	double trend_slope;//������б��
	double trend_intercept;//�����߽ؾ�

	//��������Ԥ����ɢ��
	double predictDispersion(double mean)const;

	
	//Wald����
	vector<DESeq2Result> performWaldTest(const vector<vector<double>>normalized,
		const vector<double>& dispersions,
		const CountData& data,
		const string& control_group, const string& treat_group);

	//BH����
	void performBH(vector<DESeq2Result>& results);
};
