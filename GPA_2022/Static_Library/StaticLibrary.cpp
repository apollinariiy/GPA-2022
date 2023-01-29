#include <iostream>
#include <Windows.h>
#include <string>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(suppress : 4996)

using namespace std;

extern "C" 
{
	int Strlen(char* str)
	{
		return strlen(str);
	}
	void ConsoleInt(int a)
	{
		std::cout << a << std::endl;
	}
	void ConsoleStr(char* ptr)
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);

		if (ptr == nullptr)
		{
			std::cout << std::endl;
			return;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
		{
			std::cout << ptr[i];
		}
		std::cout << std::endl;
	}
	void ConsoleBool(int a)
	{
		if (a == 1)
			std::cout << "true" << std::endl;
		else
			std::cout << "false" << std::endl;
	}
	bool Strcmp(char* str1, char* str2)
	{
		int i = NULL, len1 = NULL, len2 = NULL;
		for (; str1[len1] != '\0'; len1++);
		for (; str2[len2] != '\0'; len2++);
		int length = len1 ? len1 > len2 : len2;
		for (int k = 0; i < length; k++, i++)
		{
			if (str1[k] != str2[i])
			{
				if (str1[k] > str2[i] || str1[k] < str2[i])
					return false;
			}
		}
		return true;
	}
	void consolpause()
	{
		std::cout << "\n";
		system("pause");
	}
	char* Strcpy(char* buffer, char* str1, char* str2)
	{
		int i = NULL;
		for (int j = 0; str1[j] != '\0'; j++)
		{
			if (i == 255)
				break;
			buffer[i++] = str1[j];
		}
		buffer[i] = '\0';
		return buffer;

	}
	char* Strcat(char* buffer, char* str1, char* str2)
	{
		int i = NULL, len1 = NULL, len2 = NULL;

		for (; str1[len1] != '\0'; len1++);
		for (; str2[len2] != '\0'; len2++);

		for (int j = 0; str2[j] != '\0'; j++)
		{
			if (i == 255) break;
			buffer[i++] = str2[j];
		}
		for (int j = 0; str1[j] != '\0'; j++)
		{
			if (i == 255) break;
			buffer[i++] = str1[j];
		}
		buffer[i] = '\0';
		return buffer;

	}
}