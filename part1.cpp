#include<bits/stdc++.h>
using namespace std;
const int sz = 20010;
const int snum1 = 13;
const int snum2 = 17;
string symbol1[66] = {"const","var","procedure","begin","end","if","then","call","while","do","read","write","odd"};//�����ֱ� 
string symbol2[66] = {"+","-","*","/",",",";","<=",">=","<",">","<>",":=","=","(",")",".","#"};//�����
map <string,bool> reserved,operator_,ident;//��Ӧ�ؼ��֡���������û��Լ������ı����� 
char buf[sz];
int now_level,symnum;
class table //SYM �� class 
{
	public:
		table()
		{
			name.clear();
			kind.clear();
			level = 0;
			addr = 0;
		}
		bool set(string nm,int pos)//�жϴ���ķ��Ų�����sym 
		{
			name = nm;
		//	printf("now_level %d\n",now_level);
			if(reserved[nm])
			{
				kind = nm;
				addr = symnum;
				level = now_level;
				if(nm == "begin")
					now_level ++;
				else if(nm == "end")
					now_level --;
			}
			else if(operator_[nm])
			{
				kind = nm;
				addr = symnum;
				level = now_level;
			}
			else if(nm[0] >= '0' && nm[0] <= '9')
			{
				kind = "number";
				addr = symnum;
				level = now_level;
			} 
			else
			{
				ident[nm] = true;
				kind = "identifier";
				level = now_level;
				addr = symnum;
			}
		}
		void print()
		{
			cout << name << " " << kind
				 << " " << level << " " << addr << endl;
		}
	private:
		string name,kind;
		int level,addr;
};
class NUM_
{
	public:
		NUM_()
		{
			val = 0;
		}
		void print()
		{
			cout << "val:" << val << endl;
		}
	int val;
};
NUM_ num[sz];
table sym[sz];
int numcnt,pos;
void init()
{
	reserved.clear();
	operator_.clear();
	ident.clear();
	for(int i = 0 ; i < snum1 ; i ++)
		reserved[symbol1[i]] = true;//�ж��Ƿ�Ϊ�ؼ���
	for(int i = 0 ; i < snum2 ; i ++)
		operator_[symbol2[i]] = true;//�ж������ 
		
}
void read()
{
	char ins;
	pos = 0;
	while(scanf("%c",&ins) != EOF)
		buf[++pos] = ins;//�������buf�д��� 
	return ;
}

int split()
{
	string now;
	now.clear();
	for(int i = 1 ; i <= pos ; i ++)
	{
		//���ַ������������ 
		if(buf[i] == ':')
		{
			if(now.length() >= 1)
			{
				symnum ++;
				sym[symnum].set(now,i-1);
				now.clear();
			}
			if(i == pos || buf[i+1] != '=')
				return -1;
			i ++;
			symnum ++;
			sym[symnum].set(":=",i);
			continue;
		}
		if(buf[i] == '<')
		{
			if(now.length() >= 1)
			{
				symnum ++;
				sym[symnum].set(now,i-1);
				now.clear();
			}
			symnum ++;
			if(buf[i+1] == '=')
			{
				sym[symnum].set("<=",i);
				i ++;
			}
			else if(buf[i+1] == '>')
			{
				sym[symnum].set("<>",i);
				i ++;
			}
			else
				sym[symnum].set("<",i);
			continue;
		}
		if(buf[i] == '>')
		{
			if(now.length() >= 1)
			{
				symnum ++;
				sym[symnum].set(now,i-1);
				now.clear();
			}
			symnum ++;
			if(buf[i+1] == '=')
			{
				sym[symnum].set(">=",i);
				i ++;
			}
			else
				sym[symnum].set(">",i);
			continue;
		}
		//--------
		if(buf[i] == ',' || buf[i] == ';' || buf[i] == '\n' 
		|| buf[i] == ' ' || buf[i] == '+' || buf[i] == '-' 
		|| buf[i] == '/' || buf[i] == '*' || buf[i] == '('
		|| buf[i] == ')' || buf[i] == '=' || buf[i] == '.'
		|| buf[i] == '#')//���������ַ�������� 
		{
			if(now.length() >= 1)
			{
				if(now[0] >= '0' && now[0] <= '9')
				{
					numcnt ++;
					num[numcnt].val = atoi(now.c_str());
				}
				symnum ++;
			/*	if(stat == 1 && reserved[now])
				{
					cout << "#" << stat << endl;
					printf("reserved word error:\n");
				//	cout << now << endl;
					return 0;
				}
				cout << "stat:" << stat << endl;
				if(stat != 1 && !reserved[now] && !operator_[now] && !ident[now])
				{
					printf("keyword doesn't exist:\n");
				//	cout << now << endl;
					return 0;
				}*/
				sym[symnum].set(now,i);
				now.clear();
			}
		}
		if(buf[i] == ',' || buf[i] == '=' || buf[i] == '+'
		|| buf[i] == '-' || buf[i] == '/' || buf[i] == '*'
		|| buf[i] == '(' || buf[i] == ')' || buf[i] == ';'
		|| buf[i] == '.' || buf[i] == '#')//�������ַ�������� 
		{
			symnum ++;
			string temp;temp.clear();
			temp += buf[i];
			sym[symnum].set(temp,i);
		}
		else if(buf[i] != '\n' && buf[i] != ' ')
			now += buf[i];
	}
}
/*
const a=10;
var b,c;
procedure p;
begin
  b:=10;
  c:=b+a
end;

*/

int main()
{
	freopen("test1.txt","r",stdin);
	freopen("Lexical_analysis.out","w",stdout);
	init();
	read();
	split();
	printf("%d\n",symnum);
	for(int i = 1 ; i <= symnum ; i ++)
		sym[i].print();
/*	for(int i = 1 ; i <= numcnt ; i ++)
		num[i].print();*/
	return 0;
}

