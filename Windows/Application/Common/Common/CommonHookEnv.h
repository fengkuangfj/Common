
#pragma once

#include "CommonPublic.h"

class CCommonHookEnv
{
public:
    CCommonHookEnv();

    ~CCommonHookEnv();

    void
        ModifyErrorCode(
        _In_ CONST DWORD & dwErrorCode
        );

    void
        Store();

    void
        Restore();

    DWORD
        GetLastErrorStored();

private:
    DWORD m_dwErrorCode;
};
