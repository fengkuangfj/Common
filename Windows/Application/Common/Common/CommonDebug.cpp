
#include "CommonDebug.h"

COMMON_ERROR
    CCommonDebug::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                CommonError = CCommonProcess::GetInstance()->Init();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonProcess::GetInstance()->Init failed. CommonError(%d)", CommonError);
                    break;
                }

                CommonError = CCommonService::GetInstance()->Init();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonService::GetInstance()->Init failed. CommonError(%d)", CommonError);
                    break;
                }

                m_bService = CCommonService::GetInstance()->CheckCurrentProcessIsService();
                if (m_bService)
                {
                    m_bCanInteractWithDesktop = CCommonService::GetInstance()->CheckCurrentServiceCanInteractWithDesktop();
                }

                m_wstrProcessName = CCommonProcess::GetInstance()->GetName();

                m_wstrProcessPath = CCommonProcess::GetInstance()->GetPath();
                if (4 <= m_wstrProcessPath.length()
                    && (0 == _wcsnicmp(m_wstrProcessPath.c_str() + 3, L"Windows\\SystemApps\\", wcslen(L"Windows\\SystemApps\\"))
                    || 0 == _wcsnicmp(m_wstrProcessPath.c_str() + 3, L"Program Files\\WindowsApps\\", wcslen(L"Program Files\\WindowsApps\\"))))
                {
                    m_bSystemApps = TRUE;
                }

                m_wstrParentProcessPath = CCommonProcess::GetInstance()->GetParentPath();
                if (4 <= m_wstrParentProcessPath.length()
                    && (0 == _wcsnicmp(m_wstrParentProcessPath.c_str() + 3, L"Windows\\SystemApps\\", wcslen(L"Windows\\SystemApps\\"))
                    || 0 == _wcsnicmp(m_wstrParentProcessPath.c_str() + 3, L"Program Files\\WindowsApps\\", wcslen(L"Program Files\\WindowsApps\\"))))
                {
                    m_bParentSystemApps = TRUE;
                }

                SetInitFlag(TRUE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

COMMON_ERROR
    CCommonDebug::Unload()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (GetInitFlag())
            {
                CommonError = CCommonService::GetInstance()->Unload();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonService::GetInstance()->Unload failed. CommonError(%d)", CommonError);
                }

                CommonError = CCommonProcess::GetInstance()->Unload();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonProcess::GetInstance()->Unload failed. CommonError(%d)", CommonError);
                }

                SetInitFlag(FALSE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

BOOL
    CCommonDebug::IsThisProcess(
    _In_ CONST std::wstring & wstrProcessName
    )
{
    BOOL bRet = FALSE;


    do
    {
        if (0 == _wcsicmp(wstrProcessName.c_str(), m_wstrProcessName.c_str()))
        {
            bRet = TRUE;
        }
    } while (FALSE);

    return bRet;
}

VOID
    CCommonDebug::InterruptForRemote(
    _In_ CONST std::wstring & wstrProcessName
    )
{
    std::wstring wstrText = L"";


    do
    {
        if (!IsThisProcess(wstrProcessName))
        {
            break;
        }

        wstrText = CCommonIp::GetInstance()->GetCurrentIp();
        wstrText += L"\r\n";
        wstrText += wstrProcessName;
        wstrText += L"\r\n";
        wstrText += CCommonTool::GetInstance()->ToWstring(GetCurrentProcessId());

        if (m_bService)
        {
            if (m_bCanInteractWithDesktop)
            {
                CCommonService::GetInstance()->MessageBoxForServiceW(wstrText.c_str(), L"ForRemote", MB_OK);
            }
            else
            {
                Sleep(1000 * 10);
            }
        }
        else
        {
            if (m_bSystemApps
                || m_bParentSystemApps
                || IsWorkUnderSystemApps())
            {
                Sleep(1000 * 10);
            }
            else
            {
                MessageBox(NULL, wstrText.c_str(), L"ForRemote", MB_OK);
            }
        }
    } while (FALSE);

    return;
}

BOOL
    CCommonDebug::IsSystemApps()
{
    return m_bSystemApps;
}

BOOL
    CCommonDebug::IsParentSystemApps()
{
    return m_bParentSystemApps;
}

BOOL
    CCommonDebug::IsWorkUnderSystemApps()
{
    BOOL bRet = FALSE;

    WCHAR wchTemp[COMMON_MAX_PATH] = {0};


    do
    {
        if (0 == GetCurrentDirectory(_countof(wchTemp), wchTemp))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetCurrentDirectory failed. msdn(%d)", GetLastError());
            break;
        }

        if (4 <= wcslen(wchTemp)
            && (0 == _wcsnicmp(wchTemp + 3, L"Windows\\SystemApps\\", wcslen(L"Windows\\SystemApps\\"))
            || 0 == _wcsnicmp(wchTemp + 3, L"Program Files\\WindowsApps\\", wcslen(L"Program Files\\WindowsApps\\"))))
        {
            bRet = TRUE;
        }
    } while (FALSE);

    return bRet;
}

CCommonDebug::CCommonDebug()
{
    m_bCanInteractWithDesktop = TRUE;
    m_bService = FALSE;
    m_wstrProcessName = L"";
    m_wstrProcessPath = L"";
    m_wstrParentProcessPath = L"";
    m_bSystemApps = FALSE;
    m_bParentSystemApps = FALSE;
}

CCommonDebug::~CCommonDebug()
{
    m_bCanInteractWithDesktop = TRUE;
    m_bService = FALSE;
    m_wstrProcessName = L"";
    m_wstrProcessPath = L"";
    m_wstrParentProcessPath = L"";
    m_bSystemApps = FALSE;
    m_bParentSystemApps = FALSE;
}
