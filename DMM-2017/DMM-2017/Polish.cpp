#include "stdafx.h"
#include "Polish.h"
#include "AST.h"

namespace Polish
{

	bool is_lex_allowed(char l)
	{
		switch (l)
		{
		case LEX_INTEGER:
		case LEX_EQUALITY:
		case LEX_FUNCTION:
		case LEX_DECLARE:
		case LEX_PRINT:
		case LEX_MAIN:
		case LEX_SEMICOLON:
		case LEX_LEFTBRACE:
		case LEX_BRACELET:
			return false;
			break;
		}
		return true;
	}

	Lex_Range find_lex_range(int lextable_pos, LT::LexTable& lextable)
	{
		Lex_Range result;
		int cur = lextable_pos;
		while (cur < lextable.size && lextable.table[cur].lexema[0] != ';') // ���� �� ��������� ����� ������ ������ ��� �� ��������� ������� ';'
			cur++;
		result.end_lex = cur;
		if (cur < lextable.size) {
			cur--;
			result.end_lex = cur;
		}
		while (cur > 0 && is_lex_allowed(lextable.table[cur].lexema[0])) // ���� �� ��������� ������ ������ ������ ��� �� ��������� ����������� �������
			cur--;
		if (is_lex_allowed(lextable.table[cur].lexema[0])) {
			result.start_lex = cur;
		}
		else {
			result.start_lex = cur + 1;
		}
		return result;
	}

	int get_priority(char c)
	{
		for (int i = 0; i < P_LEN; i++)
			if (symbols[i] == c) {
				return priority[i];
			}
		return P_ID;
	}

	void Polish_add(LT::Entry* polish_notation, int& pol_length, int& comma_count, LT::LexTable& lextable, IT::IdTable& idtable, LT::Entry& lex)
	{
		if (lex.lexema[0] == 'i' && idtable.table[lex.idxTI].idtype == IT::IDTYPE::F) {
			polish_notation[pol_length++] = LT::Entry('@', lex.sn, lex.idxTI);
			idtable.table[lex.idxTI].func_param_count = comma_count + 1;
			comma_count = 0;
		}
		else {
			polish_notation[pol_length++] = lex;
		}
	}

	AST::ASTNode* getRightRange(AST::ASTNode* node) {
		if (node->child_count > 0) {
			return getRightRange(node->child[node->child_count - 1]);
		} else {
			return node;
		}
	}

	bool PolishNotationAst(AST::ASTNode* node, LT::LexTable& lextable, IT::IdTable& idtable) {
		if (node->symb == GRB::NS('E')) {
			Lex_Range range;
			range.start_lex = node->child[0]->lexem->lexemId;
			range.end_lex = getRightRange(node)->lexem->lexemId; // node->child[node->child_count - 1]->lexem->lexemId;
			PolishNotationRange(range, lextable, idtable);
		} else {
			for (int i = 0; i < node->child_count; i++)
				PolishNotationAst(node->child[i], lextable, idtable);
		}
		return true;
	}

	bool PolishNotationRange(Lex_Range range, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		if (range.end_lex - range.start_lex <= 1) {
			return false; // ���� � ��������� ������ ���� ��� ��� �������, �� ����� ��������� ������ ������������� � �������� ������
		}

		LT::Entry* polish_notation = new LT::Entry[lextable.size]; // ���� ��������� �������������� �������� �������
		int pol_length = 0;

		Pol_Stack st; // ���� ��� ��������� �������� �������
		st.top = 0; // st.top - ��������� �� ������� ��������� ������

		int comma_count = 0;

		for (int i = range.start_lex - 1; i < range.start_lex; i++)
			polish_notation[pol_length++] = lextable.table[i]; // ���������� ������� �� lextable_pos �� range.start_lex

		for (int i = range.start_lex; i <= range.end_lex; i++)
		{
			int lex_priority = get_priority(lextable.table[i].lexema[0]);
			if (lex_priority == P_ID || lextable.table[i].lexema[0] == LEX_LEFTTHESIS) {
				st.stack[st.top++] = i; // �������������, �������, ��� ����� ������ - ������ ������� � ����
				if (i + 1 < range.end_lex && lextable.table[i + 1].lexema[0] == '[') {
					i = i + 3;
				}
			}
			else if (lextable.table[i].lexema[0] == LEX_RIGHTHESIS) {
				while (st.top > 0 && lextable.table[st.stack[st.top - 1]].lexema[0] != LEX_LEFTTHESIS) { // ���� ���� �� ���� � �� ��������� ����������� ������
																										 //polish_notation[pol_length++] = lextable.table[st.stack[st.top - 1]]; // ��������� ������� �������� �����
					Polish_add(polish_notation, pol_length, comma_count, lextable, idtable, lextable.table[st.stack[st.top - 1]]);
					st.top--; // ��������� ������� �����
				}
				if (st.top == 0) {
					// �� ����� ����������� ������ (������ ����������� ������)
					delete[] polish_notation;
					return false;
				}
				st.top--; // ���������� ����������� ������
			}
			else {
				while (st.top > 0 && get_priority(lextable.table[st.stack[st.top - 1]].lexema[0]) >= lex_priority) { // ���� ���� �� ���� � ��������� ������� ������� ������ ���� ����� ���������� ������� ������� �����
																													 //polish_notation[pol_length++] = lextable.table[st.stack[st.top - 1]];
					Polish_add(polish_notation, pol_length, comma_count, lextable, idtable, lextable.table[st.stack[st.top - 1]]);
					st.top--;
				}
				if (lextable.table[i].lexema[0] != LEX_COMMA) {
					st.stack[st.top++] = i; // ���� �� �������, �� ���������� ������� � ����
				}
				else {
					comma_count++;
				}
			}
		}

		// ���� ���� �� ����, �� ���������� ��� �������� �� ����
		while (st.top > 0) {
			if (lextable.table[st.stack[st.top - 1]].lexema[0] == '(') {
				// ������ ����������� ������
				delete[] polish_notation;
				return false;
			}
			//polish_notation[pol_length++] = lextable.table[st.stack[st.top - 1]];
			Polish_add(polish_notation, pol_length, comma_count, lextable, idtable, lextable.table[st.stack[st.top - 1]]);
			st.top--;
		}

		if (lextable.size > range.end_lex) { // ���� lextable.size == range.end_lex, �� �� �� ����� ';', � ������� ��������� � ����� �� �� �����
			polish_notation[pol_length++] = lextable.table[range.end_lex + 1]; // ����� ';' � ����� polish_notation
		}

		// ����������� ����������� ������ ��������� �������� ������� ������ ������������ ������� ������
		for (int i = 0, j = range.start_lex - 1; i < pol_length; i++, j++)
			lextable.table[j] = polish_notation[i];
		// �������� ������ �������
		for (int i = range.start_lex - 1 + pol_length; i <= range.end_lex + 1; i++)
			lextable.table[i].lexema[0] = LEX_NULL;

		delete[] polish_notation;
		return true;
	}

	bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		Lex_Range range = find_lex_range(lextable_pos, lextable); // ���� ������� ��� �������������� � �������� ������

		if (range.end_lex - range.start_lex <= 1) {
			return false; // ���� � ��������� ������ ���� ��� ��� �������, �� ����� ��������� ������ ������������� � �������� ������
		}

		LT::Entry* polish_notation = new LT::Entry[lextable.size]; // ���� ��������� �������������� �������� �������
		int pol_length = 0;

		Pol_Stack st; // ���� ��� ��������� �������� �������
		st.top = 0; // st.top - ��������� �� ������� ��������� ������

		int comma_count = 0;

		for (int i = lextable_pos; i < range.start_lex; i++)
			polish_notation[pol_length++] = lextable.table[i]; // ���������� ������� �� lextable_pos �� range.start_lex

		for (int i = range.start_lex; i <= range.end_lex; i++)
		{
			int lex_priority = get_priority(lextable.table[i].lexema[0]);
			if (lex_priority == P_ID || lextable.table[i].lexema[0] == LEX_LEFTTHESIS) {
				st.stack[st.top++] = i; // �������������, �������, ��� ����� ������ - ������ ������� � ����
			}
			else if (lextable.table[i].lexema[0] == LEX_RIGHTHESIS) {
				while (st.top > 0 && lextable.table[st.stack[st.top - 1]].lexema[0] != LEX_LEFTTHESIS) { // ���� ���� �� ���� � �� ��������� ����������� ������
																										 //polish_notation[pol_length++] = lextable.table[st.stack[st.top - 1]]; // ��������� ������� �������� �����
					Polish_add(polish_notation, pol_length, comma_count, lextable, idtable, lextable.table[st.stack[st.top - 1]]);
					st.top--; // ��������� ������� �����
				}
				if (st.top == 0) {
					// �� ����� ����������� ������ (������ ����������� ������)
					delete[] polish_notation;
					return false;
				}
				st.top--; // ���������� ����������� ������
			}
			else {
				while (st.top > 0 && get_priority(lextable.table[st.stack[st.top - 1]].lexema[0]) >= lex_priority) { // ���� ���� �� ���� � ��������� ������� ������� ������ ���� ����� ���������� ������� ������� �����
																													 //polish_notation[pol_length++] = lextable.table[st.stack[st.top - 1]];
					Polish_add(polish_notation, pol_length, comma_count, lextable, idtable, lextable.table[st.stack[st.top - 1]]);
					st.top--;
				}
				if (lextable.table[i].lexema[0] != LEX_COMMA) {
					st.stack[st.top++] = i; // ���� �� �������, �� ���������� ������� � ����
				}
				else {
					comma_count++;
				}
			}
		}

		// ���� ���� �� ����, �� ���������� ��� �������� �� ����
		while (st.top > 0) {
			if (lextable.table[st.stack[st.top - 1]].lexema[0] == '(') {
				// ������ ����������� ������
				delete[] polish_notation;
				return false;
			}
			//polish_notation[pol_length++] = lextable.table[st.stack[st.top - 1]];
			Polish_add(polish_notation, pol_length, comma_count, lextable, idtable, lextable.table[st.stack[st.top - 1]]);
			st.top--;
		}

		if (lextable.size > range.end_lex) { // ���� lextable.size == range.end_lex, �� �� �� ����� ';', � ������� ��������� � ����� �� �� �����
			polish_notation[pol_length++] = lextable.table[range.end_lex + 1]; // ����� ';' � ����� polish_notation
		}

		// ����������� ����������� ������ ��������� �������� ������� ������ ������������ ������� ������
		for (int i = 0, j = lextable_pos; i < pol_length; i++, j++)
			lextable.table[j] = polish_notation[i];
		// �������� ������ �������
		for (int i = lextable_pos + pol_length; i <= range.end_lex + 1; i++)
			lextable.table[i].lexema[0] = LEX_NULL;

		delete[] polish_notation;
		return true;
	}
}