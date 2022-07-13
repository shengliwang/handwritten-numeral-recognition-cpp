#include <iostream>

#include "matrix.hpp" 
#include "myutils.hpp"



void test_matrix(void)
{
	Matrix m(10, 10);
	m.random_init(myrand_double);
	m.print();
}

int main(void)
{
	test_matrix();

	return 0;
}
