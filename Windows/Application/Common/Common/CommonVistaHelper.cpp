
#include "CommonVistaHelper.h"

COMMON_ERROR
    CCommonVistaHelper::Init()
{
    COMMON_ERROR SsDedsError = COMMON_ERROR_DEFAULT;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                if (CCommonOperationSystem::GetInstance()->IsWindowsVistaOrGreater())
                {
                    m_hShell32 = GetModuleHandle(L"Shell32.dll");
                    if (NULL == m_hShell32)
                    {
                        SsDedsError = COMMON_ERROR_GET_MODULE_HANDLE_FAILED;
                        COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"GetModuleHandle (%s) failed. msdn(%d)", L"Shell32.dll", GetLastError());
                    }
                    else
                    {
                        m_Shell32_dll_SHCreateItemFromParsingName = (Shell32_dll_SHCreateItemFromParsingName_Proc_Type)GetProcAddress(m_hShell32, "SHCreateItemFromParsingName");
                        if (NULL == m_Shell32_dll_SHCreateItemFromParsingName)
                        {
                            SsDedsError = COMMON_ERROR_GET_PROC_ADDRESS_FAILED;
                            COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"GetProcAddress (%s) failed. msdn(%d)", L"SHCreateItemFromParsingName", GetLastError());
                        }
                    }
                }

                SetInitFlag(TRUE);
            }
        }

        SsDedsError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    if (COMMON_ERROR_SUCCESS != SsDedsError)
    {
        SsDedsError = Unload();
    }

    return SsDedsError;
}

COMMON_ERROR
    CCommonVistaHelper::Unload()
{
    COMMON_ERROR SsDedsError = COMMON_ERROR_DEFAULT;


    do
    {
        if (GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (GetInitFlag())
            {
                SetInitFlag(FALSE);
            }
        }

        SsDedsError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return SsDedsError;
}

HRESULT
    CCommonVistaHelper::SHCreateItemFromParsingName(
    __in PCWSTR pszPath,
    __in_opt IBindCtx *pbc,
    __in REFIID riid,
    __deref_out void **ppv
    )
{
    if (NULL != m_Shell32_dll_SHCreateItemFromParsingName)
    {
        return m_Shell32_dll_SHCreateItemFromParsingName(
            pszPath,
            pbc,
            riid,
            ppv
            );
    }
    else
    {
        SetLastError(ERROR_NOT_SUPPORTED);
        return S_FALSE;
    }
}

CCommonVistaHelper::CCommonVistaHelper()
{
    m_hShell32= NULL;
    m_Shell32_dll_SHCreateItemFromParsingName= NULL;
}

CCommonVistaHelper::~CCommonVistaHelper()
{
    m_hShell32= NULL;
    m_Shell32_dll_SHCreateItemFromParsingName= NULL;
}
