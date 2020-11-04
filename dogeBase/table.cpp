#include "table.h"
bool table::open(const char* dbname, bool readOnly)
{
    fd = ::open(dbname, readOnly ? O_RDONLY : O_RDWR);
    if (fd < 0)
    {
        fd = 0;
        return false;
    }
    size = lseek(fd, 0, SEEK_END);
    return true;
}

void table::close()
{
    if (fd != 0)
        ::close(fd);
}

void table::read(void* block, int offset, int len)
{
    pread(fd, block, len, offset);
}

void table::write(void* block, int offset, int len)
{
    pwrite(fd, block, len, offset);
}

bool table::growing(int size)
{
    return false;
}
