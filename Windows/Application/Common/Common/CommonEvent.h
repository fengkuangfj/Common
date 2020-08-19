
#pragma once

#include "CommonPublic.h"

#include "CommonPath.h"
#include "CommonSingleInstance.h"

class CCommonEvent : public CCommonSingleInstance<CCommonEvent>
{
    friend class CCommonSingleInstance<CCommonEvent>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    HANDLE
        SetFileHandlingFlag(
        _In_ CONST std::wstring & wstrPath,
        _In_ CONST BOOL & bConvertFormat = TRUE
        );

    VOID
        ClearFileHandlingFlag(
        _Inout_ HANDLE & hEvent
        );

    BOOL
        CheckFileHandlingFlag(
        _In_ CONST std::wstring & wstrPath,
        _In_ CONST BOOL & bConvertFormat = TRUE
        );

private:
    CCommonEvent();

    ~CCommonEvent();

    std::wstring
        GetFileHandlingEventName(
        _In_ CONST std::wstring & wstrPath,
        _In_ CONST BOOL & bConvertFormat
        );
};
