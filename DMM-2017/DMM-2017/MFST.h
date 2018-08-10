#pragma once
#include <stack>
#include "GRB.h"
#include "LexAnalis.h"
#include "AST.h"
#define MFST_DIAGN_NUMBER 3
typedef std::stack<short > MFSTSTSTACK;

namespace MFST
{
	struct MfstState // ��������� ��� ���������� ��������� �������� Mfst ��� ������� �����������
	{
		short lenta_position; // ������� ��������������� ������� �� ����� (��� ������ ���� ������� �����������)
		short nrulechain; // ����� �������, � ������� �� ���������� ������� ���������� �� ����� ����� �����������
		MFSTSTSTACK st; // ������ ���������� ����� �������� Mfst
		int nrule;
		MfstState() {};
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain, int nrule);
	};

	struct Mfst
	{
		enum RC_STEP
		{
			NS_OK,
			NS_NORULE,
			NS_NORULECHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOK,
			LENTA_END,
			SURPRISE
		} cur_status;

		struct MfstDiagnosis // ��������� ��� ���������� ���������������� ���������
		{
			short lenta_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;
			MfstDiagnosis() {};
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* lenta;
		short lenta_position; // �������, ��������������� ���������
		short nrule; // ��������� ������������ ������� ��� ������������ �����������
		short nrulechain; // ��������� ������� �������, ������� ���� ������������
		short lenta_size;
		short fail_position; // ����� ������� �������, ��� ������ ���������� ��������
		GRBALPHABET fail_symbol; // ������ �� �����, ������� �� ��� ��������
		GRBALPHABET expected_symbol; // ��������� ����� ��������������� �������, expected_symbol != fail_symbol
		short out_of_rules_position;
		GRBALPHABET out_of_rules_fail_N; // ����� �� ������� �� ���� �������, �� out_of_rules_fail_N �������� �������������� ������ �� �������� �����, ��� �������� �� �����
		GRBALPHABET out_of_rules_fail_T; // ������ �� ����� (out_of_rules_fail_T), �� ��������� ��� out_of_rules_fail_N
		GRB::Greibach greibach;
		Lexer::LEX lex; // ������� ������, ������� �������������� lenta
		MFSTSTSTACK st; // ������� ���� ��������, � ������� �������� ��������� �� ����� ���������� Greibach
		std::stack<MfstState> storestate; // ���� ��� ���������� ��������� ��������
		Mfst() {};
		Mfst(Lexer::LEX plex, GRB::Greibach pgreibach);
		char* getCSt(char* buf); //���������� ����� ��� ������
		char* getCLenta(char* buf, short pos, short n = 25);
		char* getDiagnosis(short n, char* buf);
		bool savestate();
		bool reststate();
		bool push_chain(GRB::Rule::Chain chain);
		RC_STEP step();
		bool start();
		bool savediagnosis(RC_STEP pprc_step);
		char* Mfst::getLexemDescription(LT::Entry lexem, bool isExpected);
		void printDiagnosis();
		void printError();
		void printRules();
		void prepareAstLexems(AST::Ast* ast);
	};
}