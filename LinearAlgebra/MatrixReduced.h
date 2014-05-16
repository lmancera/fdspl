#pragma once

namespace fdslp
{
	class MatrixReduced
	{	
	private:
		std::vector<double> _matrix;
		size_t _rows;
		size_t _columns;

	public:
		template <size_t rows, size_t columns> 
		MatrixReduced(double (&input)[rows][columns])
		{
			_matrix = std::vector<double>(rows*columns, 0);
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
			
		~MatrixReduced() 
		{ 
		}

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

	};

};