#include "stdafx.h"
#include "LexAnalis.h"
#include "Graphs.h"

#define N_GRAPHS 25

namespace Lexer
{
	Graph graph[N_GRAPHS] =
	{
		{ 'l', Fst::FST(GRAPH_INT_LITERAL) },
		{ 'l', Fst::FST(GRAPH_STRING_LITERAL) },
		{ 'm', Fst::FST(GRAPH_MAIN) },
		{ 'c', Fst::FST(GRAPH_FOR) },
		{ 't', Fst::FST(GRAPH_INTEGER) },
		{ 't', Fst::FST(GRAPH_STRING) },
		{ 'f', Fst::FST(GRAPH_FUNCTION) },
		{ 'r', Fst::FST(GRAPH_RETURN) },
		{ 'd', Fst::FST(GRAPH_DECLARE) },
		{ 'p', Fst::FST(GRAPH_PRINT) },
		{ 'a', Fst::FST(GRAPH_ARRAY) },
		{ 'i', Fst::FST(GRAPH_ID) },
		{ '+', Fst::FST(GRAPH_ARYTHM_PLUS) },
		{ '-', Fst::FST(GRAPH_ARYTHM_MINUS) },
		{ '*', Fst::FST(GRAPH_ARYTHM_MULT) },
		{ '/', Fst::FST(GRAPH_ARYTHM_DIV) },
		{ ';', Fst::FST(GRAPH_SEMICOLON) },
		{ '=', Fst::FST(GRAPH_EQUALS) },
		{ ',', Fst::FST(GRAPH_COMMA) },
		{ '{', Fst::FST(GRAPH_LEFT_BRACE) },
		{ '}', Fst::FST(GRAPH_RIGHT_BRACE) },
		{ '(', Fst::FST(GRAPH_OPEN_PARENTHESIS) },
		{ ')', Fst::FST(GRAPH_CLOSE_PARENTHESIS) },
		{ '[', Fst::FST(GRAPH_OPEN_SQUARE) },
		{ ']', Fst::FST(GRAPH_CLOSE_SQUARE) },
	};

