
#pragma once

#include "CommonPublic.h"

#include "CommonSingleInstance.h"

class CCommonAvoidReentry : public CCommonSingleInstance<CCommonAvoidReentry>
{
    friend class CCommonSingleInstance<CCommonAvoidReentry>;

public:
    BOOL
        IsReentry();

    BOOL
        IsReentryForPost();

    VOID
        Add();

    VOID
        Add(
        _In_ CONST DWORD & dwTid
        );

    VOID
        Delete();

    VOID
        Delete(
        _In_ CONST DWORD & dwTid
        );

private:
    int m_AvoidReentryInfos[1000000];
    LONG m_lTidGetedLock;
    LONG m_lLockRef;

    CCommonAvoidReentry();

    ~CCommonAvoidReentry();

    BOOL
        GetLock(
        _Inout_ DWORD & dwCurrentTid
        );

    VOID
        FreeLock(
        _In_ CONST DWORD & dwCurrentTid
        );
};
