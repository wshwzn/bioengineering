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
		throw runtime_error("�޷����ļ�\"" + filename+"\"");
	}

	DNAsequence currentSeq;
	string line;

	while (getline(fin, line)) {
		//��������
		if (line.empty()) {
			continue;
		}
		//ͷ��д�뵱ǰ���е����б�ʶ
		else if (line[0] == '>') {
			currentSeq.header = line;
		}
		//������д�뵱ǰ���е���������
		else {
			currentSeq.sequence += line;
		}
	}
	fin.close();
	return currentSeq;
}
