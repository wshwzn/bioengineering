#include<iostream>
#include<string>
#include"Tool.h"
using namespace std;

int main()
{
	
	Tool tool;

	while (1) {
		tool.Show_Menu();
		int choice=-1;
		string inp;
		cout << "请输入您的选择" << endl;
		mycin(inp);
		//如果字符串长度为1，转换成int类型数字;
		if (inp.size() == 1) {
			choice = static_cast<int>(inp[0]) - 48;
		}
		
		switch (choice)
		{
		case 0:
			tool.exitsystem();
			break;
		case 1:
			tool.DNAsequenceAnalyse();
			break;
		case 2:
			tool.DNAsequenceAlign();
			break;
		case 3:
			tool.GEAnalyse();
			break;
		case 4:
			tool.drewpicture();
			break;
		default:
			system("cls");
			break;
		}
	}



}