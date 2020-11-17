#include "dbHead.h"
#include <fstream>
using namespace std;

/** 配置系统参数，并写入文件中
* 这里所输入的文件，是专门用韵存放数据库的参数的文件
* 结构体中包含：数据区的大小（所有文件区域大小固定），每页的大小，总页数，当前可用页数，bitmap的起始地址（可能是以bit数开始，或者冗余的1024字节）
* bitmap大小（以字节为单位)。
* 数据库中的全部空闲块是由数据库进行管理
*/
void initHeader(char *filename){

    struct DBMSHeader *header=NULL;
     header= (DBMSHeader *)malloc(sizeof(DBMSHeader));
    header->desc.dataSpaceSize = SIZE_DATA_SPACE; // 存储文件中，数据区大小
    header->desc.perPageSize = SIZE_PER_PAGE;                    // 每页的大小
    header->desc.totalPage = SIZE_DATA_SPACE/SIZE_PER_PAGE;      // 总页数
    header->desc.avaliblePage =  header->desc.totalPage;          // 当前可用的页数，初始化时为totalPage
    header->desc.bitMapAddr =  ADDR_BITMAP;                      // bitMap在存储文件中的起始地址
    header->desc.bitMapSize = header->desc.totalPage/8;           // bitmap大小=数据区大小/(每一页大小*8)，因为bitmap的大小不是以bit为单位，而是Byte
    header->desc.dataAddr =  ADDR_DATA;                          // 数据区起始位置
    header->desc.curFileNum = 0;                                 // 当前文件数

    // 将文件描述数组每个元素置初始值为-1,表示当前没有文件
    memset(header->desc.fileDesc, -1, sizeof(struct FileDesc)*MAX_FILE_NUM);

    // 把初始化的SysDesc相关参数写到文件头部
    FILE *fp = fopen(filename, "wb");
    rewind(fp);
    fwrite(&(header->desc), sizeof(struct SysDesc), 1, fp);

    // 为空闲空间映射表分配空间，所有的初始化为-1，表示空闲(-1表示ffffffff)
    header->freeSpace_bitMap = (unsigned long *)malloc(header->desc.bitMapSize);
    memset(header->freeSpace_bitMap, -1, header->desc.bitMapSize);

    // 把bitMap写入到文件中
    rewind(fp);
    fseek(fp, header->desc.bitMapAddr, SEEK_SET);
    fwrite(header->freeSpace_bitMap, 1, header->desc.bitMapSize, fp);
    fclose(fp);
}

void initDatabase(struct DBMSHeader *header, char *filename){
    //检查并创建数据库文件
    printf("initdatabase!");
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == NULL){
        cout<<"开始创建数据库文件......"<<endl;
        initHeader(filename);
        fp = fopen(filename, "rb");
    }else{
        cout<<"数据库文件已创建!"<<endl;
        //return; //change
    }
    //初始化header,给bitMap分配空间，并初始化
    //这里出现了一段重复代码段，不知道是什么原因
    //因为如果数据库文件存在，那么这一段就有用了
    rewind(fp);
    fread(&(header->desc), sizeof(struct SysDesc), 1, fp);
    header->freeSpace_bitMap = (unsigned long *)malloc(header->desc.bitMapSize);
    rewind(fp);
    fseek(fp, header->desc.bitMapAddr, SEEK_SET); // 找到文件中写bitMap的位置，让fp指向这里
    fread(header->freeSpace_bitMap, 1, header->desc.bitMapSize, fp);// 从文件中读取bitMap的内容
    fclose(fp);

    // 初始化缓冲区，将每个缓冲区块都初始化（存储的文件的页号为-1，访问时间为0，标记为未修改过）
    for(int i = 0;i < SIZE_BUFF;i++) {
        header->buffer.map[i].pageNo = -1;
        header->buffer.map[i].visitTime = 0;
        header->buffer.map[i].isEdited = false;
        memset(header->buffer.data[i], 0, SIZE_PER_PAGE);
    }
    header->buffer.curTimeStamp = 0;

    //数据库指针，指向某个文件
    header->fpdesc = fopen(filename, "rb+");
}

void showStatus(struct DBMSHeader *head){
    cout<<"----------------------数据库状态--------------------"<<endl;
    cout<<"数据库总空间:"<<head->desc.dataSpaceSize/1024/1024<<"MB"<<endl;
    cout<<"每一页大小:"<<head->desc.perPageSize/1024<<"KB"<<endl;
    cout<<"总共的页数:"<<head->desc. totalPage<<endl;
    cout<<"位示图大小:"<<head->desc.bitMapSize<<"B"<<endl;
    cout<<"当前可用页数:"<<head->desc.avaliblePage<<endl;
    cout<<"当前文件数:"<<head->desc.curFileNum<<endl;
    cout<<"--------------------------------------------------"<<endl;
}

void showFileDesc(struct DBMSHeader *head,int fid) {
    int fx = queryFileIndex(head, fid);
    long pageNum = head->desc.fileDesc[fx].filePageNum;
    long pageNo = head->desc.fileDesc[fx].fileFirstPageNo;
    int type = head->desc.fileDesc[fx].fileType;
    cout<<"-------------------**********-------------------"<<endl;
    printf("文件类型:");
    if(type == DATA_FILE)
        printf("用户表文件\n");
    else
        printf("索引文件\n");
    printf("该文件一共占用：%ld页\n",pageNum);
    printf("该文件的起始页页号为：%ld\n",pageNo);
    cout<<"-------------------**********-------------------"<<endl;

}
