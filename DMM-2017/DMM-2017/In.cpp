#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include "FST.h"
#include "Log.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

namespace In
{
	IN getin(wchar_t infile[])
	{
		IN in;
		std::ifstream ifs;
		ifs.open(infile, std::ifstream::in);
		if (ifs.fail()) {
			throw ERROR_THROW(110);
		}
		in.text = new unsigned char[IN_MAX_LEN_TEXT + 1];
		unsigned char c;
		int pos = 0;
		while ((c = ifs.get()) && (!ifs.eof())) {
			if (in.size >= IN_MAX_LEN_TEXT) {
				throw ERROR_THROW(116);
			}
			if (c != '\n' && (c < 0x20 || c > 0x7e)) {
				throw ERROR_THROW_IN(111, in.lines, pos);
			}
			in.text[in.size++] = c;
			pos++;
			if (c == '\n') {
				in.lines++;
				pos = 0;
			}
		}
		in.text[in.size] = IN_CODE_NULL;

		return in;
	}

	void addToken(IN &in, char* &buf, int &posbuf, int line, int pos)
	{
		if (in.tokensSize >= IN_MAX_LEN_TEXT) {
			throw ERROR_THROW(117);
		}
		if (posbuf >= IN_MAX_STRING_CODE) {
			throw ERROR_THROW_IN(118, in.start_line, in.start_pos);
		}
		buf[posbuf++] = 0; // После занесённого слова выставляем конец строки
		strcpy_s(in.tokens[in.tokensSize].token, posbuf, buf);
		in.tokens[in.tokensSize].line = in.start_line;
		in.tokens[in.tokensSize].pos = in.start_pos;
		in.tokensSize++;
		buf[0] = 0;
		posbuf = 0;
	}

	void push_buf(IN &in, char* &buf, int &posbuf, int line, int pos, char c) {
		if (posbuf == 0) {
			in.start_line = line;
			in.start_pos = pos;
		}
		buf[posbuf++] = c;
	}

	IN parsOfToken(IN &in)
	{
		in.tokens = new StToken[IN_MAX_LEN_TEXT + 1];
		char *buf = new char[IN_MAX_STRING_CODE + 1];
		int posBuf = 0;
		bool textLiteralFlag = false;
		int line = 0;
		int pos = 0;

		in.tokensSize = 0;

		for (int i = NULL; in.text[i] != 0; i++)
		{
			pos++;
			switch (in.text[i])
			{
				case '\n':
					line++;
					pos = 0;
					break;
				case ' ':
				{
					if (buf[0] != 0)
					{
						if (textLiteralFlag == true)
						{
							push_buf(in, buf, posBuf, line, pos, in.text[i]);
							break;
						}
						if (posBuf > 0)
						{
							addToken(in, buf, posBuf, line, pos);
						}
					}
					break;
				}
				case ')':
				case '(':
				case '+':
				case '-':
				case '*':
				case '/':
				case ';':
				case ',':
				case '[':
				case ']':
				case '{':
				case '}':
				case '=':
				{
					if (textLiteralFlag == true) {
						push_buf(in, buf, posBuf, line, pos, in.text[i]);
						break;
					}
					if (buf[0] != 0 && posBuf > 0)
					{
						addToken(in, buf, posBuf, line, pos);
					}
					push_buf(in, buf, posBuf, line, pos, in.text[i]);
					addToken(in, buf, posBuf, line, pos);
					break;
				}
				case '"':
				{
					if (textLiteralFlag == true)
					{
						textLiteralFlag = false;
					}
					else
					{
						textLiteralFlag = true;
					}
					push_buf(in, buf, posBuf, line, pos, in.text[i]);
					break;
				}
				default:
				{
					push_buf(in, buf, posBuf, line, pos, in.text[i]);
					break;
				}
			}
		}

		return in;
	}
}
