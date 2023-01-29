#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "FST.h"
#include "Lex_Analyzer.h"
#include "In.h"
#include "Log.h"
#include <vector>


int conv(char* s)
{
	int res = 0;
	for (int i = 0; i < strlen(s); ++i)
	{
		res *= 8;
		res += (s[i] - '0');
	}
	return res;
}

bool LA::CheckInVector(std::vector<const char*> vector, const char* word)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if (strcmp(word, vector[i]) == 0) return true;
	}
	return false;
}

void LA::InTables(LA::Tables& tables, int posword, int line, char* word, LA::Inf& inf)
{
	LA::Machines machines[N_GRAPHS] = {
		{LEX_INTEGER   , FST::FST GRAPH_integer				},
		{LEX_STRING    , FST::FST GRAPH_string				},
		{LEX_BOOL	   , FST::FST GRAPH_bool				},
		{LEX_WRITE     , FST::FST GRAPH_print				},
		{LEX_FUNCTION  , FST::FST GRAPH_function			},
		{LEX_VAR	   , FST::FST GRAPH_let					},
		{LEX_START     , FST::FST GRAPH_start				},
		{LEX_RETURN    , FST::FST GRAPH_return				},
		{LEX_IF        , FST::FST GRAPH_if					},
		{LEX_COMMA     , FST::FST GRAPH_COMMA				},
		{LEX_STARTBLOCK, FST::FST GRAPH_STARTBLOCK			},
		{LEX_ENDBLOCK  , FST::FST GRAPH_ENDBLOCK			},
		{LEX_LEFTHESIS , FST::FST GRAPH_LEFTHESIS			},
		{LEX_RIGHTHESIS, FST::FST GRAPH_RIGHTHESIS			},
		{LEX_LEFTBRACE , FST::FST GRAPH_LEFTBRCE			},
		{LEX_BRACELET  , FST::FST GRAPH_BRACELET			},
		{LEX_BOR       , FST::FST GRAPH_BOR                 },
		{LEX_MINUS     , FST::FST GRAPH_MINUS				},
		{LEX_PLUS      , FST::FST GRAPH_PLUS				},
		{LEX_BAND      , FST::FST GRAPH_BAND                },
		{LEX_SEMICOLON , FST::FST GRAPH_SEMICOLON			},
		{LEX_BNOT      , FST::FST GRAPH_BNOT                },
		{LEX_EQUALITY  , FST::FST GRAPH_equality			},
		{LEX_EQUAL     , FST::FST GRAPH_EQUAL				},
		{LEX_LITERAL   , FST::FST GRAPH_true				},
		{LEX_LITERAL   , FST::FST GRAPH_false				},
		{LEX_LITERAL   , FST::FST GRAPH_integerx8_literal	},
		{LEX_LITERAL   , FST::FST GRAPH_integer_literal		},
		{LEX_LITERAL   , FST::FST GRAPH_string_literal		},
		{LEX_ID		   , FST::FST GRAPH_id					},
	};

	std::string LiteralPrefix = "L";
	char lexema = (char)"";
	int indexIT = -1;
	bool executeFlag = 0;
	bool IdFlag = 0;
	union Value
	{
		unsigned int vint;							// значение integer
		struct
		{
			int len;						// количество символов в string
			char* str;						// cимволы string
		}vstr;								// значение string
		bool vbool;							// значение bool
	}value;
	value.vint = 0;
	value.vstr.str = (char*)" ";
	value.vbool = false;
	
	for (int i = 0; i < N_GRAPHS; i++)
	{
		if (FST::execute(machines[i].machine, word))
		{
			if (machines[i].lexema == 'i' || machines[i].lexema == 'l') // обработка литералов
			{
				if (i == N_GRAPHS - 6)
				{
					inf.iddatatype = IT::IDDATATYPE::BOOL;
					inf.idtype = IT::IDTYPE::L;
					value.vbool = true;
				}
				else if (i == N_GRAPHS - 5)
				{
					inf.iddatatype = IT::IDDATATYPE::BOOL;
					inf.idtype = IT::IDTYPE::L;
					value.vbool = false;
				}
				else if (i == N_GRAPHS - 4)
				{
					inf.iddatatype = IT::IDDATATYPE::INT;
					inf.idtype = IT::IDTYPE::L;
					value.vint = conv(word);
				}
				else if (i == N_GRAPHS - 3)
				{
					inf.iddatatype = IT::IDDATATYPE::INT;
					inf.idtype = IT::IDTYPE::L;
					value.vint = atoi(word);
				}
				else if (i == N_GRAPHS - 2)
				{
					inf.iddatatype = IT::IDDATATYPE::STR;
					inf.idtype = IT::IDTYPE::L;
					value.vstr = { (int)strlen(word), word };
				}
				IdFlag = true;
			}
			executeFlag = true;
			lexema = machines[i].lexema;
			break;
		}
	}
	if (executeFlag)
	{
		if (IdFlag)
		{
			if (CheckInVector(inf.functions, (const char*)word) || lexema == 'l');
			else if (inf.flagInFunc)
			{
				strcat(word, "_");
				strcat(word, inf.prefix);
			}

			indexIT = IT::IsId(tables.idTable, word);
			if (indexIT != TI_NULLIDX && inf.iddatatype != IT::IDDATATYPE::NODEF && inf.idtype != IT::IDTYPE::L && inf.idtype != IT::P) throw Error::geterrorin(123, line, posword); // повторна€ инициализаци€
			else if (indexIT == TI_NULLIDX) // добавл€ем в таблицу индентификаторов
			{
				if (inf.iddatatype == IT::IDDATATYPE::NODEF) throw Error::geterrorin(122, line, posword); // использование без объ€влени€
				indexIT = tables.idTable.size;
				IT::Entry IdTableEntry = { tables.LexTable.size, word, inf.iddatatype, inf.idtype , 0 };
				memcpy(&IdTableEntry.value, &value, sizeof(Value));
				IT::Add(tables.idTable, IdTableEntry);
				inf.iddatatype = IT::IDDATATYPE::NODEF;
				inf.idtype = IT::IDTYPE::V;
			}
			else
			{
				inf.iddatatype = IT::IDDATATYPE::NODEF;
				inf.idtype = IT::IDTYPE::V;
			}
		}
		LT::Add(tables.LexTable, { lexema, line, tables.LexTable.size, indexIT, word[0], posword });
		//inf.idtype = IT::IDTYPE::V;

	}
	else throw Error::geterrorin(113, line, posword);
}

