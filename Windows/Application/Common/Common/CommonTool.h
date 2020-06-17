
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonSingleInstance.h"

#include <Objbase.h>

//
// #include <aclapi.h>
// #include <sddl.h>
// #include <shellapi.h>
// #include <vector>
// #include <list>
//
// #ifndef SECURITY_WIN32
// #define SECURITY_WIN32
// #endif // SECURITY_WIN32
// #include <Security.h>
// #pragma comment(lib, "Secur32.lib")

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
