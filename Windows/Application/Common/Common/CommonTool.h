
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonSingleInstance.h"

#include <Objbase.h>

#pragma comment(lib, "Ole32.lib")

class CCommonTool : public CCommonSingleInstance<CCommonTool>
{
    friend class CCommonSingleInstance<CCommonTool>;

public:
    std::wstring
        GenGuidRegistryFormat();

    std::wstring
        ToWstring(
        _In_ CONST int & _Val
        );

    std::wstring
        ToWstring(
        _In_ CONST unsigned long & _Val
        );

    std::wstring
        ToWstring(
        _In_ CONST long long & _Val
        );

private:
    CCommonTool();

    ~CCommonTool();

    GUID
        GenGuidStructFormat();
};
