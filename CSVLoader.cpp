#include"CSVLoader.h"
#include<fstream>
#include<sstream>
GeneExpressionData CSVLoader::loadFromCSV(const string& filename) {
	GeneExpressionData data;
	ifstream fin(filename);
	if (!fin.is_open())
		throw runtime_error("�޷����ļ�\"" + filename + "\"");
	string line;
	//��ȡ�������ƣ���һ�У�
	if (getline(fin, line)) {
		istringstream iss(line);
		string cell;
		//������һ�С��������ơ�
		getline(iss, cell,',');
		//���ζ�ȡ������
		while (getline(iss, cell, ',')) {
			data.sample_names.push_back(cell);
		}
	}
	//���ж�ȡ����
	while (getline(fin, line)) {
		istringstream iss(line);
		string gene_name;
		//��ȡ��һ������(������)
		getline(iss, gene_name, ',');
		data.gene_names.push_back(gene_name);
		
		vector<double>expr_values;
		string value_str;
		//�����ȡÿһ������(�����)
		while (getline(iss, value_str, ',')) {
			expr_values.push_back(stod(value_str));
		}
		data.expression_matrix.push_back(expr_values);
	}
	return data;
}