
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonOperationSystem.h"
#include "CommonSession.h"
#include "CommonSingleInstance.h"

#include <wtsapi32.h>
//
// #pragma comment(lib, "Wtsapi32.lib")

class CCommonService : public CCommonSingleInstance<CCommonService>
{
    friend class CCommonSingleInstance<CCommonService>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    BOOL
        CheckCurrentProcessIsService();

    BOOL
        CheckCurrentServiceCanInteractWithDesktop();

    BOOL
        CCommonService::CheckCanInteractWithDesktop(
        _In_ CONST DWORD & dwPid
        );

    BOOL
        MessageBoxForServiceA(
        _In_ CONST std::string & strText,
        _In_ CONST std::string & strCaption,
        _In_ CONST UINT & uType
        );

    BOOL
        MessageBoxForServiceW(
        _In_ CONST std::wstring & wstrText,
        _In_ CONST std::wstring & wstrCaption,
        _In_ CONST UINT & uType
        );

private:
    CCommonService();

    ~CCommonService();
};
