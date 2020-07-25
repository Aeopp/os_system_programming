﻿#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

#define NUM_OF_GATE 6
using uint32 = unsigned int;
LONG gTotalCount = 0;

HANDLE hMutex;

void IncreaseCount()
{
	WaitForSingleObject(hMutex,INFINITE);

	gTotalCount++;

	ReleaseMutex(hMutex);
};

uint32 WINAPI ThreadProc(LPVOID lpParam)
{
	for (DWORD i = 0; i < 1000; ++i)
	{
		IncreaseCount();
	}
	return 0;
}

int _tmain(int argc, TCHAR** argv)
{
	DWORD dwThreadID[NUM_OF_GATE];
	HANDLE hThread[NUM_OF_GATE];

	// InitializeCriticalSection(&hCriticalSection);

	hMutex = 
		CreateMutex(
		NULL,
		FALSE,
		NULL);

	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		hThread[i] = (HANDLE)
			_beginthreadex(
				NULL, 0, ThreadProc,
				NULL, CREATE_SUSPENDED,
				(unsigned*)&dwThreadID[i]);

		if (hThread[i] == NULL)
		{
			_tprintf(_T("Thread creation fault! \n"));
			return -1; 
		}
	}

	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		ResumeThread(hThread[i]);
	}

	WaitForMultipleObjects(
		NUM_OF_GATE, hThread, TRUE, INFINITE);

	_tprintf(_T("total count : %d \n"), gTotalCount);

	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		CloseHandle(hThread[i]);
	}

	//DeleteCriticalSection(&hCriticalSection);
	CloseHandle(hMutex);

	return 0; 
};


