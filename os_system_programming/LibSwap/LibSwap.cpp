#define _CRT_SECURE_NO_WARNING

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
//__declspec(dllimport)
// C++ 컴파일러의 네임 맹글링 규칙을 적용하여서 머신코드로 삽입되어 있다.
// C 컴파일러에 의해 만들어진 머신코드는 당연히 네임 맹글링 규칙이 적용되어 있지 않다.
// 컴파일 이후 링커가 LibSwap.obj 에 swap 함수를 호출하는 머신코드에서 swap 를 런타임에 
// DLL 에서 링킹하면 된다고 알고 있지만 네임 맹글링된 C++ swap 함수를 당연히 찾을수 없기 때문이다.
extern "C" __declspec(dllimport) void swap(int*, int*);

typedef void (*SWAP_FUNC)(int*, int*);


int _tmain(int argc, TCHAR* argv[])
{
	HMODULE hinstLib;
	SWAP_FUNC SwapFunction;

	hinstLib = LoadLibrary(_T("SwapStaticLib"));

	if (hinstLib == NULL)
	{
		_tprintf(_T("LoadLibrary fail! \n"));
		return -1;
	}
	SwapFunction =
		(SWAP_FUNC)GetProcAddress(hinstLib, "swap");

	if (SwapFunction == NULL)
	{
		_tprintf(_T("GetProcAddress fail! \n"));
		return -1;
	}

	int a = 10;
	int b = 20;
	_tprintf(_T("Before: %d %d \n"), a, b);

	SwapFunction(&a, &b);
	_tprintf(_T("After: %d, %d \n"), a, b);

	BOOL isSuccess = FreeLibrary(hinstLib);
	if (isSuccess == NULL)
	{
		_tprintf(_T("Free Library fail!! \n"));
		return -1;
	}

	return 0;

}


