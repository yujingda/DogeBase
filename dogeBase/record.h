#pragma warning(disable:4996)
#pragma once
#include<string.h>
#include"table.h"
// int 1 float 2 long 3 char 4 varchar 5 double 6 date 7
//就当已经处理成int型的数据了，从而暂时规避分类问题
class record
{
public:
	record(char* Tschema, table* t)
	{
		memset(tempRecord, 0, 4096);
		memset(schema, 0, 1000);
		strcpy(schema, Tschema);
		tableFile = t;
	}
	record(int* Tschema, table* t)
	{
		memset(tempRecord, 0, 4096);
		tableFile = t;
		memset(schema, 0, 1000);
		for (int i = 1; i < 100; i++)
		{
			if (Tschema[i] != 0)
			{
				schemaMap[i] = Tschema[i];
				i++;
			}
		}
	}
	int createOneRecord(int onekind, void* readData, int RDP, int lengthVar);
	void schMapIntsch();
	void copyArray(char* A, int &ax, char* B, int &bx, int len);
private:
	char tempRecord[4096];
	char schema[1000];
	int schemaMap[100];
	table* tableFile;
};
/**
本函数用来将一个数组中的字节直接复制到另外一个数组之中
ax是A数组（目标数组）的起始位置（从1开始计数）
bx是B数组复制起始位置
len是复制长度
*/
void record::copyArray(char* A, int &ax, char* B, int &bx, int len)
{
	A += ax - 1;
	for (int i = bx - 1; i < len + bx - 1; i++)
	{
		*A++ = *B++;
	}
}

/**
* int 1 float 2 long 3 char 4 varchar 5 double 6 date 7
* 本函数用于从输入的数据项中创建出一条记录，并将其写入缓冲区之中
* date类型的数据，规定其格式为：YYYYMMDD
*/
int record::createOneRecord(int onekind, void* readData, int RDP, int lengthVar=-1)
{
	int countSM = 0;
	int TRPosition = 1;
	int RDPosition = RDP;
	for (; countSM != 0; countSM++)
	{
		int kindofData = schemaMap[countSM];
		switch (kindofData)
		{
		case 1:
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, 4); TRPosition += 4; break;
			//strcat(tempRecord);
		case 2:
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, 4); TRPosition += 4; break;
		case 3:
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, 8); TRPosition += 8; break;
		case 4:
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, lengthVar); TRPosition += lengthVar; break;
		case 5:
			char* tl = (char*)lengthVar;
			int tmp = 0;
			copyArray(tempRecord, TRPosition, tl, tmp, 4);//这里是想要把varchar当前的长度写入文件之中，但是可能会出现大小端的问题
			TRPosition += 4;
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, 8); 
			TRPosition += 8;
			break;
		case 6:
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, 8); TRPosition += 8; break;
		case 7:
			//数据输入格式未知，先放一放
			printf("Date is not sheji!\n");  break;
		default:
			break;
		}
	}
}