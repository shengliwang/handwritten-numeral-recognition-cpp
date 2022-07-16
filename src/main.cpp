#include <stdio.h>
#include <iostream>

#include "neuralnetwork.hpp"
#include "matrix.hpp"
#include "myutils.hpp"


/*
	函数说明参见 neuralnetwork.hpp
	note: 所有的输入均为一维列向量。即 nx1的矩阵

*/
Matrix * sigmoid_gradient_descend_update_weight_func(
double lr, unsigned int rows, unsigned int cols,
const Matrix * cl_input, const Matrix * cl_output, const Matrix * cl_error,
const Matrix * nl_input, const Matrix * nl_output, const Matrix * nl_error
)
{
	/* 参数检查 下一层节点个数应与矩阵的行数相等 */
	if (nl_output->getRowNum() != rows)
	{
		std::cout << __FUNCTION__ << " not equal" << std::endl;
		return NULL;
	}

	/* 参数检查, 当前层的节点个数应与矩阵的列相等 */
	if (cl_output->getRowNum() != cols)
	{
		std::cout << __FUNCTION__ << " not equal" << std::endl;
		return NULL;
	}

	if (nl_output->getRowNum() != nl_error->getRowNum())
	{
		std::cout << __FUNCTION__ << "error" << std::endl;
		return NULL;
	}

	Matrix *A = new Matrix(rows, 1);

	double * arr1 = A->getMutableArray();
	const double * arr2 = nl_error->getArray();
	const double * arr3 = nl_output->getArray();

	for (int i = 0; i < A->getRowNum(); ++i)
	{
		arr1[i] = arr2[i] * arr3[i] * (1 - arr3[i]) * lr;
	}

	Matrix *B = cl_output->transPose();

	Matrix *C = new Matrix(rows, cols);

	if (0 != matrix_dot(C, A, B))
	{
		std::cout << __FUNCTION__ << "error, matrix dot failed\n" << std::endl;
		delete A;
		delete B;
		delete C;
		return NULL;
	}

	delete A;
	delete B;

	return C;
}

int main(int argc, char * argv[])
{
	printf("hello\n");
}
