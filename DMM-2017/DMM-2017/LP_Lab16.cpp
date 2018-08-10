// LP_Lab13.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "Error.h"
#include "In.h"
#include "Log.h"
#include "Parm.h"
#include "FST.h"
#include "LT.h"
#include "IT.h"
#include "LexAnalis.h"
#include "Polish.h"
#include "MFST.h"
#include "AST.h"
#include "GenCode.h"

#include <iostream>
#include <fstream>

#define EXP1 72
#define EXP2 17
#define EXP3 1

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus");
	try{
		Lexer::LEX lex;
		Log::LOG logger;
		Parm::PARM parm;
		In::IN in;

		try {
			parm = Parm::getparm(argc, argv);
			in = In::getin(parm.in);
			logger = Log::getlog(parm.log);
			Log::WriteParm(logger, parm); // печатаем входные параметры программы
			In::parsOfToken(in);
			lex = Lexer::RunAnalysis(in, logger); // разбираем на лексемы
		}
		catch (Error::ERROR e){
			std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
			std::cout << "Строка " << e.inext.line + 1 << " позиция " << e.inext.col + 1 << std::endl << std::endl;;
			return 0;
		}

		MFST::Mfst mfst(lex, GRB::Greibach::getGreibach());
		AST::Ast ast;
		GENCODE::GenCode generator;

		try {
			mfst.start();
			mfst.printRules();
			ast.createHead(mfst.greibach.startN);
			mfst.prepareAstLexems(&ast);
			if (ast.rules_size > 0) {
				int rule_id = 0;
				int lex_id = 0;
				ast.buildAST(&rule_id, &lex_id, ast.head, ast.rules, ast.nrulechains, &lex);
				ast.checkId(ast.head, &lex);
				ast.checkLiteral(ast.head, &lex);
				ast.declExpressionType(ast.head, &lex);
				ast.assignCheck(ast.head, &lex);
				ast.setFuncParams(ast.head, &lex);
				ast.checkFuncParams(ast.head, &lex);

				cout << setw(4) << left << "Всего строк: " << lex.Lextable.table[lex.Lextable.size - 1].sn + 1 << ", синтаксический анализ выполнен без ошибок" << endl;

				ofstream AstOutput("ASTTree.txt", std::ofstream::out);
				ast.printAST(ast.head, AstOutput);
				AstOutput.close();

				std::ofstream* codefile = new std::ofstream("generate.asm", std::ofstream::out);
				generator.generate(codefile, ast.head, &lex);
				codefile->close();

				Log::WriteLexems(logger, lex.Lextable, lex.IDtable, "TableLT.txt"); // печатаем таблицу лексем
				Log::WriteItentifiers(logger, lex.Lextable, lex.IDtable); // печатаем таблицу идентификаторов
			} else {
				mfst.printDiagnosis();
			}
		}
		catch (Error::ERROR e) {
			std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
			std::cout << "Строка " << e.inext.line + 1 << " позиция " << e.inext.col << std::endl << std::endl;
			return 0;
		}

		try {
			Polish::PolishNotationAst(ast.head, lex.Lextable, lex.IDtable);
			Log::WriteLexems(logger, lex.Lextable, lex.IDtable, "Polish.txt"); // печатаем таблицу лексем после преобразования в польскую нотацию
		}
		catch (...) {
		}
	}
	catch(...){
	cout<< "Неизвестная ошибка. "<<endl;
	}

	return 0;
}

