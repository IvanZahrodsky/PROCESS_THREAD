#include <iostream>
#include <Windows.h>

using namespace std;
typedef unsigned char UC;

#define TO_INT(X) (int) ( * ( UC* ) X )
#define PINT(X)   ( * ( UC* ) X )
int main ( int argc , char *argv[] )
{
	int iProcNum = atoi ( argv[0] );
	HANDLE hEvent;
	hEvent = OpenEvent( EVENT_ALL_ACCESS, FALSE, L"@#$%^&*EVENT)(*&^%$" );
	if(hEvent == NULL)
	{
		cout << "OpenEvent: Error ! Process -  " << iProcNum << endl;
		return 1;
	}
	LPVOID lpFileM;
	HANDLE hMap = OpenFileMapping ( FILE_MAP_READ | FILE_MAP_WRITE, FALSE , L"#$%IVAN_FILE#@$" );
	
	if ( hMap == NULL )
	{
		cout << "OpenFileMapping: Error ! Process -  " << iProcNum << endl;
		return 1;
	}
	lpFileM = MapViewOfFile (hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if(lpFileM == 0)
	{
		cout << "MapViewOfFile: Error ! Process -  " << iProcNum << endl;
		return 1;
	}
	do
	{
		WaitForSingleObject ( hEvent, INFINITE );
		if ( TO_INT(lpFileM) % 3 == iProcNum-1 )
		{
			system ( "cls" );
			++PINT(lpFileM);
			cout << TO_INT (lpFileM) << " - Process " << iProcNum;
			Sleep(1000);
		}
		SetEvent ( hEvent );
	} while ( 1 );
	return 0;
}