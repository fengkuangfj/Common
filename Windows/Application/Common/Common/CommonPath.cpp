
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
                if (0 == GetWindowsDirectory(wchTempDir, _countof(wchTempDir)))
                {
                    m_wstrSystemRoot = L"C:\\Windows";
                }
                else
                {
                    if (L'\\' == wchTempDir[wcslen(wchTempDir) - 1])
                    {
                        wchTempDir[wcslen(wchTempDir) - 1] = L'\0';
                    }

                    m_wstrSystemRoot = wchTempDir;
                }

                if (0 == GetTempPath(_countof(wchTempDir), wchTempDir))
                {
                    if (CCommonOperationSystem::GetInstance()->IsWindowsVistaOrGreater())
                    {
                        m_wstrTempDir = L"C:\\Users\\Public\\Temp";
                    }
                    else
                    {
                        m_wstrTempDir = L"C:\\Documents and Settings\\All Users\\Temp";
                    }
                }
                else
                {
                    if (L'\\' == wchTempDir[wcslen(wchTempDir) - 1])
                    {
                        wchTempDir[wcslen(wchTempDir) - 1] = L'\0';
                    }

                    m_wstrTempDir = wchTempDir;

                    m_wstrTempDir = CCommonPath::GetInstance()->ToLong(m_wstrTempDir);
                }

                CommonError = CCommonNtHelper::GetInstance()->Init();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonNtHelper::GetInstance()->Init failed. CommonError(%d)", CommonError);
                    break;
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
                CommonError = CCommonNtHelper::GetInstance()->Unload();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonNtHelper::GetInstance()->Unload failed. CommonError(%d)", CommonError);
                }

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
            // COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"PathRemoveFileSpec (%s) failed. msdn(%d)", wchPath, GetLastError());
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
    CCommonPath::GetPath(
    _In_ CONST OBJECT_ATTRIBUTES * pObjectAttributes
    )
{
    std::wstring wstrRet = L"";

    WCHAR * pwchPath = NULL;
    int nPathSizeCh = 0;
    int nNameSizeCh = 0;
    WCHAR * pwchName = NULL;
    std::wstring wstrPathRootDirectory = L"";


    do
    {
        if (NULL == pObjectAttributes)
        {
            break;
        }

        if (NULL != pObjectAttributes->RootDirectory
            && INVALID_HANDLE_VALUE != pObjectAttributes->RootDirectory)
        {
            wstrPathRootDirectory = GetPath(pObjectAttributes->RootDirectory, FALSE);
            if (0 == wstrPathRootDirectory.length())
            {
                break;
            }

            nPathSizeCh = (int)wstrPathRootDirectory.length() + 1;

            if (NULL != pObjectAttributes->ObjectName
                && NULL != pObjectAttributes->ObjectName->Buffer)
            {
                nNameSizeCh = pObjectAttributes->ObjectName->Length / sizeof(WCHAR) + 1;
                nPathSizeCh += (nNameSizeCh - 1);
            }

            nPathSizeCh++;

            pwchPath = (WCHAR *)calloc(1, nPathSizeCh * sizeof(WCHAR));
            if (NULL == pwchPath)
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
                break;
            }

            memcpy_s(pwchPath, nPathSizeCh * sizeof(WCHAR), wstrPathRootDirectory.c_str(), wstrPathRootDirectory.length() * sizeof(WCHAR));

            if (0 != nNameSizeCh)
            {
                pwchName = (WCHAR *)calloc(1, nNameSizeCh * sizeof(WCHAR));
                if (NULL == pwchName)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
                    break;
                }

                memcpy_s(pwchName, nNameSizeCh * sizeof(WCHAR), pObjectAttributes->ObjectName->Buffer, pObjectAttributes->ObjectName->Length);

                if (L'\\' == *(pwchPath + wcslen(pwchPath) - 1))
                {
                    if (L'\\' == *pwchName)
                    {
                        if (2 <= wcslen(pwchName))
                        {
                            wcscat_s(pwchPath, nPathSizeCh, pwchName + 1);
                        }
                    }
                    else
                    {
                        wcscat_s(pwchPath, nPathSizeCh, pwchName);
                    }
                }
                else
                {
                    if (L'\\' == *pwchName)
                    {
                        wcscat_s(pwchPath, nPathSizeCh, pwchName);
                    }
                    else
                    {
                        wcscat_s(pwchPath, nPathSizeCh, L"\\");
                        wcscat_s(pwchPath, nPathSizeCh, pwchName);
                    }
                }
            }
        }
        else
        {
            if (NULL == pObjectAttributes->ObjectName
                || NULL == pObjectAttributes->ObjectName->Buffer)
            {
                break;
            }

            nPathSizeCh = pObjectAttributes->ObjectName->Length / sizeof(WCHAR) + 1;
            pwchPath = (WCHAR *)calloc(1, nPathSizeCh * sizeof(WCHAR));
            if (NULL == pwchPath)
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
                break;
            }

            memcpy_s(pwchPath, nPathSizeCh * sizeof(WCHAR), pObjectAttributes->ObjectName->Buffer, pObjectAttributes->ObjectName->Length);
        }

        {
            int nLengthTemp = (int)wcslen(pwchPath);
            if (11 == nLengthTemp)
            {
                if (0 == _wcsnicmp(pwchPath, L"\\systemroot", 11))
                {
                    memcpy_s(pwchPath, nPathSizeCh * sizeof(WCHAR), m_wstrSystemRoot.c_str(), m_wstrSystemRoot.length() * sizeof(WCHAR));
                    pwchPath[nLengthTemp - 1] = L'\0';
                }
            }
            else if (12 <= nLengthTemp)
            {
                if (0 == _wcsnicmp(pwchPath, L"\\systemroot\\", 12))
                {
                    memcpy_s(pwchPath, nPathSizeCh * sizeof(WCHAR), m_wstrSystemRoot.c_str(), m_wstrSystemRoot.length() * sizeof(WCHAR));
                    memcpy_s(pwchPath + m_wstrSystemRoot.length(), (nPathSizeCh - m_wstrSystemRoot.length()) * sizeof(WCHAR), pwchPath + 11, (nLengthTemp - 11) * sizeof(WCHAR));
                    pwchPath[nLengthTemp - 1] = L'\0';
                }
            }
        }

        TransitionToLetter(pwchPath, nPathSizeCh);

        if (L'\\' == pwchPath[wcslen(pwchPath) - 1])
        {
            pwchPath[wcslen(pwchPath) - 1] = L'\0';
        }

        if (!wcslen(pwchPath))
        {
            break;
        }

        if (IsPath(pwchPath))
        {
            wstrRet = ToLong(pwchPath);

            CCommonStringConvert::GetInstance()->ToLower(wstrRet);
        }
        else
        {
            wstrRet = pwchPath;
        }
    } while (FALSE);

    if (NULL != pwchName)
    {
        free(pwchName);
        pwchName = NULL;
    }

    if (NULL != pwchPath)
    {
        free(pwchPath);
        pwchPath = NULL;
    }

    return wstrRet;
}

