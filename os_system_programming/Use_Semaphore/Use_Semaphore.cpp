#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <Windows.h>
#include <process.h>
#include <array>


inline namespace
{
	using uint32 = unsigned int;

	static constexpr uint32 NUM_OF_CUSTOMER{ 50 };
	static constexpr uint32 RANGE_MIN{ 10 };
	static constexpr uint32 RANGE_MAX{ 30 - RANGE_MIN };
	static constexpr uint32 TABLE_CNT{ 10 };

	static HANDLE hSemaphore;
	static std::array<DWORD, NUM_OF_CUSTOMER> randTimeArr;
};

void TakeMeal(const DWORD time)
{
	WaitForSingleObject(hSemaphore, INFINITE);

	_tprintf(_T("Enter Customer %d~ \n"), GetCurrentThreadId());

	_tprintf(_T("Customer %d having launch~ \n"), GetCurrentThreadId());

	Sleep(1000 * time);

	ReleaseSemaphore(hSemaphore, 1, NULL);

	_tprintf(_T("Out Customer %d~ \n\n"), GetCurrentThreadId());
};

uint32 WINAPI ThreadProc(LPVOID lpParam)
{
	TakeMeal(reinterpret_cast<DWORD>(lpParam));

	return 0;
};

int _tmain(int argc, TCHAR* argv[])
{
	std::array<DWORD, NUM_OF_CUSTOMER> dwThreadIDs;
	std::array<HANDLE, NUM_OF_CUSTOMER> hThreads;

	srand((uint32)time(NULL));

	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		randTimeArr[i] = (DWORD)(
			((double)rand() / (double)RAND_MAX)
			* RANGE_MAX + RANGE_MIN);
	};

	hSemaphore = CreateSemaphore
	(
		NULL,
		TABLE_CNT,
		TABLE_CNT,
		NULL);

	if (hSemaphore == NULL)
	{
		_tprintf(_T("CreateSemaphore error : %d\n"), GetLastError());
	};

	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		hThreads[i] = (HANDLE)
			_beginthreadex(
				NULL, 0, ThreadProc, (void*)randTimeArr[i],
				CREATE_SUSPENDED,
				(unsigned*)&dwThreadIDs[i]);

		if (hThreads[i] == NULL)
		{
			_tprintf(_T("Thread creation fault! \n"));
			return -1; 
		}
	};

	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		ResumeThread(hThreads[i]);
	};

	WaitForMultipleObjects(
		NUM_OF_CUSTOMER, hThreads.data(), TRUE, INFINITE);

	_tprintf(_T("=============END=================\n"));

	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		CloseHandle(hThreads[i]);
	};

	CloseHandle(hSemaphore);
	return 0;
};















