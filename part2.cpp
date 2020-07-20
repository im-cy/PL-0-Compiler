#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int sz = 200010;
int n;//总的划分数
int pos = 1;//当前处理到的的位置 
class table //SYM 的 class 
{
	public:
		table()
		{
			name.clear();
			kind.clear();
			level = 0;
			addr = 0;
		}
		void scan()
		{
			cin >> name >> kind >> level >> addr;
		}
		void print()
		{
			cout << "name:" << name << "  kind:" << kind
				 << "  level:" << level << "  addr" << addr << endl;
		}
		string name,kind;
		int level,addr;
}sym[sz];
map <string,int> constnum,idtnum;
int nowlev = 1;
int codeadd = 0;
int var_num = 0;
struct symcode
{
	string f;
	int l,a;
	void print()
	{
		cout << f << " " << l << " " << a << endl;
		
	}
}code[sz];

void init();
void nxt();
int gen(string f,int l,int a);//返回生成的地址 
bool isnum(char ins);
bool isletter(char ins);
bool Program();//程序 
bool Subprogram();//分程序 
bool Constant_description();//常量说明 
bool Variable_description();//变量说明 
bool Process_description();//过程说明
bool Constant_define();//常量定义 
bool Process_head();//过程首部 

bool Identifier();//标识符
bool Unsignedint();//无符号整数 
bool Compound_statement();//复合语句 
bool Statement(); //语句 
bool If_statement();//条件语句
bool If();//条件 
bool Expression();//表达式
int Relational_operators();//关系运算符  返回关系运算符的类型 
bool Term();//项 
bool Factor();//因子 
bool Assignment_statement();//赋值语句
bool While_statement();//当型循环语句 
bool Procedure_call();//过程调用语句 
bool Read_statement();//读写语句 
bool Write_statement();

