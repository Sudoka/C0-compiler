#include<fstream>
#include<iostream>
#include<string>
#include<cctype>
#include<map>
#include<vector>
#include "lexical_analyzer.h"
#include "error.h"
using namespace std;

//��Ԫʽ
class Quadruple {
public:
	string op, expr1, expr2, res, quadstring;//��������������1�� ������2���������Ԫʽ�ַ���
	int label, jumpTo;//��ţ���ת���ı��
	int blockIndex;//�ڻ������е�λ��
	Quadruple() { label = 0; jumpTo = 0; blockIndex = 0; }
	~Quadruple() { }
};

//�﷨�ڵ�
class Node {
public:
	int lexline;//��������
	string name;//�ڵ���
	vector<Quadruple> code;//��Ԫʽ����
	static int labels;//���
	Node() { lexline = Lexer::line; }
	~Node() { }
	virtual int newlabel() { return ++labels; }//�����±��
};

//���ʽ
class Expr : public Node {
public:
	Token op;//������
	Type type;//����
	Expr() : Node() { type = Type::_null; }
	Expr(Token tok, Type p) { op = tok; type = p; }
	~Expr() { }
	virtual void gen() { }//������Ԫʽ
	virtual string toString() { return op.toString(); }
};

//��ʶ��
class Id : public Expr {
public:
	int kind, offset, defLine;//���࣬���ű���λ�ã���������
	string value;//ֵ(���Ϊ����)
	Id() : Expr() { }
	Id(Word id, Type p, int b) : Expr(id, p) { offset = b; }
	Id(Word id, Type p, int b, int k, int d) : Expr(id, p) { kind = k; offset = b; defLine = d; }
	~Id() { }
};

//��ʱ����
class Temp : public Expr {
public:
	static int count;//����
	int number;//���
	Temp() : Expr() { }
	Temp(Type p) : Expr(TEMP, p) { number = ++count; op.lexeme = toString(); }
	~Temp() { }
	virtual void gen() { }//������Ԫʽ
	virtual string toString();
};

//˫Ŀ�����
class Arith : public Expr {
public:
	Expr expr1, expr2;//����������Ҳ�����
	Temp temp;//���ɵ���ʱ����
	Arith() : Expr() { }
	Arith(Token tok, Expr x1, Expr x2);
	~Arith() { }
	virtual void gen();//������Ԫʽ
	virtual string toString() { return temp.toString() + " = " + expr1.toString() + " " + op.toString() + " " + expr2.toString(); }
};


//��Ŀ�����
class Unary : public Expr {
public:
	Expr expr;//������
	Temp temp;//���ɵ���ʱ����
	Unary() : Expr() { }
	Unary(Token tok, Expr x);
	~Unary() { }
	virtual void gen();//������Ԫʽ
	virtual string toString() { return temp.toString() + " = " + op.toString() + " " + expr.toString(); }
};

//�߼������
class Logical : public Expr {
public:
	Expr expr1, expr2;//����������Ҳ�����
	Logical() { }
	Logical(Token tok, Expr x1, Expr x2);
	~Logical() { }
	virtual void gen();//������Ԫʽ
	virtual string toString() { return expr1.toString() + " " + op.toString() + " " + expr2.toString(); }
};

//��ϵ�����
class Rel : public Logical {
public:
	int truelabel, falselabel;//���Ϊ����ת���ı�ţ����Ϊ����ת���ı��
	Rel() { }
	Rel(Token tok, Expr x1, Expr x2) : Logical(tok, x1, x2) { }
	~Rel() { }
	virtual void gen();//������Ԫʽ
};

//���
class Stmt : public Node {
public:
	int next;//�������ı��
	Stmt() { next = 0; }
	~Stmt() { }
	virtual void gen(Stmt &s);//������Ԫʽ
};

//If���
class If : public Stmt {
public:
	Rel expr;//��ϵ�����
	Stmt stmt;//if���
	If() { }
	If(Rel x, Stmt &s);
	~If() { }
	virtual void gen();//������Ԫʽ
};

//Else���
class Else : public Stmt {
public:
	Rel expr;//��ϵ�����
	Stmt stmt1, stmt2;//if��䣬else���
	Else() { }
	Else(Rel x, Stmt &s1, Stmt &s2) { expr = x; stmt1 = s1; stmt2 = s2; next = 0; }
	~Else() { }
	virtual void gen();//������Ԫʽ
};

//While���
class While : public Stmt {
public:
	Rel expr;//��ϵ�����
	Stmt stmt;//while���
	While() { }
	~While() { }
	void init(Rel x, Stmt &s) { expr = x; stmt = s; next = 0; }
	virtual void gen();//������Ԫʽ
};

