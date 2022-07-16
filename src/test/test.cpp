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
	m.print();
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
	A.print();
	A.transPose()->print();

	Matrix B(3,4);
	double b[12] = {1,2,3,4,4,2,1,3,4,5,1,2};
	B.setArray(b);
	B.print();
	B.transPose()->print();

	Matrix C(2,4);

	matrix_dot(&C, &A, &B);
	C.print();

	matrix_map(&C, &C, sigmoid_function);
	C.print();
}

void test_sigmoid_func(void)
{
	double x = 16;
	printf("sigmoid(%f) = %.16f\n", x, sigmoid_function(x));
}

int main(void)
{
	//test_matrix();
	//test_network();
	test_matrix_dot();
//	test_sigmoid_func();

	return 0;
}
