#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

using uint32 = unsigned int;
uint32 WINAPI OutputThreadFunction(LPVOID lpParam);

TCHAR string[100];
HANDLE hEvent;

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hThread;
	DWORD dwThreadID;

	hEvent = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		NULL
	);

	if (hEvent == NULL)
	{
		_fputts(_T("Event object creation error \n"), stdout);
	}

	hThread = (HANDLE)_beginthreadex
	(
		NULL, 0, OutputThreadFunction,
		NULL, 0, (uint32*)&dwThreadID);

	if (hThread == 0)
	{
		_fputts(_T("Thread creation error \n"), stdout);
		return -1;
	}

	_fputts(_T("Insert string: "), stdout);
	_fgetts(string, 30, stdin);

	SetEvent(hEvent);

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hEvent);
	CloseHandle(hThread);

	return 0;
};

uint32 WINAPI OutputThreadFunction(LPVOID lpParam)
{
	WaitForSingleObject(hEvent, INFINITE);
	
	_fputts(_T("Output string : "), stdout);
	_fputts(string, stdout);

	return 0; 
}