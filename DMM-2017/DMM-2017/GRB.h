#pragma once

#include "Error.h"

typedef short GRBALPHABET;

namespace GRB
{
	#define NS(n) Rule::Chain::N(n)
	#define TS(n) Rule::Chain::T(n)

	struct Rule
	{
		GRBALPHABET nn; //����������
		int iderror; //������������� ���������������� ���������
		short size; //���������� �������
		struct Chain
		{
			int chain_id;
			short size; //����� �������
			GRBALPHABET* nt; //������� ���������� � ������������ 
			Chain() {size = 0; nt = 0;}
			Chain(int chain_id, short psize, GRBALPHABET s, ...); //���������� �������� � �������
			char* getCChain(char* b); //�������� ������ ������� �������
			static GRBALPHABET T(char t) {return GRBALPHABET(t);} //��������
			static GRBALPHABET N(char n) {return -GRBALPHABET(n);} //����������
			static bool isT(GRBALPHABET s) {return s > 0;}
			static bool isN(GRBALPHABET s) {return !isT(s);}
			static char alphabet_to_char(GRBALPHABET s) {return isT(s)?char(s):char(-s);}
		}* chains; //������ �������
		Rule() {nn = 0x00; size = 0;}
		Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...);
		char* getCRule(char* b, short nchain); //�������� ������� ��� ����������
		int getChainById(int id);
		short getNextChain(GRBALPHABET t, Chain& pchain, short j); //�������� ��������� �������
	};

	struct Greibach
	{
		short size; //���������� ������
		GRBALPHABET startN; //��������� ������
		GRBALPHABET stbottomT; //��� �����
		Rule* rules; //��������� ������
		Greibach() {size = 0; startN = 0; stbottomT = 0; rules = 0;};
		Greibach(GRBALPHABET pstartN, GRBALPHABET psbottomT, short psize, Rule r, ...);
		short getRule(GRBALPHABET pnn, Rule& prule); //�������� �������
		Rule getRule(short n); //������� �� ������
		static Greibach getGreibach(); //�������� ����������
	};

}