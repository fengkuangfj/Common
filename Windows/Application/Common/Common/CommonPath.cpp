
#include "CommonPath.h"

COMMON_ERROR
    CCommonPath::Init()
{
    COMMON_ERROR SsDedsError = COMMON_ERROR_DEFAULT;

    WCHAR wchSystemRoot[COMMON_MAX_PATH] = {0};
    WCHAR wchTempDir[COMMON_MAX_PATH] = {0};


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                if (0 == GetWindowsDirectory(wchSystemRoot, _countof(wchSystemRoot)))
                {
                    m_wstrSystemRoot = L"C:\\Windows";
                }
                else
                {
                    if (L'\\' == wchSystemRoot[wcslen(wchSystemRoot) - 1])
                    {
                        wchSystemRoot[wcslen(wchSystemRoot) - 1] = L'\0';
                    }

                    m_wstrSystemRoot = wchSystemRoot;
                }

                if (0 == GetTempPath(_countof(wchTempDir), wchTempDir))
                {
                    m_wstrTempDir = L"C:\\Temp";
                }
                else
                {
                    if (L'\\' == wchTempDir[wcslen(wchTempDir) - 1])
                    {
                        wchTempDir[wcslen(wchTempDir) - 1] = L'\0';
                    }

                    m_wstrTempDir = wchTempDir;
                }

                SetInitFlag(TRUE);
            }
        }

        SsDedsError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return SsDedsError;
}

COMMON_ERROR
    CCommonPath::Unload()
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

std::wstring
    CCommonPath::GetParent(
    _In_ CONST std::wstring & wstrPath
    )
{
    std::wstring wstrRet = L"";

    WCHAR wchPath[COMMON_MAX_PATH] = { 0 };


    do
    {
        wcscpy_s(wchPath, _countof(wchPath), wstrPath.c_str());

        if (!PathRemoveFileSpec(wchPath))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"PathRemoveFileSpec (%s) failed. msdn(%d)", wchPath, GetLastError());
            break;
        }

        wstrRet = wchPath;
    } while (FALSE);

    return wstrRet;
}

COMMON_ERROR
    CCommonPath::MakeSureParentExist(
    _In_ CONST std::wstring & wstrPath
    )
{
    COMMON_ERROR SsDedsError = COMMON_ERROR_DEFAULT;

    std::wstring wstrParent = L"";
    int nRet = 0;
    std::wstring wstrTempPath = L"";


    do
    {
        wstrParent = GetParent(wstrPath);
        if (!wstrParent.length())
        {
            SsDedsError = COMMON_ERROR_SUCCESS;
            break;
        }

        if (MAX_PATH <wstrParent.length())
        {
            if (wstrParent.at(0) == '\\')
            {
                wstrParent = COMMON_LONG_PATH_PRE + wstrParent;
            }
            else
            {
                wstrTempPath = COMMON_LONG_PATH_PRE;
                wstrTempPath += L"\\";
                wstrParent = wstrTempPath + wstrParent;
            }
        }

        nRet = SHCreateDirectory(NULL, wstrParent.c_str());
        if (ERROR_SUCCESS != nRet)
        {
            if (ERROR_FILE_EXISTS != nRet
                && ERROR_ALREADY_EXISTS != nRet)
            {
                if (ERROR_ACCESS_DENIED == nRet
                    || ERROR_PATH_NOT_FOUND == nRet)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"SHCreateDirectory (%s) failed. msdn(%d)", wstrParent.c_str(), nRet);
                }
                else
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"SHCreateDirectory (%s) failed. msdn(%d)", wstrParent.c_str(), nRet);
                }

                SsDedsError = COMMON_ERROR_CREATE_DIRECTORY_FAILED;
                break;
            }
        }

        SsDedsError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return SsDedsError;
}

std::wstring CCommonPath::GetPath(
    _In_ CONST COMMON_PATH_TYPE & CommonPathType
    )
{
    std::wstring wstrRet = L"";


    do
    {
        switch (CommonPathType)
        {
        case COMMON_PATH_TYPE_LOG_DIR:
            {
                wstrRet = m_wstrTempDir;
                wstrRet += L"\\";
                wstrRet += COMMON_PATH_FILE_NAME_LOG_DIR;
                wstrRet += L"\\Log";

                break;
            }
        default:
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CommonPathType error. (%d)", CommonPathType);
                break;
            }
        }
    } while (FALSE);

    return wstrRet;
}

CCommonPath::CCommonPath()
{
    m_wstrSystemRoot = L"";
    m_wstrTempDir = L"";
}

CCommonPath::~CCommonPath()
{
    m_wstrSystemRoot = L"";
    m_wstrTempDir = L"";
}
