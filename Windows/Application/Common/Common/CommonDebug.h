
#pragma once

#include "CommonPublic.h"

#include "CommonProcess.h"
#include "CommonService.h"
#include "CommonSingleInstance.h"

#pragma comment(lib, "User32.lib")

class CCommonDebug : public CCommonSingleInstance<CCommonDebug>
{
    friend class CCommonSingleInstance<CCommonDebug>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    BOOL
        IsThisProcess(
        _In_ CONST std::wstring & wstrProcessName
        );

    VOID
        InterruptForRemote(
        _In_ CONST std::wstring & wstrProcessName
        );

    BOOL
        IsSystemApps();

    BOOL
        IsParentSystemApps();

    BOOL
        IsWorkUnderSystemApps();

private:
    BOOL m_bCanInteractWithDesktop;
    BOOL m_bService;
    std::wstring m_wstrProcessName;
    std::wstring m_wstrProcessPath;
    std::wstring m_wstrParentProcessPath;
    BOOL m_bSystemApps;
    BOOL m_bParentSystemApps;

    CCommonDebug();

    ~CCommonDebug();
};
