#include "GenmanString.h"
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include<time.h>
//��ʽ�ַ���
int main() {
	//���캯������
	GenmanString* a = new GenmanString("hello");	//�̴����ڴ�Ϊ�̶� 24�ֽ�
	std::cout << "a������" << a->B.shortStr.inlined << std::endl;
	std::cout << "a�Ĵ�С" << sizeof(*a) << std::endl;
	GenmanString* b = new GenmanString("123456789ABCDEFGHIjKLMN");	//����,�ڴ�ͬ��Ϊ24�ֽ�,*ptrΪ4�ֽ�
	std::cout << "b�����������ݣ������δ�洢\\0���������룩";
	std::printf(b->B.longStr.prefix);												//������������Ϊ��һ����û�д��ַ�������ֹ��
	printf("\n");
	std::cout << "b��ָ��������";

	std::printf(b->B.longStr.__ptr);
	printf("\n");
	std::cout << "b�Ĵ�С";
	std::printf("%lld\n", sizeof(*b));
	std::cout << "b���������Ĵ�С��ָ�����洢���ַ���";
	std::printf("sizeof and strlen() %lld , %lld", sizeof((b->B)), strlen((b->B.longStr.__ptr))); //����ֱ�Ӧ��Ϊ16 ��19
	//���� char��������Ϊ24���ַ���������ֹ������4��������prefix�20����*ptr�strlen���������ַ���
	printf("\n");
	printf("�±���������ز���\n((*a)[5], (*b)[6])(%c,%c)", (*a)[5], (*b)[6]);

	//���Ӷ̣����ӳ����̼ӳ����̼Ӷ̲��Լ�<<����
	GenmanString aa = *a + *a;
	std::cout << "aa = (*a) + (*a)�����ݣ�a,bΪָ�룩" << std::endl; //aaӦΪhellohello
	std::cout << aa << std::endl;
	GenmanString bb = *b + *b;
	std::cout << "bb = (*b) + (*b)������" << std::endl;
	std::cout << bb << std::endl;
	GenmanString ab = *a + *b;
	std::cout << "ab = (*a) + (*b)������" << std::endl;
	std::cout << ab << std::endl;
	GenmanString ba = *b + *a;
	std::cout << "ba = (*a) + (*b)������" << std::endl;
	std::cout << ba << std::endl;
	
	
	*a << " world";
	std::cout << "*a<<\" world\" " << std::endl;
	std::cout << *a << std::endl;

	*a += "world";
	std::cout << "*a+=\" world\" " << std::endl;
	std::cout << *a << std::endl;
	
	std::cout << "�ƶ����캯������֤ �ֱ����bb��ָ������ַ��B�ĵ�ַ" << std::endl;
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
	std::cout << "��������20000����ʱ��Ϊ(ms)" << end - start<<std::endl;

	start2 = clock();
	for (int i = 0; i < 40000; i++)
	{
		S2[i] = std::move(S1[i]);
	}
	end2 = clock();
	std::cout << "�ƶ�����20000����ʱ��Ϊ" << end2 - start2 << std::endl;

};

