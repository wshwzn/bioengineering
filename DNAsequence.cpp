#include"DNAsequence.h"
#include<iomanip>

int DNAsequence::length()const {
	return static_cast<int>(sequence.size());
}

//GC
double DNAsequence::gcContent()const {
	return gContent() + cContent();
}

//A
double DNAsequence::aAmount()const {
	if (sequence.size() == 0)return 0;
	double amount = 0;
	for (string::const_iterator it = sequence.begin(); it != sequence.end(); it++) {
		if (*it == 'A')amount++;
	}
	return amount;
}
double DNAsequence::aContent()const {
	double amount = aAmount();
	double res;
	res = amount / double(sequence.size());
	return res;
}
//T
double DNAsequence::tAmount()const {
	if (sequence.size() == 0)return 0;
	double amount = 0;
	for (string::const_iterator it = sequence.begin(); it != sequence.end(); it++) {
		if (*it == 'T')amount++;
	}
	return amount;
}
double DNAsequence::tContent()const {
	double amount = tAmount();
	double res;
	res = amount / double(sequence.size());
	return res;
}
//C
double DNAsequence::cAmount()const {
	if (sequence.size() == 0)return 0;
	double amount = 0;
	for (string::const_iterator it = sequence.begin(); it != sequence.end(); it++) {
		if (*it == 'C')amount++;
	}
	return amount;
}
double DNAsequence::cContent()const {
	double amount = cAmount();
	double res;
	res = amount / double(sequence.size());
	return res;
}
//G
double DNAsequence::gAmount()const {
	if (sequence.size() == 0)return 0;
	double amount = 0;
	for (string::const_iterator it = sequence.begin(); it != sequence.end(); it++) {
		if (*it == 'G')amount++;
	}
	return amount;
}
double DNAsequence::gContent()const {
	double amount = gAmount();
	double res;
	res = amount / double(sequence.size());
	return res;
}
//N
double DNAsequence::nAmount()const {
	if (sequence.size() == 0)return 0;
	double amount = 0;
	for (string::const_iterator it = sequence.begin(); it != sequence.end(); it++) {
		if (*it == 'N')amount++;
	}
	return amount;
}
double DNAsequence::nContent()const {
	double amount = nAmount();
	double res;
	res = amount / double(sequence.size());
	return res;
}



 ostream& operator<<(ostream& cout, DNAsequence& seq) {
	cout << "������Ϣ:  " << seq.header << endl;
	cout << "���г���:  " << seq.length() << endl;
	cout << "��������:  " << seq.sequence << endl;
	cout << "������ɷ֣�" << endl;
	cout << "A����:" << seq.aAmount() << "(" << fixed << setprecision(2) << seq.aContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	cout << "T����:" << seq.tAmount() << "(" << fixed << setprecision(2) << seq.tContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	cout << "C����:" << seq.cAmount() << "(" << fixed << setprecision(2) << seq.cContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	cout << "G����:" << seq.gAmount() << "(" << fixed << setprecision(2) << seq.gContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	cout << "δ֪����N����:" << seq.nAmount() << "(" << fixed << setprecision(2) << seq.nContent() * 100.0 << "%" << ")" << setprecision(0) << endl;
	cout << "GC����:  " << fixed << setprecision(2) << seq.gcContent() * 100.0 << "%" << endl;

	return cout;
}