	LEX RunAnalysis(In::IN& in, Log::LOG &log) {
		LEX lex_out;
		lex_out.Lextable = LT::Create(in.tokensSize);
		lex_out.IDtable = IT::Create(in.tokensSize);
		LT::Entry new_lexem;
		IT::Entry new_identifier;
		new_identifier.isLibrary = false;
		new_identifier.paramsCount = 0;
		new_identifier.createdTempP = 0;

		// ��� ��������������� ��� ���������� ������������ �������

		// ������
		IT::Entry lib_func_param;
		strcpy(lib_func_param.id, "param");
		lib_func_param.isLibrary = false;
		lib_func_param.idtype = IT::IDTYPE::P;
		lib_func_param.isArray = true;
		lib_func_param.iddatatype = IT::IDDATATYPE::INT;
		lib_func_param.func_param_count = 0;
		lib_func_param.idxfirstLE = LT_TI_NULLIDX;
		lib_func_param.paramsCount = 0;
		strcpy(lib_func_param.vis, "global");
		IT::Add(lex_out.IDtable, lib_func_param);

		// ����� ��������� �������
		strcpy(lib_func_param.id, "length");
		lib_func_param.isLibrary = false;
		lib_func_param.idtype = IT::IDTYPE::P;
		lib_func_param.isArray = false;
		lib_func_param.iddatatype = IT::IDDATATYPE::INT;
		lib_func_param.func_param_count = 0;
		lib_func_param.idxfirstLE = LT_TI_NULLIDX;
		lib_func_param.paramsCount = 0;
		strcpy(lib_func_param.vis, "global");
		IT::Add(lex_out.IDtable, lib_func_param);

		// ��� ��������������� ��� ������������ �������

		// ������� sum
		IT::Entry lib_func;
		strcpy(lib_func.id, "sum");
		lib_func.isLibrary = true;
		lib_func.idtype = IT::IDTYPE::F;
		lib_func.isArray = false;
		lib_func.iddatatype = IT::IDDATATYPE::INT;
		lib_func.func_param_count = 2;
		lib_func.idxfirstLE = LT_TI_NULLIDX;
		lib_func.paramsCount = 2;
		lib_func.params[0] = &(lex_out.IDtable.table[0]);
		lib_func.params[1] = &(lex_out.IDtable.table[1]);
		strcpy(lib_func.vis, "global");
		IT::Add(lex_out.IDtable, lib_func);
		
		// ������� max
		strcpy(lib_func.id, "max");
		lib_func.isLibrary = true;
		lib_func.idtype = IT::IDTYPE::F;
		lib_func.isArray = false;
		lib_func.iddatatype = IT::IDDATATYPE::INT;
		lib_func.func_param_count = 2;
		lib_func.paramsCount = 2;
		lib_func.params[0] = &(lex_out.IDtable.table[0]);
		lib_func.params[1] = &(lex_out.IDtable.table[1]);
		lib_func.idxfirstLE = LT_TI_NULLIDX;
		strcpy(lib_func.vis, "global");
		IT::Add(lex_out.IDtable, lib_func);

		for (int i = 0; i < in.tokensSize; i++) { // ������������� ��� ������ �� ������� �� ����������
			bool recognized = false;
			new_identifier.isArray = false;
			for (int j = 0; j < N_GRAPHS; j++) {
				Fst::FST fst_test(in.tokens[i].token, graph[j].graph);
				if (Fst::execute(fst_test)) { // ��������� in.tokens[i].token �� �������������� ������� graph[j]
					new_lexem.sn = in.tokens[i].line;
					new_lexem.pos = in.tokens[i].pos;
					recognized = true;
					if (graph[j].lexema == 'l') { // ��������� ������� �������
						new_lexem.lexema[0] = LEX_LITERAL;
						new_identifier.idtype = IT::IDTYPE::L; // ��� - �������
						if (j == 0) { // GRAPH_INT_LITERAL
							new_identifier.iddatatype = IT::IDDATATYPE::INT; // �������������
							// -2^32 <= in.tokens[i].token <= 2^32
							int temp = 0;
							try {
								temp = std::stoi(std::string(in.tokens[i].token));
							} catch (...) {
								throw ERROR_THROW_IN(219, in.tokens[i].line, in.tokens[i].pos);
							}
							new_identifier.value.vint = temp; // ����������� �� ������ � �����
						}
						else { //GRAPH_STRING_LITERAL
							new_identifier.iddatatype = IT::IDDATATYPE::STR; // ������
							new_identifier.value.vstr.len = strlen(in.tokens[i].token) - 2;
							if (new_identifier.value.vstr.len > 255) {
								throw ERROR_THROW_IN(220, in.tokens[i].line, in.tokens[i].pos);
							}
							strncpy(new_identifier.value.vstr.str, in.tokens[i].token + 1, new_identifier.value.vstr.len);
							new_identifier.value.vstr.str[new_identifier.value.vstr.len] = 0;
						}
						new_lexem.idxTI = IT::Add(lex_out.IDtable, new_identifier); // ����� ���������� ������ �������������� ��������� ��� id � �������
						int lexid = LT::Add(lex_out.Lextable, new_lexem); // ��������� id ������� ����� �� ����������
						lex_out.IDtable.table[new_lexem.idxTI].idxfirstLE = lexid; // � ������� ��������������� ��������� id �������
					}
					else if (graph[j].lexema == 't') { // ��������� ������� ���
						if (j == 4) { // GRAPH_INTEGER
							new_lexem.lexema[0] = LEX_INTEGER;
							new_lexem.isIntType = true;
						}
						else { // GRAPH_STRING
							new_lexem.lexema[0] = LEX_STRING;
							new_lexem.isIntType = false;
						}
						LT::Add(lex_out.Lextable, new_lexem);
					}
					else if (graph[j].lexema == 'i') { // ��������� ������� �������������

						{	// ��������� ��������� �������� ��������������
							new_identifier.value.vint = 0;
							strcpy(new_identifier.value.vstr.str, "");
							new_identifier.value.vstr.len = 0;
						}

						if (strlen(in.tokens[i].token) > 10) { // ������������� �� ����� ���� ������� 10 ��������, ������� �� 10
							strncpy(new_identifier.id, in.tokens[i].token, 10); // ��������� ��� ��������������
							new_identifier.id[10] = 0;
						}
						else {
							strcpy(new_identifier.id, in.tokens[i].token); // ��������� ��� ��������������
						}

						new_identifier.idtype = IT::IDTYPE::V;

						new_lexem.lexema[0] = LEX_ID; // ��� ������� - �������������
						new_identifier.iddatatype = IT::IDDATATYPE::UNKNOWN;
						new_lexem.idxTI = IT::Add(lex_out.IDtable, new_identifier);
						int lexid = LT::Add(lex_out.Lextable, new_lexem);
						lex_out.IDtable.table[new_lexem.idxTI].idxfirstLE = lexid; // � ������� ��������������� ��������� id �������
					}
					else { // ��� ��������� ������� ����������� ���������
						new_lexem.lexema[0] = graph[j].lexema;
						new_lexem.idxTI = LT_TI_NULLIDX;
						LT::Add(lex_out.Lextable, new_lexem);
					}
					break;
				}
			}
			if (!recognized) {
				throw ERROR_THROW_IN(213, in.tokens[i].line, in.tokens[i].pos); // ����� ��� ������ ��� ����������� �������
			}
		}
		return lex_out;
	}
}