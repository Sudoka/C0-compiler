#pragma warning(disable:4786)
#include<iostream>
#include<string>
#include "error.h"
using namespace std;

//functions
string intToString1(int n) {
	string s = "";

	if (n == 0) {
		s = "0";	
		return s;
	}
	int m = 1, temp = n, i = 0;
	while (temp > 0) {
		temp /= 10;
		m *= 10;
	}
	while (1) {
		m /= 10;
		if (m == 0)
			break;
		s += " ";
		s[i] = (char)(n / m + 48);
		n %= m;
		i++;
	}
	return s;
}

int Error::count = 0;

Error::Error(int type, int line) {
	//count++;
	errorType = type;
	errorLine = line;
	switch (type) {
		//�Ƿ��ַ�
		case 0:
			errorMessage = "illegal character.";
			break;
		//ȱ��'\''
		case 1:
			errorMessage = "missing '\''.";
			break;
		//ȱ��'\"'
		case 2:
			errorMessage = "missing '\"'.";
			break;
		//ȱ��'('
		case 3:
			errorMessage = "missing '('.";
			break;
		//ȱ��')'
		case 4:
			errorMessage = "missing ')'.";
			break;
		//ȱ��','
		case 5:
			errorMessage = "missing ','.";
			break;
		//ȱ��';'
		case 6:
			errorMessage = "missing ';'.";
			break;
		//ȱ��'{'
		case 7:
			errorMessage = "missing '{'.";
			break;
		//ȱ��'}'
		case 8:
			errorMessage = "missing '}'.";
			break;
		//�Ƿ�����
		case 9:
			errorMessage = "illegal token.";
			break;
		//��ʶ��δ����
		case 10:
			errorMessage = "identifier has not been definited.";
			break;
		//��ʶ���ظ�����
		case 11:
			errorMessage = "identifier has already been definited.";
			break;
		//Ӧ�Ǳ�ʶ��
		case 12:
			errorMessage = "missing identifier.";
			break;
		//Ӧ�����ͱ�ʶ��
		case 13:
			errorMessage = "missing type.";
			break;
		//����δ����
		case 14:
			errorMessage = "function has not been definited.";
			break;
		//�����ظ�����
		case 15:
			errorMessage = "function has already been definited.";
			break;
		//ȱ��'='
		case 16:
			errorMessage = "missing '='.";
			break;
		//��������ʱ��������
		case 17:
			errorMessage = "too few arguments for call.";
			break;
		//��������ʱ��������
		case 18:
			errorMessage = "too many actual parameters.";
			break;
		//��������ʱ�������Ͳ�ƥ��
		case 19:
			errorMessage = "proc: type conversion error.";
			break;
		//����û�з���ֵ
		case 20:
			errorMessage = "must return a value.";
			break;
		//void�����з���ֵ
		case 21:
			errorMessage = "\'void\' function returning a value.";
			break;
		//����ֵ���Ͳ�ƥ��
		case 22:
			errorMessage = "return: type error.";
			break;
		//���Ͳ�ƥ��
		case 23:
			errorMessage = "type conversion error.";
			break;
		//�������޸�
		case 24:
			errorMessage = "cannot assign to a variable that is const.";
			break;
		default:
			break;
	}
}

Error::Error(int type, int line, string msg) {
	//count++;
	errorType = type;
	errorLine = line;
	errorMessage = msg;
}

void Error::print() {
	count++;
	cout << "error " + intToString1(errorType) + ": " + errorMessage + "\tline " + intToString1(errorLine) << endl;
}

int Warning::count = 0;

Warning::Warning(int type, int line) {
	
	warnType = type;
	warnLine = line;
	switch (type) {
		//float����ת����int����
		case 0:
			warnMessage = "conversion from \'float\' to \'int\', possible loss of data.";
			break;
		//float����ת����char����
		case 1:
			warnMessage = "conversion from \'float\' to \'char\', possible loss of data.";
			break;
		default:
			break;
	}
}

Warning::Warning(int type, int line, string msg) {
	warnType = type;
	warnLine = line;
	warnMessage = msg;
}

void Warning::print() {
	count++;
	cout << "warning " + intToString1(warnType) + ": " + warnMessage + "\tline " + intToString1(warnLine) << endl;
}