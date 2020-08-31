
#include "CommonThread.h"

CCommonThread::CCommonThread(
    _In_opt_ CONST LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ CONST SIZE_T dwStackSize,
    _In_ CONST _beginthreadex_proc_type lpStartAddress,
    _In_opt_ CONST _endthreadex_proc_type lpBeforeStopAddress,
    _In_opt_ CONST _endthreadex_proc_type lpAfterStopAddress,
    _In_opt_ CONST LPVOID lpParameter,
    _In_ CONST BOOL bAutoFreeMemory,
    _In_ CONST DWORD dwCreationFlags,
    _In_ CONST int nPriority,
    _In_ CONST BOOL bForceTerminateThread
    )
{
    m_hThread = NULL;
    m_dwTid = 0;
    m_Exit = FALSE;
    m_lpStartAddress = NULL;
    m_lpBeforeStopAddress = NULL;
    m_lpAfterStopAddress = NULL;
    m_lpParameter = NULL;
    m_bAutoFreeMemory = FALSE;
    m_bForceTerminateThread = FALSE;
    m_bFinished = FALSE;

    BOOL bAdded = FALSE;
    BOOL bResult = FALSE;


    do
    {
        this-> m_lpStartAddress = lpStartAddress;
        this-> m_lpBeforeStopAddress = lpBeforeStopAddress;
        this-> m_lpAfterStopAddress = lpAfterStopAddress;
        this->m_lpParameter = lpParameter;
        this->m_bAutoFreeMemory = bAutoFreeMemory;
        this->m_bForceTerminateThread = bForceTerminateThread;

        m_hThread = (HANDLE)_beginthreadex(
            lpThreadAttributes,
            (unsigned int)dwStackSize,
            CCommonThread::ThreadInternal,
            this,
            dwCreationFlags | CREATE_SUSPENDED,
            (unsigned*)&m_dwTid
            );
        if (NULL == m_hThread)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"_beginthreadex failed. msdn(%d)", GetLastError());
            break;
        }

        if (!SetThreadPriority(m_hThread, nPriority))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"SetThreadPriority failed. msdn(%d)", GetLastError());
            break;
        }

        CCommonAvoidReentry::GetInstance()->Add(m_dwTid);

        bAdded =TRUE;

        if (-1 == ResumeThread(m_hThread))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"ResumeThread failed. msdn(%d)", GetLastError());
            break;
        }

        bResult = TRUE;
    } while (FALSE);

    if (!bResult && bAdded)
    {
        CCommonAvoidReentry::GetInstance()->Delete(m_dwTid);
    }
}

CCommonThread::~CCommonThread()
{
    if (NULL != m_lpBeforeStopAddress)
    {
        m_lpBeforeStopAddress(m_lpParameter);
    }

    if (NULL != m_hThread)
    {
        if (m_bForceTerminateThread)
        {
#pragma warning(push)
#pragma warning(disable : 6258)
            TerminateThread(m_hThread, 0);
#pragma warning(pop)
        }

        WaitForSingleObject(m_hThread, INFINITE);

        CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    if (NULL != m_lpAfterStopAddress)
    {
        m_lpAfterStopAddress(m_lpParameter);
    }

    if (m_bAutoFreeMemory)
    {
        if (NULL != m_lpParameter)
        {
            free(m_lpParameter);
            m_lpParameter = NULL;
        }
    }

    m_hThread = NULL;
    m_dwTid = 0;
    m_Exit = FALSE;
    m_lpStartAddress = NULL;
    m_lpBeforeStopAddress = NULL;
    m_lpAfterStopAddress = NULL;
    m_lpParameter = NULL;
    m_bAutoFreeMemory = FALSE;
    m_bFinished = FALSE;
}

unsigned int
    CCommonThread::GetMaximumCallThreads()
{
    int nRet = 0;

    SYSTEM_INFO SystemInfo = { 0 };


    do
    {
        GetSystemInfo(&SystemInfo);

        nRet = SystemInfo.dwNumberOfProcessors;
        if (0 != nRet)
        {
            break;
        }

        nRet = 1;
    } while (FALSE);

    return nRet;
}

BOOL
    CCommonThread::IsFinished()
{
    return m_bFinished;
}

unsigned
    __stdcall
    CCommonThread::ThreadInternal(
    _In_opt_ CONST LPVOID lpInstance
    )
{
    unsigned uRet = 0;

    CCommonThread * pInstance = NULL;


    do
    {
        if (NULL == lpInstance)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        pInstance = ( CCommonThread *)lpInstance;

        uRet = pInstance->m_lpStartAddress(pInstance->m_lpParameter);

        pInstance->m_bFinished = TRUE;
    } while (FALSE);

    CCommonAvoidReentry::GetInstance()->Delete();

    return uRet;
}
