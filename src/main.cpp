#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string.h>

#include "neuralnetwork.hpp"
#include "matrix.hpp"
#include "myutils.hpp"
static Matrix * sigmoid_gradient_descend_update_weight_func(
double lr, unsigned int rows, unsigned int cols,
const Matrix * cl_input, const Matrix * cl_output, const Matrix * cl_error,
const Matrix * nl_input, const Matrix * nl_output, const Matrix * nl_error
);
static double sigmoid_function(double x);

void split(char *src,const char *separator,char **dest,int *num)
{
     char *pNext;
     //记录分隔符数量 
     int count = 0;
     //原字符串为空 
     if (src == NULL || strlen(src) == 0)
        return;
    //未输入分隔符
     if (separator == NULL || strlen(separator) == 0)
        return;   
    char *strtok(char *str, const char *delim); 
    //获得第一个由分隔符分割的字符串 
    pNext = strtok(src,separator);
    while(pNext != NULL) {
        //存入到目的字符串数组中 
        *dest++ = pNext; 
        ++count;
        pNext = strtok(NULL,separator);  
    }  
    *num = count;
}

char single_line[1024*1024];

int main(int argc, char * argv[])
{
	clock_t begin = clock();
	int layer_num = 3;
	unsigned int layer_node_num[] = {784, 600, 10}; /*输入层 784个节点，隐藏层，200个，输出层10个节点*/
	double lr = 0.02; /*学习率*/

	Network net(layer_num, layer_node_num);
	net.setLearningRate(lr);

	/*下面两个函数指针的设置是相关的，若激活函数是其他的，则应设置相应的用于更新权重的函数*/
	net.setActivationFunc(sigmoid_function); /* 激活函数使用 sigmoid 函数*/
	net.setGenDeltaWeightFunc(sigmoid_gradient_descend_update_weight_func); /*使用梯度下降发更新权重*/

	if (net.ok())
	{
		std::cout << "network config ok." << std::endl;
		net.printConfig(false);
	}
	else
	{
		std::cout << "network config faild." << std::endl;
		return 1;
	}

	/*准备训练集和测试集*/
    FILE * train_fp = fopen("../mnist_dataset/mnist_train_100.csv", "r");
   // FILE * train_fp = fopen("../mnist_dataset/mnist_train.csv", "r");
    if (NULL == train_fp)
    {
        fprintf(stderr, "open failed\n");
        return 1;
    }
    FILE * test_fp = fopen("../mnist_dataset/mnist_test_10.csv", "r");
    //FILE * test_fp = fopen("../mnist_dataset/mnist_test.csv", "r");
    if (NULL == test_fp)
    {
        fclose(train_fp);
        fprintf(stderr, "open failed\n");
        return 1;
    }

    char *tmp[1000];
    int num = 0;        /*should be 785 */
    double * input_data = (double *)malloc(sizeof(double)*784);   /*大小跟输入神经元节点数相同*/
    double * target_data = (double *)malloc(sizeof(double)*10);/*大小跟输出神经元节点数相同*/
    if ((NULL == input_data) || (NULL == input_data))
    {
        fprintf(stderr, "((NULL == input_data) || (NULL == input_data))\n");
    }
    int line = 1;

    int target_num; /*记录当前图片真是的数字值*/

    /*开始训练*/
    while(NULL != fgets(single_line, sizeof(single_line), train_fp))
    {
        split(single_line, ",", tmp, &num);
        if (785 != num)
        {
            fprintf(stderr, "(785 != num)\n");
            break;
        }

        /*处理target*/
        target_num = atoi(tmp[0]);
        for (int i = 0; i < 10; ++i)
        {
            if (i == target_num)
            {
                target_data[i] = 0.99;
            }else{
                target_data[i] = 0.01;
            }
        }

        /*处理输入 input*/
        for (int i = 1; i < 784 + 1; ++i)
        {
            input_data[i-1] = atoi(tmp[i]) / 255.0 * 0.99 + 0.01;
        }

        if (0 != net.trainOneStep(input_data, 784, target_data, 10))
        {
            fprintf(stderr, "network_train failed\n");
        }

		//net.printDebugInfo();

		if (line % 100 == 0)
		{
        	printf("train %d\n", line);
		}
        line++;
		//return 0;
    }

	/*查询结果*/
    line = 1;
    double * result = (double *)malloc(sizeof(double)*10);/*大小跟输出神经元节点数相同*/
    int total = 0; int right_count = 0;
    while(NULL != fgets(single_line, sizeof(single_line), test_fp))
    {
        split(single_line, ",", tmp, &num);
        if (785 != num)
        {
            fprintf(stderr, "(785 != num)\n");
            break;
        }

        /*处理输入 input*/
        for (int i = 1; i < 784 + 1; ++i)
        {
            input_data[i-1] = atoi(tmp[i]) / 255.0 * 0.99 + 0.01;
        }
        if (0 != net.query(input_data, result))
        {
            fprintf(stderr, "network_query failed\n");
        }

        ++total;
        int max = 0;
        for (int i = 0; i < 10; ++i)
        {
            if (result[max] < result[i])
            {
                max = i;
            }
        }

        if (max == atoi(tmp[0]))
        {
            printf("\033[32m");/*绿*/
            ++right_count;
        }
        else
        {
            printf("\033[31m"); /*红*/
        }

        printf("result=[");

        for (int i = 0; i < 10; ++i)
        {
            printf("%f ", result[i]);
            if (result[max] < result[i])
            {
                max = i;
            }
        }
        printf("]. result=%d, which should be %d.\n\033[00m", max, atoi(tmp[0]));


    }

    clock_t end = clock();
	double time_consumption = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("total=%d, right=%d [%f%%]. Using %f s\n", total, right_count, right_count*1.0/total*100, time_consumption);


	
	#if 0
	/*读取测试文件*/
	std::ifstream trainfile;
	trainfile.open("../mnist_dataset/mnist_train_100.csv", std::ios::in);

	if (!trainfile)
	{
		std::cout << "open train file failed" << std::endl;
		return 1;
	}

	std::string line;
	int linenum = 0;

	while(getline(trainfile, line))
	{
		std::cout << linenum ++<< "[" << line << "]" << std::endl;
	}
	#endif



}

/*
	函数说明参见 neuralnetwork.hpp
	note: 所有的输入均为一维列向量。即 nx1的矩阵

*/
static Matrix * sigmoid_gradient_descend_update_weight_func(
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

static double sigmoid_function(double x)
{
    return 1 / (1 + exp(-x));
}

