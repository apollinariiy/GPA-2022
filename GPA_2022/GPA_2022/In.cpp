#include "In.h"
#include "Error.h"
#include <fstream>
#include <iostream>
#include <string.h>

namespace In
{
	IN getin(wchar_t infile[])
	{
		IN in;
		in.lines = 0;
		in.ignor = 0;
		in.size = 0;
		int constCode[256] = IN_CODE_TABLE;
		for (int i = 0; i < 256; i++)
			in.code[i] = constCode[i];
		std::ifstream ifs;
		ifs.open(infile, std::ifstream::in);
		if (ifs.fail())
		{
			throw ERROR_THROW(110);
		}
		char c = ifs.get();
		int curLine = 0;
		int posSymb = 0;
		while (ifs.good())
		{
			posSymb++;
			if (c == IN_CODE_ENDL)
			{
				in.lines++;
				curLine++;
				posSymb = 0;
				in.size++;
			}
			else if (in.code[c] == IN::F)
			{
				throw ERROR_THROW_IN(111, curLine, posSymb);
			}
			else if (in.code[c] == IN::I)
			{
				in.ignor++;
			}
			else if (in.code[c] == IN::T || in.code[c] == IN::S || in.code[c] == IN::P || in.code[c] == IN::Q)
			{
				in.size++;
			}
			c = ifs.get();
		}
		ifs.close();

		in.text = new unsigned char[in.size];
		ifs.open(infile, std::ifstream::in);
		c = ifs.get();
		posSymb = 0;
		while (ifs.good())
		{
			if (c == IN_CODE_ENDL)
			{
				in.text[posSymb] = IN_CODE_ENDL;
				posSymb++;
			}
			else if (in.code[c] == IN::I)
			{

			}
			else if (in.code[c] == IN::T || in.code[c] == IN::S || in.code[c] == IN::P || in.code[c] == IN::Q)
			{
				in.text[posSymb] = c;
				posSymb++;
			}
			else if (in.code[c] >= 0 && in.code[c] < 256)
			{
				in.text[posSymb] = in.code[c];
				posSymb++;
			}
			c = ifs.get();
		}
		in.text[posSymb] = 0;
		ifs.close();

		return in;
	}
}