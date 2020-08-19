
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonSingleInstance.h"

class CCommonStringConvert : public CCommonSingleInstance<CCommonStringConvert>
{
    friend class CCommonSingleInstance<CCommonStringConvert>;

public:
    std::wstring
        AnsiToWideChar(
        _In_ CONST std::string & strSrc
        );

    std::string
        AnsiToUtf8(
        _In_ CONST std::string & strSrc
        );

    std::string
        WideCharToAnsi(
        _In_ CONST std::wstring & strSrc
        );

    std::string
        WideCharToUtf8(
        _In_ CONST std::wstring & strSrc
        );

    std::string
        Utf8ToAnsi(
        _In_ CONST std::string & strSrc
        );

    std::wstring
        Utf8ToWideChar(
        _In_ CONST std::string & strSrc
        );

    VOID
        ToLower(
        _Inout_ std::wstring & wstrSrc
        );

    VOID
        Replace(
        _Inout_ std::wstring & wstrContent,
        _In_ CONST std::wstring & wstrReplace,
        _In_ CONST std::wstring & wstrDest
        );

private:
    CCommonStringConvert();

    ~CCommonStringConvert();
};
