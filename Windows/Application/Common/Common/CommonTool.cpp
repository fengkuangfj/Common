
#include "CommonTool.h"

std::wstring
    CCommonTool::GenGuidRegistryFormat()
{
    std::wstring wstrRet = L"";

    GUID Guid = { 0 };
    WCHAR wchGuid[COMMON_MAX_PATH] = { 0 };


    do
    {
        Guid = GenGuidStructFormat();

        StringCchPrintf(wchGuid, _countof(wchGuid), L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
            Guid.Data1,
            Guid.Data2,
            Guid.Data3,
            Guid.Data4[0], Guid.Data4[1],
            Guid.Data4[2], Guid.Data4[3], Guid.Data4[4], Guid.Data4[5], Guid.Data4[6], Guid.Data4[7]
        );

        wstrRet = wchGuid;
    } while (FALSE);

    return wstrRet;
}

GUID
    CCommonTool::GenGuidStructFormat()
{
    GUID GuidRet = { 0 };

    HRESULT hResult = S_OK;


    do
    {
        hResult = CoCreateGuid(&GuidRet);
        if (S_OK != hResult)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CoCreateGuid failed. msdn(%d)", hResult);
            break;
        }
    } while (FALSE);

    return GuidRet;
}

std::wstring
    CCommonTool::ToWstring(
    _In_ CONST int & _Val
    )
{
    std::wstring wstrRet = L"";

    wchar_t wchTemp[COMMON_MAX_PATH] = {0};
    errno_t error = 0;


    do
    {
        error = _itow_s(_Val, wchTemp, _countof(wchTemp), 10 );
        if (0 != error)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"_itow_s (%d) failed. msdn(%d)", _Val, error);
            break;
        }

        wstrRet = wchTemp;
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonTool::ToWstring(
    _In_ CONST unsigned long & _Val
    )
{
    std::wstring wstrRet = L"";

    wchar_t wchTemp[COMMON_MAX_PATH] = {0};
    errno_t error = 0;


    do
    {
        error = _ultow_s(_Val, wchTemp, _countof(wchTemp), 10 );
        if (0 != error)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"_ultow_s (%d) failed. msdn(%d)", _Val, error);
            break;
        }

        wstrRet = wchTemp;
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonTool::ToWstring(
    _In_ CONST long long & _Val
    )
{
    std::wstring wstrRet = L"";

    wchar_t wchTemp[COMMON_MAX_PATH] = {0};
    errno_t error = 0;


    do
    {
        error = _i64tow_s(_Val, wchTemp, _countof(wchTemp), 10 );
        if (0 != error)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"_i64tow_s (%I64d) failed. msdn(%d)", _Val, error);
            break;
        }

        wstrRet = wchTemp;
    } while (FALSE);

    return wstrRet;
}

CCommonTool::CCommonTool()
{
    ;
}

CCommonTool::~CCommonTool()
{
    ;
}
