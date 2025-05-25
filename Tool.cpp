#include "Tool.h"
#include"FASTAparser.h"
#include"Aligner.h"
#include"FileSaver.h"
#include"CSVLoader.h"
#include"DESeq2Result.h"
#include"DESeq2Analyzer.h"
#include"ResultLoader.h"
#include"Painter.h"
void Tool::Show_Menu() {
	
	cout << "**************************************************************" << endl;
	cout << "*                   ��ӭʹ�����﹤��СӦ��                   *" << endl;
	cout << "*                       0.�˳�Ӧ�ó���                       *" << endl;
	cout << "*                       1.�������з���                       *" << endl;
	cout << "*                       2.�������бȶ�                       *" << endl;
	cout << "*                       3.���������ݷ���                   *" << endl;
	cout << "*                       4.���Ʋ�������������ɽͼ         *" << endl;
	cout << "**************************************************************" << endl;
	cout << endl;
}

void Tool:: exitsystem() {
	cout << "��ӭ�´�ʹ��" << endl;
	system("pause");
	exit(0);
}

void Tool::DNAsequenceAnalyse() {
	FASTAparser fp;
	FileSaver saver;
	cout << "��������Ҫ������DNA�����ļ�����(��֧��FASTA��ʽ�ļ�)"<<endl;
	string filename;
	mycin(filename);
	try {
		DNAsequence seq = fp.singleParse(filename);
		cout <<endl<< "����������£�" << endl << endl;
		cout << seq;
		cout << endl;

		cout << "�Ƿ���Ҫ����������" << endl;
		cout << "1 ��" << endl;
		cout << "0 ��" << endl;
		int choice = -1;
		while (1) {
			string inp;
			mycin(inp);
			if (inp.size() == 1) {
				int tp = inp[0] - 48;
				if (tp == 0 || tp == 1) {
					choice = tp;
					break;
				}
			}
			cout << "����ȷ����ѡ��" << endl;
		}
		if (choice == 1) {
			string savename = "���з������.txt";
			saver.namePlus(savename);
			saver.saveAnalysis(savename, seq);
			cout << "��������ѳɹ��������ļ�\"" << savename << "\"��" << endl;
		}

	}
	catch (runtime_error& e) {
		cerr << e.what() <<endl;
	}
	

	system("pause");
	system("cls");
	return;
}

void Tool::DNAsequenceAlign() {
	FASTAparser fp;
	DNAsequence dnaseq1;
	DNAsequence dnaseq2;
	bool iferror = false;

	cout << "��������Ҫ���бȶԵ�1��DNA�����ļ�����(��֧��FASTA��ʽ�ļ�)" << endl;
	string filename1;
	mycin(filename1);
	try {
		dnaseq1 = fp.singleParse(filename1);
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
		iferror = true;
	}
	if (iferror) {
		system("pause");
		system("cls");
		return;
	}
	cout << "��������Ҫ���бȶԵ�2��DNA�����ļ�����(��֧��FASTA��ʽ�ļ�)" << endl;
	string filename2;
	while (1) {
		mycin(filename2);
		if (filename2 == filename1) {
			cout << "���ܱȶ���ͬ��DNA�����ļ�������������" << endl;
		}
		else
			break;
	}
	try {
		
		dnaseq2 = fp.singleParse(filename2);
		
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
		iferror = true;
	}
	if (iferror) {
		system("pause");
		system("cls");
		return;
	}
	Aligner aligner;
	AlignmentResult alignmentresult;
	alignmentresult = aligner.NeedlemanWunch(dnaseq1, dnaseq2);
	FileSaver saver;
	string savename = "���бȶԽ��.html";
	saver.namePlus(savename);
	saver.saveAlignment(savename, alignmentresult);
	cout << endl;
	cout << "�ȶԽ���ѳɹ��������ļ�\"" << savename << "\"�У������в鿴" << endl;
	

	system("pause");
	system("cls");
	return;
}

void Tool::GEAnalyse() {
	CSVLoader loader;
	bool iferror = false;
	cout << "����������������ļ���" << endl;
	string filename;
	mycin (filename);
	CountData data;
	try {
		data = loader.loadFromCSV(filename);
	}
	catch(runtime_error&e){
		cerr << e.what() << endl;
		iferror = true;
	}
	if (iferror) {
		system("pause");
		system("cls");
		return;
	}
	
	vector<DESeq2Result>results = DESeq2Analyzer().analyze(data,"HC","RA");
	FileSaver saver;
	string savename = "������������.csv";
	saver.namePlus(savename);
	saver.saveDESeq2(savename, results);
	cout << "��������ѱ������ļ�\"" << savename << "\"�У������в鿴" << endl;
	string vsavename="��ɽͼ.png";
	saver.namePlus(vsavename);



	Painter painter;
	//����ͼ��
	cv::Mat volcanoPlot = painter.createVolcanoPlot(results);
	// ��ʾ������ͼ��
	cv::imshow("Volcano Plot", volcanoPlot);
	cv::imwrite(vsavename, volcanoPlot);
	cv::waitKey(0);
	cout << "�������ͼ���ѱ���Ϊ\"" << vsavename << "\"" << endl;

	system("pause");
	system("cls");
	return;
}

void Tool::drewpicture() {
	cout << "����������������ļ���" << endl;
	string filename;
	bool iferror = false;
	mycin(filename);
	ResultLoader loader;
	vector<DESeq2Result>data;
	try {
		data = loader.readDESeq2Results(filename);
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
		iferror = true;
	}
	if (iferror) {
		system("pause");
		system("cls");
		return;
	}
	Painter painter;
	FileSaver saver;
	string savename = "volcano_plot.png";
	saver.namePlus(savename);
	//����ͼ��
	cv::Mat volcanoPlot = painter.createVolcanoPlot(data);
	// ��ʾ������ͼ��
	cv::imshow("Volcano Plot", volcanoPlot);
	cv::imwrite(savename, volcanoPlot);
	cv::waitKey(0);
	cout << "ͼ���ѱ���Ϊ\"" << savename << "\"" << endl;
	system("pause");
	system("cls");
	return;
}

void mycin(string& str) {
	if (cin.rdbuf()->in_avail() != 0)
		cin.ignore(100, '\n');
	getline(cin, str);
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1);
}