int gen(string f,int l,int a)
{
	codeadd ++;
	code[codeadd].f = f;
	code[codeadd].l = l;
	code[codeadd].a = a;
	return codeadd;
}
void init()
{
	scanf("%d",&n);
	constnum.clear();
	idtnum.clear();
	for(int i = 1 ; i <= n ; i ++)
		sym[i].scan();
}
void Bug(string BugType)//error报告 
{
	cout << BugType << " error in " << sym[pos].addr << endl;
}
string now()//返回现在的kind 
{
	//cout << "#" << pos << " " << sym[pos].kind <<endl;
	return sym[pos].kind;
}
bool isnum(char ins)
{
	if(ins >= '0' && ins <= '9')
		return true;
	return false;
}
bool isletter(char ins)
{
	if(ins >= 'a' && ins <= 'z')
		return true;
	if(ins >= 'A' && ins <= 'Z')
		return true;
	return false;
}
void nxt()
{
	pos ++;
	if(sym[pos].name == "begin")
		nowlev ++;
	if(sym[pos].name == "end")
		nowlev --;
}
bool Program()//程序 
{
	bool ans = true;
	ans = Subprogram();//分程序
	if(!ans)
		Bug("Subprogram");
	if(now() != ".")
	{
		Bug("Missing symbol .");
		return false;
	}
	nxt();
	return true;
}
bool Subprogram()//分程序 
{
	bool ans = true;
	int cadd = gen("jmp",0,-1);//-1为待填 
	var_num = 3;
	ans = Constant_description();//常量说明 
	if(!ans)
	{
		Bug("Constant_description");
		return false;
	}
	ans = Variable_description();//变量说明 
	if(!ans)
	{
		Bug("Variable_description");
		return false;
	}
	ans = Process_description();//过程说明
	if(!ans)
	{
		Bug("Process_description");
		return false;
	}
	code[cadd].a = codeadd;//填入待填值
	gen("int",0,var_num);
	
	ans = Statement();
	if(!ans)
	{
		Bug("Statement error");
		return false;
	}
	gen("opr",0,0);
	return true;
}
bool Constant_description()
{
	bool ans = true;
	if(now() != "const")
		return true;//未定义常量，常量说明部分直接结束 
	nxt();
	ans = Constant_define();
	if(!ans)
	{
		Bug("Constant_define error");
		return false;
	}
	while(now() == ",")
	{
		nxt();
		ans = Constant_define();
		if(!ans)
		{
			Bug("Constant_define error");
			return false;
		}
	}
	if(now() != ";")
	{
		Bug("Missing symbol ;");
		return false;
	}
	nxt();
	return true;
}
bool Constant_define()//常量定义 
{
	bool ans = true;
	string nowname = sym[pos].name;
	ans = Identifier();
	if(!ans)
	{
		Bug("Identifier");
		return false;
	}
	
	if(now() != "=")
	{
		Bug("Missing symbol =");
		return false;
	}
	nxt();
	
	ans = Unsignedint();
	if(!ans)
	{
		Bug("Unsignedint error");
		return false;
	}
	string nownum = sym[pos-1].name;
	constnum[nowname] = atoi(nownum.c_str());
	return true;
}
bool Identifier()//标识符
{
	if(now() != "identifier")//如果词法分析中这里不是一个用户定义的标识符 
	{
	//	cout << "error" << now() << endl;
		Bug("Missing symbol identifier");
		return false;
	}
	string temp = sym[pos].name;
	int len = temp.length();
	if(!isletter(temp[0]))
	{
		Bug("This should be a letter");
		return false;
	}
	for(int i = 1 ; i < len ; i ++)
	{
		if(!isletter(temp[i]) && !isnum(temp[i]))
		{
			Bug("This should be a letter or number");
			return false;
		}
	}
	nxt();
	return true;
}
bool Unsignedint()
{
	string temp = sym[pos].name;
	int len = temp.length();
	for(int i = 0 ; i < len ; i ++)
	{
		if(!isnum(temp[i]))
		{
			Bug("This should be a number");
			return false;
		}
	}
	nxt();
	return true;
}
bool Variable_description()
{
	bool ans = true;
	if(now() != "var")
		return true;
	nxt();
	string varname = sym[pos].name;
	ans = Identifier();
	idtnum[varname] = var_num++;
	if(!ans)
	{
		Bug("Identifier error");
		return false;
	}
	while(now() == ",")
	{
		nxt();
		varname = sym[pos].name;
		ans = Identifier();
		idtnum[varname] = var_num++;
		if(!ans)
		{
			Bug("Identifier error");
			return false;
		}
	}
	if(now() != ";")
	{
		Bug("Missing symbol ;");
		return false;
	}
	nxt();
	return true;
}
bool Process_description()//过程说明
{
	if(now() != "procedure")
		return true;//无过程说明部分 
	bool ans = true;
	ans = Process_head();
	if(!ans)
	{
		Bug("Process_head");
		return false;
	}
	ans = Subprogram(); 
	if(!ans)
	{
		Bug("Subprogram error");
		return false;
	}
	
	if(now() != ";")
	{
		Bug("Missing symbol ;");
		return false;
	}
	nxt();
	while(now() == "procedure")
	{
		ans = Process_description();
		if(!ans)
		{
			Bug("Process_description");
			return false;
		}
	}
	return true;
}
bool Process_head()
{
	if(now() != "procedure")
	{
		Bug("Missing symbol procedure");
		return false;
	}
	nxt();
	string varname = sym[pos].name;
	bool ans = Identifier();
	if(!ans)
	{
		Bug("Identifier error");
		return false;
	}
	var_num ++;
	idtnum[varname] = var_num;
	
	if(now() != ";")
	{
		Bug("Missing symbol ;");
		return false;
	}
	nxt();
	return true; 
}
bool Compound_statement()
{
	bool ans = true;
	if(now() != "begin")
	{
		Bug("Missing symbol begin");
		return false;
	}
	nxt();
	ans = Statement();
	if(!ans)
	{
		Bug("Statement error");
		return false;
	}
	
	while(now() == ";")
	{
		nxt();
		ans = Statement();
		if(!ans)
		{
			Bug("Statement error");
			return false;
		}
	}
	if(now() != "end")
	{
		Bug("Missing symbol end");
		return false;
	}
	nxt();
	return true;
}
bool Statement()
{
	bool ans = true;
	if(now() == "identifier")//赋值语句 
	{
		ans = Assignment_statement();
		if(!ans)
		{
			Bug("Assignment_statement error");
			return false;
		}
		return true;
	}
	if(now() == "if")//条件语句 
	{
		ans = If_statement();
		if(!ans)
		{
			Bug("If_statement error");
			return false;
		}
		return true;
	}
	if(now() == "while")//当型循环语句 
	{
		ans = While_statement();
		if(!ans)
		{
			Bug("While_statement error");
			return false;
		}
		return true;
	}
	if(now() == "call")//过程调用语句 
	{
		ans = Procedure_call();
		if(!ans)
		{
			Bug("Procedure_call error");
			return false;
		}
		return true;
	}
	if(now() == "read")//读语句 
	{
		ans = Read_statement();
		if(!ans)
		{
			Bug("Read_statement error");
			return false;
		}
		return true;
	}
	if(now() == "write")//写语句 
	{
		ans = Write_statement();
		if(!ans)
		{
			Bug("Write_statement error");
			return false;
		}
		return true;
	}
	if(now() == "begin")
	{
		ans = Compound_statement();
		if(!ans)
		{
			Bug("Compound_statement error");
			return false;
		}
		return true;
	}
	return true;
}
bool If_statement()
{
	bool ans = true;
	if(now() != "if")
	{
		Bug("Missing symbol if");
		return false;
	}
	nxt();
	ans = If();
	if(!ans)
	{
		Bug("If error");
		return false;
	}
	if(now() != "then")
	{
		Bug("Missing symbol then");
		return false;
	}
	nxt();
	int cd = gen("jpc",0,-1);
	ans = Statement();
	code[cd].a = codeadd;
	if(!ans)
	{
		Bug("Statement error");
		return false;
	}
	return true;
}
bool If()
{
	bool ans = true;
	if(now() == "odd")
	{
		nxt();
		ans = Expression();
		if(!ans)
		{
			Bug("Expression error");
			return false;
		}
		gen("odd",0,6);
		return true;
	}
	ans = Expression();
	if(!ans)
	{
		Bug("Expression error");
		return false;
	}
	int opr = Relational_operators();
	if(!opr)
	{
		Bug("Relational_operators error");
		return false;
	}
	ans = Expression();
	if(!ans)//此处可添加odd 
	{
		Bug("Expression error");
		return false;
	}
	gen("opr",0,opr+7);
	return true;
}
bool Expression()
{
	bool ans = true;
	string sign;
	if(now() == "+" || now() == "-")
	{
		sign = now();
		nxt();
	}
	if(sign == "-")
		gen("opr",0,1);
	ans = Term();
	if(!ans)
	{
		Bug("Term error");
		return false;
	}
	string op;
	while(now() == "+" || now() == "-")
	{
		op = now();
		nxt();
		ans = Term();
		if(!ans)
		{
			Bug("Term error");
			return false;
		}
		if(op == "+")
			gen("opr",0,2);
		else
			gen("opr",0,3);
	}
	return true;
}
bool Term()
{
	bool ans = true;
	ans = Factor();
	if(!ans)
	{
		Bug("Factor error");
		return false;
	}
	string op;
	while(now() == "*" || now() == "/")
	{
		op = now();
		nxt();
		ans = Factor();
		if(!ans)
		{
			Bug("Factor error");
			return false;
		}
		if(op == "*")
			gen("opr",0,4);
		else
			gen("opr",0,5);
	}
	return true;
}
bool Factor()
{
	bool ans = true;
	if(now() == "identifier")
	{
		string varname = sym[pos].name; 
		int levtmp = nowlev;
		ans = Identifier();
		
		if(!ans)
		{
			Bug("Identifier error");
			return false;
		}
		if(constnum[varname])
		{
			gen("lit",0,constnum[varname]);
			
		}
		else
		{
			gen("lod",levtmp-sym[pos-1].level,idtnum[varname]);
			
		}
		return true;
	}
	if(now() == "(")
	{
		nxt();
		ans = Expression();
		if(!ans)
		{
			Bug("Expression error");
			return false;
		}
		if(now() != ")")
		{
			Bug("Missing symbol )");
			return false;
		}
		nxt();
		return true;
	}
	int num = atoi(sym[pos].name.c_str());
	ans = Unsignedint();
	if(!ans)
	{
		Bug("Unsigned error");
		return false;
	}
	if(num > 2047)
	{
		Bug("number too lagre");
		return false;
	}
	gen("lit",0,num);
	return true;
}
string relaop[8] = {"","=","#","<",">=",">","<=","<>"};
int Relational_operators()
{
	for(int i = 1 ; i <= 7 ; i ++)
		if(now() == relaop[i])
		{
			nxt();
			if(i == 7)
				return 1;
			return i;
		}
	Bug("Missing relational_operators");
	return 0;
}
bool Assignment_statement()
{
	bool ans = true;
	string varname = sym[pos].name;
	int postmp = pos;
	int levtmp = nowlev;
	ans = Identifier();
	if(!ans)
	{
		Bug("Identifier error");
		return false;
	}
	if(now() != ":=")
	{
		Bug("Missing symbol :=");
		return false;
	}
	nxt();
	ans = Expression();
	if(!ans)
	{
		Bug("Expression error");
		return false;
	}
	gen("sto",levtmp-sym[postmp].level,idtnum[varname]);
	return true;
}
bool While_statement()
{
	bool ans = true;
	if(now() != "while")
	{
		Bug("Missing symbool while");
		return false;
	}
	int cd_ = codeadd;
	nxt();
	ans = If();
	if(!ans)
	{
		Bug("If error");
		return false;
	}
	int cd = gen("jpc",0,0);
	if(now() != "do")
	{
		Bug("Missing symbol do");
		return false;
	}
	nxt();
	ans = Statement();
	gen("jmp",0,cd_);
	code[cd].a = codeadd;
	if(!ans)
	{
		Bug("Statement error");
		return false;
	}
	return true;
}
bool Procedure_call()
{
	bool ans = true;
	if(now() != "call")
	{
		Bug("Missing symbol call");
		return false;
	}
	nxt();
	ans = Identifier();
	gen("cal",nowlev-sym[pos-1].level,idtnum[sym[pos-1].name]);
	if(!ans)
	{
		Bug("Identifier error");
		return false;
	}
	return true;
}
bool Read_statement()
{
	bool ans = true;
	if(now() != "read")
	{
		Bug("Missing symbool read");
		return false;
	}
	nxt();
	
	if(now() != "(")
	{
		Bug("Missing symbool (");
		return false;
	}
	nxt();
	string varname = sym[pos].name;
	ans = Identifier();
	if(!ans)
	{
		Bug("Identifier error");
		return false;
	}
	gen("opr",0,16);
	gen("sto",nowlev-sym[pos-1].level,idtnum[varname]);
	while(now() == ",")
	{
		nxt();
		string varname = sym[pos].name;
		ans = Identifier();
		if(!ans)
		{
			Bug("Identifier error");
			return false;
		}
		gen("opr",0,16);
		gen("sto",nowlev-sym[pos-1].level,idtnum[varname]);
	}
	if(now() != ")")
	{
		Bug("Missing symbool )");
		return false;
	}
	nxt();
	return true;
}
bool Write_statement()
{
	bool ans = true;
	if(now() != "write")
	{
		Bug("Missing symbool write");
		return false;
	}
	nxt();
	
	if(now() != "(")
	{
		Bug("Missing symbool (");
		return false;
	}
	nxt();
	
	ans = Expression();
	if(!ans)
	{
		Bug("Expression error");
		return false;
	}
	gen("opr",0,14);
	while(now() == ",")
	{
		nxt();
		ans = Expression();
		if(!ans)
		{
			Bug("Exoression error");
			return false;
		}
		gen("opr",0,14);
	}
	if(now() != ")")
	{
		Bug("Missing symbool )");
		return false;
	}
	nxt();
	gen("opr",0,15);
	return true;
}
void print_code()
{
	for(int i = 1 ; i <= codeadd ; i ++)
		code[i].print();
}
int main()
{
	freopen("Lexical_analysis.out","r",stdin);
	freopen("Grammatical_analysis.out","w",stdout); 
	init();
	bool ans = true;
	ans = Program();//程序
/*	if(!ans)
		printf("Compile Error!\n");
	else
		printf("Compilation successful\n");*/
	print_code();
	fclose(stdin);
	fclose(stdout);
	return 0;
}
 
