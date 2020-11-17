#include"block.h"
#include"qujian.h"
#include"record.h"
#include"segment.h"
#include"table.h"
#include"tablespace.h"

int main(int argc, char* argv[])
{
	FILE* testtable = fopen(argv[1], "rt");
	char temp[100];
	memset(temp, 0, 100);
	char* c = fgets(temp, 100, testtable);
	printf("%s\n", temp);
	system("pause");

}