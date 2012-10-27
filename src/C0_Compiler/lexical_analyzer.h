#include<fstream>
#include<iostream>
#include<string>
#include<cctype>
#include<map>
using namespace std;
//������
enum Tag {
	Null = 0,
	CONST,
	INT,
	FLOAT,
	CHAR,
	VOID,
	IF,
	ELSE,
	WHILE,
	FOR,
	RETURN,
	MAIN,
	PRINTF,
	SCANF,
	ID,
	CHARACTER,
	STRING,
	INTEGER,
	REAL,
	PLUS,
	MINUS,
	STAR,
	DIV,
	LT,
	GT,
	ASSIGN,
	LPAR,
	RPAR,
	LBRACE,
	RBRACE,
	SQUOTE,
	DQUOTE,
	DOT,
	COMMA,
	SEMI,
	LE,
	GE,
	NE,
	EQU,
	CONSTANT,
	VAR,
	PARA,
	MAINPROC,
	VOIDPROC,
	PROC,
	NEG,
	TEMP
};

//�ʷ���Ԫ
class Token {
public:
	int tag;//����
	string lexeme;//����
	Token() {}
	Token(int t) { tag = t; }
	Token(int t, string s) { tag = t; lexeme = s; }
	~Token() { }
	string toString() { return lexeme; }
	void print() { cout << tag << endl; }
};

//����
class Num : public Token {
public:
	int value;//ֵ
	Num(int v) : Token(INTEGER) { value = v; }
	~Num() { }
	void print() { cout << tag << "," << toString() << endl; }
};

//����(�����ؼ��ֺͱ�ʶ��)
class Word : public Token {
public:
	static Word le, ge, ne, eq, neg, temp;
	Word() : Token() {}
	Word(string s, int tag) : Token(tag) { lexeme = s; }
	~Word() { }
	void print() { cout << tag << "," << toString() << endl; }
};

//ʵ��
class Real : public Token {
public:
	float value;//ֵ
	Real(float v) : Token(REAL) { value = v; }
	~Real() { }
	void print() { cout << tag << "," << toString() << endl; }
};

//�ַ�
class Char : public Token {
public:
	char value;//ֵ
	Char(char c) : Token(CHARACTER) {value = c; }
	~Char() { }
	int toInteger() { return ((int)value - 48); }
	void print() { cout << tag << "," << toString() << endl; }
};

//���ͱ�ʶ��
class Type : public Word {
public:
	int width;//��ռ�ڴ��С
	static Type _int, _float, _char, _null;
	Type() : Word() { }
	Type(string s, int tag, int w) : Word(s, tag) { width = w; }
	~Type() { }
	static bool numeric(Type p);//�Ƿ�Ϊ�Ϸ�����
	static Type max(Type p1, Type p2);//�����Զ�����
};

//�ʷ�������
class Lexer {	
public:
	char peek;//��ǰ�ַ�
	map<string, Word> words;
	static int line;//��ǰ����
	Lexer();
	~Lexer() { }
	void reserve(Word w) { words.insert(map<string, Word>::value_type(w.lexeme, w)); }//����ؼ���
	void readch(ifstream &file);//��ǰ��һ�ַ�
	bool readch(char c, ifstream &file);//��ǰ��һ�ַ����ж��Ƿ�Ϊָ���ַ�
	Token scan(ifstream &file);//��ȡ�ʷ���Ԫ������
};
