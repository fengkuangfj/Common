
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


    do
    {
        hRet = CreateFileHandlingFlag(wstrPath, bConvertFormat);
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


    do
    {
        hEvent = CreateFileHandlingFlag(wstrPath, bConvertFormat);
        if (NULL == hEvent)
        {
            break;
        }

        if (ERROR_ALREADY_EXISTS != GetLastError())
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

HANDLE
    CCommonEvent::CreateFileHandlingFlag(
    _In_ CONST std::wstring & wstrPath,
    _In_ CONST BOOL & bConvertFormat
    )
{
    HANDLE hRet = NULL;

    std::wstring wstrEvent = L"";


    do
    {
        if (!wstrPath.length())
        {
            break;
        }

        wstrEvent = wstrPath;

        if (bConvertFormat)
        {
            wstrEvent = CCommonPath::GetInstance()->ToLong(wstrEvent);

            CCommonStringConvert::GetInstance()->ToLower(wstrEvent);
        }

        CCommonStringConvert::GetInstance()->Replace(wstrEvent, L"\\", L"_");

        wstrEvent += L"_";
        wstrEvent += L"{D7932C6D-9115-491A-B713-7BDE7C05667F}";

        hRet = CreateEvent(NULL, FALSE, FALSE, wstrEvent.c_str());
        if (NULL == hRet)
        {
            break;
        }
    } while (FALSE);

    return hRet;
}
