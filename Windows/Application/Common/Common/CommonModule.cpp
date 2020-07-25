
#include "CommonModule.h"

std::wstring
    CCommonModule::GetPath(
    _In_ CONST HMODULE & hModule
    )
{
    std::wstring wstrRet = L"";

    WCHAR wchPath[COMMON_MAX_PATH] = { 0 };


    do
    {
        if (0 == GetModuleFileName(hModule, wchPath, _countof(wchPath)))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetModuleFileName failed. msdn(%d)", GetLastError());
            break;
        }

        wstrRet = CCommonPath::GetInstance()->ToLong(wchPath);

        CCommonStringConvert::GetInstance()->ToLower(wstrRet);
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonModule::GetName(
    _In_ CONST HMODULE & hModule
    )
{
    std::wstring wstrRet = L"";

    std::wstring wstrPath = L"";


    do
    {
        wstrPath = GetPath(hModule);
        if (!wstrPath.length())
        {
            break;
        }

        wstrRet = PathFindFileName(wstrPath.c_str());
    } while (FALSE);

    return wstrRet;
}

CCommonModule::CCommonModule()
{
    ;
}

CCommonModule::~CCommonModule()
{
    ;
}
