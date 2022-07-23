#include <iostream>
#include <string.h>


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

	_activation_func = NULL;
	_genDeltaWeight_func = NULL;

	if (NULL == layer_node_num) {std::cout << "layer node num is null" << std::endl; return;}


	_layer_num = layer_num;

	/* 申请权重矩阵, 权重矩阵的数目应该是层数减一 */
	for (int i = 0; i < _layer_num - 1; ++i)
	{
		/* 权重矩阵形状，比如前一层节点数为5,后一层节点数为3,则权重矩阵的形状为3x5*/
		_weights[i] = new Matrix(layer_node_num[i+1], layer_node_num[i]);
		if (NULL == _weights[i]){return;}
		_weights[i]->random_init(myrand_double); /*使用-0.5~0.5之间的随机数*/
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
	for (int i = 0; i < _layer_num; ++i)
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
		std::cout << "nodes num of layer" << i << ": " << _getLayerNodeNum(i) << std::endl;;
	}
	printf("activation function: %p\n", _activation_func);
	printf("update weight function: %p\n", _genDeltaWeight_func);

	if (verbose)
	{
		for (int i = 0; i < _layer_num -1 ; ++i)
		{
			std::cout << "weigts matrix " << i << " : " << std::endl;
			_weights[i]->dump();
		}
	}

	printf("=====================================\n");
}


bool Network::ok()
{
	return _resources_malloced;
}

int Network::trainOneStep(double * train_data, unsigned int data_num, 
						double * target_data, unsigned int target_num)
{
	/* 输入信号前向传播 */
	if (0 != _forward_propagation(train_data, data_num))
	{
		return 1;
	}
	
	/* 误差反向传播 */
	if (0 != _backward_propagation(target_data, target_num))
	{
		return 1;
	}

	/* 更新权重 */
	if (0 != _update_weights())
	{
		return 1;
	}

	return 0;
}

int Network::query(double * input_data, double *result)
{
	if ( 0 != _forward_propagation(input_data, _getLayerNodeNum(0)))
	{
		std::cout << "query failed" <<std::endl;
		return 1;
	}

	Matrix * output = _getLayerOutputSignal(_layer_num - 1);

	for (int i = 0; i < output->getRowNum(); ++i)
	{
		result[i] = output->getArray()[i];
	}

	return 0;
}


unsigned int Network::_getLayerNodeNum(int layer_id)
{
	CHECK_PARAM_RET(layer_id < MAX_LAYER_NUM, 0);
	return _layer_signal_inputs[layer_id]->getRowNum();
}	

int Network::_forward_propagation(double * train_data, unsigned int data_num)
{
	/*保存输入矩阵*/
	Matrix * inputs = _getLayerInputSignal(0);
	if (data_num != inputs->getRowNum())
	{
		std::cout << __FUNCTION__ << "not equal" << std::endl;
		return 1;
	}

	inputs->setArray(train_data);
	/*设置第一层的信号输出，这里直接把输入作为输出*/
	Matrix * first_layer_outputs = _getLayerOutputSignal(0);
	first_layer_outputs->setArray(train_data);

	/* 信号前向传播 In = W * Oc( In为下一层的输入信号， W为权重矩阵， O为当前层的输出信号) */
	/* 根据上一层的输出信号和权重矩阵，计算下一层的输入信号*/
	for (int i = 0; i < _layer_num-1; ++i)
	{
		Matrix * W = _getWeight(i, i+1);
		Matrix * Oc = _getLayerOutputSignal(i);
		Matrix * In = _getLayerInputSignal(i+1);

		/* 计算矩阵乘法 I = W * O*/
		if (0 != matrix_dot(In, W, Oc))
		{
			std::cout << __FUNCTION__ << "matrixdot failed" << std::endl;
			return 1;
		}

		/* 下一层的输入信号在激活函数的作用下生成输出信号*/
		matrix_map(_getLayerOutputSignal(i+1), _getLayerInputSignal(i+1), _activation_func);
		//std::cout << "in function propagation " << i + 1 << "matrix: " << std::endl;
		//_getLayerOutputSignal(i+1)->dump();
	}

	return 0;
}

