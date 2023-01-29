#include "IT.h";
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Error.h"
using namespace std;

using namespace IT;

IdTable IT::Create()
{
	IdTable IdTable = {};
	IdTable.maxsize = TI_MAXSIZE;
	IdTable.size = 0;
	IdTable.table = new Entry[IdTable.maxsize];
	if (IdTable.size > TI_MAXSIZE) throw ERROR_THROW(117);
	return IdTable;
}

void IT::Add(IdTable& idtable, Entry entry)
{
	idtable.table[idtable.size] = entry;
	idtable.size++;
}

void IT::Delete(IdTable& idtable)
{
	IdTable temp = {};
	idtable = temp;
}

Entry IT::GetEntry(IdTable& idtable, int n)
{
	return idtable.table[n];
	if (n > idtable.size) throw ERROR_THROW(119);
}

int IT::IsId(IdTable& idtable, char* id)
{
	for (int i = idtable.size - 1; i >= 0; i--)
	{
		if (strcmp(id, idtable.table[i].id) == 0)
		{
			return i;
		}
	}
	return TI_NULLIDX;
}
void IT::PrintTable(IdTable& idtable) {
	
	ofstream fout("IT.txt.it");
	if (!fout.is_open()) {
		throw ERROR_THROW(114);
	}
	Entry e;
	fout << endl << "\t\t\t\t\t\t~ ~ ~TABLE OF IDENTIFIERS~ ~ ~" << endl;
	fout << endl;
	fout << "" << setw(130) << setfill('—') << "—" << endl;
	fout << "\tTYPE:\t\t\t|" << "ID:" << "\t\t\t\t" << "|DATA TYPE:" << "\t\t" << "|№ IN LT"  << "\t\t" << "|VALUE(PARM):" << endl;
	fout << "" << setw(130) << setfill('—') << "—" << endl;
	for (int i = 0; i < idtable.size; i++)
	{
		e = idtable.table[i];
		switch (e.idtype)
		{
			
		case IT::L:
			switch (e.iddatatype)
			{
			case IT::INT:
				fout << "\tLITERAL\t\t\t|" << e.id << "\t\t\t\t|" << "INT" << "\t\t\t|" << e.idxfirstLE << "\t\t\t|" << e.value.vint << endl;
				break;
			case IT::STR:
				fout << "\tLITERAL\t\t\t|"  << e.id << "\t\t\t\t|" << "STR" << "\t\t\t|" << e.idxfirstLE << "\t\t\t|"  << e.value.vstr.str << endl;
				break;
			case IT::BOOL:
				fout << "\tLITERAL\t\t\t|"  << e.id <<"\t\t\t\t|" << "BOOL" << "\t\t\t|" << e.idxfirstLE << "\t\t\t|" << e.value.vbool << endl;
				break;
			}
			break;
		case IT::V:
			switch (e.iddatatype)
			{
			case IT::INT:
				fout << "\tVARIABLE\t\t|" << e.id << "\t\t\t|" << "INT" << "\t\t\t|" << e.idxfirstLE /*<< "\t\t\t|" << e.value.vint */<< endl;
				break;
			case IT::STR:
				fout << "\tVARIABLE\t\t|" << e.id << "\t\t\t|" << "STR" << "\t\t\t|" << e.idxfirstLE /*<< "\t\t\t" << e.value.vstr.str */<< endl;
				break;
			case IT::BOOL:
				fout << "\tVARIABLE\t\t|" << e.id << "\t\t\t|" << "BOOL" << "\t\t\t|" << e.idxfirstLE /*<< "\t\t\t|" << e.value.vbool */<< endl;
				break;
			}
			break;
		case IT::F:
			switch (e.iddatatype)
			{
			case IT::INT:
				fout << "\tFUNCTION\t\t|" << e.id << "\t\t\t\t|" << "INT" << "\t\t\t|" << e.idxfirstLE /*<< "\t|\t\t\t\t\t" << e.value.vint*/ << endl;
				break;
			case IT::STR:
				fout << "\tFUNCTION\t\t|" << e.id << "\t\t\t\t|" << "STR" << "\t\t\t|" << e.idxfirstLE /*<< "\t|\t\t\t\t\t" << e.value.vstr.str */<< endl;
				break;
			case IT::BOOL:
				fout << "\tFUNCTION\t\t|" << e.id << "\t\t\t\t|" << "BOOL" << "\t\t\t|" << e.idxfirstLE /*<< "\t|\t\t\t\t\t" << e.value.vbool*/ << endl;
				break;
			}
			break;
		case IT::P:
			switch (e.iddatatype)
			{
			case IT::INT:
				fout << "\tPARAMETER\t\t|" << e.id << "\t\t\t|" << "INT" << "\t\t\t|" << e.idxfirstLE /*<< "\t\t\t\t\t\t|" << e.value.vint */<< endl;
				break;
			case IT::STR:
				fout << "\tPARAMETER\t\t|" << e.id << "\t\t\t|" << "STR" << "\t\t\t|" << e.idxfirstLE/* << "\t\t\t\t\t\t|" << e.value.vstr.str */<< endl;
				break;
			case IT::BOOL:
				fout << "\tPARAMETER\t\t|" << e.id << "\t\t\t|" << "BOOL" << "\t\t\t|" << e.idxfirstLE /*<< "\t\t\t\t\t\t|" << e.value.vbool */<< endl;
				break;
			}
			break;
		}
		
	}
	fout << "" << setw(130) << setfill('—') << "—" << endl;
	//////////////////////////////////////////////////
	//fout << "Таблица идентификаторов\n";
	//for (int i = 0; i < idtable.size; i++) 
	//{
	//	/*fout << "Индекс: " << i << endl;*/
	//	e = idtable.table[i];
	//	fout << "Идентификатор: " << e.id << '\n';
	//	fout << "Тип данных: ";
	//	switch (e.iddatatype)
	//	{
	//	case IT::INT:
	//		fout << "INT";
	//		break;
	//	case IT::STR:
	//		fout << "STR";
	//		break;
	//	case IT::BOOL:
	//		fout << "BOOL";
	//		break;
	//	}
	//	fout << '\n';
	//	
	//	fout << "Тип идентификатора: ";
	//	switch (e.idtype)
	//	{
	//	case IT::V:
	//		fout << "Переменная";
	//		break;
	//	case IT::F:
	//		fout << "Функция";
	//		break;
	//	case IT::P:
	//		fout << "Параметр";
	//		break;
	//	case IT::L:
	//		fout << "Литерал";
	//		break;
	//	}
	//	
	//	if (e.idtype == IT::L) {
	//		switch (e.iddatatype)
	//		{
	//		case IT::INT:
	//			fout << "\nЗначение: " << e.value.vint;
	//			break;
	//		case IT::STR:
	//			fout << "\nЗначение: " << e.value.vstr.str;
	//			break;
	//		case IT::BOOL:
	//			fout << "\nЗначение: " << e.value.vbool;
	//			break;
	//		}
	//		
	//	}
	//	fout << endl << endl;
	//}
	fout.close();
}


