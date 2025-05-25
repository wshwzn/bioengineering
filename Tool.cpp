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
	cout << "*                   欢迎使用生物工程小应用                   *" << endl;
	cout << "*                       0.退出应用程序                       *" << endl;
	cout << "*                       1.基因序列分析                       *" << endl;
	cout << "*                       2.基因序列比对                       *" << endl;
	cout << "*                       3.基因表达数据分析                   *" << endl;
	cout << "*                       4.绘制差异表达分析结果火山图         *" << endl;
	cout << "**************************************************************" << endl;
	cout << endl;
}

void Tool:: exitsystem() {
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}

void Tool::DNAsequenceAnalyse() {
	FASTAparser fp;
	FileSaver saver;
	cout << "请输入需要分析的DNA序列文件名称(仅支持FASTA格式文件)"<<endl;
	string filename;
	mycin(filename);
	try {
		DNAsequence seq = fp.singleParse(filename);
		cout <<endl<< "分析结果如下：" << endl << endl;
		cout << seq;
		cout << endl;

		cout << "是否需要保存分析结果" << endl;
		cout << "1 是" << endl;
		cout << "0 否" << endl;
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
			cout << "请正确输入选项" << endl;
		}
		if (choice == 1) {
			string savename = "序列分析结果.txt";
			saver.namePlus(savename);
			saver.saveAnalysis(savename, seq);
			cout << "分析结果已成功保存至文件\"" << savename << "\"中" << endl;
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

	cout << "请输入需要进行比对的1号DNA序列文件名称(仅支持FASTA格式文件)" << endl;
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
	cout << "请输入需要进行比对的2号DNA序列文件名称(仅支持FASTA格式文件)" << endl;
	string filename2;
	while (1) {
		mycin(filename2);
		if (filename2 == filename1) {
			cout << "不能比对相同的DNA序列文件，请重新输入" << endl;
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
	string savename = "序列比对结果.html";
	saver.namePlus(savename);
	saver.saveAlignment(savename, alignmentresult);
	cout << endl;
	cout << "比对结果已成功保存至文件\"" << savename << "\"中，请自行查看" << endl;
	

	system("pause");
	system("cls");
	return;
}

void Tool::GEAnalyse() {
	CSVLoader loader;
	bool iferror = false;
	cout << "请输入基因表达数据文件名" << endl;
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
	string savename = "差异表达分析结果.csv";
	saver.namePlus(savename);
	saver.saveDESeq2(savename, results);
	cout << "分析结果已保存至文件\"" << savename << "\"中，可自行查看" << endl;
	string vsavename="火山图.png";
	saver.namePlus(vsavename);



	Painter painter;
	//创建图像
	cv::Mat volcanoPlot = painter.createVolcanoPlot(results);
	// 显示并保存图像
	cv::imshow("Volcano Plot", volcanoPlot);
	cv::imwrite(vsavename, volcanoPlot);
	cv::waitKey(0);
	cout << "分析结果图像已保存为\"" << vsavename << "\"" << endl;

	system("pause");
	system("cls");
	return;
}

void Tool::drewpicture() {
	cout << "请输入基因表达数据文件名" << endl;
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
	//创建图像
	cv::Mat volcanoPlot = painter.createVolcanoPlot(data);
	// 显示并保存图像
	cv::imshow("Volcano Plot", volcanoPlot);
	cv::imwrite(savename, volcanoPlot);
	cv::waitKey(0);
	cout << "图像已保存为\"" << savename << "\"" << endl;
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