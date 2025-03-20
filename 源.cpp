#include "GenmanString.h"
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include<time.h>
//德式字符串
int main() {
	//构造函数测试
	GenmanString* a = new GenmanString("hello");	//短串，内存为固定 24字节
	std::cout << "a的内容" << a->B.shortStr.inlined << std::endl;
	std::cout << "a的大小" << sizeof(*a) << std::endl;
	GenmanString* b = new GenmanString("123456789ABCDEFGHIjKLMN");	//长串,内存同样为24字节,*ptr为4字节
	std::cout << "b的索引区内容（设计中未存储\\0所以有乱码）";
	std::printf(b->B.longStr.prefix);												//后面乱码是因为这一部分没有存字符串的终止符
	printf("\n");
	std::cout << "b的指针区内容";

	std::printf(b->B.longStr.__ptr);
	printf("\n");
	std::cout << "b的大小";
	std::printf("%lld\n", sizeof(*b));
	std::cout << "b中索引区的大小和指针区存储的字符数";
	std::printf("sizeof and strlen() %lld , %lld", sizeof((b->B)), strlen((b->B.longStr.__ptr))); //结果分别应该为16 和19
	//其中 char数组输入为24个字符（包括终止符），4个在索引prefix里，20个在*ptr里，strlen不包括空字符。
	printf("\n");
	printf("下标运算符重载测试\n((*a)[5], (*b)[6])(%c,%c)", (*a)[5], (*b)[6]);

	//长加短，长加长，短加长，短加短测试及<<重载
	GenmanString aa = *a + *a;
	std::cout << "aa = (*a) + (*a)的内容（a,b为指针）" << std::endl; //aa应为hellohello
	std::cout << aa << std::endl;
	GenmanString bb = *b + *b;
	std::cout << "bb = (*b) + (*b)的内容" << std::endl;
	std::cout << bb << std::endl;
	GenmanString ab = *a + *b;
	std::cout << "ab = (*a) + (*b)的内容" << std::endl;
	std::cout << ab << std::endl;
	GenmanString ba = *b + *a;
	std::cout << "ba = (*a) + (*b)的内容" << std::endl;
	std::cout << ba << std::endl;
	
	
	*a << " world";
	std::cout << "*a<<\" world\" " << std::endl;
	std::cout << *a << std::endl;

	*a += "world";
	std::cout << "*a+=\" world\" " << std::endl;
	std::cout << *a << std::endl;
	
	std::cout << "移动构造函数的验证 分别输出bb的指针区地址和B的地址" << std::endl;
	std::cout << (int)bb.B.longStr.__ptr << std::endl;
	GenmanString B = std::move(bb);
	std::cout << (int)B.B.longStr.__ptr <<std::endl;
	GenmanString* S = new GenmanString[40000];
	for (int i = 0; i < 40000; i++)
	{
		S[i] = *new GenmanString("HELLOW1234567891asss");
	}
	GenmanString* S1 = new GenmanString[40000];
	GenmanString* S2 = new GenmanString[40000];
	clock_t start, end, start2, end2;
	
	start = clock();
	for (int i = 0; i < 40000; i++)
	{
		S1[i] = *new GenmanString(S[i]);
	}
	end = clock();
	std::cout << "拷贝构造20000个的时间为(ms)" << end - start<<std::endl;

	start2 = clock();
	for (int i = 0; i < 40000; i++)
	{
		S2[i] = std::move(S1[i]);
	}
	end2 = clock();
	std::cout << "移动构造20000个的时间为" << end2 - start2 << std::endl;

};

