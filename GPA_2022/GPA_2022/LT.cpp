#include <iostream>
#include "LT.h"
#include <fstream>
#include <string>
#include <iomanip>
#include "Error.h"
using namespace LT;
using namespace std;

LexTable LT::Create()
{
	LexTable LexTable = {};
	LexTable.size = 0;
	
	LexTable.maxsize = LT_MAXSIZE;
	LexTable.table = new Entry[LexTable.maxsize];
	if (LexTable.size > LT_MAXSIZE) throw ERROR_THROW(116);
	return LexTable;
}

void LT::Add(LexTable& lextable, Entry entry)
{
	lextable.table[lextable.size] = entry;
	lextable.size++;

}

void LT::Delete(LexTable& lextable)
{
	LexTable temp = {};
	lextable = temp;
}

Entry LT::GetEntry(LexTable& lextable, int n)
{
	if (n > lextable.size) throw ERROR_THROW(118);
	return lextable.table[n];
}

void LT::PrintTable(LexTable& lextable) {
	fstream fout("LT.txt.lt", fstream::out);
	
	if (!fout.is_open()) {
		throw ERROR_THROW(115);
	}
	Entry e;
	fout << "\t\t\t\t~ ~ ~TABLE OF LEXEMES~ ~ ~";
	fout << endl;
	int dist = 8;
	fout << "" << setw(100) << setfill('—') << "—" << endl;
	fout << "\t|LEXEME\t\t"
		<< "|¹ IN IT\t\t"
		<< "|¹ IN CODE\t\t"<<"|OPERATOR VALUE"<<endl;
	fout << "" << setw(100) << setfill('—') << "—" << endl;
	for (int i = 0; i < lextable.size; i++)
	{
		e = lextable.table[i];

		fout << "\t" << e.lexema << "\t\t"
			<<(e.indID == -1 ? "" : std::to_string(e.indID)) << "\t\t\t"
			<< e.sn << "\t\t\t" << e.operatorValue << endl;
	}
	/*int num_string = 0;
	for (int i = 0; i < lextable.size; i++)
	{
		e = lextable.table[i];
		if (num_string == e.sn)
			fout << e.lexema;
		else
		{
			num_string++;
			if (e.sn != num_string)
			{
				continue;
			}
			fout << '\n' << num_string << ".\t";

			if (num_string == e.sn)
				fout << e.lexema;
		}
	}*/

	fout.close();
}