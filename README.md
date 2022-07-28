## 手写数字识别(神经网络c++实现)

## 说明
本代码使用c++实现神经网络的一个简单应用，不借助任何框架。目的是加深对神经网络工作流程的理解。目前提供使用mnist手写数据集的识别，识别准确率大概在95%左右。   
代码实现, 参考书籍 <https://github.com/jash-git/Book-Python-Neural-Network>   
代码结构如下：
```shell
.
├── CMakeLists.txt				# 工程文件
├── examples
│   ├── mnist_dataset			# mnist 数据集，git代码库不提供，请按照下面的链接自行下载。
│   │   ├── mnist_test_10.csv
│   │   ├── mnist_test.csv
│   │   ├── mnist_train_100.csv
│   │   └── mnist_train.csv
│   ├── mnist_train.cpp			# 使用mnist数据集训练的例子
│   └── test.cpp				# 测试代码
├── imgs
├── include
│   ├── matrix.hpp				# 提供矩阵相关操作
│   ├── myutils.hpp				# 提供随机数发生器等
│   └── neuralnetwork.hpp		# 提供神经网络相关类和接口
├── README.md
└── src
    ├── matrix.cpp				# 矩阵相关操作实现
    ├── myutils.cpp				
    └── neuralnetwork.cpp		# 神经网络实现

```
1. <http://www.pjreddie.com/media/files/mnist_train.csv>
2. <http://www.pjreddie.com/media/files/mnist_test.csv>
3. <https://raw.githubusercontent.com/makeyourownneuralnetwork/makeyourownneuralnetwork/master/mnist_dataset/mnist_test_10.csv>
4. <https://raw.githubusercontent.com/makeyourownneuralnetwork/makeyourownneuralnetwork/master/mnist_dataset/mnist_train_100.csv>

## 编译与运行
运行前，请先下载mnist手写数字训练数据集和测试数据集。
并在 `examples/mnist_train.cpp` 中修改代码，指定使用文件。
```shell
git clone https://github.com/shengliwang/handwritten-numeral-recognition-cpp.git
cd handwritten-numeral-recognition-cpp
mkdir build		## 创建临时路径用于代码编译
cd build/		
cmake ..		## 生成makefile
make			## 使用makefile进行编译
./mnist_train	## 运行
```

## 使用 openmp 加速矩阵运算
### archlinux 安装 openmp (并行计算库)
```shell
sudo pacman -S openmp
```
注意，在archlinux下安装openmp包后，`CMakeLists.txt` 中用的指令是 find_package(OpenMPTarget).
其他平台，比如ubuntu下面可能是 find_package(OpenMP). 如果编译不通过，请提交issues。

### 编译与运行
```shell
mkdir build
cd build/
cmake .. -DUSINGOPENMP=yes
make
./mnist_train
```
使用 openmp 加速后，在我的电脑上，加速比：106s/39s=2.71(未加速用时106s，使用openmp加速用时39s)    
并行计算参考： <https://zhuanlan.zhihu.com/p/181669611>


## 其他
遇到如下错误以及解决办法
```shell
CMake Error at /usr/share/cmake/Modules/FindOpenMP.cmake:616 (message):
  FindOpenMP requires the C, CXX or Fortran languages to be enabled
Call Stack (most recent call first):
  /usr/lib/cmake/openmp/FindOpenMPTarget.cmake:84 (find_package)
  CMakeLists.txt:11 (find_package)


CMake Error at /usr/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find OpenMPTarget (missing: OpenMPTarget_NVPTX_FOUND NVPTX)
Call Stack (most recent call first):
  /usr/share/cmake/Modules/FindPackageHandleStandardArgs.cmake:594 (_FPHSA_FAILURE_MESSAGE)
  /usr/lib/cmake/openmp/FindOpenMPTarget.cmake:328 (find_package_handle_standard_args)
  CMakeLists.txt:11 (find_package)
```

解决方法：
<https://stackoverflow.com/questions/49937059/findopenmp-requires-c-or-cxx-language-to-be-enabled-error-on-ubuntu-16-04-and-c>

<https://reviews.llvm.org/D95556>