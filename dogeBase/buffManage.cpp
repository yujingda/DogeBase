#include "dbHead.h"
#include<stdio.h>
using namespace std;

//查询一页是否在缓冲区中，若在，则返回它在缓冲区中的下标
int queryPage( struct DBMSHeader *head, long queryPageNo){
    for(int i=0; i < SIZE_BUFF; i++ ) {
        if((head->buffer).map[i].pageNo == queryPageNo) {
            return i;
        }
    }
    return BUFF_NOT_HIT;
}

//将下标为mapNo的缓冲区块 替换为 页号为pageNo的页
int replacePage( struct DBMSHeader *head, int mapNo, long pageNo){
    //该缓冲区块已被编辑，需要写回磁盘中
    if( (head->buffer).map[mapNo].isEdited == true) {
        rewind(head->fpdesc);
        fseek(head->fpdesc, head->desc.dataAddr + (head->buffer).map[mapNo].pageNo * SIZE_PER_PAGE, SEEK_SET );
        fwrite(head->buffer.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc );
    }
    rewind(head->fpdesc);
    fseek( head->fpdesc, head->desc.dataAddr + pageNo * SIZE_PER_PAGE, SEEK_SET  );
    fread( head->buffer.data[mapNo], sizeof(char), SIZE_PER_PAGE, head->fpdesc );
    head->buffer.map[mapNo].isEdited = false;
    head->buffer.map[mapNo].pageNo = pageNo;
    return 0;
}

//调度算法，替换出最久没有使用的缓冲区块，返回下标号
int scheBuff( struct DBMSHeader *head ){
    int min = 0;
    for(int i = 0; i < SIZE_BUFF; i++ ){
        //该缓冲区块空闲，可直接分配
        if( head->buffer.map[i].pageNo < 0 ) {
            return i;
        }
        else {
            if( head->buffer.map[i].visitTime < head->buffer.map[min].visitTime ) {
                min = i;
            }
        }
    }
    return min;
}


//请求读写一个页，若该页不在缓冲区中，则调用替换算法把该页调到缓冲区中，返回该页在缓冲区中的下标
int reqPage( struct DBMSHeader *head, long queryPageNo){
    //cout<<"请求读写页面"<<endl;
    int mapNo = queryPage( head, queryPageNo );
    //若不在缓冲区内，则调度，找一块替换
    if( mapNo == BUFF_NOT_HIT ) {
        cout<<"不在缓冲区内，进行调度"<<endl;
        mapNo = scheBuff( head );
        cout<<"被替换的页："<<mapNo<<endl;
        replacePage( head, mapNo, queryPageNo );
        cout<<"加载到缓冲区成功！"<<endl;
    }
    head->buffer.curTimeStamp++;
    head->buffer.map[mapNo].visitTime = head->buffer.curTimeStamp;

    //???
    //不理解的代码，似乎是重新初始化
    if( (1<<30) == head->buffer.curTimeStamp ) {
        for(int i=0; i < SIZE_BUFF; i++ ) {
            head->buffer.map[i].visitTime = 0;
            head->buffer.curTimeStamp = 0;//移出循环
        }
    }
    return mapNo;
}
