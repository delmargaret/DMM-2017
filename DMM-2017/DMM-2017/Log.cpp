#include "stdafx.h"
#include "Log.h"
#include "Error.h"
#include <fstream>
#include <stdio.h>  
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include "Parm.h"
#include "LexAnalis.h"

namespace Log
{
	LOG getlog(wchar_t logfile[])// ������� � ��������� ��������� �����
	{
		LOG log;
		wcscpy_s(log.logfile, logfile);
		log.stream = new std::ofstream(logfile, std::ofstream::out);
		if (log.stream->fail()) {
			throw ERROR_THROW(112);
		}
		return log;
	}

	void WriteLine(LOG log, char*c, ...)
	{
		char** pc = &c;            //����� ������� ���������
		int len = 0;
		while (*pc != "") { // ���� ��� ����������� ����� ����� ���������� �����
			len += strlen(*pc);
			pc++;
		}
		char *str = new char[len + 1];       //������ ��� ������
		str[0] = 0;                       // ������� ������
		pc = &c;                       // ��������� �� 1-� ��������
		while (*pc != "") // ���� ��� ��������� �����
		{
			strcat(str, *pc);             // ���������� 
			pc++;                       // ������������ �� ���������
		}
		log.stream->write(str, len);
		delete[] str;
	}

	void WriteLine(LOG log, wchar_t*c, ...)
	{
		wchar_t** pc = &c; //����� ������� ��������� 
		int len = 0;
		while (*pc != L"") { // ���� ��� ����������� ����� ����� ���������� �����
			len += wcslen(*pc);
			pc++;
		}
		wchar_t *str = new wchar_t[len + 1]; //������ ��� ������ 
		char* strl = new char[len + 1];
		str[0] = 0; // ������� ������
		pc = &c; // ��������� �� 1-� �������� 
		while (*pc != L"") // ���� ��� ��������� ����� 
		{
			wcscat(str, *pc); // ���������� 
			pc++; // ������������ �� ��������� 
		}
		wcstombs(strl, str, len + 1);
		log.stream->write(strl, len);
		delete[] str;
	}

