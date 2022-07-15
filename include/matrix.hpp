#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__


typedef double (*random_func_t)(void);

/* 二维矩阵定义 */
class Matrix
{
public:
	Matrix(int row_num, int col_num);
	~Matrix();

	/*使用 函数f 对矩阵进行初始化 */
	void random_init(random_func_t f);
	unsigned int getRowNum(void);
	unsigned int getColNum(void);

	void print();

private:
	int _rows;
	int _cols;
	int _total_size;
	double * _array;


};



#endif
