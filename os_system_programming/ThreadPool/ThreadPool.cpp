#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define WORK_MAX 10000
#define THREAD_MAX 50

using WORK = void(*)(void);

DWORD AddWorkToPool(WORK work);
WORK GetWorkFromPool(void);
DWORD MakeThreadPool(DWORD nunOfThread);
void WorkerThreadFunction(LPVOID pParam);

struct WorkerThread
{
	HANDLE hThread;
	DWORD idThread;
};

struct ThreadPool
{
	WORK workList[WORK_MAX];

	WorkerThread workerThreadList[THREAD_MAX];
	HANDLE workerEventList[THREAD_MAX];

	DWORD idxOfCurrentWork;
	DWORD idxOfLastAddedWork;

	DWORD threadIdx;
};
ThreadPool gThreadpool;

static HANDLE mutex = NULL;

void InitMutex(void)
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}

void DeInitMutex(void)
{
	BOOL ret = CloseHandle(mutex);
}

void AcquireMutex(void)
{
	DWORD ret = WaitForSingleObject(mutex, INFINITE);

	if (ret == WAIT_FAILED)
		_tprintf(_T("Error Occur! \n"));
}

void ReleaseMutex(void)
{
	BOOL ret = ReleaseMutex(mutex);
	if (ret == 0)
		_tprintf(_T("Error Occur! \n"));
}

DWORD AddWorkToPool(WORK work)
{
	AcquireMutex();

	if (gThreadpool.idxOfLastAddedWork >= WORK_MAX)
	{
		_tprintf(_T("AddWorkTopool fail! \n"));
		return NULL;
	}

	gThreadpool.workList[gThreadpool.idxOfLastAddedWork++] = work;

	for (DWORD i = 0; i < gThreadpool.threadIdx; ++i)
	{
		SetEvent(gThreadpool.workerEventList[i]);
	}

	ReleaseMutex();
	return 1;
};

WORK GetWorkFromPool(void)
{
	WORK work = NULL;
	AcquireMutex();

	if (!(gThreadpool.idxOfCurrentWork < gThreadpool.idxOfLastAddedWork))
	{
		ReleaseMutex();
		return NULL;
	};

	work = gThreadpool.workList[gThreadpool.idxOfCurrentWork++];
	ReleaseMutex();

	return work;
};

DWORD MakeThreadPool(DWORD numOfThread)
{
	InitMutex();
	DWORD capacity = WORK_MAX - (gThreadpool.threadIdx);

	if (capacity < numOfThread)
		numOfThread = capacity;

	for (DWORD i = 0; i < numOfThread; ++i)
	{
		DWORD idThread;
		HANDLE hThread;

		gThreadpool.workerEventList[gThreadpool.threadIdx] = CreateEvent(NULL, FALSE, FALSE, NULL);

		hThread = CreateThread(
			NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadFunction, (LPVOID)gThreadpool.threadIdx,
			0, &idThread);

		gThreadpool.workerThreadList[gThreadpool.threadIdx].hThread = hThread;

		gThreadpool.workerThreadList[gThreadpool.threadIdx].idThread = idThread;

		++gThreadpool.threadIdx;
	};
	return numOfThread;
};

void WorkerThreadFunction(LPVOID pParam)
{
	WORK workFunction;
	HANDLE event = gThreadpool.workerEventList[(DWORD)pParam];

	while (true)
	{
		workFunction = GetWorkFromPool();

		if (workFunction == NULL)
		{
			WaitForSingleObject(event, INFINITE);
			continue;
		}

		workFunction();
	};
};

void TestFunction()
{
	static int i = 0;
	++i;

	_tprintf(
		_T("Good Test --%d : Processing thread: %d--\n\n"),
		i, GetCurrentThreadId()
	);
};

int _tmain(int argc, TCHAR* argv[])
{
	MakeThreadPool(10);
	Sleep(10000);

	for (int i = 0; i < 100; ++i)
	{
		AddWorkToPool(TestFunction);
	}

	Sleep(50000);
	return 0; 

}
