
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonMultipleInstance.h"
#include "CommonAvoidReentry.h"

#include <process.h>

class CCommonThread : public CCommonMultipleInstance<CCommonThread>
{
public:
    CCommonThread(
        _In_opt_ CONST LPSECURITY_ATTRIBUTES lpThreadAttributes,
        _In_ CONST SIZE_T dwStackSize,
        _In_ CONST _beginthreadex_proc_type lpStartAddress,
        _In_opt_ CONST _endthreadex_proc_type lpBeforeStopAddress,
        _In_opt_ CONST _endthreadex_proc_type lpAfterStopAddress,
        _In_opt_ CONST LPVOID lpParameter,
        _In_ CONST BOOL bAutoFreeMemory,
        _In_ CONST DWORD dwCreationFlags,
        _In_ CONST int nPriority,
        _In_ CONST BOOL bForceTerminateThread = FALSE
        );

    ~CCommonThread();

    static
        unsigned int
        WINAPI
        GetMaximumCallThreads();

    BOOL
        IsFinished();

private:
    HANDLE m_hThread;
    DWORD m_dwTid;
    BOOL m_Exit;
    BOOL m_bFinished;
    _beginthreadex_proc_type m_lpStartAddress;
    _endthreadex_proc_type m_lpBeforeStopAddress;
    _endthreadex_proc_type m_lpAfterStopAddress;
    LPVOID m_lpParameter;
    BOOL m_bAutoFreeMemory;
    BOOL m_bForceTerminateThread;

    static
        unsigned
        __stdcall
        ThreadInternal(
        _In_opt_ CONST LPVOID lpInstance
        );
};
