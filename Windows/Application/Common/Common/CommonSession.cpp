
#include "CommonSession.h"

DWORD
    CCommonSession::GetCurrentProcessSessionId()
{
    DWORD dwRet = 0xFFFFFFFF;

    DWORD dwPid = 0;


    do
    {
        dwPid = GetCurrentProcessId();
        if (0 == dwPid)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetCurrentProcessId failed. msdn(%d)", GetLastError());
            break;
        }

        if (!ProcessIdToSessionId(dwPid, &dwRet))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"ProcessIdToSessionId (%d) failed. msdn(%d)", dwPid, GetLastError());
            break;
        }
    } while (FALSE);

    return dwRet;
}

CCommonSession::CCommonSession()
{
    ;
}

CCommonSession::~CCommonSession()
{
    ;
}
