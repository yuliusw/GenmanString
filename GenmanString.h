#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef GenmanStrings
#define GenmanStrings
#endif
#include <cstring>//for strlen() &s
#include <iostream>
#include <stdexcept>
//参考文献 https://space.shantou.university/posts/4415/german_strings/
//一种很酷的字符串设计,用于数据库等情形,尝试自己实现。
//字符串状态标识，用于区分生命周期。只实现持久化状态persistent。

/*
	mainpage: yuliusw.github.io
	e-mail: 2023011673@bistu.edu.cn
*/

class GenmanString
{
	//private:
public:
	int __len;//为方便内存测试改为public
	union base		//使用 union 字段确保长短字符串仅有一种实现，因为使用指针进行长字符串构造时的重分配，舍弃了capacity，详见参考文献；
	{				//因此对字符串的增删改操作开销较大，适合静态字符串情形（读取，分析）；
		struct L {
			char* __ptr;
			char prefix[4];
		}longStr;
		struct S {			//为保证内存对齐，封装一个struct
			char inlined[16];
		}shortStr;
	}B;
	//public:
	GenmanString() {
		__len = 0;
		B.shortStr.inlined[0] = '\0';
	};
	GenmanString(const char src) {
		__len = 1;
		B.shortStr.inlined[0] = src;
		B.shortStr.inlined[1] = '\0';
	}
	GenmanString(const char* src) {
		if (src == nullptr)					//空字符串初始化，std：：string 的实现是不包括空字符的
		{
			__len = 0;
			B.shortStr.inlined[0] = '\0';
		}
		else								//分别实现短字符串和长字符串初始化
		{
			int lemgth = strlen(src);
			__len = lemgth;
			if (lemgth > 15)					//str____len()不包括空字符,确保短字符串预留足够空间
			{
				for (int i = 0; i < 4; i++)
				{
					B.longStr.prefix[i] = src[i];		//[0]~[3]
				}
				B.longStr.__ptr = new char[lemgth - 3];	//lemgth+1(空字符)-4（prefix）

				for (int i = 0; i < lemgth - 4; i++)
				{
					B.longStr.__ptr[i] = src[i + 4];		//src[4]~lemgth
				}
			}
			else
			{

				strcpy(B.shortStr.inlined, src);
			}
		}
	};
	GenmanString(const int len) {
		__len = len;
		if (len > 15)
		{
			B.longStr.__ptr = new char[len - 3];
		}
		else
		{
			B.shortStr.inlined[0] = '\0';
		}

	};
	GenmanString(const GenmanString& other) {
		this->__len = other.__len;
		if (__len > 15)
		{	//debug:堆栈错误 
			//strcpy(this->B.longStr.prefix, other.B.longStr.prefix); 
			//操作prefix时不能使用strcpy，因为strcpy的终止条件为\0;但prefix不存\0,但long.ptr可以使用strcpy；
			for (int i = 0; i < 4; i++)
			{
				this->B.longStr.prefix[i] = other.B.longStr.prefix[i];
			}

			B.longStr.__ptr = new char[__len - 3];
			strcpy(B.longStr.__ptr, other.B.longStr.__ptr);		//开新内存并且复制长字符串的内容
		}
		else
		{
			strcpy(this->B.shortStr.inlined, other.B.shortStr.inlined);   //用类对象存储内容
		}
	};
	GenmanString  (GenmanString&& other) {
		//对于移动构造函数，将成员union的值直接赋值给新对象（两者的union成员不共用地址），无需考虑字符串长短。
		//不实现union的移动构造并使用this->B = &other.B 是因为 B是字符串对象内固定大小的空间，union的移动构造将导致对两个对象析构时重复析构第一个union空间。
		this->__len = other.__len;
		this->B = other.B;
		other.B.longStr.__ptr = nullptr;
	
	}
	~GenmanString(void) {							//析构函数中仅需要析构长字符串中的__ptr所指向内存块，剩余的成员将由默认析构函数提供；
		if (__len > 15)
		{
			delete(B.longStr.__ptr);
		}
	};

