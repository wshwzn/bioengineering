#pragma once
#include<iostream>
using namespace std;
class Tool
{
public:
	//��ʾ����
	void Show_Menu();

	//�˳�����
	void exitsystem();

	//DNA���з�������
	void DNAsequenceAnalyse();

	//DNA���бȶԹ���
	void DNAsequenceAlign();

	//���������ݷ�������
	void GEAnalyse();

	//���Ʋ�������������ɽͼ����
	void drewpicture();
};

//�Զ��������ַ���
void mycin(string& str);