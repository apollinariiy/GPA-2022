#pragma once
#define LEXEMA_FIXSIZE	1
#define LT_MAXSIZE		4096
#define LT_TI_NULLIDX	0xffffffff
#define LEX_INTEGER		't'
#define LEX_STRING		't'
#define LEX_BOOL		't'
#define LEX_ID			'i'
#define LEX_LITERAL		'l'
#define LEX_FUNCTION	'f'
#define LEX_VAR			'v'
#define LEX_RETURN      'r'
#define LEX_WRITE		'w'
#define LEX_IF			'?'
#define LEX_SEMICOLON	';'
#define LEX_COMMA		','
#define LEX_LEFTBRACE	'{'
#define LEX_BRACELET	'}'
#define LEX_STARTBLOCK  '['
#define LEX_ENDBLOCK    ']'
#define LEX_LEFTHESIS	'('
#define LEX_RIGHTHESIS	')'
#define LEX_PLUS		'o'
#define LEX_MINUS		'o'
#define LEX_STAR		'o'
#define LEX_DIRSLASH	'o'
#define LEX_PERCENT     'o'
#define LEX_EQUALITY    'o'
#define LEX_EQUAL       '='
#define LEX_START		's'
#define LEX_BOR 		'o'
#define LEX_BAND     	'o'
#define LEX_BNOT        'o'

namespace LT
{
	
	struct Entry
	{
		char lexema; // лексема
		int sn;						// номер строки
		int idxLT;					// индекс в таблице лексем
		int indID;					// индес в таблицце индентификаторов
		char operatorValue;			// значение оператора дл€ операторов.
		int posWord;				// позици€ в строке
	};

	struct LexTable
	{
		int maxsize;				 // макс размер таблицы лексем
		int size;					 // текущий размер
		Entry* table; 				 // массив строк таблицы лексем
	};


	LexTable Create();

	void Add(
		LexTable& lextable,			 // экземпл€р табл лексем
		Entry entry					 // строка таблицы лексем
	);

	Entry GetEntry(					 // получить строку в таблицу лексем
		LexTable& lextable,			 // экземпл€р табл лексем
		int n						 // номер получаемой строки			
	);

	void Delete(LexTable& lextable); // удалить таблицу лексем

	void PrintTable(LexTable& lextable); // печать таблицы лексем
};