	GenmanString& operator =(const GenmanString& other) {		//完全复制，实现长赋值长，短赋值短，短赋值长，长赋值短
		if (this == &other)
			return *this;
		else
		{
			int prelen = this->__len;
			this->__len = other.__len;
			if (prelen > 15 && __len > 15)
			{
				delete(this->B.longStr.__ptr);
				for (int i = 0; i < 4; i++)
				{
					B.longStr.prefix[i] = other.B.longStr.prefix[i];
				}
				B.longStr.__ptr = new char[__len - 3];
				strcpy(B.longStr.__ptr, other.B.longStr.__ptr);
			}
			else if (prelen > 15 && __len < 15)
			{
				delete(this->B.longStr.__ptr);
				this->B.shortStr.inlined[0] = '\0';
				strcpy(B.shortStr.inlined, other.B.shortStr.inlined);
			}
			else if (prelen <= 15 && __len <= 15)
			{
				strcpy(B.shortStr.inlined, other.B.shortStr.inlined);
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					B.longStr.prefix[i] = other.B.longStr.prefix[i];
				}
				B.longStr.__ptr = new char[__len - 3];
				strcpy(B.longStr.__ptr, other.B.longStr.__ptr);
			}
		}
		return *(this);
	};
	char& operator[](int indexx) {
		if (indexx > this->__len)
		{
			std::cout << "数组越界" << std::endl;
		}
		if (this->__len > 15)
		{
			if (indexx < 4)
			{
				return this->B.longStr.prefix[indexx];
			}
			else
			{
				return this->B.longStr.__ptr[indexx - 4];
			}
		}
		return this->B.shortStr.inlined[indexx - 1];
	};
	GenmanString operator+(GenmanString& str4) {
		if (str4.__len == 0)
		{
			return *(new GenmanString(*this));
		}
		int new_length = this->__len + str4.__len;
		GenmanString* res = new GenmanString(new_length+1);
		if (new_length > 15)									//为\0留一位
		{													//复制this内容,复制后最后一位字符占用了__ptr[this->len-5]
			if (this->__len > 15)
			{
				//strcpy(res->B.longStr.prefix, this->B.longStr.prefix);  debug 今天发现堆栈溢出了，strcpy判定条件是空字符，prefix没有空字符；
				for (int i = 0; i < 4; i++)
				{
					res->B.longStr.prefix[i] = this->B.longStr.prefix[i];
				}
				strcpy(res->B.longStr.__ptr, this->B.longStr.__ptr);
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					res->B.longStr.prefix[i] = this->B.shortStr.inlined[i];
				}
				for (int i = 4; i < this->__len; i++)
				{
					res->B.longStr.__ptr[i - 4] = this->B.shortStr.inlined[i];
				}
			}
			int nowindex = this->__len - 4;						//方便后续复制
			if (str4.__len > 15)
			{
				for (int i = 0; i < 4; i++)
				{
					res->B.longStr.__ptr[nowindex + i] = str4.B.longStr.prefix[i]; //最后一位在[this.len]
				}
				nowindex += 4;
				for (int i = 0; i < str4.__len - 4; i++)
				{
					res->B.longStr.__ptr[nowindex] = str4.B.longStr.__ptr[i];
					nowindex++;
				}

			}
			else
			{
				for (int i = 0; i < str4.__len; i++)
				{
					res->B.longStr.__ptr[nowindex] = str4.B.shortStr.inlined[i];
					nowindex++;
				}

			}//
			//复制完毕，添加\0 
			res->B.longStr.__ptr[new_length - 4] = '\0';
		}								//
		else                         //处理结果为短字符串的情况，此情况下只有短字符串相加得出
		{
			strcpy(res->B.shortStr.inlined, this->B.shortStr.inlined);	//有效字符最后位占用this.len - 1
			int nowindex = this->__len - 1;
			for (int i = 0; i < str4.__len; i++)
			{
				res->B.shortStr.inlined[nowindex + i] = str4.B.shortStr.inlined[i];
			}//复制完毕，添加\0
			res->B.shortStr.inlined[new_length] = '\0';
		}
		return *res;
	};
	friend std::ostream& operator<<(std::ostream& cout, GenmanString & g) {
		if (g.__len>15) {
			for (int i = 0; i < 4; i++)
			{
				cout << g.B.longStr.prefix[i];
			}
			cout << g.B.longStr.__ptr;
		}
		else
		{
			cout << g.B.shortStr.inlined;
		}
		return cout;
	};
	GenmanString& operator<<(const char* c) {
		int coun = strlen(c);
		if (this->__len > 15) {			
			char* new_ptr = new char[ this->__len +coun- 3 ];
			strcpy(new_ptr, this->B.longStr.__ptr);
			int j = 0;
			for (int i =  this->__len - 4 ;i < this->__len+coun-4; i++)
			{
				new_ptr[i] = c[j]; j++;
			}
			this->__len += coun;
			delete(this->B.longStr.__ptr);
			this->B.longStr.__ptr = new_ptr;
		}
		else
		{
			if (coun+this->__len>15)
			{
				char mid[4];			//对union区的修改将导致原数据失效，因此这里需要一个中间变量暂存
				char* new_ptr = new char[this->__len + coun - 3];
	
				if (this->__len<4)
				{
					strcpy(mid, this->B.shortStr.inlined);
					int nowindex = 4-this->__len;
					int j = 0;						//2
					for (int i = this->__len; i < 4; i++)
					{
						mid[i] = c[j];
						j++;
					}
					j = 0;
					for (int i = nowindex; i < coun; i++)
					{	
						new_ptr[j] = c[i];
						j++;
					}
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						mid[i] = this->B.shortStr.inlined[i];
					}
					int j = 0;
					for (int i = 4; i < this->__len; i++)
					{
						new_ptr[j] = this->B.shortStr.inlined[i];
						j++;
					}
					for (int i =0; i < coun; i++)
					{
						new_ptr[j] = c[i];
						j++;
					}
				}
				for (int i = 0; i < 4; i++)
				{
					this->B.longStr.prefix[i] = mid[i];
				}
				this->B.longStr.__ptr = new_ptr;
				this->__len += coun;
				this->B.longStr.__ptr[this->__len - 4] = '\0';
			}
			else
			{
				int	j = this->__len;
				for (int i = 0; i < coun; i++)
				{
					this->B.shortStr.inlined[j] = c[i];
					j++;
				}
				this->__len += coun;
				this->B.shortStr.inlined[this->__len+1] = '\0';
			}
		}
		return *this;
	};
	GenmanString& operator+=(const char* c) {
		return *this << c;
	}
};

