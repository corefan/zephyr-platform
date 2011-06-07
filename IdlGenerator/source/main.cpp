#include "../include/HeaderFile.h"
#include "../include/interfaceElement.h"
#include "../include/nameSpace.h"
#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/FullTypeClass.h"
#include "../include/baseElement.h"
#include "../../Public/tpl/include/TplPool.h"


using namespace Zephyr;

#define TEST_CONST 6

int main(int argc,char *pArgv[])
{
    CHeaderFile file;
    int t = TEST_CONST - 1;
    INIT_STATIC_POOL(CBaseElement,64);
    INIT_STATIC_POOL(CFullTypeDef,128);
    INIT_STATIC_POOL(CHeaderFile,16);
    INIT_STATIC_POOL(CInterfaceElement,32);
    INIT_STATIC_POOL(CMethod,128);
    INIT_STATIC_POOL(CNamespace,16);
    INIT_STATIC_POOL(CParamerter,16);

    file.GeneratorIdl("test.h");
    return t;
}