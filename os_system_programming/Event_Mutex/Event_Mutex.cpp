
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

using uint32 = unsigned int;

uint32 WINAPI OutputThreadFunction(LPVOID lpParam);
uint32 WINAPI CountThreadFucntion(LPVOID lpParam);

struct _SynchString
{
	TCHAR string[100];
	HANDLE hEvent;
	HANDLE hMutex;
}; using SynchString = _SynchString;

SynchString gSynString;

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hThreads[2];
	DWORD dwThreadIDs[2];

	gSynString.hEvent = CreateEvent(
		NULL, TRUE, FALSE, NULL);

	gSynString.hMutex = CreateMutex(
		NULL, FALSE, NULL);

	if (gSynString.hEvent == NULL || gSynString.hMutex == NULL)
	{
		_fputts(_T("kernel object creation error \n"), stdout);
		return -1;
	};

	hThreads[0] = (HANDLE)_beginthreadex(
		NULL, 0, OutputThreadFunction,
		NULL, 0,
		(uint32*)&dwThreadIDs[0]);

	hThreads[1] = (HANDLE)_beginthreadex(
		NULL, 0, CountThreadFucntion, NULL, 0, (uint32*)&dwThreadIDs[1]);

	if (hThreads[0] == 0 || hThreads[1] == 0)
	{
		_fputts(_T("Thread creation error \n"), stdout);
		return -1;
	}

	_fputts(_T("Insert string: "), stdout);
	_fgetts(gSynString.string, 30, stdin);

	SetEvent(gSynString.hEvent);

	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	CloseHandle(gSynString.hEvent);
	CloseHandle(gSynString.hMutex);
	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);

	return 0;
};

uint32 WINAPI OutputThreadFunction(LPVOID lpParam)
{
	WaitForSingleObject(gSynString.hEvent, INFINITE);
	WaitForSingleObject(gSynString.hMutex, INFINITE);

	_fputts(_T("Output string : "), stdout);
	_fputts(gSynString.string, stdout);

	ReleaseMutex(gSynString.hMutex);
	return 0;
};

uint32 WINAPI CountThreadFucntion(LPVOID lpParam)
{
	WaitForSingleObject(gSynString.hEvent, INFINITE);
	WaitForSingleObject(gSynString.hMutex, INFINITE);

	_tprintf(
		_T("output string length : %d \n"),
		_tcslen(gSynString.string) - 1);

	ReleaseMutex(gSynString.hMutex);
	return 0; 
};





