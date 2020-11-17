#pragma once
#include<stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>

/**
页头信息包含了页号、页内记录数量、当前空闲空间的大小，以及前继页号
和后继页号。这使得数据库管理系统凭借文件的起始页号，就能通过页结构中的
后继页号依次找出文件的每一页。在页中插入记录会从表尾向表头插，数据库管
理系统会为插入的记录分配偏移量表项，存储了记录在当前页中的记录号和相对
于页尾地址的偏移量大小，因此通过偏移量就可以读取页内的每条记录。
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
	/*每一个块，或者说页，有两种创建方式，无参数创建，通常用于初始一个新块，或者读入文件指针，但是
	读入文件指针时需要传入一个标识符，以标识文件的存储格式是全字符串存储，还是全二进制文件存储*/
	//操，不对劲。从文件中读取和建立应该是缓冲区的事情
	/**传入参数：页号
	* 存储方式：按照二进制格式进行存储
	* 按照字节进行处理
	* 偏移量表暂时指向表头结束位置
	* 下一页页号设置为0
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
	//pagebody是一个长长的char数组，长度为4072，用于存储偏移量表和记录本身；
	char* pageBody;
};

