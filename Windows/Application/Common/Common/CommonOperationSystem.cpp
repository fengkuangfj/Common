
#include "CommonOperationSystem.h"

COMMON_ERROR
    CCommonOperationSystem::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;

    OSVERSIONINFOEX OsVersionInfoEx = { 0 };
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuildNumber = 0;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                CCommonNtHelper::GetInstance()->RtlGetNtVersionNumbers(&dwMajorVersion, &dwMinorVersion, &dwBuildNumber);
                m_VersionNumber = (float)dwMajorVersion + (float)dwMinorVersion / 10;

                OsVersionInfoEx.dwOSVersionInfoSize = sizeof(OsVersionInfoEx);
                if (!GetVersionEx((LPOSVERSIONINFO)&OsVersionInfoEx))
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetVersionEx failed. msdn(%d)", GetLastError());
                    break;
                }

                m_wServicePackMajor = OsVersionInfoEx.wServicePackMajor;
                m_wProductType = OsVersionInfoEx.wProductType;

                SetInitFlag(TRUE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    if (COMMON_ERROR_SUCCESS != CommonError)
    {
        CommonError = Unload();
    }

    return CommonError;
}

COMMON_ERROR
    CCommonOperationSystem::Unload()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


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

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

BOOL
    CCommonOperationSystem::IsWindowsXPOrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)51.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (0 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsXPSP1OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)51.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (1 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsXPSP2OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)51.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (2 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsXPSP3OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)51.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (3 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsVistaOrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)60.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (0 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsVistaSP1OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)60.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (1 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsVistaSP2OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)60.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (2 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindows7OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)61.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (0 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindows7SP1OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)61.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (1 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindows8OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)62.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (0 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindows8Point1OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)63.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (0 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindows10OrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)100.0 / (float)10.0 > m_VersionNumber)
        {
            break;
        }

        if (0 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsServerOrGreater()
{
    BOOL bRet = FALSE;


    do
    {
        if ((float)100.0 / (float)10.0 > m_VersionNumber)
        {
            if (VER_NT_WORKSTATION == m_wProductType)
            {
                break;
            }
        }
        else if ((float)62.0 / (float)10.0 > m_VersionNumber)
        {
            if (VER_NT_WORKSTATION == m_wProductType)
            {
                break;
            }
        }
        else if ((float)61.0 / (float)10.0 > m_VersionNumber)
        {
            if (VER_NT_WORKSTATION == m_wProductType)
            {
                break;
            }
        }
        else if ((float)60.0 / (float)10.0 > m_VersionNumber)
        {
            if (VER_NT_WORKSTATION == m_wProductType)
            {
                break;
            }
        }
        else if ((float)52.0 / (float)10.0 > m_VersionNumber)
        {
            if (VER_NT_WORKSTATION == m_wProductType)
            {
                break;
            }
        }

        if (0 > m_wServicePackMajor)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsProcessor64()
{
    BOOL bRet = FALSE;

    SYSTEM_INFO systemInfo = { 0 };


    do
    {
        GetNativeSystemInfo(&systemInfo);

        bRet = (PROCESSOR_ARCHITECTURE_IA64 == systemInfo.wProcessorArchitecture
            || PROCESSOR_ARCHITECTURE_ALPHA64 == systemInfo.wProcessorArchitecture
            || PROCESSOR_ARCHITECTURE_AMD64 == systemInfo.wProcessorArchitecture);
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonOperationSystem::IsWindowsXP()
{
    BOOL bRet = FALSE;

    do
    {
        if ( fabs(m_VersionNumber-5.1) <= EPSILON)
        {
            bRet = TRUE;
        }

    } while (FALSE);

    return bRet;
}

CCommonOperationSystem::CCommonOperationSystem()
{
    m_wServicePackMajor = 0;
    m_wProductType = 0;
    m_VersionNumber = 0;
}

CCommonOperationSystem::~CCommonOperationSystem()
{
    m_wServicePackMajor = 0;
    m_wProductType = 0;
    m_VersionNumber = 0;
}
