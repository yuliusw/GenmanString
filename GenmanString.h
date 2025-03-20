#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef GenmanStrings
#define GenmanStrings
#endif
#include <cstring>//for strlen() &s
#include <iostream>
#include <stdexcept>
//�ο����� https://space.shantou.university/posts/4415/german_strings/
//һ�ֺܿ���ַ������,�������ݿ������,�����Լ�ʵ�֡�
//�ַ���״̬��ʶ�����������������ڡ�ֻʵ�ֳ־û�״̬persistent��

/*
	mainpage: yuliusw.github.io
	e-mail: 2023011673@bistu.edu.cn
*/

class GenmanString
{
	//private:
public:
	int __len;//Ϊ�����ڴ���Ը�Ϊpublic
	union base		//ʹ�� union �ֶ�ȷ�������ַ�������һ��ʵ�֣���Ϊʹ��ָ����г��ַ�������ʱ���ط��䣬������capacity������ο����ף�
	{				//��˶��ַ�������ɾ�Ĳ��������ϴ��ʺϾ�̬�ַ������Σ���ȡ����������
		struct L {
			char* __ptr;
			char prefix[4];
		}longStr;
		struct S {			//Ϊ��֤�ڴ���룬��װһ��struct
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
		if (src == nullptr)					//���ַ�����ʼ����std����string ��ʵ���ǲ��������ַ���
		{
			__len = 0;
			B.shortStr.inlined[0] = '\0';
		}
		else								//�ֱ�ʵ�ֶ��ַ����ͳ��ַ�����ʼ��
		{
			int lemgth = strlen(src);
			__len = lemgth;
			if (lemgth > 15)					//str____len()���������ַ�,ȷ�����ַ���Ԥ���㹻�ռ�
			{
				for (int i = 0; i < 4; i++)
				{
					B.longStr.prefix[i] = src[i];		//[0]~[3]
				}
				B.longStr.__ptr = new char[lemgth - 3];	//lemgth+1(���ַ�)-4��prefix��

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
		{	//debug:��ջ���� 
			//strcpy(this->B.longStr.prefix, other.B.longStr.prefix); 
			//����prefixʱ����ʹ��strcpy����Ϊstrcpy����ֹ����Ϊ\0;��prefix����\0,��long.ptr����ʹ��strcpy��
			for (int i = 0; i < 4; i++)
			{
				this->B.longStr.prefix[i] = other.B.longStr.prefix[i];
			}

			B.longStr.__ptr = new char[__len - 3];
			strcpy(B.longStr.__ptr, other.B.longStr.__ptr);		//�����ڴ沢�Ҹ��Ƴ��ַ���������
		}
		else
		{
			strcpy(this->B.shortStr.inlined, other.B.shortStr.inlined);   //�������洢����
		}
	};
	GenmanString  (GenmanString&& other) {
		//�����ƶ����캯��������Աunion��ֱֵ�Ӹ�ֵ���¶������ߵ�union��Ա�����õ�ַ�������迼���ַ������̡�
		//��ʵ��union���ƶ����첢ʹ��this->B = &other.B ����Ϊ B���ַ��������ڹ̶���С�Ŀռ䣬union���ƶ����콫���¶�������������ʱ�ظ�������һ��union�ռ䡣
		this->__len = other.__len;
		this->B = other.B;
		other.B.longStr.__ptr = nullptr;
	
	}
	~GenmanString(void) {							//���������н���Ҫ�������ַ����е�__ptr��ָ���ڴ�飬ʣ��ĳ�Ա����Ĭ�����������ṩ��
		if (__len > 15)
		{
			delete(B.longStr.__ptr);
		}
	};

	GenmanString& operator =(const GenmanString& other) {		//��ȫ���ƣ�ʵ�ֳ���ֵ�����̸�ֵ�̣��̸�ֵ��������ֵ��
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
			std::cout << "����Խ��" << std::endl;
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
		if (new_length > 15)									//Ϊ\0��һλ
		{													//����this����,���ƺ����һλ�ַ�ռ����__ptr[this->len-5]
			if (this->__len > 15)
			{
				//strcpy(res->B.longStr.prefix, this->B.longStr.prefix);  debug ���췢�ֶ�ջ����ˣ�strcpy�ж������ǿ��ַ���prefixû�п��ַ���
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
			int nowindex = this->__len - 4;						//�����������
			if (str4.__len > 15)
			{
				for (int i = 0; i < 4; i++)
				{
					res->B.longStr.__ptr[nowindex + i] = str4.B.longStr.prefix[i]; //���һλ��[this.len]
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
			//������ϣ����\0 
			res->B.longStr.__ptr[new_length - 4] = '\0';
		}								//
		else                         //������Ϊ���ַ�����������������ֻ�ж��ַ�����ӵó�
		{
			strcpy(res->B.shortStr.inlined, this->B.shortStr.inlined);	//��Ч�ַ����λռ��this.len - 1
			int nowindex = this->__len - 1;
			for (int i = 0; i < str4.__len; i++)
			{
				res->B.shortStr.inlined[nowindex + i] = str4.B.shortStr.inlined[i];
			}//������ϣ����\0
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
				char mid[4];			//��union�����޸Ľ�����ԭ����ʧЧ�����������Ҫһ���м�����ݴ�
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

