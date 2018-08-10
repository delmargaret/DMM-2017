#pragma once
#include "stdafx.h"

#define ID_MAXSIZE			11 // C-Style strings are 0-ended
#define TI_MAXSIZE			4096
#define TI_INT_DEFAULT		0x00000000
#define TI_STR_DEFAULT		0x00
#define TI_NULLIDX			0xffffffff
#define TI_STR_MAXSIZE		255
#define TI_MAX_PARAMS_COUNT 15


namespace IT //таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2, UNKNOWN = 3 }; //типы данных идентификаторов
	enum IDTYPE     { V = 1, F = 2, L = 3, P = 4 }; //типы идентификаторов
													//(V переменная, F функция, L литерал, P параметр)

	struct Entry
	{
		int				idxfirstLE; //индекс первой строки в таблице лексем
		char			id[ID_MAXSIZE]; //идентификатор
		IDDATATYPE		iddatatype; //тип данных
		IDTYPE			idtype; //тип идентификатора
		char            vis[ID_MAXSIZE]; //область видимости
		int				func_param_count; 
		bool            isArray; //флаг (определение массива)
		bool		    isLibrary; //флаг (определение библиотечной функции)
		Entry*			params[TI_MAX_PARAMS_COUNT + 1]; //параметры функции
		int				paramsCount; //количество параметров функции
		int				createdTempP;
		union
		{
			int vint; //значение целого типа
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE + 1]; 
			} vstr; //значение строкового типа
		}value; //значение идентификатора
	};


	struct IdTable //таблица идентификаторов
	{
		int maxsize; //максимальный размер таблицы
		int size; //текущий размер таблицы
		Entry* table; //массив строк таблицы
	};
	IdTable Create(int size); //создать таблицу
	int		Add(IdTable& idtable, Entry entry); //добавить строку в таблицу
	Entry	GetEntry(IdTable& idtable, int n); //получить строку таблицы
	int		IsId(IdTable& idtable, char id[ID_MAXSIZE], char* visibility); //возврат номера строки, если идентификатор
	void	Delete(IdTable& idtable); //удалить таблицу
};