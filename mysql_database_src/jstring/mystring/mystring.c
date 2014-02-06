#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>

wchar_t* string_create(int id, wchar_t *question, wchar_t* A, wchar_t* B, wchar_t *C, wchar_t *D, wchar_t *correct, wchar_t *path)
{
	//look for the length of the id 
//	char charid[3];
//	itoa (id, charid, 10);//convert id to the char

	//finding the length of the string needed
//	int length = 26+wcslen(question)+wcslen(A)+wcslen(B)+wcslen(C)+wcslen(D)+wcslen(correct)+wcslen(path);
	
	//create a string with memory allocation
	wchar_t* string;
	string = (wchar_t*) malloc (600);

	//concatenate all objects required
	swprintf(string, 600, L"{'%d','%s','%s','%s','%s','%s','%s','%s'}", id, question, A, B, C, D, correct, path);
	return string;
}

int main()
{
	 wchar_t *question = string_create(1, "question", "A", "B", "C", "D", "correct", "path");
        setlocale(LC_ALL, "");
        wprintf(L"%ls\n", question);

	return 0;
}	
