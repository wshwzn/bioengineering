#include "FileSaver.h"
#include<iomanip>
#include<iostream>
#include<fstream>
#include<string>


void FileSaver::namePlus(string& filename){
	//获取基础文件名
	string basename = filename.substr(0, filename.find('.'));
	//获取文件后缀
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
	fout << "序列信息:  " << seq.header << endl;
	fout << "序列长度:  " << seq.length() << endl;
	fout << "序列数据:  " << seq.sequence << endl;
	fout << "核苷酸成分：" << endl;
	fout << "A含量:" << seq.aAmount() << "(" << fixed << setprecision(2) << seq.aContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "T含量:" << seq.tAmount() << "(" << fixed << setprecision(2) << seq.tContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "C含量:" << seq.cAmount() << "(" << fixed << setprecision(2) << seq.cContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "G含量:" << seq.gAmount() << "(" << fixed << setprecision(2) << seq.gContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "未知序列N含量:" << seq.nAmount() << "(" << fixed << setprecision(2) << seq.nContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	fout << "GC含量:  " << fixed << setprecision(2) << seq.gcContent() * 100.0 << "%" << endl;
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
        /* 外层滚动容器 */
        .scroll-container {
            overflow-x: auto;  /* 显示横向滚动条 */
            max-width: 100%;   /* 限制容器宽度 */
            border: 1px solid #ddd;
            background:#E8E8E8;
            padding: 10px;
            line-height:0;
        }
        /* 每行文本容器 */
        .line {
            white-space: pre;  /* 保留空格，禁止文本换行 */
            margin: 0px 0;       /* 垂直间距 */
            padding: 6px;
            background: #white;
            /*border: 1px solid #ccc;*/
        }
    </style>
</head>
<body>
    <h2 align="center">序列比对结果</h2>
    <h3>被比对序列信息：</h3>
    <div>
        <p>)" << alignmentresult.seq1header << u8R"(<br>序列长度：)" << alignmentresult.seq1lenth << u8R"(</p>
        <p>)" << alignmentresult.seq2header << u8R"(<br>序列长度：)" << alignmentresult.seq2lenth << u8R"(</p>
    </div>
    <h3>比对结果:</h3>
    <div class="scroll-container">  <!-- 外层滚动容器 -->

        <div class="line">)" << alignmentresult.aligned_seq1 << u8R"(</div>

        <div class="line">)" << alignmentresult.match_line << u8R"(</div>

        <div class="line">)" << alignmentresult.aligned_seq2 << u8R"(</div>

    </div>
</body></html>)";
	fout.close();
}
