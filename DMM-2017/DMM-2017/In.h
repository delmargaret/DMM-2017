#pragma once

#include "stdafx.h"
#include "LT.h"
#include "IT.h"

#define IN_MAX_LEN_TEXT 1024*1024
#define IN_MAX_STRING_CODE 1024
#define IN_CODE_ENDL '\n'
#define IN_CODE_SPACE ' '
#define IN_CODE_NULL '\0'

#define IN_CODE_TABLE {\
    IN::F,     IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,   IN::F,   IN::T,    IN::F,   IN::F,   IN::F,   IN::F, IN::F,\
	IN::F,     IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,   IN::F,   IN::F,    IN::F,   IN::F,   IN::F,   IN::F, IN::F,\
	IN::T,     IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,     IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,     IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,     IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,     IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,     IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,	  IN::T,   IN::T, IN::T,\
}

namespace In
{
	struct StToken
	{
		int  line; // строка в которой находится фраза
		int pos; // позиция в строке
		char token[IN_MAX_STRING_CODE];
	};

		struct IN
	{
		enum { LNBREAK = 512, T = 1024, F = 2048, I = 4096, SEP = 8192, STAB = 16384, SPACE = 32768, L = 65536};
		int code[128];
		int size = NULL;
		int lines = NULL;
		int ignor = NULL;
		unsigned char *text;
		StToken *tokens;
		int tokensSize;  // Количество фраз
		int start_line;
		int start_pos;
	};
	IN getin(wchar_t infile[]);
	IN parsOfToken(IN &tokens);
	void addToken(IN &in, char* &buffer, int &posbuf, int line, int pos);
};