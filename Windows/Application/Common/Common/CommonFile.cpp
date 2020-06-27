
#include "CommonFile.h"

COMMON_ERROR
    CCommonFile::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
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
    CCommonFile::Unload()
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

COMMON_ERROR
    CCommonFile::GetTime(
    _In_ CONST std::wstring & wstrPath,
    _Inout_opt_ FILETIME * pftCreationTime,
    _Inout_opt_ FILETIME * pftLastAccessTime,
    _Inout_opt_ FILETIME * pftLastWriteTime
    )
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;

    WIN32_FIND_DATA Win32FindData = { 0 };
    HANDLE hFind = INVALID_HANDLE_VALUE;


    do
    {
        if (0 == wstrPath.length()
            || (NULL == pftCreationTime && NULL == pftLastAccessTime && NULL == pftLastWriteTime ))
        {
            CommonError = COMMON_ERROR_INVALID_PARAMETER;
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        hFind = FindFirstFile(wstrPath.c_str(), &Win32FindData);
        if (INVALID_HANDLE_VALUE == hFind)
        {
            CommonError = COMMON_ERROR_NOT_FOUND;
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"FindFirstFile (%s) failed. msdn(%d)", wstrPath.c_str(), GetLastError());
            break;
        }

        if (NULL != pftCreationTime)
        {
            *pftCreationTime = Win32FindData.ftCreationTime;
        }

        if (NULL != pftLastAccessTime)
        {
            *pftLastAccessTime = Win32FindData.ftLastAccessTime;
        }

        if (NULL != pftLastWriteTime)
        {
            *pftLastWriteTime = Win32FindData.ftLastWriteTime;
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return CommonError;
}

BOOL
    CCommonFile::IsExist(
    _In_ CONST std::wstring & wstrPath
    )
{
    BOOL bRet = FALSE;

    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA Win32FindData = { 0 };


    do
    {
        if (0 == wstrPath.length())
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        hFind = FindFirstFile(wstrPath.c_str(), &Win32FindData);
        if (INVALID_HANDLE_VALUE == hFind)
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return bRet;
}

BOOL
    CCommonFile::IsFile(
    _In_ CONST std::wstring & wstrPath
    )
{
    BOOL bRet = FALSE;

    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA Win32FindData = { 0 };
    LARGE_INTEGER liTemp = {0};


    do
    {
        if (0 == wstrPath.length())
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        hFind = FindFirstFile(wstrPath.c_str(), &Win32FindData);
        if (INVALID_HANDLE_VALUE == hFind)
        {
            if ( ERROR_PATH_NOT_FOUND == GetLastError()
                || ERROR_FILE_NOT_FOUND == GetLastError() )
            {
                bRet = TRUE;
            }

            break;
        }

        if (BooleanFlagInclude(Win32FindData.dwFileAttributes , FILE_ATTRIBUTE_DIRECTORY))
        {
            break;
        }

        liTemp.LowPart = Win32FindData.ftCreationTime.dwLowDateTime;
        liTemp.HighPart = Win32FindData.ftCreationTime.dwHighDateTime;

        if (0 != liTemp.QuadPart)
        {
            bRet = TRUE;
            break;
        }

        liTemp.LowPart = Win32FindData.ftLastAccessTime.dwLowDateTime;
        liTemp.HighPart = Win32FindData.ftLastAccessTime.dwHighDateTime;

        if (0 != liTemp.QuadPart)
        {
            bRet = TRUE;
            break;
        }

        liTemp.LowPart = Win32FindData.ftLastWriteTime.dwLowDateTime;
        liTemp.HighPart = Win32FindData.ftLastWriteTime.dwHighDateTime;

        if (0 != liTemp.QuadPart)
        {
            bRet = TRUE;
            break;
        }
    } while (FALSE);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return bRet;
}

BOOL
    CCommonFile::IsFile(
    _In_ CONST HANDLE & hFile
    )
{
    BOOL bRet = FALSE;

    IO_STATUS_BLOCK IoStatusBlock = { 0 };
    FILE_STANDARD_INFORMATION FileStandardInfo = { 0 };
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;


    do
    {
        if (NULL == hFile
            || INVALID_HANDLE_VALUE == hFile)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        ntStatus = CCommonNtHelper::GetInstance()->NtQueryInformationFile(
            hFile,
            &IoStatusBlock,
            &FileStandardInfo,
            sizeof(FileStandardInfo),
            (FILE_INFORMATION_CLASS)FileStandardInformation
            );
        if (!NT_SUCCESS(ntStatus))
        {
            break;
        }

        bRet = !FileStandardInfo.Directory;
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonFile::IsFileOrSectionObjectType(
    _In_ CONST HANDLE & hFile
    )
{
    BOOL bRet = FALSE;

    CHAR chData[COMMON_MAX_PATH] = {0};
    PUBLIC_OBJECT_TYPE_INFORMATION * pPublicObjectTypeInformation = NULL;
    ULONG ulSizeB = 0;


    do
    {
        if (NULL == hFile
            || INVALID_HANDLE_VALUE == hFile)
        {
            break;
        }

        ulSizeB = sizeof(chData);

        pPublicObjectTypeInformation = (PUBLIC_OBJECT_TYPE_INFORMATION *)chData;

        CCommonNtHelper::GetInstance()->NtQueryObject(
            hFile,
            (OBJECT_INFORMATION_CLASS)ObjectTypeInformation,
            pPublicObjectTypeInformation,
            ulSizeB,
            &ulSizeB
            );
        if (0 == pPublicObjectTypeInformation->TypeName.Length
            || NULL == pPublicObjectTypeInformation->TypeName.Buffer)
        {
            break;
        }

        if (8 == pPublicObjectTypeInformation->TypeName.Length)
        {
            if (0 == _wcsnicmp(L"File", pPublicObjectTypeInformation->TypeName.Buffer, 4))
            {
                bRet = TRUE;
            }
        }
        else if (14 == pPublicObjectTypeInformation->TypeName.Length)
        {
            if ( 0 == _wcsnicmp(L"Section", pPublicObjectTypeInformation->TypeName.Buffer, 7))
            {
                bRet = TRUE;
            }
        }
    } while (FALSE);

    return bRet;
}

BOOL
    CCommonFile::IsFileOption(
    IN ULONG & Options
    )
{
    BOOL bRet = FALSE;


    do
    {
        if ( BooleanFlagInclude(Options, FILE_DIRECTORY_FILE))
        {
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

LONGLONG
    CCommonFile::GetEndOfFile(
    _In_ CONST HANDLE & hFile
    )
{
    LONGLONG llRet = 0;

    LARGE_INTEGER liEndOfFile = {0};


    do
    {
        if (NULL == hFile
            || INVALID_HANDLE_VALUE == hFile)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        if (!GetFileSizeEx(hFile, &liEndOfFile))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetFileSizeEx failed. msdn(%d)", GetLastError());
            break;
        }

        llRet = liEndOfFile.QuadPart;
    } while (FALSE);

    return llRet;
}

LONGLONG
    CCommonFile::GetEndOfFile(
    _In_ CONST std::wstring & wstrPath
    )
{
    LONGLONG llRet = 0;

    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA Win32FindData = { 0 };
    LARGE_INTEGER liTemp = {0};


    do
    {
        if (0 == wstrPath.length())
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        hFind = FindFirstFile(wstrPath.c_str(), &Win32FindData);
        if (INVALID_HANDLE_VALUE == hFind)
        {
            break;
        }

        liTemp.LowPart = Win32FindData.nFileSizeLow;
        liTemp.HighPart = Win32FindData.nFileSizeHigh;

        llRet = liTemp.QuadPart;
    } while (FALSE);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return llRet;
}

LONGLONG
    CCommonFile::GetPositon(
    _In_ CONST HANDLE & hFile
    )
{
    LONGLONG llRet = 0;


    do
    {
        if (NULL == hFile
            || INVALID_HANDLE_VALUE == hFile)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        llRet = SetFilePointer(hFile, 0 , NULL , FILE_CURRENT);
        if (INVALID_SET_FILE_POINTER == llRet
            && NO_ERROR != GetLastError())
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"SetFilePointer failed. msdn(%d)", GetLastError());
            break;
        }
    } while (FALSE);

    return llRet ;
}

COMMON_ERROR
    CCommonFile::SetPosition(
    _In_ CONST HANDLE & hFile,
    _In_ CONST LONGLONG & llPosition
    )
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;

    LARGE_INTEGER liPos = {0};


    do
    {
        if (NULL == hFile
            || INVALID_HANDLE_VALUE == hFile)
        {
            CommonError = COMMON_ERROR_INVALID_PARAMETER;
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"invalid parameter");
            break;
        }

        liPos.QuadPart = llPosition;

        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, liPos.LowPart, &liPos.HighPart , FILE_BEGIN)
            && NO_ERROR != GetLastError())
        {
            CommonError = COMMON_ERROR_SET_FILE_POINTER_FAILED;
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"SetFilePointer failed. msdn(%d)", GetLastError());
            break;
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

BOOL
    CCommonFile::CanWrite(
    _In_ CONST ULONG & Win32Protect
    )
{
    BOOL bRet = FALSE;


    do
    {
        if (BooleanFlagInclude(Win32Protect, PAGE_EXECUTE_READWRITE)
            || BooleanFlagInclude(Win32Protect, PAGE_EXECUTE_WRITECOPY)
            || BooleanFlagInclude(Win32Protect, PAGE_READWRITE)
            || BooleanFlagInclude(Win32Protect, PAGE_WRITECOPY))
        {
            bRet = TRUE;
        }

        if (BooleanFlagInclude(Win32Protect, PAGE_EXECUTE)
            || BooleanFlagInclude(Win32Protect, PAGE_EXECUTE_READ)
            || BooleanFlagInclude(Win32Protect, PAGE_NOACCESS)
            || BooleanFlagInclude(Win32Protect, PAGE_READONLY))
        {
            bRet = FALSE;
        }
    } while (FALSE);

    return bRet;
}

CCommonFile::CCommonFile()
{
    ;
}

CCommonFile::~CCommonFile()
{
    ;
}
