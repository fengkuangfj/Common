
#include "CommonPath.h"

COMMON_ERROR
    CCommonPath::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;

    WCHAR wchTempDir[COMMON_MAX_PATH] = {0};


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
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

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

COMMON_ERROR
    CCommonPath::Unload()
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
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;

    std::wstring wstrParent = L"";
    int nRet = 0;
    std::wstring wstrTempPath = L"";


    do
    {
        wstrParent = GetParent(wstrPath);
        if (!wstrParent.length())
        {
            CommonError = COMMON_ERROR_SUCCESS;
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

                CommonError = COMMON_ERROR_CREATE_DIRECTORY_FAILED;
                break;
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
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

std::wstring
    CCommonPath::ToLong(
    _In_ CONST std::wstring & wstrPath
    )
{
    std::wstring wstrRet = L"";

    WCHAR * pwchPath = NULL;
    int nLengthCh = 0;
    int nResult = 0;


    do
    {
        if (0 == wstrPath.length())
        {
            break;
        }

        nLengthCh = COMMON_MAX_PATH;

        do
        {
            if (NULL != pwchPath)
            {
                free(pwchPath);
                pwchPath = NULL;
            }

            pwchPath = ( WCHAR *)calloc(1, nLengthCh * sizeof(WCHAR));
            if (NULL == pwchPath)
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
                wstrRet = wstrPath;
                break;
            }

            nResult = GetLongPathName(wstrPath.c_str(), pwchPath, nLengthCh);
            if (0 == nResult)
            {
                // COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetLongPathName (%s) failed. msdn(%d)", wstrPath.c_str(), GetLastError());
                wstrRet = wstrPath;
                break;
            }

            if (nResult < nLengthCh)
            {
                wstrRet = pwchPath;
                break;
            }

            nLengthCh = nResult;
        } while (TRUE);
    } while (FALSE);

    if (NULL != pwchPath)
    {
        free(pwchPath);
        pwchPath = NULL;
    }

    return wstrRet;
}

CCommonPath::CCommonPath()
{
    m_wstrTempDir = L"";
}

CCommonPath::~CCommonPath()
{
    m_wstrTempDir = L"";
}
