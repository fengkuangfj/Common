
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonNtHelper.h"
#include "CommonSingleInstance.h"

class CCommonOperationSystem : public CCommonSingleInstance<CCommonOperationSystem>
{
    friend class CCommonSingleInstance<CCommonOperationSystem>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    BOOL
        IsWindowsXPOrGreater();

    BOOL
        IsWindowsXPSP1OrGreater();

    BOOL
        IsWindowsXPSP2OrGreater();

    BOOL
        IsWindowsXPSP3OrGreater();

    BOOL
        IsWindowsVistaOrGreater();

    BOOL
        IsWindowsVistaSP1OrGreater();

    BOOL
        IsWindowsVistaSP2OrGreater();

    BOOL
        IsWindows7OrGreater();

    BOOL
        IsWindows7SP1OrGreater();

    BOOL
        IsWindows8OrGreater();

    BOOL
        IsWindows8Point1OrGreater();

    BOOL
        IsWindows10OrGreater();

    BOOL
        IsWindowsServerOrGreater();

    BOOL
        IsProcessor64();

    BOOL
        IsWindowsXP();

private:
    WORD m_wServicePackMajor;
    BYTE m_wProductType;
    float m_VersionNumber;

    CCommonOperationSystem();

    ~CCommonOperationSystem();
};
