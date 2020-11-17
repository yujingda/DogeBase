#ifndef dbHead_h
#define dbHead_h

#define SIZE_DATA_SPACE (100*1024*1024)                 // 数据库总空间
#define SIZE_PER_PAGE (4*1024)                          // 每一页的大小

#define ADDR_BITMAP 1024                                // bitmap起始位置
#define SIZE_BITMAP (SIZE_DATA_SPACE/(SIZE_PER_PAGE*8)) // bitmap大小=数据区大小/(每一页大小*8)

#define MAX_FILE_NUM 64                                 // 最大文件数
#define ADDR_DATA (ADDR_BITMAP+SIZE_BITMAP)             // 数据区起始位置

#define SIZE_BUFF 1000                                  // buff大小

#define MAP_FILE 0
#define DATA_FILE 1

#define ALLO_FAIL -1
#define PAGE_AVAI 1
#define PAGE_UNAVAI 0

#define BUFF_NOT_HIT -1

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<vector>

/**
 *
 * 块内的偏移量表从块的前端向后增长
 * 块内的记录是从后向前放置
 *
 **/
struct offsetInPage{
    int recordID;
    int offset;                                 //该记录相对于块尾地址的偏移量
    bool isDeleted;
};

struct pageHead{
    long pageNo;                                // 页号
    int curRecordNum;                           // 当前该页存储的记录个数
    long prePageNo;                             //前继页号
    long nextPageNo;                             //后继页号
    long freeSpace;                             // 该页的空余空间大小
};

struct FileDesc{
    int fileID;                                 //文件号
    int fileType;                               //文件类型
    long fileFirstPageNo;                       //文件指向的第一页
    long filePageNum;                           //文件占用了多少页
};

struct SysDesc{
    long dataSpaceSize;                         // 文件中数据区的大小
    long perPageSize;                           // 每一个页的大小
    long totalPage;                             // 总共的页数
    long avaliblePage;                          // 当前有多少可用的页
    long bitMapAddr;                            // 文件中bitMap的起始地址
    long bitMapSize;                            // bitMap的大小，以字节为单位
    long dataAddr;                              // 文件中数据区的起始位置
    int curFileNum;                             // 目前有多少个文件，最多为MAX_FILE_NUM
    struct FileDesc fileDesc[MAX_FILE_NUM];        // 对每一个文件进行描述
};

struct BufferBlock{
    long pageNo;//页号
    long visitTime;
    bool isEdited;
};

struct BufferSpace{
    struct BufferBlock map[SIZE_BUFF];
    char data[SIZE_BUFF][SIZE_PER_PAGE];
    long curTimeStamp;
};

 struct  DBMSHeader{
    struct SysDesc desc;
    unsigned long *freeSpace_bitMap;//空闲空间管理
    struct BufferSpace buffer;
    FILE *fpdesc;
};

//utils.cpp
void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);  //字符串分割函数

//initial.cpp
//配置系统参数，并写入文件 文件不存在时需创建一个新的文件，并对文件头进行初始化
void initHeader(char *filename);
//初始化数据库
void initDatabase(struct DBMSHeader *head, char *filename);
void showStatus(struct DBMSHeader *head);
void showFileDesc(struct DBMSHeader *head, int fid);

//fileOption.cpp
//创建数据库文件
int createFile(struct DBMSHeader *head, int type ,long reqPageNum);
//给定文件号 fid，查询其在数据库中的下标号
int queryFileIndex(struct DBMSHeader *head, int fid);
//找到合适的页面并将记录写入文件
void writeFile(struct DBMSHeader *head, int fid, int length, const char *str);
//读取文件内容
void readFile(struct DBMSHeader *head, int fid, char *des);
//删除文件
void deleteFile(struct DBMSHeader *head, int fid);

//buffManage.cpp
//查询页号query是否在缓冲区中，若在则返回它在缓冲区中的下标
int queryPage( struct DBMSHeader *head, long queryPageNo );
// 把下标为mapNo的缓冲块替换为页号pageNo的页
int replacePage( struct DBMSHeader *head, int mapNo, long pageNo );
//调度算法FIFO，替换出最久没有使用的页
int scheBuff( struct DBMSHeader *head );
//请求读写一个页，若不在缓冲区中，则调用scheBuff调到缓冲区中
int reqPage( struct DBMSHeader *head, long queryPageNo );

//pageOption.cpp
//读取偏移量信息
int getBit(unsigned long num, long pos);
//设置偏移量信息
int setBit( unsigned long *num, long pos, int setValue);
//分配页面
long allocatePage( struct DBMSHeader *head, long reqPageNum);
//回收某一页面
void recyOnePage( struct DBMSHeader *head ,long pageNo);
void recyAllPage(struct DBMSHeader *head);

#endif /* dbHead_h */
