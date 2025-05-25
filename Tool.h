#pragma once
#include<iostream>
using namespace std;
class Tool
{
public:
	//显示功能
	void Show_Menu();

	//退出功能
	void exitsystem();

	//DNA序列分析功能
	void DNAsequenceAnalyse();

	//DNA序列比对功能
	void DNAsequenceAlign();

	//差异表达数据分析功能
	void GEAnalyse();

	//绘制差异表达分析结果火山图功能
	void drewpicture();
};

//自定义输入字符串
void mycin(string& str);