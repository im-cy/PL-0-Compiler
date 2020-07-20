#include<bits/stdc++.h>
using namespace std;
const int sz = 200100;
struct node
{
	string f;
	int l,a;
}ins[sz];
int cnt = 0;
int sta[sz],pc,bas;
void init()
{
	string str;
	int p1,p2;
	while(cin >> str >> p1 >> p2)
	{
		ins[cnt].f = str;
		ins[cnt].l = p1;
		ins[cnt].a = p2;
		cnt ++;
	}
	return;
}
int prefind(int y)
{
	if(y == 0)
		return bas;
	int k = bas;
	while(y --)
	{
		k = sta[k];
	}
	return k;
}
void work()
{
	int p1,p2;
	sta[0] = 1;
	sta[1] = 1;
	sta[2] = 0;
	bas = 1;
	pc = 0;
	do{
		p1 = ins[pc].l;
		p2 = ins[pc].a;
		if(ins[pc].f == "lit")
		{
			sta[++sta[0]] = p2;
			pc ++;
		}
		else if(ins[pc].f == "lod")
		{
			sta[++sta[0]] = sta[prefind(p1)+p2];
			pc ++;
		}
		else if(ins[pc].f == "sto")
		{
			sta[prefind(p1)+p2] = sta[sta[0]--];
			pc ++;
		}
		else if(ins[pc].f == "cal")
		{
			sta[bas+1] = ++pc;
			bas = sta[0]+1;
			sta[0] += 2;
			pc = p2;
		}
		else if(ins[pc].f == "int")
		{
			for(int i = 1 ; i <= p2 ; i ++)
				sta[sta[0]+i] = 0;
			sta[0] += p2;
			pc ++;
		}
		else if(ins[pc].f == "jmp")
		{
			pc = p2;
		}
		else if(ins[pc].f == "jpc")
		{
			if(sta[sta[0]--] == 0)
				pc = p2;
			else
				pc ++;
		}
		else if(ins[pc].f == "opr")
		{
			if(p2 == 0)
			{
				sta[0] = bas-1;
				bas = sta[bas];
				pc = sta[bas+1];
			}
			else
			{
				int op1 = sta[sta[0]-1];
				int op2 = sta[sta[0]--];
				if(p2 == 1)
				{
					sta[sta[0]] = -p2;
				}
				else if(p2 == 2)
				{
					sta[sta[0]] = op1 + op2;
				}
				else if(p2 == 3)
				{
					sta[sta[0]] = op1 - op2;
				}
				else if(p2 == 4)
				{
					sta[sta[0]] = op1 * op2;
				} 
   				else if(p2 == 5)
				{
					sta[sta[0]] = op1 / op2;;
				}
				
				else if(p2 == 6)
				{
					sta[sta[0]] = op2 % 2;
				}
				else if(p2 == 7)
				{
					sta[sta[0]] = (op1 < op2);
				}
				else if(p2 == 8)
				{
					sta[sta[0]] = (op1 == op2);
				}
				else if(p2 == 9)
				{
					sta[sta[0]] = (op1 != op2);
				}
				else if(p2 == 10)
				{
					sta[sta[0]] = (op1 < op2);
				}
				else if(p2 == 11)
				{
					sta[sta[0]] = (op1 >= op2);
				}
				else if(p2 == 12)
				{
					sta[sta[0]] = (op1 > op2);
				}
				else if(p2 == 13)
				{
					sta[sta[0]] = (op1 <= op2);
				}
				else if(pc == 14)
				{
					printf("%d\n",sta[sta[0]--]);
				}
				else if(pc == 15)
				{
					puts("");
				}
				else if(pc == 16)
				{
					scanf("%d",&sta[sta[0]+1]);
					sta[0]++;
				}
				pc ++;
			}
		}
	}while(pc && pc < cnt);
	return;
}
int main()
{
	freopen("Grammatical_analysis.out","r",stdin);
	init();
	work();
	return 0;
}
