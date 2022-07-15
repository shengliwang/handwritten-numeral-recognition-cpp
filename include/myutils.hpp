#ifndef __MY_UTILS_HPP__
#define __MY_UTILS_HPP__


/* 生成 -0.5~0.5之间的随机数todo*/
double myrand_double();

#define CHECK_PARAM(condition)   \
do{				\
	if (!(condition)){ \
		printf("%s:%d condition check failed\n", __FUNCTION__, __LINE__);\
	}\
}while(0)

#define CHECK_PARAM_RET(condition, ret)   \
do{				\
	if (!(condition)){ \
		printf("%s:%d condition check failed\n", __FUNCTION__, __LINE__);\
		return (ret); \
	}\
}while(0)

#endif
