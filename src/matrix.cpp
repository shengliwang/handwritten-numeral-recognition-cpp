#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#include "matrix.hpp"

using namespace std;


Matrix::Matrix(int row_num, int col_num)
{
	_total_size = row_num * col_num;
	_rows = row_num;
	_cols = col_num;

	_array = (double*)malloc(_total_size*sizeof(double));
	if (NULL == _array)
	{
		_rows = _cols = 0;
		std::cout << "matrix malloc failed" << std::endl;
	}
}

Matrix::~Matrix()
{
	if (NULL != _array)
	{
		free(_array);
	}
}

void Matrix::random_init(random_func_t f)
{
	if (NULL != f)
	{
		for (int i = 0; i < _total_size; ++i)
		{
			_array[i] = f();
		}
	}
}

void Matrix::print()
{
	if (NULL != _array)
	{
		//printf("Arrary%dx%d: [\n", _rows, _cols);
		std::cout << _rows << "x" << _cols << ": [" << std::endl;
		for (int row = 0; row < _rows; ++row)
		{
			std::cout << row << ": [";
			int col;
			for (col = 0; col < _cols-1; ++col)
			{
				std::cout << _array[row*_cols + col] << ", ";
			}
			std::cout << _array[row*_cols + col] << "]" << std::endl;
		}

		std::cout << "]" << std::endl;
		
	}
	else
	{
		std::cout << "matrix Null" << std::endl;
	}
}