std::wstring
    CCommonPath::GetPath(
    _In_ CONST HANDLE & hFile,
    _In_ CONST BOOL & bToLong
    )
{
    std::wstring wstrRet = L"";

    OBJECT_NAME_INFORMATION * pObjectNameInformation = NULL;
    WCHAR * pwchPath = NULL;
    ULONG ulSizeB = 0;


    do
    {
        if (NULL == hFile
            || INVALID_HANDLE_VALUE == hFile)
        {
            break;
        }

        ulSizeB = sizeof(OBJECT_NAME_INFORMATION) + sizeof(WCHAR) * COMMON_MAX_PATH;
        pObjectNameInformation = (OBJECT_NAME_INFORMATION *)calloc(1, ulSizeB);
        if (NULL == pObjectNameInformation)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        CCommonNtHelper::GetInstance()->NtQueryObject(hFile, (OBJECT_INFORMATION_CLASS)ObjectNameInformation, pObjectNameInformation, ulSizeB, &ulSizeB);
        if (0 == pObjectNameInformation->Name.Length)
        {
            break;
        }

        pwchPath = (WCHAR *)calloc(1, pObjectNameInformation->Name.Length + sizeof(WCHAR));
        if (NULL == pwchPath)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        memcpy_s(pwchPath, pObjectNameInformation->Name.Length + sizeof(WCHAR), pObjectNameInformation->Name.Buffer, pObjectNameInformation->Name.Length);

        TransitionToLetter(pwchPath, (pObjectNameInformation->Name.Length + sizeof(WCHAR)) / sizeof(WCHAR));

        if (L'\\' == pwchPath[wcslen(pwchPath) - 1])
        {
            pwchPath[wcslen(pwchPath) - 1] = L'\0';
        }

        if (!wcslen(pwchPath))
        {
            break;
        }

        if (IsPath(pwchPath))
        {
            if (bToLong)
            {
                wstrRet = ToLong(pwchPath);
            }
            else
            {
                wstrRet = pwchPath;
            }

            CCommonStringConvert::GetInstance()->ToLower(wstrRet);
        }
        else
        {
            wstrRet = pwchPath;
        }
    } while (FALSE);

    if (NULL != pwchPath)
    {
        free(pwchPath);
        pwchPath = NULL;
    }

    if (NULL != pObjectNameInformation)
    {
        free(pObjectNameInformation);
        pObjectNameInformation = NULL;
    }

    return wstrRet;
}


