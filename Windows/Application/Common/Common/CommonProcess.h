
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonModule.h"
#include "CommonSingleInstance.h"

class CCommonProcess : public CCommonSingleInstance<CCommonProcess>
{
    friend class CCommonSingleInstance<CCommonProcess>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    std::wstring
        GetPath();

    std::wstring
        GetName();

    std::wstring
        GetPath(
        _In_ CONST DWORD & dwPid
        );

    std::wstring
        GetName(
        _In_ CONST DWORD & dwPid
        );

    DWORD
        GetParentPid( );

    std::wstring
        GetParentPath( );

private:
    CCommonProcess();

    ~CCommonProcess();
};
