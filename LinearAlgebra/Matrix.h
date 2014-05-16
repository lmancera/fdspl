#pragma once

#include <vector>
#include <algorithm>

#include <iostream>

namespace dsp4ms
{
	enum concat { ROWS, COLUMNS };

	#define PRODUCT_OF_MATRICES_DIMENSION_MISMATCH "Number of columns of the multiplicand should match number of rows of the multiplier";
	#define ALL_COLUMNS_REQUIRE_SAME_SIZE "All column vectors are required to be the same size";
	#define INDEX_OUT_OF_BOUNDS "Index out of bounds";

	template <typename T>
	class Matrix
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
		size_t _rows;
		size_t _columns;
		
		size_t toIdx(size_t row, size_t column)
		{
			return row*_columns + column;
		}

		std::vector<T> asVectorConcatRows(void)
		{
			return _matrix;
		}

		std::vector<T> asVectorConcatColumns(void)
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

		Matrix(void) {} // Note: Inmutable

		Matrix(std::vector<T> values, size_t rows, size_t columns)
		{
			_matrix.swap(values);
			_rows = rows;
			_columns = columns;
		}

	public:
		
		Matrix(std::vector<vector<T>> values)
		{
			size_t rows = values.size();
			size_t columns = values[0].size();
			std::vector<T> concatenated;
			for (size_t i = 0; i != values.size(); i++)
			{
				if (values[i].size() != columns) 
					throw ALL_COLUMNS_REQUIRE_SAME_SIZE
				concatenated.insert(concatenated.end(), values[i].begin(), values[i].end());
			}
			this(concatenated, rows, columns);
		}

		template <size_t rows, size_t columns> Matrix(T (&input)[rows][columns])
		{
			_matrix = std::vector<T>(rows*columns, 0);
			size_t k = 0;
			for (size_t rowIdx = 0; rowIdx != rows; rowIdx++)
			{
				for (size_t colIdx = 0; colIdx != columns; colIdx++)
				{
					_matrix[k++] = input[rowIdx][colIdx];
				}
			}
			_rows = rows;
			_columns = columns;
		}

		~Matrix(void) {}

		size_t rows(void) const
		{
			return _rows;
		}

		size_t columns(void) const
		{
			return _columns;
		}

		size_t size(void) const
		{
			return _matrix.size();
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

		std::vector<T> operator()(concat concatType)
		{
			if (concatType == ROWS) return asVectorConcatRows();
			else return asVectorConcatColumns();
		}

		bool operator==(Matrix<T> rhs)
		{
			if (rows() != rhs.rows()) return false;
			if (columns() != rhs.columns()) return false;
			std::vector<T> toCompare = rhs(ROWS);
			return std::equal(_matrix.begin(), _matrix.end(), toCompare.begin());
		}

		bool operator!=(Matrix<T> rhs)
		{
			return !(operator==(rhs));
		}

		void operator=(Matrix<T> rhs)
		{
			_rows = rhs.rows();
			_columns = rhs.columns();
			_matrix = rhs(ROWS);
		}

		Matrix<T> operator*(Matrix<T> rhs)
		{
			if (_columns != rhs.rows()) 
				throw PRODUCT_OF_MATRICES_DIMENSION_MISMATCH;
			size_t rows = _rows;
			size_t columns = rhs.columns();
			std::vector<T> multiplicand = rhs(ROWS);
			size_t productSize = rows*columns;
			std::vector<T> product(productSize, 0);

			for (size_t k = 0; k != productSize; k++)
			{
				product[k] = 0;
				for (size_t idx = 0; idx != _columns; idx++)
				{
					size_t i = ((k/_rows) * _columns) + idx;
					size_t j = (rows*idx) + (k%rows);
					product[k] += _matrix[i] * multiplicand[j];
				}
			}
			return Matrix<T>(product, rows, columns);
		}
		
	};

};