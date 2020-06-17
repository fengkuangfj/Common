
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonSingleInstance.h"

class CCommonModule : public CCommonSingleInstance<CCommonModule>
{
    friend class CCommonSingleInstance<CCommonModule>;

public:
    std::wstring
        GetPath(
        _In_ CONST HMODULE & hModule
        );

    std::wstring
        GetName(
        _In_ CONST HMODULE & hModule
        );

private:
    CCommonModule();

    ~CCommonModule();
};
