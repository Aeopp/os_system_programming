#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BEST_TEST_EXCPETION ((DWORD)0xE0000008L)

void SoftwareException(void);

int _tmain(int argc, _TCHAR* argv[])
{
	SoftwareException();
	_tprintf(_T("End of the _tmain \n"));
	return 0; 
}

void SoftwareException(void)
{
	__try
	{
		RaiseException(BEST_TEST_EXCPETION, 0, NULL, NULL);
		_tprintf(_T("It's work! \n"));
	}
	__except (EXCEPTION_NONCONTINUABLE)
	{
		DWORD exptType = GetExceptionCode();

		if (exptType == BEST_TEST_EXCPETION)
		{
			_tprintf(_T("BEST_TEST_EXCPETION excpetion occurred\n"));

		}
	}
}