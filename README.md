## 手写数字识别(神经网络c++实现)
NOTE: **还未完成**


## 编译与运行
```shell
git clone https://github.com:shengliwang/handwritten-numeral-recognition-cpp.git
cd handwritten-numeral-recognition-cpp
mkdir build
cd build/
cmake ..
make
./test
```


## 其他
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


### archlinux 安装 openmp (并行计算库)
```shell
sudo pacman -S openmp
```
