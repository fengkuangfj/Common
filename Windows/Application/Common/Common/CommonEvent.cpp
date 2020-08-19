
#include "CommonEvent.h"

COMMON_ERROR
    CCommonEvent::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                SetInitFlag(TRUE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

COMMON_ERROR
    CCommonEvent::Unload()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (GetInitFlag())
            {
                SetInitFlag(FALSE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

HANDLE
    CCommonEvent::SetFileHandlingFlag(
    _In_ CONST std::wstring & wstrPath,
    _In_ CONST BOOL & bConvertFormat /*= TRUE */
    )
{
    HANDLE hRet = NULL;

    std::wstring wstrEvent = L"";


    do
    {
        wstrEvent = GetFileHandlingEventName(wstrPath, bConvertFormat);
        if (!wstrEvent.length())
        {
            break;
        }

        hRet = CreateEvent(NULL, FALSE, FALSE, wstrEvent.c_str());
    } while (FALSE);

    return hRet;
}

VOID
    CCommonEvent::ClearFileHandlingFlag(
    _Inout_ HANDLE & hEvent
    )
{
    do
    {
        if (NULL != hEvent)
        {
            CloseHandle(hEvent);
            hEvent = NULL;
        }
    } while (FALSE);

    return;
}

BOOL
    CCommonEvent::CheckFileHandlingFlag(
    _In_ CONST std::wstring & wstrPath,
    _In_ CONST BOOL & bConvertFormat /*= TRUE */
    )
{
    BOOL bRet = FALSE;

    HANDLE hEvent = NULL;
    std::wstring wstrEvent = L"";


    do
    {
        wstrEvent = GetFileHandlingEventName(wstrPath, bConvertFormat);
        if (!wstrEvent.length())
        {
            break;
        }

        hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, wstrEvent.c_str());
        if (NULL == hEvent)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    ClearFileHandlingFlag(hEvent);

    return bRet;
}

CCommonEvent::CCommonEvent()
{
    ;
}

CCommonEvent::~CCommonEvent()
{
    ;
}

std::wstring
    CCommonEvent::GetFileHandlingEventName(
    _In_ CONST std::wstring & wstrPath,
    _In_ CONST BOOL & bConvertFormat
    )
{
    std::wstring wstrRet = L"";


    do
    {
        if (!wstrPath.length())
        {
            break;
        }

        wstrRet = wstrPath;

        if (bConvertFormat)
        {
            wstrRet = CCommonPath::GetInstance()->ToLong(wstrRet);

            CCommonStringConvert::GetInstance()->ToLower(wstrRet);
        }

        CCommonStringConvert::GetInstance()->Replace(wstrRet, L"\\", L"_");

        wstrRet += L"_";
        wstrRet += L"{D7932C6D-9115-491A-B713-7BDE7C05667F}";
    } while (FALSE);

    return wstrRet;
}
