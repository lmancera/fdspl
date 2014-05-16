#include "stdafx.h"
#include "LinearAlgebraTests.h"
#include "MatrixTests.h"

#include <iostream>
using namespace std;

LinearAlgebraTests::LinearAlgebraTests(void) {
}

LinearAlgebraTests::~LinearAlgebraTests(void) {
}

int LinearAlgebraTests::doTests(){
	UnitTest * tests = new MatrixTests;
	if (!(doTestsFor(tests))) return error(tests);
	//if (!(doTestsFor(new MatrixTests, tests))) return error(tests);
	return successful(tests);
}

int LinearAlgebraTests::error(UnitTest * tests)
{
	delete tests;
	return 0;
}

int LinearAlgebraTests::successful(UnitTest * tests)
{
	int numTestsRun = tests->testsRun();
	delete tests;
	return numTestsRun;
}

int LinearAlgebraTests::doTestsFor(UnitTest * tests) {
	return doTestsFor(tests, 1);
}

int LinearAlgebraTests::doTestsFor(UnitTest * tests, UnitTest * previousTests) {
	int status = doTestsFor(tests, previousTests->testsRun());
	previousTests->testsRun(tests->testsRun());
	delete tests;
	return status;
}

int LinearAlgebraTests::doTestsFor(UnitTest * tests, int testCounter){
	tests->testsRun(testCounter);
	if (!tests->check()) return 0;
	else return 1;
}