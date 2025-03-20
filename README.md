# GenmanString

#### 参考/reference https://space.shantou.university/posts/4415/german_strings/

A string class and its useing test for it
For my c++ leason classroom assignment
看在我手搓了300行的份上别给我打29分pls。。。

## 对原设计的改动：

- 内部采用 int 而非 uint32_t ，（无论采用 uint32_t 还是 int ，它们占用内存的大小相同)，以方便写数组索引等操作；
  
- 为与长字符串实现内存对齐，将短字符串数组大小改为了16 char （在VS提供的内存布局分析中，一个char*加上一个容量4的char[]共占用了16字节。我的编译器是MVSC，VS2022(V143) 工具集）
  

## 测试代码：

包括了以下项目：

1. 对构造函数，拷贝构造函数，移动构造函数正常产生对象的验证，
  
2. 对+,+,[],<<等函数重载正确产生结果的验证
  
3. 对移动构造函数与拷贝构造函数产生的效率差异验证
