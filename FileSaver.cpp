#include "FileSaver.h"
#include<iomanip>
#include<iostream>
#include<fstream>
#include<string>


void FileSaver::namePlus(string& filename){
	//��ȡ�����ļ���
	string basename = filename.substr(0, filename.find('.'));
	//��ȡ�ļ���׺
	string suffix = filename.substr(filename.find('.'));
	
	for (int i = 1;; i++) {
		ifstream filetext(filename);
		if (filetext.good()) {
			filetext.close();
			filename = basename + "_" + to_string(i) + suffix;
		}
		else
			return;
	}
}

void FileSaver::saveAnalysis(const string& filename,const DNAsequence & seq) {
	ofstream fout(filename);
	fout << "������Ϣ:  " << seq.header << endl;
	fout << "���г���:  " << seq.length() << endl;
	fout << "��������:  " << seq.sequence << endl;
	fout << "������ɷ֣�" << endl;
	fout << "A����:" << seq.aAmount() << "(" << fixed << setprecision(2) << seq.aContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "T����:" << seq.tAmount() << "(" << fixed << setprecision(2) << seq.tContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "C����:" << seq.cAmount() << "(" << fixed << setprecision(2) << seq.cContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "G����:" << seq.gAmount() << "(" << fixed << setprecision(2) << seq.gContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "δ֪����N����:" << seq.nAmount() << "(" << fixed << setprecision(2) << seq.nContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "GC����:  " << fixed << setprecision(2) << seq.gcContent() * 100.0 << "%" << endl;
	fout.close();
	return;
}

void FileSaver::saveAlignment(const string& filename, const AlignmentResult& alignmentresult) {
	ofstream fout(filename);
	fout << u8R"(<html><head>
    <meta charset="UTF-8">
    <title>AlignmentResult</title>
    <style>
        body {
            margin: 0;
            padding: 20px;
            font-family:monospace;  
        }
        /* ���������� */
        .scroll-container {
            overflow-x: auto;  /* ��ʾ��������� */
            max-width: 100%;   /* ����������� */
            border: 1px solid #ddd;
            background:#E8E8E8;
            padding: 10px;
            line-height:0;
        }
        /* ÿ���ı����� */
        .line {
            white-space: pre;  /* �����ո񣬽�ֹ�ı����� */
            margin: 0px 0;       /* ��ֱ��� */
            padding: 6px;
            background: #white;
            /*border: 1px solid #ccc;*/
        }
    </style>
</head>
<body>
    <h2 align="center">���бȶԽ��</h2>
    <h3>���ȶ�������Ϣ��</h3>
    <div>
        <p>)" << alignmentresult.seq1header << u8R"(<br>���г��ȣ�)" << alignmentresult.seq1lenth << u8R"(</p>
        <p>)" << alignmentresult.seq2header << u8R"(<br>���г��ȣ�)" << alignmentresult.seq2lenth << u8R"(</p>
    </div>
    <h3>�ȶԽ��:</h3>
    <div class="scroll-container">  <!-- ���������� -->

        <div class="line">)" << alignmentresult.aligned_seq1 << u8R"(</div>

        <div class="line">)" << alignmentresult.match_line << u8R"(</div>

        <div class="line">)" << alignmentresult.aligned_seq2 << u8R"(</div>

    </div>
</body></html>)";
	fout.close();
}
