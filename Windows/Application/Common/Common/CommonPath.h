
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
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
        ToLong(
        _In_ CONST std::wstring & wstrPath
        );

private:
    std::wstring m_wstrTempDir;

    CCommonPath();

    ~CCommonPath();
};
