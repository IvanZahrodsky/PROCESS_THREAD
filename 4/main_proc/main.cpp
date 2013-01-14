#include <iostream>
#include <Windows.h>

using namespace std;
typedef unsigned char UC;
#define COUNT_PROC 3
int main ( int argc , char *argv[] )
{

	STARTUPINFO si[COUNT_PROC];
	PROCESS_INFORMATION pi[COUNT_PROC];
	HANDLE hMap;
	LPVOID lpFileM;
	HANDLE hEvent;
	hEvent = CreateEvent ( NULL, FALSE, FALSE, L"@#$%^&*EVENT)(*&^%$" );
	if(hEvent == NULL)
	{
		cout << "CreateEvent : Error!\n";
		return 1;
	}
	hMap = CreateFileMapping ( (HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1, L"#$%IVAN_FILE#@$" ); 
	if (hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		cout << "CreateFileMapping : Error!";
		CloseHandle(hMap); 
		hMap = NULL; 
		return 1;
	} 
	lpFileM = MapViewOfFile (hMap,FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if(lpFileM == 0)
	{
		cout << "MapViewOfFile: Error!";
		return 1;
	}
	* (UC*) lpFileM = 0;
//	* (((UC*) lpFileM )+1) = 1;
	ZeroMemory ( &si[0], sizeof ( STARTUPINFO ) );
	ZeroMemory ( &si[1], sizeof ( STARTUPINFO ) );
	ZeroMemory ( &si[2], sizeof ( STARTUPINFO ) );
	ZeroMemory ( &pi[0], sizeof ( PROCESS_INFORMATION ) );
	ZeroMemory ( &pi[1], sizeof ( PROCESS_INFORMATION ) );
	ZeroMemory ( &pi[2], sizeof ( PROCESS_INFORMATION ) );
	
	if ( !CreateProcess ( L"Task_4.exe", L"1", NULL, NULL, FALSE, 0, NULL, NULL, &si[0], &pi[0] ) ) {
		cout << "CreateProcess: Error!";
		CloseHandle ( pi[0].hProcess );
		CloseHandle	( pi[0].hThread );
		return 1;
	}
	if ( !CreateProcess ( L"Task_4.exe", L"2", NULL, NULL, FALSE, 0, NULL, NULL, &si[1], &pi[1] ) ) {
		cout << "CreateProcess: Error!";
		CloseHandle ( pi[1].hProcess );
		CloseHandle	( pi[1].hThread );
		return 1;
	}
	if ( !CreateProcess ( L"Task_4.exe", L"3", NULL, NULL, FALSE, 0, NULL, NULL, &si[2], &pi[2] ) ) {
		cout << "CreateProcess: Error!";
		CloseHandle ( pi[2].hProcess );
		CloseHandle	( pi[2].hThread );
		return 1;
	}
	HANDLE hProc[COUNT_PROC];
	for ( register int i = 0; i < COUNT_PROC; ++i )
	{
		hProc[i] = pi[i].hProcess;
	}
	SetEvent ( hEvent );
	WaitForMultipleObjects ( COUNT_PROC, hProc, false,  INFINITE );
	UnmapViewOfFile ( lpFileM );
	CloseHandle ( hMap );
	
	return 0;
}