#include"CSVLoader.h"
#include<fstream>
#include<sstream>
CountData CSVLoader::loadFromCSV(const string& filename) {
	CountData data;
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
		
		vector<int>expr_values;
		string value_str;
		//逐个读取每一列数据(表达量)
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