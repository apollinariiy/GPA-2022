#define _CRT_SECURE_NO_WARNINGS
#include "IT.h"
#include"LT.h"
#include"In.h"
#include <vector>
#include "Lex_Analyzer.h"
#include "Log.h"
#include "Generation.h" 
#include "semantic.h"
#include <string>

void FixID(LA::Tables& tables)
{
	char* id = (char*)"";
	char* prefix = new char[256];
	prefix = (char*)"_GPA";
	int nLiteral = 1;
	char* buffer = new char[256];
	char* L = new char[256];
	L[0] = 'L';
	L[1] = '\0';

	for (int i = 0; i < tables.idTable.size; i++)
	{
		if (tables.idTable.table[i].idtype == IT::L)
		{
			_itoa_s(nLiteral, buffer, 256, 10);
			strcat(L, buffer);
			tables.idTable.table[i].id = L;
			nLiteral++;
			L = NULL;
			delete[] L;
			L = new char(256);
			L[0] = 'L';
			L[1] = '\0';
		}
		else
		{
			for (int j = 0; j < strlen(tables.idTable.table[i].id); j++)
				buffer[j] = tables.idTable.table[i].id[j];
			buffer[strlen(tables.idTable.table[i].id)] = '\0';
			strcat(buffer, prefix);
			id = buffer;
			tables.idTable.table[i].id;
		}

	}
	L = NULL;
	delete[]L;
	buffer = NULL;
	delete[]buffer;


}

void GEN::GetData(std::string& data, LA::Tables tables)
{
	data += "\n\tbuffer BYTE 256 dup(0)";

	for (int i = 0; i < tables.LexTable.size; i++)
	{
		if (tables.LexTable.table[i].lexema == 'v')
		{
			data += "\n\t";
			data += tables.idTable.table[tables.LexTable.table[i + 2].indID].id;
			if (tables.idTable.table[tables.LexTable.table[i + 2].indID].iddatatype == IT::BOOL)
				data += " DD 0";
			else if (tables.idTable.table[tables.LexTable.table[i + 2].indID].iddatatype == IT::INT)
				data += " DD 0";
			else if (tables.idTable.table[tables.LexTable.table[i + 2].indID].iddatatype == IT::STR)
				data += " DD ?";
		}
	}
	data += "\n\n";
}

void GEN::GetConst(std::string& code, LA::Tables tables)
{
	std::string value = "";
	for (int i = 0; i < tables.idTable.size; i++)
	{
		if (tables.idTable.table[i].idtype == IT::L)
		{
			code += "\t";
			code += tables.idTable.table[i].id;
			if (tables.idTable.table[i].iddatatype == IT::INT)
			{
				code += " dd ";
				value = std::to_string(tables.idTable.table[i].value.vint);
				code += value;
			}
			else if (tables.idTable.table[i].iddatatype == IT::BOOL)
			{
				code += " dd ";
				if (tables.idTable.table[i].value.vbool) value = "1";
				else value = "0";
				code += value;
			}
			else if (tables.idTable.table[i].iddatatype == IT::STR)
			{
				if (strcmp(tables.idTable.table[i].value.vstr.str, "''") == 0)
				{
					code += " db 0";
				}
				else
				{
					code += " db ";
					code += tables.idTable.table[i].value.vstr.str;
					code += " , 0";
				}

			}
			code += "\n";
		}
	}
	code += "\n";
}

