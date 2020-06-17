
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonSingleInstance.h"

class CCommonSession : public CCommonSingleInstance<CCommonSession>
{
    friend class CCommonSingleInstance<CCommonSession>;

public:
    DWORD
        GetCurrentProcessSessionId();

private:
    CCommonSession();

    ~CCommonSession();
};