void LA::GetInf(LA::Inf& inf, char* word)
{
	if (strcmp(word, "int") == 0) inf.iddatatype = IT::IDDATATYPE::INT;
	else if (strcmp(word, "string") == 0) inf.iddatatype = IT::IDDATATYPE::STR;
	else if (strcmp(word, "bool") == 0)
		inf.iddatatype = IT::IDDATATYPE::BOOL;
	else if (strcmp(word, "Func") == 0)
	{
		inf.idtype = IT::IDTYPE::F;
		inf.flagFunc = true;
	}
	else if (strcmp(word, "(") == 0 && inf.flagFunc)
	{
		inf.flagParam = true;
		inf.flagInFunc = true;
	}
	else if (strcmp(word, ")") == 0 && inf.flagFunc)
	{
		inf.flagParam = false;
		inf.flagFunc = false;
	}
	else if (inf.idtype == IT::IDTYPE::F || strcmp(word, "start") == 0)
	{
		for (int i = 0; i <= strlen(word); i++) inf.prefix[i] = word[i];
		if (strcmp(word, "start") != 0) inf.functions.push_back((const char*)word);
	}
	else if (inf.flagParam) inf.idtype = IT::IDTYPE::P;
	else if (strcmp(word, "{") == 0) inf.flagInFunc = true;
	else if (strcmp(word, "}") == 0)
	{
		inf.prefix = NULL;
		inf.prefix = new char(256);
		inf.flagInFunc = false;
	}
}

LA::Tables LA::Lex_analyz(In::IN in) {


	LA::Tables tables;
	tables.LexTable = LT::Create();
	tables.idTable = IT::Create();
	LA::Inf inf;
	inf.functions.push_back("strlen");
	inf.functions.push_back("strcmp");
	inf.functions.push_back("strcpy");
	inf.functions.push_back("strcat");
	IT::Entry IdTableEntry = { -1, (char*)"strlen", IT::IDDATATYPE::INT, IT::IDTYPE::F , 0 };
	IT::Add(tables.idTable, IdTableEntry);
	IdTableEntry = { -1, (char*)"strcmp", IT::IDDATATYPE::BOOL, IT::IDTYPE::F , 0 };
	IT::Add(tables.idTable, IdTableEntry);
	IdTableEntry = { -1, (char*)"strcpy", IT::IDDATATYPE::STR, IT::IDTYPE::F , true, 0 };
	IT::Add(tables.idTable, IdTableEntry);
	IdTableEntry = { -1, (char*)"strcat", IT::IDDATATYPE::STR, IT::IDTYPE::F , true, 0 };
	IT::Add(tables.idTable, IdTableEntry);

	int i = 0; // индекс по in.text
	int wordIndex = 0;
	int posWord = 0; // позици€ слова в строке
	int line = 1; // текуща€ строка
	char* word = new char[256];

	if (in.text == (unsigned char*)' ') i++;
	while (i < in.size)
	{
		if (in.text[i] == '#')
		{
			while (in.text[i] != '\n')
				i++;
		}
		else if (in.text[i] == '\'')
		{
			while (true)
			{
				word[wordIndex++] = in.text[i++];
				posWord++;
				if (in.text[i] == '\n')
				{
					line++;
					posWord = 0;
				}
				if (in.text[i] == '\'' || in.text[i] == '\0') break;
			}
			word[wordIndex++] = in.text[i++];
			word[wordIndex] = '\0';
			GetInf(inf, word);
			InTables(tables, posWord, line, word, inf);
			word = NULL;
			word = new char[256];
			wordIndex = 0;
		}
		else if (in.code[in.text[i]] == in.T)
		{
			while (in.code[in.text[i]] == in.T)
			{
				word[wordIndex++] = in.text[i++];
				posWord++;
			}
			if (in.code[in.text[i]] == in.S || in.code[in.text[i]] == in.P || in.code[in.text[i]] == in.Q)
			{
				word[wordIndex] = '\0';
				wordIndex = 0;
				GetInf(inf, word);
				InTables(tables, posWord, line, word, inf);
				word = NULL;
				word = new char[256];
			}
		}
		else if (in.code[in.text[i]] == in.S)
		{
			word[wordIndex++] = in.text[i++];
			word[wordIndex] = '\0';
			GetInf(inf, word);
			InTables(tables, posWord, line, word, inf);
			posWord++;
			wordIndex = 0;
			word = NULL;
			word = new char[256];
		}
		else if (in.code[in.text[i]] == in.P)
		{
			if (in.text[i] == IN_CODE_ENDL)
			{
				line++;
				posWord = 0;
			}
			wordIndex = 0;
			i++;
		}
	}
	return tables;
}