	void WriteLog(LOG log)
	{
		time_t curTime;
		tm timeStruct;// ��������� � ����� � ��������
		time(&curTime);

		char strTime[30];
		localtime_s(&timeStruct, &curTime);// ����������� ������� � ������� ����

		strftime(strTime, 1024, "%d.%m.%Y %H:%M:%S", &timeStruct);// ����� � ���� � ������
		*log.stream << "---- �������� ------ " << strTime << " ------------------" << std::endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		*log.stream << "---- ��������� ------" << std::endl;

		char temp[PARM_MAX_SIZE + 1];
		wcstombs(temp, PARM_LOG, wcslen(PARM_LOG) + 1);
		*log.stream << temp << " ";
		wcstombs(temp, parm.log, wcslen(parm.log) + 1);
		*log.stream << temp << std::endl;

		wcstombs(temp, PARM_IN, wcslen(PARM_IN) + 1);
		*log.stream << temp << " ";
		wcstombs(temp, parm.in, wcslen(parm.in) + 1);
		*log.stream << temp << std::endl;
	}

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "---- �������� ������ ------" << std::endl;
		*log.stream << "���������� ��������: " << in.size << std::endl;
		*log.stream << "���������������    : " << in.ignor << std::endl;
		*log.stream << "���������� �����   : " << in.lines << std::endl;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		*log.stream << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << ",������� " << error.inext.col << std::endl;
	}

	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
		log.stream = NULL;
	}

	void WriteLexems(LOG log, const LT::LexTable& lex, const IT::IdTable& ids, const char* outputFilename)
	{
		LOG logg;
		logg.stream = new std::ofstream(outputFilename, std::ofstream::out);
		if (logg.stream->fail()) {
			throw ERROR_THROW(112);
		}
		Log::WriteLine(logg, "---- ������� ������ ---- \n", "");
		int j = 0;
		while (j < lex.size)
		{
			int cur_line = lex.table[j].sn; // ���������� ����� ������ ��� ������� �������
			Log::WriteLine(logg, (char *)std::to_string(cur_line).c_str(), " ", ""); // �������� ����� ������
			while (j < lex.size && cur_line == lex.table[j].sn) {
				char lexname[2];
				lexname[0] = lex.table[j].lexema[0];
				lexname[1] = 0;
				Log::WriteLine(logg, (char *)lexname, "");
				if (lex.table[j].lexema[0] == '@') {
					Log::WriteLine(logg, (char *)std::to_string(ids.table[lex.table[j].idxTI].func_param_count).c_str(), "");
				}
				//Log::WriteLine(log, (char *)std::to_string(j).c_str(), ""); - ����� ������� ����� ������ ��� ������ ������ �������� �������
				j++;
			}
			Log::WriteLine(logg, "\n", "");
		}
	}

	void WriteItentifiers(LOG log, const LT::LexTable& lex, const IT::IdTable& ids)
	{
		LOG loog;
		loog.stream = new std::ofstream("TableIT.txt", std::ofstream::out);
		if (loog.stream->fail()) {
			throw ERROR_THROW(112);
		}
		Log::WriteLine(loog, "----������� ���������������---- \n", "");
		*loog.stream << std::setw(10) << "��� " << std::setw(18) << "��� ������ " << std::setw(22) << "��� �������������� " << std::setw(22) << "����� ������� ������ " <<
			std::setw(20) << "������� ��������� " << std::setw(20) << "����������� �������" << std::setw(22) << "����. �� ���������" << std::endl;
		for (int i = 0; i < ids.size; i++)
		{
			int idx = i;
			if (ids.table[i].idxfirstLE != LT_TI_NULLIDX) {
				idx = lex.table[ids.table[i].idxfirstLE].idxTI;
			}
			if (ids.table[idx].idxfirstLE < 0) {
				continue;
			}

			if (ids.table[idx].idtype != IT::IDTYPE::L) {
				*loog.stream << std::setw(10) << ids.table[idx].id;
			}
			else {
				*loog.stream << std::setw(10) << "Literal";

			}

			if (ids.table[idx].isArray) {
				*loog.stream << std::setw(15) << "Array";
			} else if (ids.table[idx].iddatatype == IT::IDDATATYPE::INT) {
				*loog.stream << std::setw(15) << "Entero";
			}
			else if (ids.table[idx].iddatatype == IT::IDDATATYPE::STR) {
				*loog.stream << std::setw(15) << "Cuerda";
			} else {
				*loog.stream << std::setw(15) << "Unknown";
			}

			if (ids.table[idx].idtype == IT::IDTYPE::V) {
				*loog.stream << std::setw(20) << "Variable";
			} else if (ids.table[idx].idtype == IT::IDTYPE::F) {
				*loog.stream << std::setw(20) << "Function";
			}
			else if (ids.table[idx].idtype == IT::IDTYPE::P) {
				*loog.stream << std::setw(20) << "Parameter";
			}
			else if (ids.table[idx].idtype == IT::IDTYPE::L) {
				*loog.stream << std::setw(20) << "Literal";
			}

			*loog.stream << std::setw(18) << ids.table[idx].idxfirstLE;

			if (!(ids.table[idx].idtype == IT::IDTYPE::L)) {
				*loog.stream << std::setw(22) << ids.table[idx].vis;
			} else {
				*loog.stream << std::setw(20) << "";
			}

			if (ids.table[idx].idtype == IT::IDTYPE::V && ids.table[idx].isArray) {
				*loog.stream << std::setw(20) << ids.table[idx].value.vint;
			}
			else {
				*loog.stream << std::setw(20) << "Not array";
			}

			if (ids.table[idx].idtype == IT::IDTYPE::L) {
				if (ids.table[idx].iddatatype == IT::IDDATATYPE::INT) {
					*loog.stream << std::setw(22) << ids.table[idx].value.vint;
				}
				else {
					*loog.stream << std::setw(19) << "\'" << ids.table[idx].value.vstr.str << "\'";
				}
			}
			else {
				*loog.stream << std::setw(22) << "n/a";
			}

			*loog.stream << std::endl;


			if (ids.table[idx].idtype == IT::IDTYPE::F && !ids.table[idx].isLibrary) {
				*loog.stream << std::endl;
				*loog.stream << "-------------------- Param count: " << ids.table[idx].paramsCount << std::endl;
				for (int j = 0; j < ids.table[idx].paramsCount; j++) {
					*loog.stream << "-------------------- Type ";
					if (ids.table[idx].params[j]->iddatatype == IT::IDDATATYPE::INT) {
						*loog.stream << std::setw(10) << "Entero";
					} else if (ids.table[idx].params[j]->iddatatype == IT::IDDATATYPE::STR) {
						*loog.stream << std::setw(10) << "Cuerda";
					}
					*loog.stream << " Name " << ids.table[idx].params[j]->id << std::endl;
				}
				*loog.stream << std::endl;
			}
		}
	}
}