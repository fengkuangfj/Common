
#include "CommonHookEnv.h"

CCommonHookEnv::CCommonHookEnv()
{
    m_dwErrorCode = 0;

    Store();
}

CCommonHookEnv::~CCommonHookEnv()
{
    Restore();

    m_dwErrorCode = 0;
}

void
    CCommonHookEnv::ModifyErrorCode(
    _In_ CONST DWORD & dwErrorCode
    )
{
    m_dwErrorCode = dwErrorCode;
}

void
    CCommonHookEnv::Store()
{
    m_dwErrorCode = GetLastError();
}

void
    CCommonHookEnv::Restore()
{
    SetLastError(m_dwErrorCode);
}

DWORD
    CCommonHookEnv::GetLastErrorStored()
{
    return m_dwErrorCode;
}
