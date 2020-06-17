
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonModule.h"
#include "CommonSingleInstance.h"

class CCommonProcess : public CCommonSingleInstance<CCommonProcess>
{
    friend class CCommonSingleInstance<CCommonProcess>;

public:
    std::wstring
        GetPath();

    std::wstring
        GetName();

private:
    CCommonProcess();

    ~CCommonProcess();
};
