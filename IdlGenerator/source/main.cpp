#include "../include/HeaderFile.h"

using namespace Zephyr;
int main(int argc,char *pArgv[])
{
    CHeaderFile file;
    file.GeneratorIdl("test.h");
    return 0;
}