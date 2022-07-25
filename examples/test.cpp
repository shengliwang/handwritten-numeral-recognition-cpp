#include <iostream>
#include <math.h>

#include "matrix.hpp" 
#include "myutils.hpp"
#include "neuralnetwork.hpp"


double sigmoid_function(double x)
{
    return 1 / (1 + exp(-x));
}

void test_matrix(void)
{
	Matrix m(10, 10);
	m.random_init(myrand_double);
	m.dump();
}

/*测试神经网络*/
void test_network(void)
{
	unsigned int layer_num = 4;
	unsigned int layer_node_num[4] = {3, 4, 5, 2};

	/*创建4层的神经网络，每层的神经节点个数为3,4,5,2*/
	/* 其中输入层节点个数为3,输出层为2,隐藏层有两层，节点个数分别为4,5*/
	Network net(layer_num, layer_node_num);
	net.setLr(0.02); /*设置学习率*/
	
	if (net.ok())
	{
		std::cout << "net init ok" <<std::endl;
		net.printConfig(true);
	}
	else
	{
		std::cout << "net init failed" <<std::endl;
	}
}

void test_matrix_dot(void)
{
	Matrix A(2,3);
	double a[6] = {1,2,3,4,5,6};
	A.setArray(a);
	A.dump();
	A.transPose()->dump();

	Matrix B(3,4);
	double b[12] = {1,2,3,4,4,2,1,3,4,5,1,2};
	B.setArray(b);
	B.dump();
	B.transPose()->dump();

	Matrix C(2,4);

	matrix_dot(&C, &A, &B);
	C.dump();

	matrix_map(&C, &C, sigmoid_function);
	C.dump();
}

void test_sigmoid_func(void)
{
	double x = 16;
	printf("sigmoid(%f) = %.16f\n", x, sigmoid_function(x));
}

/*
	函数说明参见 neuralnetwork.hpp
	note: 所有的输入均为一维列向量。即 nx1的矩阵

*/
static Matrix * update_func(
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
void test_network2(void)
{
	int layer_num = 3;
	unsigned int layer_node_num[] = {2, 3, 2}; /*输入层 784个节点，隐藏层，200个，输出层10个节点*/
	double lr = 0.2; /*学习率*/

	Network net(layer_num, layer_node_num);
	net.setLearningRate(lr);

	/*下面两个函数指针的设置是相关的，若激活函数是其他的，则应设置相应的用于更新权重的函数*/
	net.setActivationFunc(sigmoid_function); /* 激活函数使用 sigmoid 函数*/
	net.setGenDeltaWeightFunc(update_func); /*使用梯度下降发更新权重*/

	double w0[] = {0.141418, 0.217921, 
					0.0104187, 0.518317,
					0.248873, 0.685384};
	double w1[] = {0.346809, 0.221923, 0.312742, 
				0.125819, 0.650231, 0.390969};

	net.getWeight(0,1)->setArray(w0);
	net.getWeight(1,2)->setArray(w1);

	if (net.ok())
	{
		std::cout << "network config ok." << std::endl;
		net.printConfig(true);
	}
	else
	{
		std::cout << "network config faild." << std::endl;
		return ;
	}

	double train_data[] = {0.5, 0.4};
	double target_data[] = {0.01, 0.09};

	if ( 0 != net.trainOneStep(train_data, 2, target_data, 2))
	{
		std::cout << "train failed" << std::endl;
	}

	net.printDebugInfo();
}

int main(void)
{
	//test_matrix();
	//test_network();
	//test_matrix_dot();
//	test_sigmoid_func();
	
	test_network2();

	return 0;
}
