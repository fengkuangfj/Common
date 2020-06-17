
#include "CommonProcess.h"

std::wstring
    CCommonProcess::GetPath()
{
    std::wstring wstrRet = L"";


    do
    {
        wstrRet = CCommonModule::GetInstance()->GetPath(NULL);
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonProcess::GetName()
{
    std::wstring wstrRet = L"";


    do
    {
        wstrRet = CCommonModule::GetInstance()->GetName(NULL);
    } while (FALSE);

    return wstrRet;
}

CCommonProcess::CCommonProcess()
{
    ;
}

CCommonProcess::~CCommonProcess()
{
    ;
}