std::wstring
    CCommonPath::GetPath(
    _In_ CONST std::wstring & wstrSrcPath,
    _In_ CONST FILE_RENAME_INFORMATION * pFileRenameInformation
    )
{
    std::wstring wstrRet = L"";

    int nNameSizeCh = 0;
    WCHAR * pwchName = NULL;
    std::wstring wstrPathRootDirectory = L"";


    do
    {
        if (NULL == pFileRenameInformation
            || NULL == pFileRenameInformation->FileName
            || 0 >= pFileRenameInformation->FileNameLength)
        {
            break;
        }

        nNameSizeCh = pFileRenameInformation->FileNameLength / sizeof(WCHAR) + 1;

        pwchName = (WCHAR *)calloc(1, nNameSizeCh * sizeof(WCHAR));
        if (NULL == pwchName)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        memcpy_s(pwchName, nNameSizeCh * sizeof(WCHAR), pFileRenameInformation->FileName, pFileRenameInformation->FileNameLength);

        TransitionToLetter(pwchName, nNameSizeCh);

        wstrPathRootDirectory = GetParent(pwchName);
        if (wstrPathRootDirectory.length())
        {
            wstrRet = pwchName;
        }
        else
        {
            if (NULL != pFileRenameInformation->RootDirectory
                && INVALID_HANDLE_VALUE != pFileRenameInformation->RootDirectory)
            {
                wstrPathRootDirectory = GetPath(pFileRenameInformation->RootDirectory, FALSE);
            }
            else
            {
                wstrPathRootDirectory = GetParent(wstrSrcPath);
            }

            wstrRet = wstrPathRootDirectory;
            wstrRet += L"\\";
            wstrRet += pwchName;
        }

        wstrRet = ToLong(wstrRet);

        CCommonStringConvert::GetInstance()->ToLower(wstrRet);
    } while (FALSE);

    if (NULL != pwchName)
    {
        free(pwchName);
        pwchName = NULL;
    }

    return wstrRet;
}

std::wstring
    CCommonPath::GetString(
    _In_ CONST WCHAR * pwchBuffer,
    _In_ CONST ULONG ulLengthCh
    )
{
    std::wstring wstrRet = L"";

    WCHAR * pwchTemp = NULL;


    do
    {
        pwchTemp = (WCHAR * )calloc(1, (ulLengthCh + 1) * sizeof(WCHAR));
        if (NULL == pwchTemp)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        memcpy_s(pwchTemp, (ulLengthCh + 1) * sizeof(WCHAR), pwchBuffer, ulLengthCh * sizeof(WCHAR));

        wstrRet = pwchTemp;
    } while (FALSE);

    if (NULL != pwchTemp)
    {
        free(pwchTemp);
        pwchTemp = NULL;
    }

    return wstrRet;
}

