#include"CSVLoader.h"
#include<fstream>
#include<sstream>
CountData CSVLoader::loadFromCSV(const string& filename) {
	CountData data;
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
		
		vector<int>expr_values;
		string value_str;
		//�����ȡÿһ������(�����)
		while (getline(iss, value_str, ',')) {
			expr_values.push_back(stoi(value_str));
		}
		data.count_matrix.push_back(expr_values);
	}
	for (string sam : data.sample_names) {
		string sub = sam.substr(0, 1);
		if (sub == "P"||sub=="R"||sub=="E"||sub=="S")
			data.sample_to_group.insert(make_pair(sam, "RA"));
		else 
			data.sample_to_group.insert(make_pair(sam, "HC"));
		//data.sample_to_group.insert(make_pair(sam, sam.substr(0, 2)));
	}

	return data;
}