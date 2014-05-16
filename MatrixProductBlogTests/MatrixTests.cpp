#include "stdafx.h"
#include "MatrixTests.h"
#include "Matrix.h"
#include "MatrixReduced.h"
#include "TemplatedMatrix.h"

using namespace dsp4ms;
using namespace fdslp;

MatrixTests::MatrixTests(void)
{
}

MatrixTests::~MatrixTests(void)
{
}

int MatrixTests::doTests(void)
{
	typedef double dataType;		
	dataType input[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
	const size_t rows = sizeof(input)/sizeof(input[0]);
	const size_t columns = sizeof(input[0])/sizeof(input[0][0]);	
	
	dsp4ms::Matrix<dataType> A(input);
	fdslp::MatrixReduced Ar(input);
	
	std::string label = "Matrix has appropriate number of rows";
	bool cond1 = Ar.rows() == rows;
	UnitTest::assert(label, cond1);

	label = "Matrix has appropriate number of columns";
	bool cond2 = Ar.columns() == columns;
	UnitTest::assert(label, cond2);

	label = "Matrix has appropriate number of elements";
	bool cond3 = Ar.size() == rows*columns;
	UnitTest::assert(label, cond3);

	// TODO: Test matrices with different element type

	label = "Matrix - test subscriptor";
	bool cond4 = A.at(0,0) == 1;
	cond3 &= A.at(1,2) == 7;
	cond3 &= A.at(2,3) == 12;
	UnitTest::assert(label, cond4);

	label = "Matrix - test == operator";
	dsp4ms::Matrix<dataType> B(input);
	bool cond5 = A == B;
	UnitTest::assert(label, cond5);

	label = "Matrix - test != operator";
	input[1][2] = 13;
	dsp4ms::Matrix<dataType> C(input);
	bool cond6 = A != C;
	UnitTest::assert(label, cond6);

	label = "Matrix - test != operator";
	dataType differentRows[2][4] = {{1,2,3,4},{5,6,7,8}};
	dsp4ms::Matrix<dataType> D(differentRows);
	bool cond7 = A != D;
	UnitTest::assert(label, cond7);

	label = "Matrix - test != operator";
	dataType differentColumns[3][1] = {{1},{5},{9}};
	dsp4ms::Matrix<dataType> E(differentColumns);
	bool cond8 = A != E;
	UnitTest::assert(label, cond8);

	label = "Matrix - test = operator";
	// TODO: Vector transpose
	dataType anotherInput[4][3] = {{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
	dsp4ms::Matrix<dataType> At(anotherInput);
	dsp4ms::Matrix<dataType> F = A * At;
	bool cond9 = F.rows() == A.rows();
	cond9 = F.columns() == At.columns();
	UnitTest::assert(label, cond9);
	UnitTest::areEqual(label, F.rows(), A.rows());
	UnitTest::areEqual(label, F.columns(), At.columns());

	label = "Matrix - test [] operator";
	std::vector<dataType> rowCrop = F[0];
	bool cond10 = rowCrop.size() == F.columns();
	cond10 &= rowCrop[0] == 70;
	cond10 &= rowCrop[1] == 80;
	cond10 &= rowCrop[2] == 90;
	UnitTest::assert(label, cond10);

	label = "Matrix - test * operator";
	bool cond11 = F[0][0] == 70;
	cond11 &= F[0][1] == 80;
	cond11 &= F[0][2] == 90;
	cond11 &= F[1][0] == 158;
	cond11 &= F[1][1] == 184;
	cond11 &= F[1][2] == 210;
	cond11 &= F[2][0] == 246;
	cond11 &= F[2][1] == 288;
	cond11 &= F[2][2] == 330;
	UnitTest::assert(label, cond11);

	label = "Matrix - test () operator";
	std::vector<dataType> asVector = D(ROWS);
	bool cond12 = asVector.size() == 8;
	cond12 &= asVector[0] == 1;
	cond12 &= asVector[1] == 2;
	cond12 &= asVector[2] == 3;
	cond12 &= asVector[3] == 4;
	cond12 &= asVector[4] == 5;
	cond12 &= asVector[5] == 6;
	cond12 &= asVector[6] == 7;
	cond12 &= asVector[7] == 8;
	asVector = D(COLUMNS);
	cond12 &= asVector.size() == 8;
	cond12 &= asVector[0] == 1;
	cond12 &= asVector[1] == 5;
	cond12 &= asVector[2] == 2;
	cond12 &= asVector[3] == 6;
	cond12 &= asVector[4] == 3;
	cond12&= asVector[5] == 7;
	cond12 &= asVector[6] == 4;
	cond12 &= asVector[7] == 8;
	UnitTest::assert(label, cond12);
		
	label = "TemplatedMatrix - test + operator";
	const size_t newRows = 2;
	const size_t newColumns = 2;
	dataType newInput[newRows][newColumns] = {{1,2},{5,6}};
	dataType newInput2[newRows][newColumns] = {{3,4},{7,8}};
	dsp4ms::TemplatedMatrix<dataType, newRows, newColumns> newA(newInput);
	dsp4ms::TemplatedMatrix<dataType, newRows, newColumns> newB(newInput2);
	dsp4ms::TemplatedMatrix<dataType, newRows, newColumns> newC = newA + newB;
	bool cond13 = newC.rows() == newRows;
	cond13 &= newC.columns() == newColumns;
	if (cond13)
		for (size_t i = 0; i != newC.rows(); i++)
			for (size_t j = 0; j != newC.columns(); j++)
				cond13 &= newC[i][j] == newA[i][j] + newB[i][j];
	UnitTest::assert(label, cond13);

	return cond1 & cond2 & cond3 & cond4 & cond5 & cond6 & cond7
		& cond8 & cond9 & cond10 & cond11 & cond12 & cond13;
}