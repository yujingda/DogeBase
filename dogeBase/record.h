#pragma warning(disable:4996)
#pragma once
#include<string.h>
#include"table.h"
// int 1 float 2 long 3 char 4 varchar 5 double 6 date 7
//�͵��Ѿ������int�͵������ˣ��Ӷ���ʱ��ܷ�������
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
������������һ�������е��ֽ�ֱ�Ӹ��Ƶ�����һ������֮��
ax��A���飨Ŀ�����飩����ʼλ�ã���1��ʼ������
bx��B���鸴����ʼλ��
len�Ǹ��Ƴ���
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
* ���������ڴ�������������д�����һ����¼��������д�뻺����֮��
* date���͵����ݣ��涨���ʽΪ��YYYYMMDD
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
			copyArray(tempRecord, TRPosition, tl, tmp, 4);//��������Ҫ��varchar��ǰ�ĳ���д���ļ�֮�У����ǿ��ܻ���ִ�С�˵�����
			TRPosition += 4;
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, 8); 
			TRPosition += 8;
			break;
		case 6:
			copyArray(tempRecord, TRPosition, (char*)readData, RDPosition, 8); TRPosition += 8; break;
		case 7:
			//���������ʽδ֪���ȷ�һ��
			printf("Date is not sheji!\n");  break;
		default:
			break;
		}
	}
}