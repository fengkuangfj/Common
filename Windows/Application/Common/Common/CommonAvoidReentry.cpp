
#include "CommonAvoidReentry.h"

BOOL
    CCommonAvoidReentry::IsReentry()
{
    BOOL bRet = FALSE;

    DWORD dwCurrentTid = 0;


    do
    {
        if (!GetLock(dwCurrentTid))
        {
            bRet = TRUE;
            break;
        }

        bRet = (0 < m_AvoidReentryInfos[dwCurrentTid]);
    } while (FALSE);

    FreeLock(dwCurrentTid);

    return bRet;
}

VOID
    CCommonAvoidReentry::Add()
{
    DWORD dwCurrentTid = 0;


    do
    {
        GetLock(dwCurrentTid);

        if (0 < m_AvoidReentryInfos[dwCurrentTid])
        {
            (m_AvoidReentryInfos[dwCurrentTid])++;
        }
        else
        {
            m_AvoidReentryInfos[dwCurrentTid] = 1;
        }
    } while (FALSE);

    FreeLock(dwCurrentTid);

    return;
}

VOID
    CCommonAvoidReentry::Add(
    _In_ CONST DWORD & dwTid
    )
{
    DWORD dwCurrentTid = 0;


    do
    {
        GetLock(dwCurrentTid);

        if (0 < m_AvoidReentryInfos[dwTid])
        {
            (m_AvoidReentryInfos[dwTid])++;
        }
        else
        {
            m_AvoidReentryInfos[dwTid] = 1;
        }
    } while (FALSE);

    FreeLock(dwCurrentTid);

    return;
}

VOID
    CCommonAvoidReentry::Delete()
{
    DWORD dwCurrentTid = 0;


    do
    {
        GetLock(dwCurrentTid);

        if (0 < m_AvoidReentryInfos[dwCurrentTid])
        {
            --(m_AvoidReentryInfos[dwCurrentTid]);
        }
    } while (FALSE);

    FreeLock(dwCurrentTid);

    return;
}

VOID
    CCommonAvoidReentry::Delete(
    _In_ CONST DWORD & dwTid
    )
{
    DWORD dwCurrentTid = 0;


    do
    {
        GetLock(dwCurrentTid);

        if (0 < m_AvoidReentryInfos[dwTid])
        {
            --(m_AvoidReentryInfos[dwTid]);
        }
    } while (FALSE);

    FreeLock(dwCurrentTid);

    return;
}

CCommonAvoidReentry::CCommonAvoidReentry()
{
    m_lTidGetedLock = 0;
    m_lLockRef = 0;
    ZeroMemory(m_AvoidReentryInfos, sizeof(m_AvoidReentryInfos));
}

CCommonAvoidReentry::~CCommonAvoidReentry()
{
    m_lTidGetedLock = 0;
    m_lLockRef = 0;
    ZeroMemory(m_AvoidReentryInfos, sizeof(m_AvoidReentryInfos));
}

BOOL
    CCommonAvoidReentry::GetLock(
    _Inout_ DWORD & dwCurrentTid
    )
{
    BOOL bRet = FALSE;

    LONG lResult = 0;
    int nThreadPriority = GetThreadPriority(GetCurrentThread());

    dwCurrentTid = GetCurrentThreadId();

    do
    {
        lResult = InterlockedCompareExchange(&m_lTidGetedLock, (LONG)dwCurrentTid, 0);
        if (0 == lResult)
        {
            // m_lTidGetedLock from 0 to dwCurrentTid
            InterlockedIncrement(&m_lLockRef);
            bRet = TRUE;
            break;
        }
        else
        {
            if ((LONG)dwCurrentTid == lResult)
            {
                // m_lTidGetedLock now is dwCurrentTid
                InterlockedIncrement(&m_lLockRef);
                break;
            }
        }

        if (THREAD_PRIORITY_NORMAL < nThreadPriority)
        {
            Sleep(1);
        }
    } while (TRUE);

    return bRet;
}

VOID
    CCommonAvoidReentry::FreeLock(
    _In_ CONST DWORD & dwCurrentTid
    )
{
    do
    {
        if (0 == InterlockedDecrement(&m_lLockRef))
        {
            InterlockedCompareExchange(&m_lTidGetedLock, 0, (LONG)dwCurrentTid);
        }
    } while (FALSE);

    return;
}
