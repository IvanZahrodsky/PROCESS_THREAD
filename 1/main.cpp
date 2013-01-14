#include <iostream>
#include <Windows.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>


using namespace std;

int current_th;
vector<int> vec;

HANDLE hEvnt;

DWORD WINAPI Thread ( void *p )
{
	int ThNum = *(int*)p;
	do
	{
		WaitForSingleObject ( hEvnt, INFINITE );
		if ( vec.at ( current_th ) == ThNum )
		{
			break;
		}
		SetEvent ( hEvnt );
	} while ( 1 );
	
	cout << current_th << " : Thread - " << ThNum << endl;
	++current_th;
	Sleep(2000);
	SetEvent ( hEvnt );
	return 0;
}

int main ( int argc, char *argv[] )
{
	current_th=0;
	srand ( (unsigned int ) time (NULL) );
	int temp;
	while ( vec.size() != 1000 )
	{
		temp = rand() % 1000;
		if ( find ( vec.begin(), vec.end(), temp ) == vec.end() )
		{
			vec.push_back(temp);
		}
	}
	FILE *fl;
	fl = fopen ("File.txt", "w" );
	if (fl != NULL)
	{
		for (int i=0;i<1000;++i)
		{
			fprintf(fl,"%d  - %d\n",i,vec.at(i));
		}
		fclose (fl);
	}
	hEvnt = CreateEvent ( NULL, false, FALSE, L"EVENTIVAN#$@$#@FDS#$@");
	if(hEvnt == NULL)
	{
		cout << "CreateEvent : Error!\n";
		return 1;
	}
	
	HANDLE hTh[1000];
	DWORD dwTh[1000];
	for ( int i=0; i < 1000; ++i )
	{
		hTh[i] = CreateThread (NULL, NULL, &Thread, &vec.at(i), 0, &dwTh[i] );
		if (hTh[i] == NULL)
		{
			cout << "CreateTHread: Error! \n";
		}

	}
	
	SetEvent ( hEvnt );
	//WaitForMultipleObjects ( 1000 , hTh, TRUE, INFINITE );
	while ( current_th < 1000 )
		Sleep ( 10000 );
	for ( int i=0; i < 1000; ++i )
	{
		CloseHandle ( hTh[i] );
	}
	CloseHandle ( hEvnt );
	return 0;
}