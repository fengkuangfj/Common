
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonMacro.h"
#include "CommonNtHelper.h"
#include "CommonSingleInstance.h"

class CCommonFile : public CCommonSingleInstance<CCommonFile>
{
    friend class CCommonSingleInstance<CCommonFile>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    COMMON_ERROR
        GetTime(
        _In_ CONST std::wstring & wstrPath,
        _Inout_opt_ FILETIME * pftCreationTime,
        _Inout_opt_ FILETIME * pftLastAccessTime,
        _Inout_opt_ FILETIME * pftLastWriteTime
        );

    BOOL
        IsExist(
        _In_ CONST std::wstring & wstrPath
        );

    BOOL
        IsFile(
        _In_ CONST std::wstring & wstrPath
        );

    BOOL
        IsFile(
        _In_ CONST HANDLE & hFile
        );

    ACCESS_MASK
        GetDesiredAccess(
        _In_ CONST HANDLE & hFile
        );

    BOOL
        IsFileOrSectionObjectType(
        _In_ CONST HANDLE & hFile
        );

    BOOL
        IsFileOption(
        IN ULONG & Options
        );

    LONGLONG
        GetEndOfFile(
        _In_ CONST HANDLE & hFile
        );

    LONGLONG
        GetEndOfFile(
        _In_ CONST std::wstring & wstrPath
        );

    LONGLONG
        GetPositon(
        _In_ CONST HANDLE & hFile
        );

    COMMON_ERROR
        SetPosition(
        _In_ CONST HANDLE & hFile,
        _In_ CONST LONGLONG & llPosition
        );

    BOOL
        CanWrite(
        _In_ CONST ULONG & Win32Protect
        );

    HANDLE
        OpenForRead(
        _In_ CONST std::wstring & wstrPath
        );

    BOOL
        ReadFile(
        __in HANDLE hFile,
        __out_bcount_part_opt(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
        __in DWORD nNumberOfBytesToRead,
        __out_opt LPDWORD lpNumberOfBytesRead
        );

    BOOL
        WriteFile(
        __in HANDLE hFile,
        __in_bcount_opt(nNumberOfBytesToWrite) LPCVOID lpBuffer,
        __in DWORD nNumberOfBytesToWrite,
        __out_opt LPDWORD lpNumberOfBytesWritten
        );

private:
    CCommonFile();

    ~CCommonFile();
};
