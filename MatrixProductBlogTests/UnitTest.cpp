#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "UnitTest.h"

#define ERROR_MESSAGE "Tests failed!";
#define SUCCESS_MESSAGE "All tests successful!";
#define SUCCESS_LBL "success";
#define FAIL_LBL "failed";

UnitTest::UnitTest(void) {
	testCounter = 1;
	errorColour = 12;
	successColour = 10;
	normalColour = 7;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

UnitTest::~UnitTest(void)
{
}

void  UnitTest::setErrorOutputColour(void) {
	SetConsoleTextAttribute(hConsole, errorColour);
}

void  UnitTest::setSuccessOutputColour(void) {
	SetConsoleTextAttribute(hConsole, successColour);
}

void  UnitTest::setNormalOutputColour(void) {
	SetConsoleTextAttribute(hConsole, normalColour);
}

void UnitTest::showError(){
	setErrorOutputColour();
	cout << ERROR_MESSAGE;
	cout << "\n";
	setNormalOutputColour();
}

void UnitTest::showSuccess(){
	setSuccessOutputColour();
	cout << SUCCESS_MESSAGE;
	cout << "\n";
	setNormalOutputColour();
}

void UnitTest::printStatus(string label, bool cond){
	if (cond)
	{
		setSuccessOutputColour();
		cout << testCounter << ": " << label << " -> " << SUCCESS_LBL;
	}
	else
	{
		setErrorOutputColour();
		cout << testCounter << ": " << label << " -> " << FAIL_LBL;
	}
	setNormalOutputColour();
	cout << "\n";
}

void UnitTest::assert(string label, bool cond){
	printStatus(label, cond);
	testCounter++;
}

void UnitTest::areEqual(string label, int value1, int value2){
	bool cond = value1 == value2;
	std::ostringstream oss;
	oss << label << ": " << value1;
	if (cond) oss << " == ";
	else oss << " != ";
	oss << value2;
	assert(oss.str(), cond);
}


int UnitTest::doTests(){
	// Should be overriden by heirs
	return 0;
}

void UnitTest::testsRun(int anInteger) {
	testCounter = anInteger;
}

int UnitTest::testsRun() {
	return testCounter;
}

int UnitTest::check(){
	int result = doTests();
	if (result) showSuccess();
	else showError();
	return result;
}