void GEN::GetCode(std::string& code, LA::Tables tables)
{
	GEN::Inf inf;
	for (int i = 0; i < tables.LexTable.size; i++)
	{
		if (tables.LexTable.table[i].lexema == 'f') // объявление функции
		{
			inf.inFunc = true;
			inf.indFun = tables.LexTable.table[i + 1].indID;
			i += 2;
			code += tables.idTable.table[inf.indFun].id;
			code += " PROC ";
			while (tables.LexTable.table[i].lexema != ')')
			{
				if (tables.LexTable.table[i].lexema == 'i')
				{
					inf.indI = tables.LexTable.table[i].indID;
					if (tables.idTable.table[inf.indI].iddatatype == IT::INT)
					{
						code += tables.idTable.table[inf.indI].id;
						code += " : DWORD ";
						if (tables.LexTable.table[i + 1].lexema != ')') code += ", ";
					}
					else if (tables.idTable.table[inf.indI].iddatatype == IT::BOOL)
					{
						code += tables.idTable.table[inf.indI].id;
						code += " : DWORD ";
						if (tables.LexTable.table[i + 1].lexema != ')') code += ", ";
					}
					else if (tables.idTable.table[inf.indI].iddatatype == IT::STR)
					{
						code += tables.idTable.table[inf.indI].id;
						code += " : DWORD ";
						if (tables.LexTable.table[i + 1].lexema != ')') code += ", ";
					}
				}
				i++;
			}
			code += "\n\n";
		}

		else if (tables.LexTable.table[i].lexema == '}') // конец функции или конец main
		{
			if (inf.inFunc)
			{
				code += tables.idTable.table[inf.indFun].id;
				code += " ENDP\n\n";
				inf.inFunc = false;
			}
			else if (inf.inStart)
			{
				code += "push 0\ncall consolpause\ncall ExitProcess\nmain ENDP\nend main";
			}
		}

		else if (tables.LexTable.table[i].lexema == 's') // main
		{
			code += "\nmain PROC\nSTART:\n\n";
			inf.inStart = true;
		}

		else if (tables.LexTable.table[i].lexema == 'r') // return
		{
			if (tables.idTable.table[inf.indFun].iddatatype == IT::STR)
			{
				code += "mov eax, offset ";
				code += tables.idTable.table[tables.LexTable.table[i + 1].indID].id;
				code += "\nret\n";
			}
			else
			{
				code += "mov eax, ";
				code += tables.idTable.table[tables.LexTable.table[i + 1].indID].id;
				code += "\nret\n";
			}
		}

		else if (tables.LexTable.table[i].lexema == 'w') // вывод в консоль
		{
			inf.indI = tables.LexTable.table[i + 2].indID;
			inf.id = tables.idTable.table[inf.indI].id;

			if (tables.idTable.table[inf.indI].iddatatype == IT::STR)
			{
				if (tables.idTable.table[inf.indI].idtype == IT::L)
				{
					code += "mov EAX, offset ";
					code += tables.idTable.table[inf.indI].id;
					code += "\npush EAX";
					code += "\ncall ConsoleStr\n\n";
				}
				else
				{
					code += "mov EAX, ";
					code += tables.idTable.table[inf.indI].id;
					code += "\npush EAX";
					code += "\ncall ConsoleStr\n\n";
				}
			}
			else if (tables.idTable.table[inf.indI].iddatatype == IT::INT)
			{
				code += "mov EAX, ";
				code += tables.idTable.table[inf.indI].id;
				code += "\npush EAX";
				code += "\ncall ConsoleInt\n\n";

			}
			else if (tables.idTable.table[inf.indI].iddatatype == IT::BOOL)
			{
				code += "mov EAX, ";
				code += tables.idTable.table[inf.indI].id;
				code += "\npush EAX";
				code += "\ncall ConsoleBool\n\n";

			}

		}

		else if (tables.LexTable.table[i].lexema == '=')
		{
			inf.indExpr = tables.LexTable.table[i - 1].indID;
			while (tables.LexTable.table[i].lexema != ';')
			{
				if (tables.LexTable.table[i].lexema == 'i' || tables.LexTable.table[i].lexema == 'l')
				{
					inf.indI = tables.LexTable.table[i].indID;
					if (tables.idTable.table[inf.indI].iddatatype == IT::STR && tables.idTable.table[inf.indI].idtype == IT::L)
					{
						code += "push offset ";
						code += tables.idTable.table[inf.indI].id;
						code += "\n";
					}
					else
					{
						code += "push ";
						code += tables.idTable.table[inf.indI].id;
						code += "\n";
					}
				}
				else if (tables.LexTable.table[i].lexema == 'o')
				{
					if (tables.LexTable.table[i].operatorValue == '+')
					{
						code += "pop EAX\n";
						code += "pop EBX\n";
						code += "add EAX, EBX\n";
						code += "push EAX\n";
					}
					else if (tables.LexTable.table[i].operatorValue == '-')
					{
						code += "pop EBX\n";
						code += "pop EAX\n";
						code += "sub EAX, EBX\n";
						code += "push EAX\n";
					}
					else if (tables.LexTable.table[i].operatorValue == '&')
					{
						code += "pop EAX\n";
						code += "pop EBX\n";
						code += "AND EAX, EBX\n";
						code += "push EAX\n";
					}
					else if (tables.LexTable.table[i].operatorValue == '|')
					{
						code += "pop EAX\n";
						code += "pop EBX\n";
						code += "OR EAX, EBX\n";
						code += "push EAX\n";
					}
					else if (tables.LexTable.table[i].operatorValue == '~')
					{
						code += "pop EAX\n";
						code += "NOT EAX\n";
						code += "push EAX\n";
					}
				}
				else if (tables.LexTable.table[i].lexema == '@')
				{
					if (tables.idTable.table[tables.LexTable.table[i].indID].kek)
					{
						code += "push offset buffer\n";
					}
					code += "call ";
					code += tables.idTable.table[tables.LexTable.table[i].indID].id;
					code += "\npush EAX\n";
				}

				i++;
			}
			code += "pop ";
			code += tables.idTable.table[inf.indExpr].id;
			code += "\n\n";

		}

		else if (tables.LexTable.table[i].lexema == '?')
		{
			inf.jmpTrue = "true";
			inf.jmpExit = "exit";
			char* buffer = new char[256];
			_itoa_s(inf.nIF, buffer, 256, 10);
			inf.jmpTrue += buffer;
			inf.jmpExit += buffer;
			inf.inIf = true;
			inf.nIF++;
			inf.StExit.push(inf.jmpExit);
		}

		else if (tables.LexTable.table[i].lexema == '(' && inf.inIf)
		{
			i++;
			if ((tables.LexTable.table[i].lexema == 'i' || tables.LexTable.table[i].lexema == 'l') && tables.LexTable.table[i + 1].lexema == ')') // true или false
			{
				code += "\nmov EAX,";
				code += tables.idTable.table[tables.LexTable.table[i].indID].id;
				code += "\nmov EBX, 1\n";
				code += "sub EAX, EBX\n";
				code += "je ";
				code += inf.jmpTrue;
				code += "\njmp ";
				code += inf.jmpExit;
				code += "\n\n";
				inf.inIf = false;
			}
			else
			{
				code += "\nmov EAX,";
				code += tables.idTable.table[tables.LexTable.table[i].indID].id;
				code += "\nmov EBX, ";
				code += tables.idTable.table[tables.LexTable.table[i + 2].indID].id;
				code += "\nsub EAX, EBX\n";
				code += "je ";
				code += inf.jmpTrue;
				code += "\njmp ";
				code += inf.jmpExit;
				code += "\n\n";
				inf.inIf = false;
			}
		}

		else if (tables.LexTable.table[i].lexema == '[')
		{
			code += inf.jmpTrue;
			code += ":\n";
		}

		else if (tables.LexTable.table[i].lexema == ']')
		{
			code += "\n";
			code += inf.StExit.top();
			code += ":\n";
			inf.StExit.pop();
		}

	}
}

void GEN::Generation(wchar_t* file, LA::Tables tables, std::vector<SA::Function> funcs)
{

	GEN::AsmCode AsmCode;
	std::ofstream out(file);
	FixID(tables);
	GetData(AsmCode.Data, tables);
	GetConst(AsmCode.Const, tables);
	GetCode(AsmCode.Code, tables);

	out << AsmCode.Head;
	out << AsmCode.Stack;
	out << AsmCode.Const;
	out << AsmCode.Data;
	out << AsmCode.Code;


}