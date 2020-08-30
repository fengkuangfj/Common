
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonNtDefine.h"
#include "CommonNtHelper.h"
#include "CommonSingleInstance.h"

class CCommonPath : public CCommonSingleInstance<CCommonPath>
{
    friend class CCommonSingleInstance<CCommonPath>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    std::wstring
        GetParent(
        _In_ CONST std::wstring & wstrPath
        );

    COMMON_ERROR
        MakeSureParentExist(
        _In_ CONST std::wstring & wstrPath
        );

    std::wstring GetPath(
        _In_ CONST COMMON_PATH_TYPE & CommonPathType
        );

    std::wstring
        GetPath(
        _In_ CONST OBJECT_ATTRIBUTES * pObjectAttributes
        );

    std::wstring
        GetPathByFileId(
        _In_ CONST OBJECT_ATTRIBUTES * pObjectAttributes
        );

    std::wstring
        GetPath(
        _In_ CONST HANDLE & hFile,
        _In_ CONST BOOL & bToLong
        );

    std::wstring
        GetPath(
        _In_ CONST std::wstring & wstrSrcPath,
        _In_ CONST FILE_RENAME_INFORMATION * pFileRenameInformation
        );

    std::wstring
        GetString(
        _In_ CONST WCHAR * pwchBuffer,
        _In_ CONST ULONG ulLengthCh
        );

    std::wstring
        ToLong(
        _In_ CONST std::wstring & wstrPath
        );

    BOOL
        IsPath(
        _In_ CONST std::wstring & wstrPath
        );

    BOOL
        IsNetworkPath(
        _In_ CONST std::wstring & wstrPath
        );

    VOID
        TransitionToLetter(
        _Inout_ WCHAR * pwchPath,
        _In_ CONST int & nSizeCh
        );

    VOID
        TransitionToMup(
        _Inout_ WCHAR * pwchPath,
        _In_ CONST int & nSizeCh
        );

private:
    std::wstring m_wstrSystemRoot;
    std::wstring m_wstrTempDir;

    CCommonPath();

    ~CCommonPath();
};
