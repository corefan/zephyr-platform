#include "../include/HeaderFile.h"

using namespace Zephyr;

#define TEST_CONST 6

int main(int argc,char *pArgv[])
{
    CHeaderFile file;
    int t = TEST_CONST - 1;
    file.GeneratorIdl("test.h");
    return t;
}