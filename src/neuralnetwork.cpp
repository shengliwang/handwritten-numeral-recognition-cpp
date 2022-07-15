#include <iostream>

#include "matrix.hpp"
#include "neuralnetwork.hpp"
#include "myutils.hpp"



Network::Network(unsigned int layer_num, unsigned int * layer_node_num)
{
	_resources_malloced = false;
	if (layer_num > MAX_LAYER_NUM)
	{
		std::cout << "layer_num is " << layer_num << "max=" << MAX_LAYER_NUM << std::endl;
		return ;
	}

	if (NULL == layer_node_num) {std::cout << "layer node num is null" << std::endl; return;}


	_layer_num = layer_num;

	/* 申请权重矩阵, 权重矩阵的数目应该是层数减一 */
	for (int i = 0; i < _layer_num - 1; ++i)
	{
		_weights[i] = new Matrix(layer_node_num[i], layer_node_num[i+1]);
		if (NULL == _weights[i]){return;}
		_weights[i]->random_init(myrand_double);
	}

	/* 申请每层的输入输出信号(一维矩阵) */
	for (int i =0 ; i < _layer_num; ++i)
	{
		_layer_signal_inputs[i] = new Matrix(layer_node_num[i], 1);
		if (NULL == _layer_signal_inputs[i]) {return;}
		_layer_signal_outputs[i] = new Matrix(layer_node_num[i], 1);
		if (NULL == _layer_signal_outputs[i]) {return;}
	}

	/*申请每层的误差值(一维矩阵)*/
	for (int i = 0; i < _layer_num-1; ++i)
	{
		_layer_errors[i] = new Matrix(layer_node_num[i], 1);
		if (NULL == _layer_errors[i]) {return;}
	}

	/*申请目标值矩阵*/
	_target_output = new Matrix(layer_node_num[layer_num-1], 1);
	if (NULL == _target_output) {return;}

	_resources_malloced = true;
}

Network::~Network()
{
	if (false == _resources_malloced)
	{
		return;
	}

	for (int i = 0; i < _layer_num - 1; ++i)
	{
		delete _weights[i];
	}

	for (int i =0 ; i < _layer_num; ++i)
	{
		delete _layer_signal_inputs[i]; 
		delete _layer_signal_outputs[i];
	}

	/*申请每层的误差值(一维矩阵)*/
	for (int i = 0; i < _layer_num-1; ++i)
	{
		delete _layer_errors[i]; 
	}

	/*申请目标值矩阵*/
	delete _target_output;

	_resources_malloced = false;
}

void Network::printConfig(bool verbose)
{
	printf("=====================================\n");
	printf("network config:");
	std::cout << "learning rate: " << _lr << std::endl;
	std::cout << "total layer num: " << _layer_num << std::endl;
	for (int i = 0; i < _layer_num; ++i)
	{
		std::cout << "nodes num of layer" << i << ": " << getLayerNodeNum(i) << std::endl;;
	}

	if (verbose)
	{
		for (int i = 0; i < _layer_num -1 ; ++i)
		{
			std::cout << "weigts matrix " << i << " : " << std::endl;
			_weights[i]->print();
		}
	}

	printf("=====================================\n");
}


bool Network::ok()
{
	return _resources_malloced;
}


unsigned int Network::getLayerNodeNum(int layer_id)
{
	CHECK_PARAM_RET(layer_id < MAX_LAYER_NUM, 0);
	return _layer_signal_inputs[layer_id]->getRowNum();
}	
