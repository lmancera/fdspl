#pragma once

#include <vector>
#include <algorithm>
using namespace std;

namespace dsp4ms
{
	enum concat2 { NEWROWS, NEWCOLUMNS };
	
	#define PRODUCT_OF_MATRICES_DIMENSION_MISMATCH "Number of columns of the multiplicand should match number of rows of the multiplier";
	#define ALL_COLUMNS_REQUIRE_SAME_SIZE "All column vectors are required to be the same size";
	#define INDEX_OUT_OF_BOUNDS "Index out of bounds";
	#define INPUT_VECTOR_DIMENSION_MISMATCH "Input vector dimension mismatch"

	template <typename T, size_t _rows, size_t _columns, size_t foo=0>
	class TemplatedMatrix
	{	
		class ColumnIterator
		{
		private:
			size_t _index;
			size_t _rows;
			size_t _columns;
			size_t _total;
			size_t _offset;

			ColumnIterator() {};

			void next(void)
			{
				_index += _columns;
				if (_index > _total)
				{
					_offset = (_offset == _columns-1) ? 0 : _offset+1;
					_index = _offset;
				}
			}

		public:
			ColumnIterator(size_t rows, size_t columns)
			{
				_rows = rows;
				_columns = columns;
				_total = (rows*columns)-1;
				_offset = 0;
				_index = 0;
			}

			void operator=(size_t index)
			{
				_index = index;
				_offset = _index/_columns;
			}

			size_t operator()(void)
			{
				return _index;
			}
						
			size_t& operator++(void)
			{
				next();
				return _index;
			}

			size_t operator++(int value)
			{
				next();
				return value;
			}

			size_t current(void)
			{
				return _index;
			}
		};

	private:
				
		std::vector<T> _matrix;
		
		size_t toIdx(size_t row, size_t column)
		{
			return row*_columns + column;
		}

		std::vector<T> asVectorConcatRows(void) const
		{
			return _matrix;
		}

		std::vector<T> asVectorConcatColumns(void) const
		{
			std::vector<T> byColumns(_matrix.size(),0);
			ColumnIterator j(_rows, _columns);
			j = 0;
			for (size_t i = 0; i != _matrix.size(); i++)
			{
				byColumns[i] = _matrix[j.current()];
				j++;
			}
			return byColumns;
		}

		TemplatedMatrix(void) {} // Note: Inmutable

	public:
		
		TemplatedMatrix(std::vector<T> values)
		{
			if (values.size() != _rows*_columns)
				throw INPUT_VECTOR_DIMENSION_MISMATCH;
			_matrix.swap(values);
		}

		TemplatedMatrix(T (&input)[_rows][_columns])
		{
			_matrix = std::vector<T>(_rows*_columns, 0);
			size_t k = 0;
			for (size_t rowIdx = 0; rowIdx != _rows; rowIdx++)
			{
				for (size_t colIdx = 0; colIdx != _columns; colIdx++)
				{
					_matrix[k++] = input[rowIdx][colIdx];
				}
			}
		}

		~TemplatedMatrix(void) {}

		size_t rows(void) const
		{
			return _rows;
		}

		size_t columns(void) const
		{
			return _columns;
		}

		T at(size_t row, size_t column)
		{
			return _matrix.at(toIdx(row, column));
		}

		std::vector<T> operator[](size_t row)
		{
			if ((row < 0) || (row >= _rows)) 
				throw INDEX_OUT_OF_BOUNDS;
			std::vector<T> rowCrop(_columns, 0);
			size_t idxBegin = row*_columns;
			size_t idxEnd = (row+1)*_columns;
			std::copy(_matrix.begin() + idxBegin, _matrix.begin() + idxEnd, rowCrop.begin());
			return rowCrop;
		}

		std::vector<T> operator()(concat2 concatType) const
		{
			if (concatType == NEWROWS) return asVectorConcatRows();
			else return asVectorConcatColumns();
		}

		TemplatedMatrix<T, _rows, _columns> operator+(const TemplatedMatrix<T, _rows, _columns> &rhs)
		{
			std::vector<T> toAdd = rhs(NEWROWS);
			std::transform(_matrix.begin(), _matrix.end(), toAdd.begin(), toAdd.begin(), plus<double>());
			return TemplatedMatrix<T, _rows, _columns>(toAdd);
		}


		template <size_t rows, size_t columns> void operator=(Matrix<T> rhs)
		{
			_rows = rows;
			_columns = columns;
			_matrix = rhs(NEWROWS);
		}
		
	};

};