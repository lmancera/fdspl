#pragma once
#include "UnitTest.h"

class LinearAlgebraTests: public UnitTest 
{
protected:
	int doTests();
	int doTestsFor(UnitTest*);
	int doTestsFor(UnitTest*,UnitTest*);
	int doTestsFor(UnitTest*, int);
	int error(UnitTest*);
	int successful(UnitTest*);
public:
	LinearAlgebraTests(void);
	~LinearAlgebraTests(void);
};
