#pragma once
#include "stdafx.h"

#define ID_MAXSIZE			11 // C-Style strings are 0-ended
#define TI_MAXSIZE			4096
#define TI_INT_DEFAULT		0x00000000
#define TI_STR_DEFAULT		0x00
#define TI_NULLIDX			0xffffffff
#define TI_STR_MAXSIZE		255
#define TI_MAX_PARAMS_COUNT 15


namespace IT //������� ���������������
{
	enum IDDATATYPE { INT = 1, STR = 2, UNKNOWN = 3 }; //���� ������ ���������������
	enum IDTYPE     { V = 1, F = 2, L = 3, P = 4 }; //���� ���������������
													//(V ����������, F �������, L �������, P ��������)

	struct Entry
	{
		int				idxfirstLE; //������ ������ ������ � ������� ������
		char			id[ID_MAXSIZE]; //�������������
		IDDATATYPE		iddatatype; //��� ������
		IDTYPE			idtype; //��� ��������������
		char            vis[ID_MAXSIZE]; //������� ���������
		int				func_param_count; 
		bool            isArray; //���� (����������� �������)
		bool		    isLibrary; //���� (����������� ������������ �������)
		Entry*			params[TI_MAX_PARAMS_COUNT + 1]; //��������� �������
		int				paramsCount; //���������� ���������� �������
		int				createdTempP;
		union
		{
			int vint; //�������� ������ ����
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE + 1]; 
			} vstr; //�������� ���������� ����
		}value; //�������� ��������������
	};


	struct IdTable //������� ���������������
	{
		int maxsize; //������������ ������ �������
		int size; //������� ������ �������
		Entry* table; //������ ����� �������
	};
	IdTable Create(int size); //������� �������
	int		Add(IdTable& idtable, Entry entry); //�������� ������ � �������
	Entry	GetEntry(IdTable& idtable, int n); //�������� ������ �������
	int		IsId(IdTable& idtable, char id[ID_MAXSIZE], char* visibility); //������� ������ ������, ���� �������������
	void	Delete(IdTable& idtable); //������� �������
};