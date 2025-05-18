#pragma once
#include<iostream>
#include<string>
using namespace std;

class DNAsequence {
public:
	string header;//���б�ʶ
	string sequence;//ʵ����������
	


	//���캯��
	DNAsequence():header(""),sequence(""){}

	//�������г���
	int length()const;

	//����A����
	double aAmount()const;
	double aContent()const;

	//����T����
	double tAmount()const;
	double tContent()const;

	//����C����
	double cAmount()const;
	double cContent()const;

	//����G����
	double gAmount()const;
	double gContent()const;

	//����N����
	double nAmount()const;
	double nContent()const;

	//����GC����
	double gcContent()const;


};

//����DNAsequence�������͵����
ostream& operator<<(ostream& cout, DNAsequence& seq);