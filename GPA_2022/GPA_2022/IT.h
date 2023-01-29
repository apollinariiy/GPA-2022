#pragma once
#define ID_MAXSIZE		5
#define TI_MAXSIZE		4096
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE	255

namespace IT {

	enum IDDATATYPE { INT = 1, STR = 2, BOOL = 3, INT8 = 4, NODEF = 0 };		// типы данных индентификаторов: integer , string
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };    // типы идентификаторов: переменная, функция, параметр, литерал

	struct Entry {
		int idxfirstLE;							// индекс первой строки в таблице лексем
		char* id;								// идентификатор
		IDDATATYPE iddatatype;					// тип данных
		IDTYPE idtype;							// тип идентификатора
		bool kek = false;
		union Value
		{
			unsigned int vint;							// значение integer
			
			struct
			{
				int len;						// количество символов в string
				char* str;	// cимволы string
			}vstr;								// значение string
			bool vbool;							// значение bool
		}value;									 // значение идентификатора

	};

	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create();

	void Add(IdTable& idtable, Entry entry);

	Entry GetEntry(IdTable& idtable, int n);

	int IsId(IdTable& idtable, char* id);		// есть ли индентификатор в таблице

	void Delete(IdTable& idtable);
	void PrintTable(IdTable& idtable);
}