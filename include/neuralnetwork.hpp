#ifndef __NEURAL_NETWORK_HPP__
#define __NEURAL_NETWORK_HPP__

#include "matrix.hpp"

#define MAX_LAYER_NUM 5 /*神经网络最大的层数, 可调*/
#define MAX_NODE_NUM 8192 /* 每层最大神经节点数，可调*/

typedef double (* activation_func_t) (double);

/*参数说明：

根据当前层和下一层的相关信息，计算当前层和下一层之间的矩阵更新

cl ==> current_layer
nl ==> next_layer
cl_input: 当前层的信号输入
cl_output:   当前层的信号输出
cl_error: 当前层的error
rows, cols 用来制定返回矩阵的规模
*/
typedef Matrix * (* update_weight_func_t)(
double lr, unsigned int rows, unsigned int cols,
const Matrix * cl_input, const Matrix * cl_output, const Matrix * cl_error,
const Matrix * nl_input, const Matrix * nl_output, const Matrix * nl_error
);

class Network
{
public:
	Network(unsigned int layer_num, unsigned int * layer_node_num);
	~Network();
	
	void setLearningRate(double lr); /* 设置学习率 */
	void setActivationFunc(activation_func_t f);
	void setGenDeltaWeightFunc(update_weight_func_t);
	void printConfig(bool verbose); /*打印配置*/
	int trainOneStep(double * train_data, unsigned int data_num, 
						double * target_data, unsigned int target_num); /*根据输入与目标值，进行一次正向传播与反向传播*/
	int query(double * input_data, double *result);	/*查询*/

	bool ok(void);	/*判断神经网络是否初始化成功*/
	void setLr(double lr) {_lr = lr;}  /* 设置学习率 */

	void printDebugInfo(void); /*调试时用，用于打印神经网络中的内部各种矩阵的数据*/
	
	Matrix * getWeight(int prelayerid, int nextlayerid); // for debug
private://类内使用函数
	Matrix * _getWeight(int prelayerid,int nextlayerid); /* 获取权重矩阵，类内使用*/
	Matrix * _getLayerInputSignal(int layerid); /* 获取layerid层的信号输入*/
	Matrix * _getLayerOutputSignal(int layerid); /* 获取layerid层的信号输出 */
	Matrix * _getLayerError(int layerid);	/* 获取layerid层的误差*/
	unsigned int _getLayerNodeNum(int layer_id);

	int _forward_propagation(double * train_data, unsigned int data_num); 	/*信号正向传播*/
	int _backward_propagation(double * target_data, unsigned int target_num);		/*误差反向传播*/
	int _update_weights(void);		/*更新权重*/

private:
	double _lr; /*学习率*/
	int _layer_num; /* 神经元的层数 */

	/*_activation_func 和 _genDeltaWeight_func两个函数指针由用户定义
	因为，根据激活函数，比如激活函数设置为sigmoid函数，更新权重的方法为梯度下降法
	那么就要自定义更新权重的方法。*/
	activation_func_t _activation_func;
	update_weight_func_t _genDeltaWeight_func; 

	Matrix * _weights[MAX_LAYER_NUM-1]; /*权重矩阵数组*/

	Matrix * _layer_signal_inputs[MAX_LAYER_NUM]; /*记录每一层神经元的输入信号*/
	Matrix * _layer_signal_outputs[MAX_LAYER_NUM];	/*记录每一层神经元的输出信号*/

	Matrix * _layer_errors[MAX_LAYER_NUM]; /*记录每一层的误差值, 注意输入层没有误差值, 所以下标为0的不用*/
	Matrix * _target_output;  /*目标值*/

	bool _resources_malloced;
};


#endif