std::wstring
    CCommonPath::GetPathByFileId(
    _In_ CONST OBJECT_ATTRIBUTES * pObjectAttributes
    )
{
    std::wstring wstrRet = L"";

    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    HANDLE hFile = NULL;
    IO_STATUS_BLOCK IoStatusBlock = { 0 };


    do
    {
        if (NULL == pObjectAttributes)
        {
            break;
        }

        ntStatus = CCommonNtHelper::GetInstance()->NtOpenFile(
            &hFile,
            FILE_GENERIC_READ,
            (POBJECT_ATTRIBUTES)pObjectAttributes,
            &IoStatusBlock,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            FILE_OPEN_BY_FILE_ID | FILE_COMPLETE_IF_OPLOCKED
            );
        if (!NT_SUCCESS(ntStatus))
        {
            ntStatus = CCommonNtHelper::GetInstance()->NtOpenFile(
                &hFile,
                FILE_GENERIC_READ,
                (POBJECT_ATTRIBUTES)pObjectAttributes,
                &IoStatusBlock,
                FILE_SHARE_READ ,
                FILE_OPEN_BY_FILE_ID | FILE_COMPLETE_IF_OPLOCKED
                );
            if (!NT_SUCCESS(ntStatus))
            {
                ntStatus = CCommonNtHelper::GetInstance()->NtOpenFile(
                    &hFile,
                    FILE_GENERIC_READ,
                    (POBJECT_ATTRIBUTES)pObjectAttributes,
                    &IoStatusBlock,
                    FILE_SHARE_WRITE,
                    FILE_OPEN_BY_FILE_ID | FILE_COMPLETE_IF_OPLOCKED
                    );
                if (!NT_SUCCESS(ntStatus))
                {
                    ntStatus = CCommonNtHelper::GetInstance()->NtOpenFile(
                        &hFile,
                        FILE_GENERIC_READ,
                        (POBJECT_ATTRIBUTES)pObjectAttributes,
                        &IoStatusBlock,
                        FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE,
                        FILE_OPEN_BY_FILE_ID | FILE_COMPLETE_IF_OPLOCKED
                        );
                    if (!NT_SUCCESS(ntStatus))
                    {
                        ntStatus = CCommonNtHelper::GetInstance()->NtOpenFile(
                            &hFile,
                            FILE_GENERIC_READ,
                            (POBJECT_ATTRIBUTES)pObjectAttributes,
                            &IoStatusBlock,
                            FILE_SHARE_READ | FILE_SHARE_DELETE,
                            FILE_OPEN_BY_FILE_ID | FILE_COMPLETE_IF_OPLOCKED
                            );
                        if (!NT_SUCCESS(ntStatus))
                        {
                            ntStatus = CCommonNtHelper::GetInstance()->NtOpenFile(
                                &hFile,
                                FILE_GENERIC_READ,
                                (POBJECT_ATTRIBUTES)pObjectAttributes,
                                &IoStatusBlock,
                                FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                FILE_OPEN_BY_FILE_ID | FILE_COMPLETE_IF_OPLOCKED
                                );
                            if (!NT_SUCCESS(ntStatus))
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (NULL == hFile)
        {
            break;
        }

        wstrRet = GetPath(hFile, TRUE);
    } while (FALSE);

    if (NULL != hFile)
    {
        CCommonNtHelper::GetInstance()->NtClose(hFile);
        hFile = NULL;
    }

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
    std::wstring wstrParent = L"";
    WCHAR * pwchFileName = NULL;


    do
    {
        if (0 == wstrPath.length())
        {
            break;
        }

        if (NULL == StrChrI(wstrPath.c_str() , L'~'))
        {
            wstrRet = wstrPath;
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

            if (PathFileExists(wstrPath.c_str()))
            {
                nResult = GetLongPathName(wstrPath.c_str(), pwchPath, nLengthCh);
                if (0 == nResult)
                {
                    // COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetLongPathName (%s) failed. msdn(%d)", wstrPath.c_str(), GetLastError());
                    wstrRet = wstrPath;
                    break;
                }
            }
            else
            {
                pwchFileName = PathFindFileName(wstrPath.c_str());
                if (NULL != pwchFileName)
                {
                    wstrParent = GetParent(wstrPath);
                    if (wstrParent.length())
                    {
                        wstrParent = ToLong(wstrParent);

                        wstrRet = wstrParent;
                        wstrRet += L"\\";
                        wstrRet += pwchFileName;
                        break;
                    }
                }
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

BOOL
    CCommonPath::IsPath(
    _In_ CONST std::wstring & wstrPath
    )
{
    BOOL bRet = FALSE;


    do
    {
        if (0 == wstrPath.length())
        {
            break;
        }

        if ((2 <= wstrPath.length()
            && L':' == *(wstrPath.c_str() + 1)
            && (((L'a' <= *wstrPath.c_str() && L'z' >= *wstrPath.c_str())) || ((L'A' <= *wstrPath.c_str() && L'Z' >= *wstrPath.c_str()))))
            ||
            IsNetworkPath(wstrPath))
        {
            bRet = TRUE;
        }
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonPath::IsNetworkPath(
    _In_ CONST std::wstring & wstrPath
    )
{
    BOOL bRet = FALSE;

    WCHAR wchVolume[4] = { 0 };
    UINT unDriveType = 0;
    WCHAR * pwchEnd = NULL;
    WCHAR wchIp[COMMON_IP_MAX_LENGTH + 1] = { 0 };
    WCHAR * pwchBegin = NULL;
    int nCount = 0;
    int nSubIp = 0;
    int nIp = 0;


    do
    {
        wchVolume[0] = *wstrPath.c_str();
        wchVolume[1] = L':';
        wchVolume[2] = L'\\';
        unDriveType = GetDriveType(wchVolume);
        if (DRIVE_REMOTE == unDriveType)
        {
            bRet = TRUE;
            break;
        }

        if (10 >= wstrPath.length())
        {
            break;
        }

        if (0 != _wcsnicmp(wstrPath.c_str(), L"\\\\", 2))
        {
            break;
        }

        pwchEnd = StrChr(wstrPath.c_str() + 2, L'\\');
        if (NULL == pwchEnd)
        {
            break;
        }

        memcpy_s(wchIp, sizeof(wchIp), wstrPath.c_str() + 2, (pwchEnd - wstrPath.c_str() - 2) * sizeof(WCHAR));

        pwchBegin = wchIp;

        do
        {
            WCHAR wchTemp[4] = { 0 };

            nCount++;

            pwchEnd = StrChr(pwchBegin, L'.');
            if (NULL != pwchEnd)
            {
                if (3 < nCount)
                {
                    break;
                }

                memcpy_s(wchTemp, sizeof(wchTemp), pwchBegin, (pwchEnd - pwchBegin) * sizeof(WCHAR));

                nSubIp = _wtoi(wchTemp);
                if (0 > nSubIp || 255 < nSubIp)
                {
                    break;
                }

                if (wchIp + wcslen(wchIp) - 1 <= pwchEnd)
                {
                    break;
                }

                pwchBegin = pwchEnd + 1;
                pwchEnd = NULL;
            }
            else
            {
                if (4 == nCount)
                {
                    wcscpy_s(wchTemp, _countof(wchTemp), pwchBegin);

                    nIp = _wtoi(wchTemp);
                    if (0 > nIp || 255 < nIp)
                    {
                        break;
                    }

                    bRet = TRUE;
                }

                break;
            }
        } while (TRUE);
    } while (FALSE);

    return bRet;
}

VOID
    CCommonPath::TransitionToLetter(
    _Inout_ WCHAR * pwchPath,
    _In_ CONST int & nSizeCh
    )
{
    WCHAR * pwchLetter = NULL;
    BOOL bDevice = FALSE;


    do
    {
        if (NULL == pwchPath
            || 0 == nSizeCh)
        {
            break;
        }

        if (2 <= wcslen(pwchPath)
            && L':' == *(pwchPath + 1)
            && ((L'a' <= *pwchPath && L'z' >= *pwchPath) || (L'A' <= *pwchPath && L'Z' >= *pwchPath)))
        {
            break;
        }
        else if (6 <= wcslen(pwchPath)
            && L':' == *(pwchPath + 5)
            && ((L'a' <= *(pwchPath + 4) && L'z' >= *(pwchPath + 4)) || (L'A' <= *(pwchPath + 4) && L'Z' >= *(pwchPath + 4)))
            && (0 == _wcsnicmp(pwchPath, L"\\??\\", 4) || 0 == _wcsnicmp(pwchPath, L"\\\\.\\", 4)))
        {
            MoveMemory(pwchPath, pwchPath + 4, (wcslen(pwchPath) - 3) * sizeof(TCHAR));
            break;
        }
        else if (8 <= wcslen(pwchPath)
            && 0 == _wcsnicmp(pwchPath, L"\\??\\unc", 7))
        {
            MoveMemory(pwchPath + 1, pwchPath + 7, (wcslen(pwchPath) - 6) * sizeof(WCHAR));
            break;
        }
        else if (9 <= wcslen(pwchPath)
            && 0 == _wcsnicmp(pwchPath, L"\\device\\", 8))
        {
            bDevice = TRUE;
        }
        else if (23 <= wcslen(pwchPath)
            && (0 == _wcsnicmp(pwchPath, L"\\??\\GlobalRoot\\device\\", 22) || 0 == _wcsnicmp(pwchPath, L"\\\\.\\GlobalRoot\\device\\", 22)))
        {
            MoveMemory(pwchPath, pwchPath + 15, (wcslen(pwchPath) - 14) * sizeof(TCHAR));

            bDevice = TRUE;
        }
        else
        {
            break;
        }

        if (bDevice)
        {
            if (12 <= wcslen(pwchPath)
                && 0 == _wcsnicmp(pwchPath, L"\\Device\\Mup", 11))
            {
                MoveMemory(pwchPath + 1, pwchPath + 11, (wcslen(pwchPath) - 10) * sizeof(WCHAR));
                break;
            }

            pwchLetter = (LPTSTR)calloc(1, 3 * sizeof(TCHAR));
            if (NULL == pwchLetter)
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc (%s) failed. msdn(%d)", pwchPath, GetLastError());
                break;
            }

            pwchLetter[0] = L'c';
            pwchLetter[1] = L':';

            do
            {
                WCHAR wchDevice[COMMON_MAX_PATH] = { 0 };
                if (0 != QueryDosDevice(pwchLetter, wchDevice, _countof(wchDevice)))
                {
                    if (39 <= wcslen(wchDevice)
                        && 0 == _wcsnicmp(wchDevice, L"\\device\\lanmanredirector\\;", 26))
                    {
                        TransitionToMup(wchDevice, _countof(wchDevice));
                    }

                    if (0 == _wcsnicmp(pwchPath, wchDevice, wcslen(wchDevice)))
                    {
                        MoveMemory(pwchPath, pwchLetter, 4);
                        MoveMemory(pwchPath + 2, pwchPath + wcslen(wchDevice), (wcslen(pwchPath) + 1 - wcslen(wchDevice)) * sizeof(WCHAR));
                        break;
                    }
                }
                else
                {
                    if (ERROR_ACCESS_DENIED != GetLastError()
                        && ERROR_FILE_NOT_FOUND != GetLastError())
                    {
                        COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"QueryDosDevice (%s) failed. msdn(%d)", pwchLetter, GetLastError());
                    }
                }

                pwchLetter[0] += (L'b' - L'a');
            } while (L'a' <= pwchLetter[0] && L'z' >= pwchLetter[0]);
        }
    } while (FALSE);

    if (NULL != pwchLetter)
    {
        free(pwchLetter);
        pwchLetter = NULL;
    }

    return ;
}

VOID
    CCommonPath::TransitionToMup(
    _Inout_ WCHAR * pwchPath,
    _In_ CONST int & nSizeCh
    )
{
    WCHAR * pwchBegin = NULL;
    WCHAR * pwchEnd = NULL;
    int nCount = 0;


    do
    {
        if (NULL == pwchPath
            || 0 == nSizeCh)
        {
            break;
        }
        for (WCHAR * pwchPosition = pwchPath; pwchPosition < pwchPath + wcslen(pwchPath); pwchPosition++)
        {
            if (L'\\' == *pwchPosition)
            {
                nCount++;

                if (2 == nCount)
                {
                    pwchBegin = pwchPosition;
                }

                else if (4 == nCount)
                {
                    pwchEnd = pwchPosition;
                    break;
                }
            }
        }

        if (NULL == pwchBegin
            || NULL == pwchEnd)
        {
            break;
        }

        if (CCommonOperationSystem::GetInstance()->IsWindows7OrGreater())
        {
            MoveMemory(pwchPath + 8, L"mup", 6);
            *(pwchPath + 11) = L'\0';
            wcscat_s(pwchPath, nSizeCh, pwchEnd);
        }
        else
        {
            MoveMemory(pwchPath + 8, L"lanmanredirector", 32);
            *(pwchPath + 24) = L'\0';
            wcscat_s(pwchPath, nSizeCh, pwchEnd);
        }
    } while (FALSE);

    return ;
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
