#include "semantic.h"
#include "PolishNotation.h"
#include "IT.h"
#include"LT.h"
#include"In.h"
#include "Error.h"
#include <vector>

void SA::GetFlags(char lexema, SA::Flags& flags)
{
	if (lexema == 'f') flags.flagDecFunction = true;
	else if (lexema == '(' && flags.flagDecFunction)  flags.flagParametres = true;
	else if (lexema == '(' && flags.flagCallFunction) flags.flagParametres = true;
	else if (lexema == ')' && flags.flagCallFunction)
	{
		flags.flagCallFunction = false;
		flags.flagParametres = false;
		flags.indF = -1;
	}
	else if (lexema == '{' && flags.flagDecFunction)
	{
		flags.flagDecFunction = false;
		flags.flagParametres = false;
		flags.flagInFunction = true;
	}
	else if (lexema == 'r' && flags.flagInFunction) flags.flagReturn = true;
	else if (lexema == ';' && flags.flagReturn) flags.flagReturn = false;
	else if (lexema == '}' && flags.flagInFunction) flags.flagInFunction = false;
}

int SA::CheckInFunctions(std::vector<SA::Function> functions, char* id)
{
	for (int i = 0; i < functions.size(); i++)
	{
		if (strcmp(functions[i].function.id, id) == 0)
			return i;
	}
	return -1;
}

std::vector<SA::Function> SA::SemAnalysis(LA::Tables tables)
{
	SA::Flags flags;
	std::vector<SA::Function> functions;
	functions.push_back({ tables.idTable.table[0] });
	functions[0].parameters.push_back(IT::STR);
	functions.push_back({ tables.idTable.table[1] });
	functions[1].parameters.push_back(IT::STR);
	functions[1].parameters.push_back(IT::STR);
	functions.push_back({ tables.idTable.table[2] });
	functions[2].parameters.push_back(IT::STR);
	functions[2].parameters.push_back(IT::STR);
	functions.push_back({ tables.idTable.table[3] });
	functions[3].parameters.push_back(IT::STR);
	functions[3].parameters.push_back(IT::STR);

	int indID;
	int indPar;
	int nPar;

	for (int i = 0; i < tables.LexTable.size; i++)
	{
		indID = tables.LexTable.table[i].indID;
		SA::GetFlags(tables.LexTable.table[i].lexema, flags);
		if (indID != TI_NULLIDX)
		{
			if (tables.LexTable.table[i].lexema == 'i' && tables.LexTable.table[i + 1].lexema == '(' && tables.LexTable.table[i - 1].lexema != 'f')
			{
				if (CheckInFunctions(functions, tables.idTable.table[indID].id) == -1) throw Error::geterrorin(122, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
			}
			if (flags.flagExpression)// провер€ем типы операндов в выражении
			{
				if (tables.idTable.table[indID].iddatatype != flags.ExpressionType) throw Error::geterrorin(127, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
				if (tables.LexTable.table[i + 1].lexema == 'o')
				{
					if (tables.idTable.table[indID].iddatatype != IT::INT) throw Error::geterrorin(120, tables.LexTable.table[i + 1].sn, tables.LexTable.table[i + 1].posWord);
				}
			}
			if (flags.flagReturn)
			{
				if (functions[functions.size() - 1].function.iddatatype != tables.idTable.table[indID].iddatatype) throw Error::geterrorin(121, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
				else if (tables.idTable.table[indID].idtype == IT::F) throw Error::geterrorin(121, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
			}
			else if (tables.idTable.table[indID].idtype == IT::F && flags.flagDecFunction) // записываем функцию в vector при объ€влении
			{
				functions.push_back({ tables.idTable.table[indID] });
			}
			else if (flags.flagParametres) // записываем типы параметров в vector
			{
				if (flags.flagDecFunction) // запись параметров функции
					functions[functions.size() - 1].parameters.push_back(tables.idTable.table[indID].iddatatype);
			}
			else if (CheckInFunctions(functions, tables.idTable.table[indID].id) != -1) // попали на вызов функции
			{
				flags.indF = CheckInFunctions(functions, tables.idTable.table[indID].id);
				flags.flagCallFunction = true;
			}
		}
		else if (flags.flagCallFunction) // проверка параметров
		{

			indPar = 0;
			while (tables.LexTable.table[i].lexema != ')')
			{

				if (tables.LexTable.table[i].lexema == 'i' || tables.LexTable.table[i].lexema == 'l')
				{
					if (functions[flags.indF].parameters.size() <= indPar) throw Error::geterrorin(128, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
					else if (functions[flags.indF].parameters[indPar] != tables.idTable.table[indID].iddatatype) throw Error::geterrorin(128, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
					indPar++;
				}
				i++;
				indID = tables.LexTable.table[i].indID;
			}
			if (functions[flags.indF].parameters.size() != indPar) throw Error::geterrorin(128, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
			SA::GetFlags(tables.LexTable.table[i].lexema, flags);
		}
		else if (tables.LexTable.table[i].lexema == '=') // обнаруженно выражение
		{
			flags.flagExpression = true;
			flags.ExpressionType = tables.idTable.table[tables.LexTable.table[i - 1].indID].iddatatype;
			flags.ExpressionIndLT = tables.LexTable.table[i - 1].idxLT;
		}
		else if (tables.LexTable.table[i].lexema == ';' && flags.flagExpression) // конец выражени€
		{
			flags.flagExpression = false;
			flags.ExpressionType = IT::NODEF;
			bool i = PN::PolishNotation(flags.ExpressionIndLT, tables.LexTable, tables.idTable);
		}
		else if (tables.LexTable.table[i].lexema == '?') // обнаружен условный оператор
		{
			i += 2;
			indID = tables.LexTable.table[i].indID;
			if (tables.LexTable.table[i + 1].lexema == 'o')
			{
				if (tables.LexTable.table[i + 1].operatorValue != 'e') throw Error::geterrorin(125, tables.LexTable.table[i + 1].sn, tables.LexTable.table[i + 1].posWord);
				if (tables.idTable.table[indID].iddatatype != IT::INT || tables.idTable.table[indID].idtype == IT::F) throw Error::geterrorin(125, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
				indID = tables.LexTable.table[i + 2].indID;
				if (tables.idTable.table[indID].iddatatype != IT::INT || tables.idTable.table[indID].idtype == IT::F) throw Error::geterrorin(125, tables.LexTable.table[i + 2].sn, tables.LexTable.table[i + 2].posWord);
			}
			else
			{
				if (tables.idTable.table[indID].iddatatype != IT::BOOL || tables.idTable.table[indID].idtype == IT::F) throw Error::geterrorin(125, tables.LexTable.table[i].sn, tables.LexTable.table[i].posWord);
			}
		}
	}
	return functions;
}