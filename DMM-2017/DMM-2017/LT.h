#pragma once
#include "stdafx.h"

#define LEXEMA_FIXSIZE		1
#define LT_MAXSIZE			4096
#define LT_TI_NULLIDX		0xffffffff
#define LEX_INTEGER			't'
#define LEX_STRING			't'
#define LEX_ARRAY			'a'
#define LEX_ID				'i'
#define LEX_LITERAL			'l'
#define LEX_FUNCTION		'f'
#define LEX_DECLARE			'd'
#define LEX_RETURN			'r'
#define LEX_PRINT			'p'
#define LEX_MAIN			'm'
#define LEX_SEMICOLON		';'
#define LEX_COMMA			','
#define LEX_LEFTBRACE		'{'
#define LEX_BRACELET		'}'
#define LEX_LEFTTHESIS		'('
#define LEX_RIGHTHESIS		')'
#define LEX_PLUS			'+'
#define LEX_MINUS			'-'
#define LEX_STAR			'*'
#define LEX_DIRSLASH		'/'
#define LEX_EQUALITY		'='

#define LEX_NULL			'%'


namespace LT //таблица лексем
{
	struct Entry //строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE]; //лексема
		bool isIntType; //флаг (определяет целочисленный ли тип)
		int sn; //номер строки в исходном тексте
		int pos; //позиция
		int idxTI; //индекс в таблице идентификаторов
		int lexemId; // индекс в таблице лексем
		Entry() {};
		Entry(char plexema, int psn, int pidxTI) {this->lexema[0] = plexema; this->sn = psn; this->idxTI = pidxTI; this->pos = 0;};
	};

	struct LexTable //экземпляр таблицы лексем
	{
		int maxsize; //максимальный размер таблицы
		int size; //текущий размер таблицы
		Entry* table; //массив строк таблицы
	};

	LexTable Create(int size); //создать таблицу лексем
	int		 Add(LexTable& lextable, Entry entry); //добавить строку в таблицу
	Entry	 GetEntry(LexTable& lextable, int n); //получить строку таблицы
	void     Delete(LexTable& lextable); //удалить таблицу
};