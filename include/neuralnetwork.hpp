#ifndef __NEURAL_NETWORK_HPP__
#define __NEURAL_NETWORK_HPP__

#include "matrix.hpp"

#define MAX_LAYER_NUM 5 /*神经网络最大的层数, 可调*/
#define MAX_NODE_NUM 8192 /* 每层最大神经节点数，可调*/

class Network
{
public:
	Network(unsigned int layer_num, unsigned int * layer_node_num);
	~Network();
	
	void setLearningRate(double lr); /* 设置学习率 */
	void printConfig(bool verbose); /*打印配置*/
	int trainOneStep(); /*根据输入与目标值，进行一次正向传播与反向传播*/
	int query();	/*查询*/
	bool ok(void);	/*判断神经网络是否初始化成功*/
	void setLr(double lr) {_lr = lr;}  /* 设置学习率 */

private:
	Matrix * getInputHiddenWeight(void); /* 获取输入到隐藏层之间的权重矩阵，类内使用*/
	Matrix * getHiddenOutputWeight(void); /* 获取隐藏层到输出层之间的权重矩阵，类内使用*/
	unsigned int getLayerNodeNum(int layer_id);

	int forward_propagation(); 	/*信号正向传播*/
	int back_propagation();		/*误差反向传播*/
	int update_weights();		/*更新权重*/

private:
	double _lr; /*学习率*/
	int _layer_num; /* 神经元的层数 */

	Matrix * _weights[MAX_LAYER_NUM-1]; /*权重矩阵数组*/

	Matrix * _layer_signal_inputs[MAX_LAYER_NUM]; /*记录每一层神经元的输入信号*/
	Matrix * _layer_signal_outputs[MAX_LAYER_NUM];	/*记录每一层神经元的输出信号*/

	Matrix * _layer_errors[MAX_LAYER_NUM]; /*记录每一层的误差值, 注意输入层没有误差值, 所以下标为0的不用*/
	Matrix * _target_output;  /*目标值*/

	bool _resources_malloced;



};

#endif
