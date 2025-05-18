#include<iostream>
#include<string>
using namespace std;

#include"CSVLoader.h"
#include"GeneExpressionData.h"
#include"Tool.h"
int main()
{
	
		
	Tool tool;
	CSVLoader loader;

	//while (1) {
	//	tool.Show_Menu();
	//	int choice=-1;
	//	string inp;
	//	cout << "请输入您的选择" << endl;
	//	//如果缓冲区不为空则清空后再进行输入
	//	if (cin.rdbuf()->in_avail() != 0)
	//		cin.ignore(100, '\n');
	//	//获取输入的字符串inp并清除前后空格
	//	getline(cin, inp);
	//	inp.erase(0, inp.find_first_not_of(' '));
	//	inp.erase(inp.find_last_not_of(' ')+1);
	//	//如果字符串大小为1，转换成int类型数字;
	//	if (inp.size() == 1) {
	//		choice = static_cast<int>(inp[0]) - 48;
	//	}
	//	
	//	switch (choice)
	//	{
	//	case 0:
	//		tool.exitsystem();
	//		break;
	//	case 1:
	//		tool.DNAsequenceAnalyse();
	//		break;
	//	case 2:
	//		tool.DNAsequenceAlign();
	//		break;
	//	default:
	//		system("cls");
	//		break;
	//	}
	//}
	GeneExpressionData data;
	data=loader.loadFromCSV("test.csv");
	cout << "\t";
	/*for (vector<string>::iterator it = data.sample_names.begin(); it != data.sample_names.end(); it++) {
		cout << *it << "\t";
	}
	cout << endl;
	for (vector<string>::iterator it = data.gene_names.begin(); it != data.gene_names.end(); it++) {
		cout << *it << endl;
	}*/
	for (int i = 0; i < data.expression_matrix.size(); i++) {
		for (auto it = data.expression_matrix[i].begin(); it !=data.expression_matrix[i].end(); it++) {
			cout << *it<<"\t";
		}
		cout << endl;
	}
}