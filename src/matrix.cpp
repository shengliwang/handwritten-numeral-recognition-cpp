#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>


#include "matrix.hpp"

using namespace std;


Matrix::Matrix(int row_num, int col_num)
{
	_rows = row_num;
	_cols = col_num;
	_total_size = row_num * col_num;

	if (0 == _total_size)
	{
		_array = NULL;
		return;
	}

	_array = (double*)malloc(_total_size*sizeof(double));
	if (NULL == _array)
	{
		_rows = _cols = 0;
		std::cout << "matrix malloc failed" << std::endl;
	}
}

Matrix::~Matrix()
{
//	std::cout << "~Matrix called" << std::endl;
//	print();
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

void Matrix::dump(void) const
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

unsigned int Matrix::getRowNum(void) const
{
	return _rows;
}

unsigned int Matrix::getColNum(void) const
{
	return _cols;
}




int matrix_dot(Matrix * dst, const Matrix * src1, const Matrix * src2)
{
	if (src1->_cols != src2->_rows)
	{
		std::cout << "function matrixdot row of src1->!= col of src2->" << std::endl;
		return 1;
	}

	if (dst->_rows != src1->_rows || dst->_cols != src2->_cols)
	{
		std::cout << "function matridotdst->_rows != src1->_rows || dst->_cols != src2->_cols" << std::endl;
		return 1;
	}

	for (int row = 0; row < dst->_rows; ++row)
	{
		for (int col = 0; col < dst->_cols; ++col)
		{
			double tmp = 0;
			for (int i = 0; i < src1->_cols; ++i)
			{
				tmp += src1->_array[row*src1->_cols + i] * src2->_array[i*src2->_cols + col];
			}

			dst->_array[row * dst->_cols + col] = tmp;
		}
	}

	return 0;
}
int matrix_map(Matrix * dst, const Matrix * src, map_func_t func)
{
	if (NULL == func)
	{
		printf("func is null\n");
		return 1;
	}

	if (dst->_cols != src->_cols || dst->_rows != src->_rows)
	{
		std::cout << __FUNCTION__ << "not equal" <<  std::endl;
		return 1;
	}

	for (int i = 0; i < src->_total_size; ++i)
	{
		dst->_array[i] = func(src->_array[i]);
	}


	return 0;
}


void Matrix::setArray(double * arr)
{
	for (int i = 0; i < _total_size; ++i)
	{
		_array[i] = arr[i];
	}
	//memcpy(_array, arr, sizeof(double)*_total_size);
}

int matrix_sub(Matrix * dst, const Matrix * A, const Matrix * B)
{
	if (A->_cols != B->_cols || A->_rows != B->_rows)
	{
		std::cout << __FUNCTION__ << "not equal" << std::endl;
		return 1;
	}

	if (A->_cols != dst->_cols || A->_rows != dst->_rows)
	{
		std::cout << __FUNCTION__ << "not equal" << std::endl;
		return 1;
	}



	for (int i = 0; i < dst->_total_size; ++i)
	{
		dst->_array[i] = A->_array[i] - B->_array[i];
	}
	
	return 0;
}

int matrix_add(Matrix * dst, const Matrix * A, const Matrix * B)
{
	if (A->_cols != B->_cols || A->_rows != B->_rows)
	{
		std::cout << __FUNCTION__ << "not equal" << std::endl;
		return 1;
	}

	if (A->_cols != dst->_cols || A->_rows != dst->_rows)
	{
		std::cout << __FUNCTION__ << "not equal" << std::endl;
		return 1;
	}



	for (int i = 0; i < dst->_total_size; ++i)
	{
		dst->_array[i] = A->_array[i] + B->_array[i];
	}
	
	return 0;
}

Matrix * Matrix::transPose(void) const
{
	Matrix * dst = new Matrix(this->_cols, this->_rows);

	if (NULL == dst)
	{
		std::cout << __FUNCTION__ << "NULL" << std::endl;
		return NULL;
	}

	for (int row = 0; row < dst->_rows; ++row)
	{
		for (int col = 0; col < dst->_cols; ++col)
		{
			dst->_array[row*dst->_cols + col] = this->_array[col*this->_cols + row];
		}
	}

	return dst;
}

const double * Matrix::getArray(void) const
{
	return _array;
}

double * Matrix::getMutableArray(void)
{
	return _array;
}
