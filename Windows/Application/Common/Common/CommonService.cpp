
#include "CommonService.h"

CCommonService::CCommonService()
{
    ;
}

CCommonService::~CCommonService()
{
    ;
}

COMMON_ERROR
    CCommonService::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                CommonError = CCommonOperationSystem::GetInstance()->Init();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonOperationSystem::GetInstance()->Init failed. CommonError(%d)", CommonError);
                    break;
                }

                SetInitFlag(TRUE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

COMMON_ERROR
    CCommonService::Unload()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (GetInitFlag())
            {
                CommonError = CCommonOperationSystem::GetInstance()->Unload();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonOperationSystem::GetInstance()->Unload failed. CommonError(%d)", CommonError);
                }

                SetInitFlag(FALSE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

BOOL
    CCommonService::CheckCurrentProcessIsService()
{
    BOOL bRet = FALSE;

    DWORD dwSessionId = 0;
    SC_HANDLE hScManager = NULL;
    LPENUM_SERVICE_STATUS_PROCESS lpServices = NULL;
    DWORD dwcbBytesNeeded = 0;
    DWORD dwServicesReturned = 0;


    do
    {
        if (CCommonOperationSystem::GetInstance()->IsWindowsVistaOrGreater())
        {
            dwSessionId = CCommonSession::GetInstance()->GetCurrentProcessSessionId();
            if (0 == dwSessionId)
            {
                bRet = TRUE;
            }
        }
        else
        {
            hScManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
            if (NULL == hScManager)
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"OpenSCManager failed. msdn(%d)", GetLastError());
                break;
            }

#pragma warning(push)
#pragma warning(disable : 6031)
            EnumServicesStatusEx(hScManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (LPBYTE)lpServices, dwcbBytesNeeded, &dwcbBytesNeeded, &dwServicesReturned, NULL, NULL);
#pragma warning(pop)
            if (0 == dwcbBytesNeeded)
            {
                break;
            }

            lpServices = (LPENUM_SERVICE_STATUS_PROCESS)calloc(1, dwcbBytesNeeded);
            if (NULL == lpServices)
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
                break;
            }

            if (!EnumServicesStatusEx(hScManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (LPBYTE)lpServices, dwcbBytesNeeded, &dwcbBytesNeeded, &dwServicesReturned, NULL, NULL))
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"EnumServicesStatusEx failed. msdn(%d)", GetLastError());
                break;
            }

            for (DWORD i = 0; i < dwServicesReturned; i++)
            {
                if (GetCurrentProcessId() == lpServices[i].ServiceStatusProcess.dwProcessId)
                {
                    bRet = TRUE;
                    break;
                }
            }
        }
    } while (FALSE);

    if (NULL != lpServices)
    {
        free(lpServices);
        lpServices = NULL;
    }

    return bRet;
}

BOOL
    CCommonService::CheckCurrentServiceCanInteractWithDesktop()
{
    BOOL bRet = TRUE;

    DWORD dwPid = 0;


    do
    {
        dwPid = GetCurrentProcessId();
        if (0 == dwPid)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetCurrentProcessId failed. msdn(%d)", GetLastError());
            break;
        }

        bRet = CheckCanInteractWithDesktop(dwPid);
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonService::CheckCanInteractWithDesktop(
    _In_ CONST DWORD & dwPid
    )
{
    BOOL bRet = FALSE;

    SC_HANDLE hScManager = NULL;
    LPENUM_SERVICE_STATUS_PROCESS lpServices = NULL;
    DWORD dwcbBytesNeeded = 0;
    DWORD dwServicesReturned = 0;


    do
    {
        if (0 == dwPid)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        hScManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
        if (NULL == hScManager)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"OpenSCManager failed. msdn(%d)", GetLastError());
            break;
        }

#pragma warning(push)
#pragma warning(disable : 6031)
        EnumServicesStatusEx(hScManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (LPBYTE)lpServices, dwcbBytesNeeded, &dwcbBytesNeeded, &dwServicesReturned, NULL, NULL);
#pragma warning(pop)
        if (0 == dwcbBytesNeeded)
        {
            break;
        }

        lpServices = (LPENUM_SERVICE_STATUS_PROCESS)calloc(1, dwcbBytesNeeded);
        if (NULL == lpServices)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        if (!EnumServicesStatusEx(hScManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (LPBYTE)lpServices, dwcbBytesNeeded, &dwcbBytesNeeded, &dwServicesReturned, NULL, NULL))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"EnumServicesStatusEx failed. msdn(%d)", GetLastError());
            break;
        }

        for (DWORD i = 0; i < dwServicesReturned; i++)
        {
            if (dwPid == lpServices[i].ServiceStatusProcess.dwProcessId)
            {
                if (SERVICE_INTERACTIVE_PROCESS == (SERVICE_INTERACTIVE_PROCESS & lpServices[i].ServiceStatusProcess.dwServiceType))
                {
                    bRet = TRUE;
                }

                break;
            }
        }
    } while (FALSE);

    if (NULL != lpServices)
    {
        free(lpServices);
        lpServices = NULL;
    }

    return bRet;
}

BOOL
    CCommonService::MessageBoxForServiceA(
    _In_ CONST std::string & strText,
    _In_ CONST std::string & strCaption,
    _In_ CONST UINT & uType
    )
{
    DWORD dwResponse = 0;

    return WTSSendMessageA(
        WTS_CURRENT_SERVER_HANDLE,
        WTSGetActiveConsoleSessionId(),
        (LPSTR)strCaption.c_str(),
        (DWORD)strCaption.length() * sizeof(CHAR),
        (LPSTR)strText.c_str(),
        (DWORD)strText.length() * sizeof(CHAR),
        uType | MB_SERVICE_NOTIFICATION,
        0,
        &dwResponse,
        TRUE
        );
}

BOOL
    CCommonService::MessageBoxForServiceW(
    _In_ CONST std::wstring & wstrText,
    _In_ CONST std::wstring & wstrCaption,
    _In_ CONST UINT & uType
    )
{
    DWORD dwResponse = 0;

    return WTSSendMessageW(
        WTS_CURRENT_SERVER_HANDLE,
        WTSGetActiveConsoleSessionId(),
        (LPTSTR)wstrCaption.c_str(),
        (DWORD)wstrCaption.length() * sizeof(WCHAR),
        (LPTSTR)wstrText.c_str(),
        (DWORD)wstrText.length() * sizeof(WCHAR),
        uType | MB_SERVICE_NOTIFICATION,
        0,
        &dwResponse,
        TRUE
        );
}
