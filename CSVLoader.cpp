#include"CSVLoader.h"
#include<fstream>
#include<sstream>
GeneExpressionData CSVLoader::loadFromCSV(const string& filename) {
	GeneExpressionData data;
	ifstream fin(filename);
	if (!fin.is_open())
		throw runtime_error("无法打开文件\"" + filename + "\"");
	string line;
	//读取样本名称（第一行）
	if (getline(fin, line)) {
		istringstream iss(line);
		string cell;
		//跳过第一列“基因名称”
		getline(iss, cell,',');
		//依次读取样本名
		while (getline(iss, cell, ',')) {
			data.sample_names.push_back(cell);
		}
	}
	//逐行读取数据
	while (getline(fin, line)) {
		istringstream iss(line);
		string gene_name;
		//读取第一列数据(基因名)
		getline(iss, gene_name, ',');
		data.gene_names.push_back(gene_name);
		
		vector<double>expr_values;
		string value_str;
		//逐个读取每一列数据(表达量)
		while (getline(iss, value_str, ',')) {
			expr_values.push_back(stod(value_str));
		}
		data.expression_matrix.push_back(expr_values);
	}
	return data;
}