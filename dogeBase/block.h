#pragma once
#include<stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>

/**
ҳͷ��Ϣ������ҳ�š�ҳ�ڼ�¼��������ǰ���пռ�Ĵ�С���Լ�ǰ��ҳ��
�ͺ��ҳ�š���ʹ�����ݿ����ϵͳƾ���ļ�����ʼҳ�ţ�����ͨ��ҳ�ṹ�е�
���ҳ�������ҳ��ļ���ÿһҳ����ҳ�в����¼��ӱ�β���ͷ�壬���ݿ��
��ϵͳ��Ϊ����ļ�¼����ƫ��������洢�˼�¼�ڵ�ǰҳ�еļ�¼�ź����
��ҳβ��ַ��ƫ������С�����ͨ��ƫ�����Ϳ��Զ�ȡҳ�ڵ�ÿ����¼��
*/
class block
{
public:
	int open();
	int writetoRecord();
	int writetoBuffer();
	int readfromBuffer();
	int readfromRecord();
	int read();
	/*ÿһ���飬����˵ҳ�������ִ�����ʽ���޲���������ͨ�����ڳ�ʼһ���¿飬���߶����ļ�ָ�룬����
	�����ļ�ָ��ʱ��Ҫ����һ����ʶ�����Ա�ʶ�ļ��Ĵ洢��ʽ��ȫ�ַ����洢������ȫ�������ļ��洢*/
	//�٣����Ծ������ļ��ж�ȡ�ͽ���Ӧ���ǻ�����������
	/**���������ҳ��
	* �洢��ʽ�����ն����Ƹ�ʽ���д洢
	* �����ֽڽ��д���
	* ƫ��������ʱָ���ͷ����λ��
	* ��һҳҳ������Ϊ0
	*/
	block(int pageno, char* pagebody)
	{
		this->pageno = pageno;
		this->pageBody = pagebody;
		recordnum = 0;
		canUseSpace = 4072;
		if (pageno > 1)
			prevpageno = pageno - 1;
		else
			prevpageno = 0;
		nextpageno = 0;
		pianyiEndPos = 24;
	}

	block(FILE* file)
	{
		oneTable = file;
	}
private:
	int pageno;
	int recordnum;
	int canUseSpace;
	int prevpageno;
	int nextpageno;
	int pianyiEndPos;
	FILE* oneTable;
	//pagebody��һ��������char���飬����Ϊ4072�����ڴ洢ƫ������ͼ�¼����
	char* pageBody;
};

