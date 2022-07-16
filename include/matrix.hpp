#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__


typedef double (*random_func_t)(void);
typedef double (*map_func_t)(double);

/* 二维矩阵定义 */
class Matrix
{
public:
	Matrix(int row_num, int col_num);
	~Matrix();

	/*使用 函数f 对矩阵进行初始化 */
	void random_init(random_func_t f);
	void setArray(double * arr); // use for test
	unsigned int getRowNum(void);
	unsigned int getColNum(void);
	Matrix * transPose(void);

	void print();

	friend int matrix_dot(Matrix * dst, const Matrix * A, const Matrix * B); // dst = A * B 点乘
	friend int matrix_map(Matrix * dst, const Matrix * src, map_func_t func); // src--> map --> dst
	friend int matrix_sub(Matrix * dst, const Matrix * A, const Matrix * B); // dst = A - B 
	friend int matrix_add(Matrix * dst, const Matrix * A, const Matrix * B); // dst = A + B 

private:
	int _rows;
	int _cols;
	int _total_size;
	double * _array;

};


#endif
