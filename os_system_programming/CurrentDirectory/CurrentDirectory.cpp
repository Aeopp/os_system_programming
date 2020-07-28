#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LEN 100
TCHAR fileData[] = _T("abcdefghijklmnopqrstuvwxyz");

int _tmain(int argc, TCHAR* argv[])
{
	SetCurrentDirectory(_T("C:\\"));


	TCHAR fileName[] = _T("abcd.dat");
	TCHAR readBuf[STRING_LEN];

	HANDLE hFileWrite;
	HANDLE hFileRead;
	DWORD numOfByteWritten;
	
	hFileWrite = CreateFile(
		fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);

	WriteFile(hFileWrite, fileData, sizeof(fileData),
		&numOfByteWritten, NULL);

	CloseHandle(hFileWrite);

	hFileRead = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (hFileRead == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File open Error ! \n"));
		return -1; 
	}

	ReadFile(
		hFileRead, readBuf, sizeof(readBuf),
		&numOfByteWritten, NULL);

	_tprintf(_T("%s \n"), readBuf);
	CloseHandle(hFileRead);

	return 0;
}