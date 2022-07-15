#include <iostream>

#include "matrix.hpp" 
#include "myutils.hpp"
#include "neuralnetwork.hpp"


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

int main(void)
{
	//test_matrix();
	test_network();

	return 0;
}
