#include"FASTAparser.h"


//pair<DNAsequence, DNAsequence>  FASTAparser::parse(const string& filename1, const string& filename2)
//{
//	vector<DNAsequence> sequences;
//	DNAsequence seq1 = singleParse(filename1);
//	DNAsequence seq2=singleParse(filename2);
//	return make_pair(seq1,seq2);
//}

DNAsequence  FASTAparser::singleParse(const string& filename) {
	ifstream fin(filename);
	if (!fin.is_open())
	{
		throw runtime_error("无法打开文件\"" + filename+"\"");
	}

	DNAsequence currentSeq;
	string line;

	while (getline(fin, line)) {
		//跳过空行
		if (line.empty()) {
			continue;
		}
		//头行写入当前序列的序列标识
		else if (line[0] == '>') {
			currentSeq.header = line;
		}
		//数据行写入当前序列的序列数据
		else {
			currentSeq.sequence += line;
		}
	}
	fin.close();
	return currentSeq;
}
