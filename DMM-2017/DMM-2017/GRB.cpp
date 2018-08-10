#include "stdafx.h"
#include "GRB.h"
#include <stdio.h>
#define GRB_ERROR_SERIES 300
namespace GRB
{
	Greibach greibach( NS('S'), TS('$'), 7,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, 2, // программные блоки: main{...}; | type function identifier(params list) {...};
			Rule::Chain(100, 14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')), // tfi(F){NrE;};S

			Rule::Chain(101, 8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')) // m{NrE;};
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1, 18, // операторы: define type identifier; | identifier = expression; | ...
			Rule::Chain(100, 7, TS('d'), TS('a'), TS('i'), TS('['), TS('l'), TS(']'), TS(';')), //dai[l]; - chain 0
			Rule::Chain(101, 8, TS('d'), TS('a'), TS('i'), TS('['), TS('l'), TS(']'), TS(';'), NS('N')), //dai[l];N - chain 1

			Rule::Chain(102, 4, TS('d'), TS('t'), TS('i'), TS(';')), //dti; - chain 2
			Rule::Chain(103, 5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')), //dti;N - chain 3

			Rule::Chain(116, 6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')), //dti=E; - chain 16
			Rule::Chain(117, 7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')), //dti=E;N - chain 17

			Rule::Chain(106, 4, TS('i'), TS('='), NS('E'), TS(';')), //i=E; - chain 6
			Rule::Chain(107, 5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')), //i=E;N - chain 7 

			Rule::Chain(108, 7, TS('i'), TS('['), TS('l'), TS(']'), TS('='), NS('E'), TS(';')), //i[l]=E; - chain 8
			Rule::Chain(109, 8, TS('i'), TS('['), TS('l'), TS(']'), TS('='), NS('E'), TS(';'), NS('N')), //i[l]=E;N - chain 9
			Rule::Chain(110, 7, TS('i'), TS('['), TS('i'), TS(']'), TS('='), NS('E'), TS(';')), //i[i]=E; - chain 10
			Rule::Chain(111, 8, TS('i'), TS('['), TS('i'), TS(']'), TS('='), NS('E'), TS(';'), NS('N')), //i[i]=E;;N - chain 11

			//Rule::Chain(112, 8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')), //dtfi(F); - chain 12 - игнорируем
			//Rule::Chain(113, 9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')), //dtfi(F);N - chain 13 - игнорируем

			Rule::Chain(114, 3, TS('p'), TS('i'), TS(';')), //pi; - chain 14
			Rule::Chain(115, 4, TS('p'), TS('i'), TS(';'), NS('N')), //pi;N - chain 15

			Rule::Chain(118, 8, TS('c'), TS('('), TS('l'), TS(')'), TS('{'), NS('C'), TS('}'), TS(';')), //c(l){C}; - chain 18
			Rule::Chain(119, 9, TS('c'), TS('('), TS('l'), TS(')'), TS('{'), NS('C'), TS('}'), TS(';'), NS('N')), //c(l){C}; - chain 19
			Rule::Chain(120, 8, TS('c'), TS('('), TS('i'), TS(')'), TS('{'), NS('C'), TS('}'), TS(';')), //c(i){C}; - chain 20
			Rule::Chain(121, 9, TS('c'), TS('('), TS('i'), TS(')'), TS('{'), NS('C'), TS('}'), TS(';'), NS('N')) //c(i){C}; - chain 21
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2, 14, // выражения: (identifier + literal)*function(params)
			Rule::Chain(100, 2, TS('-'), NS('E')), //-E 0
		    Rule::Chain(101, 2, TS('+'), NS('E')), //+E 1

			Rule::Chain(102, 1, TS('i')), //i 2
			Rule::Chain(103, 2, TS('i'), NS('M')), //iM 3

			Rule::Chain(104, 1, TS('l')), //l 4
			Rule::Chain(105, 2, TS('l'), NS('M')), //lM 5

			Rule::Chain(106, 3, TS('('), NS('E'), TS(')')), //(E) 6
			Rule::Chain(107, 4, TS('('), NS('E'), TS(')'), NS('M')), //(E)M 7

			Rule::Chain(108, 4, TS('i'), TS('('), NS('W'), TS(')')), //i(W) 8
			Rule::Chain(109, 5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')), //i(W)M 9

			Rule::Chain(110, 4, TS('i'), TS('['), TS('l'), TS(']')), //i[l] 10
			Rule::Chain(111, 5, TS('i'), TS('['), TS('l'), TS(']'), NS('M')), //i[l]M 11

			Rule::Chain(112, 4, TS('i'), TS('['), TS('i'), TS(']')), //i[i] 12
			Rule::Chain(113, 5, TS('i'), TS('['), TS('i'), TS(']'), NS('M')) //i[i]M 13
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 3, 2, // список параметров функции в ее определении
			Rule::Chain(100, 2, TS('t'), TS('i')), //ti
			Rule::Chain(101, 4, TS('t'), TS('i'), TS(','), NS('F')) //ti,F
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 4, 4, // список параметров функции при вызове
			Rule::Chain(100, 1, TS('i')), //i 0
			Rule::Chain(101, 3, TS('i'), TS(','), NS('W')), //i,W 1

			Rule::Chain(102, 1, TS('l')), //l 2
			Rule::Chain(103, 3, TS('l'), TS(','), NS('W')) //l,W 3
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 5, 4, // вспомогательный нетерминал для 'E', позволяет рекурсивно определять составные выражения
			Rule::Chain(100, 2, TS('+'), NS('E')), // +E 0
			Rule::Chain(101, 2, TS('-'), NS('E')), // -E 1
			Rule::Chain(102, 2, TS('*'), NS('E')), // *E 2
			Rule::Chain(103, 2, TS('/'), NS('E')) // /E 3
		),
		Rule(NS('C'), GRB_ERROR_SERIES + 6, 8, // операторы внутри цикла
			Rule::Chain(100, 4, TS('i'), TS('='), NS('E'), TS(';')), //i=E; 0
			Rule::Chain(101, 5, TS('i'), TS('='), NS('E'), TS(';'), NS('C')), //i=E;N 1

			Rule::Chain(102, 7, TS('i'), TS('['), TS('l'), TS(']'), TS('='), NS('E'), TS(';')), //i[l]=E; 2
			Rule::Chain(103, 8, TS('i'), TS('['), TS('l'), TS(']'), TS('='), NS('E'), TS(';'), NS('C')), //i[l]=E;N 3
			Rule::Chain(104, 7, TS('i'), TS('['), TS('i'), TS(']'), TS('='), NS('E'), TS(';')), //i[i]=E; 4
			Rule::Chain(105, 8, TS('i'), TS('['), TS('i'), TS(']'), TS('='), NS('E'), TS(';'), NS('C')), //i[i]=E;;N 5

			Rule::Chain(106, 3, TS('p'), TS('i'), TS(';')), //pi; 6
			Rule::Chain(107, 4, TS('p'), TS('i'), TS(';'), NS('C')) //pi;N 7
		)
	);

	// конструктор для Chain
	Rule::Chain::Chain(int chain_id, short psize, GRBALPHABET s, ...)
	{
		this->chain_id = chain_id;
		this->size = psize; // количество терминалов и нетерминалов в правой части правила грамматики
		this->nt = new GRBALPHABET[this->size];
		this->nt[0] = s;
		for (int i = 1; i < this->size; i++)
			this->nt[i] = *(&s + i*sizeof(GRBALPHABET)) ; // каждый следующий параметр функции находится по смещению +2 байти в стеке
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
			b[i] = Rule::Chain::alphabet_to_char(this->nt[i]);
		b[this->size] = 0;
		return b;
	}

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) // компилятор сам разбирается с нужным смещением для следующего параметра
	{
		this->nn = pnn;
		this->iderror = piderror ;
		this->size = psize;
		this->chains = new Rule::Chain[this->size];
		this->chains[0] = c;
		for (int i = 1; i < this->size; i++)
			this->chains[i] = *(&c + i); 
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[256];
		sprintf(b, "%c->%s", Rule::Chain::alphabet_to_char(this->nn), this->chains[nchain].getCChain(buf));
		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) 
	{
		while (j < this->size && this->chains[j].nt[0] != t)
			j++;
		if (j == this->size) {
			return this->size;
		}
		pchain = this->chains[j];
		return j;
	}

	int Rule::getChainById(int id)
	{
		for (int i = 0; i < this->size; i++) {
			if (this->chains[i].chain_id == id) {
				return i;
			}
		}
		return -1;
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET psbottomT, short psize, Rule r, ...)
	{
		this->startN = pstartN;
		this->stbottomT = psbottomT;
		this->size = psize;
		this->rules = new Rule[this->size];
		this->rules[0] = r;
		for (int i = 1; i < this->size; i++)
			this->rules[i] = *(&r + i);
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		for (int i = 0; i < this->size; i++)
			if (this->rules[i].nn == pnn) {
				prule = this->rules[i];
				return i;
			}
		return -1;
	}

	Rule Greibach::getRule(short n)
	{
		return this->rules[n];
	}

	Greibach Greibach::getGreibach()
	{
		return greibach;
	}
}