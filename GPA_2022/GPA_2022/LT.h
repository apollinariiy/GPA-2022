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
		char lexema; // �������
		int sn;						// ����� ������
		int idxLT;					// ������ � ������� ������
		int indID;					// ����� � �������� ����������������
		char operatorValue;			// �������� ��������� ��� ����������.
		int posWord;				// ������� � ������
	};

	struct LexTable
	{
		int maxsize;				 // ���� ������ ������� ������
		int size;					 // ������� ������
		Entry* table; 				 // ������ ����� ������� ������
	};


	LexTable Create();

	void Add(
		LexTable& lextable,			 // ��������� ���� ������
		Entry entry					 // ������ ������� ������
	);

	Entry GetEntry(					 // �������� ������ � ������� ������
		LexTable& lextable,			 // ��������� ���� ������
		int n						 // ����� ���������� ������			
	);

	void Delete(LexTable& lextable); // ������� ������� ������

	void PrintTable(LexTable& lextable); // ������ ������� ������
};