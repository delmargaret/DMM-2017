#pragma once

#include "LT.h"
#include "GRB.h"
#include "LexAnalis.h"
#include <fstream>
#include <string>

using namespace std;

namespace AST
{
	struct ASTNode {
		short symb; // ������ ���������� � ������� ������� ������ - �������� ��� ����������
		LT::Entry* lexem;
		ASTNode* parent; // ��������� �� ������������ �������
		int child_count; // ���������� �������� ������
		ASTNode** child; // ��������� �� �������� �������
		int chain_id; // ��� ��������� ����� �������������� �������
		bool isInteger; // isInteger ==  true, ������ ��� ������� symb == 'E' ������� ��������� �������������, ����� ���������
	};

	struct Ast {
		ASTNode* head;
		GRB::Rule* rules;
		short* nrulechains;
		int rules_size;

		void createHead(GRBALPHABET startSymb);
		void buildAST(int* rule_id, int* lex_id, ASTNode* node, GRB::Rule* rules, short* nrulechains, Lexer::LEX* lex);
		void printAST(ASTNode* node, ofstream& AstOutput);
		static char* getFunctionName(ASTNode* node, Lexer::LEX* lex);
		void checkId(ASTNode* node, Lexer::LEX* lex);
		void checkLiteral(ASTNode* node, Lexer::LEX* lex);
		void declExpressionType(ASTNode* node, Lexer::LEX* lex);
		void assignCheck(ASTNode* node, Lexer::LEX* lex);
		void setFuncParams(ASTNode* node, Lexer::LEX* lex);
		void checkFuncParams(ASTNode* node, Lexer::LEX* lex);
		int getParamIdx(ASTNode* node, Lexer::LEX* lex);
		char* getFunctionCallName(ASTNode* node, Lexer::LEX* lex);
		char* setVariableITindex(ASTNode* node, Lexer::LEX* lex);
	};
}