//��ֵ���
class Set : public Stmt {
public:
	Id id;//����ʶ��
	Expr expr;//�Ҳ���ʽ
	Set() { }
	Set(Id i, Expr x);
	~Set() { }
	static Type check(Type p1, Type p2, int lexline);//���ͼ�����ʽת��
	virtual void gen();//������Ԫʽ
};

//For���
class For : public Stmt {
public:
	Rel expr;//��ϵ�����
	Stmt inits, paces, stmt;//��ʼ����䣬������䣬ѭ����
	For() { }
	~For() { }
	void init(Stmt s1, Rel x, Stmt s2, Stmt &s);
	virtual void gen();//������Ԫʽ
};

//�����
class Seq : public Stmt {
public:
	Stmt stmt1, stmt2;
	Seq(Stmt &s1, Stmt &s2) { stmt1 = s1; stmt2 = s2; }
	~Seq() { }
	virtual void gen();//������Ԫʽ
};

//���ű�
class Table {
public:
	map<string, Id> table, global;//�������ű�ȫ�ַ��ű�
	map<string, Temp> temps;//��ʱ������Ϣ
	vector<Quadruple> quad;//��Ԫʽ
	vector<int> paramsType;//��������
	vector<string> params;//������
	int paramsNum, tempNum;//������������ʱ��������
	Type type;//��������ֵ����
	Lexer lex;//�ʷ�������
	Table();
	Table(Lexer l) { lex = l; }
	~Table() { }
	void put(string s, Id sym);//����ű��в�������������Ͳ���
	void putTemp(string s, Temp temp);//����ű��в�����ʱ����
	Id get(string s);//�ӷ��ű���ȡ�������������Ͳ���
	Temp getTemp(string s);//�ӷ��ű���ȡ����ʱ����
};

//���ű��ֵ�
class Tables {
public:
	map<string, Table> tables;//���к������ű��ȫ�ַ��ű�
	Lexer lex;//�ʷ�������
	Tables();
	~Tables() { }
	void put(string s, Table t);//����ű��ֵ��в�����ű�
	Table* get(string s);//�ӷ��ű��ֵ���ȡ�����ű�
};

//�﷨������
class Parser {
public:
	Tables tables;//���ű��ֵ�
	Lexer lex;//�ʷ�������
	Token look;//��ǰ�ʷ���Ԫ
	int used;
	string proc;//��ǰ������

	Parser(Lexer l, ifstream &file);
	~Parser() { }
	void move(ifstream &file) { look = lex.scan(file); }//��ǰ��ȡ�ʷ���Ԫ
	//void error(string s) { cout << s << endl; }
	void match(int t, ifstream &file);//��ȡ�ʷ���Ԫ����ָ���ʷ���Ԫƥ��
	void match(int t, ifstream &file, string msg);//��ƥ�������������Ϣ
	void match(int t, ifstream &file, Error e);//��ƥ���򱨴�
	void program(ifstream &file);//����
	void mainProc(ifstream &file, Stmt &s);//������
	void block(ifstream &file, Stmt &s);//�������
	void stmts(ifstream &file, Stmt &s);//�����
	Stmt stmt(ifstream &file);//���
	Stmt ifs(ifstream &file);//if���
	Stmt whiles(ifstream &file);//while���
	Stmt fors(ifstream &file);//for���
	Stmt assign(ifstream &file);//��ֵ���
	Stmt assign(ifstream &file, Token tok);
	Stmt scan(ifstream &file);//scanf���
	Stmt print(ifstream &file);//printf���
	Stmt returns(ifstream &file);//�������
	void procDef(ifstream &file, Token tok, Type type);//�з���ֵ��������
	void voidProcDef(ifstream &file);//�޷���ֵ��������
	Stmt procCall(ifstream &file, Token tok);//�з���ֵ��������
	Stmt procCall(ifstream &file);
	Expr procCallToExpr(ifstream &file, Token tok);
	Stmt voidProcCall(ifstream &file, Token tok);//�з���ֵ��������
	Stmt voidProcCall(ifstream &file);
	Rel rel(ifstream &file);//����
	Expr expr(ifstream &file);//���ʽ
	Expr term(ifstream &file);//��
	Expr factor(ifstream &file);//����
	Stmt pace(ifstream &file, Token tok);//����
	void varDecls(ifstream &file, Token tok1, Token tok2);//��������
	void varDecls(ifstream &file);
	void varDef(ifstream &file, Token tok1, Token tok2);//��������
	void varDef(ifstream &file);
	void constDecls(ifstream &file);//��������
	void constDef(ifstream &file, Token tok1, Token tok2);//��������
	void constDef(ifstream &file);
};
