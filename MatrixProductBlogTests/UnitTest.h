#pragma once
#include <string>
#include <Windows.h>
#include "conio.h"
using namespace std;

class UnitTest
{
private:
	int errorColour;
	int successColour;
	int normalColour;
	HANDLE hConsole;
	void setErrorOutputColour(void);
	void setSuccessOutputColour(void);
	void setNormalOutputColour(void);
protected:
	virtual int doTests();
	void printStatus(string,bool);
	void assert(string,bool);
	void areEqual(string, int, int);
	int testCounter;
public:
	UnitTest(void);
	~UnitTest(void);
	void testsRun(int);
	int testsRun();
	int check();
	void showError();
	void showSuccess();
};