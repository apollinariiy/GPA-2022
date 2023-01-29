#include <cwchar>
#include <iostream>
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "FST.h"
#include "LT.h"
#include "Lex_Analyzer.h"
#include <stack>
#include "PolishNotation.h"
#include "GRB.h"
#include "MFST.h"
#include "semantic.h"
#include "Generation.h"
#include <fstream>
#include <string>
#include <cstring>
using namespace std;


int wmain(int argc, wchar_t* argv[]) {

	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INTLOG;

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog((parm.log));
		wcout << "-in:" << parm.in << ", -out" << parm.out << ", -log: " << parm.log << endl;
		In::IN in = In::getin(parm.in);


		LA::Tables Tables = LA::Lex_analyz(in);

		LA::Inf inf;
		cout << inf.iddatatype;

	/*	cout << "\n----------------------------|\n";
		cout << " № | лексема | номер строки |\n";
		cout << "----------------------------|\n";

		for (int i = 0; i < Tables.LexTable.size; i++) {
			cout << " " << Tables.LexTable.table[i].idxLT << " |    " << Tables.LexTable.table[i].lexema << "    |      " << Tables.LexTable.table[i].sn << "       |\t\t" << Tables.LexTable.table[i].indID << endl;
		}


		for (int i = 0; i < Tables.idTable.size; i++)
		{
			cout << i << "|\t" << Tables.idTable.table[i].id << "\t\t\t\t" << Tables.idTable.table[i].idtype << "\t\t\t" << Tables.idTable.table[i].iddatatype << "\t\t" << Tables.idTable.table[i].idxfirstLE << "\t\t" << Tables.idTable.table[i].value.vint << endl;
		}*/


		MFST_TRACE_START
			MFST::Mfst mfst(Tables, GRB::getGreibach());
		mfst.start();
		mfst.printrules();

		std::vector<SA::Function> functions = SA::SemAnalysis(Tables);
		/*cout << "" << setw(97) << setfill('-') << "-" << endl;
		cout << " № \t| лексема | номер строки |  индекс в таблицце индентификаторов\t | значение оператора   |\n";
		cout << "" << setw(97) << setfill('-') << "-" << endl;

		for (int i = 0; i < Tables.LexTable.size; i++) {
			cout << " " << Tables.LexTable.table[i].idxLT << "\t|\t" << Tables.LexTable.table[i].lexema << " |\t" << Tables.LexTable.table[i].sn << "\t |\t\t" << (Tables.LexTable.table[i].indID == -1 ? "" : std::to_string(Tables.LexTable.table[i].indID)) << "\t\t\t |\t" << Tables.LexTable.table[i].operatorValue << "\t\t|" << endl;
		}
		cout << "" << setw(97) << setfill('-') << "-" << endl << endl;*/
		GEN::Generation(parm.out, Tables, functions);
		
		/*cout << "" << setw(124) << setfill('-') << "-" <<endl;
		cout << " №\t| Индентификатор\t\t\t| Тип идентификатора:\t|Тип данных:\t|1-ое вхождение:\t|Значение:  |\n";
		cout << "" << setw(124) << setfill('-') << "-" << endl;

		for (int i = 0; i < Tables.idTable.size; i++)
		{
			cout << i << "\t|\t" << Tables.idTable.table[i].id << "\t\t\t\t" << (Tables.idTable.table[i].idtype == IT::F ? "F" :
																		         Tables.idTable.table[i].idtype == IT::L ? "L" :
																			     Tables.idTable.table[i].idtype == IT::P ? "P" :
																														   "V") << "\t\t\t" << (Tables.idTable.table[i].iddatatype == IT::BOOL ? "BOOL" :
																																				 Tables.idTable.table[i].iddatatype == IT::STR ? "STR" : 
																																																 "INT") 
				<< "\t\t" << (Tables.idTable.table[i].idxfirstLE == -1 ? " " : std::to_string(Tables.idTable.table[i].idxfirstLE)) << "\t\t\t" << Tables.idTable.table[i].value.vint << "\t     " << "|" << endl;
		}
		cout << "" << setw(124) << setfill('-') << "-" << endl;*/

		Log::WriteLine(log, " ТЕСТ:", " без ошибок\n", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);
		Log::WriteLine(log, "\n\n", "");

		IT::PrintTable(Tables.idTable);
		LT::PrintTable(Tables.LexTable);
		
		char* iddatatype;
		char* idtype;
		char* indLT;
		char* value = (char*)"";

		Log::WriteLine(log, "\n\n", "");

		Log::Close(log);

	} 
	catch (Error::ERROR error) {

		cout << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ",позиция " << error.inext.col << std::endl;
		Log::WriteError(log, error);
		system("pause");
	}
	system("pause");
}



