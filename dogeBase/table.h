#pragma once
#include "unistd.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <cstdlib>
#include <cstdio>
class table
{
    public:
        bool open(const char* dbname, bool readOnly);
        virtual void read(void* block, int offset, int len);
        virtual void write(void* block, int offset, int len);
        virtual bool growing(int size);
        virtual void close();
        int size;
        int fd;
    private:
};

