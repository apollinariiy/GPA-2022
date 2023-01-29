#pragma once
#include "IT.h"
#include"LT.h"
#include"In.h"
#include <vector>
#include "Lex_Analyzer.h"

namespace SA
{
	struct Flags
	{
		bool flagDecFunction = false;   // ����������� ���������� �������
		bool flagCallFunction = false;  // ��������� ����� �������
		int indF = -1;					// ������ ������� � vector
		bool flagParametres = false;    // ���������� ���������� ������� // jVW6T
		bool flagInFunction = false;    // ����� � ���� �������
		bool flagReturn = false;        // ��������� return
		bool flagExpression = false;    // ���������� �� ���������
		IT::IDDATATYPE ExpressionType = IT::NODEF; // ��� ���������
		int ExpressionIndLT = -1;		// ������ ��������� � ������� ������
		bool flag�ondition = false;     // ��������� �������� ��������
	};

	struct Function
	{
		IT::Entry function;
		std::vector<IT::IDDATATYPE> parameters;
	};

	std::vector<SA::Function> SemAnalysis(LA::Tables tables);

	void GetFlags(char lexema, SA::Flags& flags);

	int CheckInFunctions(std::vector<SA::Function> functions, char* id);
}
