#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"Calculator.lib")
#include <stdio.h>
#include <tchar.h>
#include "Calculator.h"


int _tmain(int argc, TCHAR* argv[])
{
	_tprintf(_T("result Add: %d \n"), Add(5, 3));
	_tprintf(_T("result Min: %d \n"), Min(5, 3));
	_tprintf(_T("result Mul: %e \n"), Mul(5.0, 3.0));
	_tprintf(_T("result Div: %e \n"), Div(5.0, 3.0));

	return 0; 
}