// Dsp4msTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "LinearAlgebraTests.h"

#include <conio.h>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	LinearAlgebraTests tests;
	tests.check();
	_getch();
	return 0;
}