/* 误差反向传播的函数 */
int Network::_backward_propagation(double * target_data, unsigned int target_num)
{
	/* 更新目标值*/
	if (target_num != _target_output->getRowNum())
	{
		std::cout << __FUNCTION__ << "not equal" << std::endl;
		return 1;
	}
	_target_output->setArray(target_data);
	Matrix * last_layer_error = _getLayerError(_layer_num - 1);

	/* 计算输出层的error */
	if (0 != matrix_sub(last_layer_error, _target_output, _getLayerOutputSignal(_layer_num-1)))
	{
		std::cout << __FUNCTION__ << "matrix_sub failed" << std::endl;
		return 1;
	}

	/*计算所有隐藏层的error，因为输入层计算error没有意义，所以输入层的error这里不计算*/
	/* Ec = Wt * En, Ec是当前层的误差，Wt是权重矩阵的转置， En 是下一层的误差*/
	/* 所以这个循环的目的是，根据下一层的误差计算当前层的误差，所以叫误差反向传播*/
	for (int layerid = _layer_num - 2; layerid >= 1; --layerid)
	{
		Matrix * Wt = _getWeight(layerid, layerid+1)->transPose();
		Matrix * En = _getLayerError(layerid + 1);
		Matrix * Ec = _getLayerError(layerid);
		
		if (0 != matrix_dot(Ec, Wt, En))
		{
			std::cout << __FUNCTION__ << "matrix dot failed" << std::endl;
			return 1;
		}

		delete Wt;
	}

	return 0;
}

int Network::_update_weights()
{
	if (NULL == _genDeltaWeight_func)
	{
		std::cout << "NULL == _genDeltaWeight" << std::endl;
		return 1;
	}

	for (int layerid = 0; layerid < _layer_num-1; ++layerid)
	{
		/* 获取当前层信号输入，输出，误差信息*/
		Matrix * cl_input = _getLayerInputSignal(layerid);
		Matrix * cl_output = _getLayerOutputSignal(layerid);
		Matrix * cl_error = _getLayerError(layerid);

		/* 获取下一层层信号输入，输出，误差信息*/
		Matrix * nl_input = _getLayerInputSignal(layerid+1);
		Matrix * nl_output = _getLayerOutputSignal(layerid+1);
		Matrix * nl_error = _getLayerError(layerid+1);

		/*计算权重更新*/
		Matrix * currentW = _getWeight(layerid, layerid+1);
		unsigned int rows = currentW->getRowNum();
		unsigned int cols = currentW->getColNum();

		Matrix * deltaW = _genDeltaWeight_func(_lr, rows, cols,
								cl_input, cl_output, cl_error,
								nl_input, nl_output, nl_error);
		

		if (NULL == deltaW)
		{
			std::cout << "deltaW == NULL" << std::endl;
			return 1;
		}
		
		if (0 != matrix_add(currentW, currentW, deltaW))
		{
			delete deltaW;
			std::cout << "matrix_add faild" << std::endl;
			return 1;
		}
		delete deltaW;
	}

	return 0;
}

Matrix * Network::_getLayerInputSignal(int layerid)
{
	return _layer_signal_inputs[layerid];
}

Matrix * Network::_getLayerOutputSignal(int layerid)
{
	return _layer_signal_outputs[layerid];
}

Matrix *  Network::_getWeight(int prelayerid, int nextlayerid)
{
	if (nextlayerid - prelayerid  != 1)
	{
		std::cout << __FUNCTION__<< "error" << std::endl;
		return NULL;
	}

	if (prelayerid < 0 || nextlayerid > _layer_num -1)
	{
		std::cout << __FUNCTION__<< "error" << std::endl;
		return NULL;
	}

	return _weights[prelayerid];
}

Matrix * Network::getWeight(int prelayerid, int nextlayerid)
{
	return _getWeight(prelayerid, nextlayerid);
}

Matrix * Network::_getLayerError(int layerid)
{
	return _layer_errors[layerid];
}
void Network::setActivationFunc(activation_func_t f)
{
	_activation_func = f;
}
void Network::setGenDeltaWeightFunc(update_weight_func_t f)
{
	_genDeltaWeight_func = f;
}

void Network::setLearningRate(double lr)
{
	_lr = lr;
}

void Network::printDebugInfo(void)
{
	std::cout << "=================== network info ======================" << std::endl;
	std::cout << "learning rate = " << _lr << std::endl;
	std::cout << "<---weight matrix info:--->" << std::endl;
	for (int i = 0; i < _layer_num - 1; ++i)
	{
		std::cout << "weight matrix " << i <<  " : " << std::endl;
		_getWeight(i, i+1)->dump();
	}
	std::cout << "<------------------------->" << std::endl;

	std::cout << "<---signal input output info:--->" << std::endl;
	for (int i = 0; i < _layer_num; ++i) 
	{
		std::cout << "layer " << i << " input signal:" << std::endl;
		_getLayerInputSignal(i)->dump();
		std::cout << "layer " << i << " output signal:" << std::endl;
		_getLayerOutputSignal(i)->dump();
	}
	std::cout << "<------------------------->" << std::endl;

	std::cout << "<--- layer error --->" << std::endl;
	
	for (int layerid = 1; layerid < _layer_num; ++layerid)
	{
		std::cout << "layer " << layerid << " error:" << std::endl;
		_getLayerError(layerid)->dump();
	}

	std::cout << "<------------------------->" << std::endl;

	std::cout << "=======================================================" << std::endl;
}
