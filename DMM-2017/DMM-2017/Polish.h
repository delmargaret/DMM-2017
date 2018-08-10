#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include "AST.h"

namespace Polish
{
	const int STACK_MAXSIZE = 10000;

	struct Lex_Range // ������ � ��������� ������� ��� ������ � ������� �������
	{
		int start_lex;
		int end_lex;
	};

	struct Pol_Stack // ���� ����������� ������ � �������� �������
	{
		int stack[STACK_MAXSIZE]; // ������� ������
		int top;
	};

	const char symbols[] = { '(', ')', ',', '*', '/', '+', '-'}; // ������� ������ � �������
	const int priority[] = { 0, 0, 1, 2, 2, 3, 3 }; // ����������
	const int P_LEN = 7; // ����� �������� symbols � priority
	const int P_ID = 100; // �������� �� ��������� ��� get_priority()

	bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
	bool PolishNotationAst(AST::ASTNode* node, LT::LexTable& lextable, IT::IdTable& idtable);
	bool PolishNotationRange(Lex_Range range, LT::LexTable& lextable, IT::IdTable& idtable);
	int get_priority(char c); // �������� �������� ���������� �� ������� �������
	Lex_Range find_lex_range(int lextable_pos, LT::LexTable& lextable); // ���� ������� ������ � ��������� ������� � ������� ������ ��� ������� � �������� �������
	bool is_lex_allowed(char l); // ��������, ��������� �� ������ ������� � ������� �������� �������
}