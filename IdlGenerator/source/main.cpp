#include "../include/HeaderFile.h"
#include "../include/interfaceElement.h"
#include "../include/nameSpace.h"
#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/FullTypeClass.h"
#include "../include/baseElement.h"
#include "../include/Struct.h"
#include "../../Public/tpl/include/TplPool.h"
#include "iostream"

using namespace Zephyr;

using namespace std;

#define TEST_CONST 6
using namespace std;

int main(int argc,char *pArgv[])
{
    char szFileName[128];
    char szPath[256];
    if (argc != 3)
    {
        cout<<"Input Interface fileName:";
        cin>>szFileName;
        cout<<"Input outPut path:";
        cin>>szPath;
        //return -1;
    }
    else
    {
        strncpy_s(szFileName,pArgv[1],sizeof(szFileName));
        strncpy_s(szPath,pArgv[2],sizeof(szPath));
    }
    CHeaderFile file;
    INIT_STATIC_POOL(CBaseElement,64);
    INIT_STATIC_POOL(CFullTypeDef,128);
    INIT_STATIC_POOL(CHeaderFile,16);
    INIT_STATIC_POOL(CInterfaceElement,32);
    INIT_STATIC_POOL(CMethod,128);
    INIT_STATIC_POOL(CNamespace,16);
    INIT_STATIC_POOL(CParamerter,16);
    INIT_STATIC_POOL(CStruct,16);
    CBaseElement::InitTypeMap();
    file.GeneratorIdl(szFileName,szPath);
    return 1;
}