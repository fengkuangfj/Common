
#pragma once

#include "CommonPublic.h"

#include "CommonSingleInstance.h"

class CCommonIp : public CCommonSingleInstance<CCommonIp>
{
    friend class CCommonSingleInstance<CCommonIp>;

public:
    std::wstring
        GetCurrentIp();

private:
    CCommonIp();

    ~CCommonIp();
};
