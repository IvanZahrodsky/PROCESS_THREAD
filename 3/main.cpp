#include <iostream>
#include <Windows.h>

using namespace std;
#define INCREMENT1 4
#define INCREMENT2 3

#define LIMIT 43

#define IFPRINT 6

volatile int q_thrd;
int value;
HANDLE hEvn;

DWORD WINAPI ThreadIncrement ( void* p )
{
	int t1;
	int t2;
	t1 = * ( (int*) p + 1) ;
	t2 = * ( (int*) p ) ;
	do
	{
		WaitForSingleObject ( hEvn, INFINITE );
		if ( q_thrd == t2 )
		{
			value += t1;
			cout << "Thread  " << (t2 == 0 ? t2 + 1 : t2 )  << " : Increment for  " << t1 << " new value - " << value << endl;
			Sleep(1000);
			++q_thrd;
			q_thrd &= 3;
		}
		SetEvent ( hEvn );
		//Sleep(0);
	} while( 1 );
	return 0;
}

DWORD WINAPI ThreadPrint ( void* p )
{
	do
	{
		WaitForSingleObject ( hEvn, INFINITE );
		if ( q_thrd == 1 )
		{
			cout << "Thread  3 :   "; 
			if ( value % IFPRINT == 0 )
			{
				cout << "Value - " << value ;
			}
			cout << endl;
			Sleep(1000);
			++q_thrd;
			q_thrd &= 3;
		}
		SetEvent ( hEvn );
		//Sleep(0);
	} while( 1 );
	return 0;
}

DWORD WINAPI ThreadReset ( void* p )
{
	do
	{
		WaitForSingleObject ( hEvn, INFINITE );
		if ( q_thrd == 3 )
		{
			cout << "Thread  4 :   "; 
			if ( value > LIMIT )
			{
				value = 0;
				cout << "Reset! Value - 0";
			}
			cout << endl;
			Sleep(1000);
			++q_thrd;
			q_thrd &= 3;
		}
		SetEvent ( hEvn );
		//Sleep(0);
	} while( 1 );
	return 0;
}
int main ( int argc, char *argv[] )
{
	hEvn = CreateEvent(NULL, false, false , L"@#$%TFRS#$W$" );
	if(hEvn == NULL)
	{
		cout << "CreateEvent : Error!\n";
		return 1;
	}
	q_thrd = 0;
	value = 0;
	HANDLE hThrd[4];
	DWORD dwThreadId[4];
	long long temp1 = 0;
	long long temp2 = 0;
	temp1 = INCREMENT1;
	temp1 = ( temp1 << 32 );
	temp2 = INCREMENT2;
	temp2 = ( temp2 << 32 ) | 2;
	hThrd[0] = CreateThread (NULL, NULL, &ThreadIncrement, &temp1, 0, &dwThreadId[0] );
	if (hThrd[0] == NULL)
	{
		cout << "CreateTHread: Error! \n";
		return 1;
	}
	hThrd[1] = CreateThread (NULL, NULL, &ThreadIncrement, &temp2, 0, &dwThreadId[1] );
	if (hThrd[1] == NULL)
	{
		cout << "CreateTHread: Error! \n";
		return 1;
	}
	hThrd[2] = CreateThread (NULL, NULL, &ThreadPrint, NULL, 0, &dwThreadId[2] );
	if (hThrd[2] == NULL)
	{
		cout << "CreateTHread: Error! \n";
		return 1;
	}
	hThrd[3] = CreateThread (NULL, NULL, &ThreadReset, NULL, 0, &dwThreadId[3] );
	if (hThrd[3] == NULL)
	{
		cout << "CreateTHread: Error! \n";
		return 1;
	}
	SetEvent ( hEvn );
	WaitForMultipleObjects ( 4, hThrd, true, INFINITE );
	return 0;
}
