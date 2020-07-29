
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonSingleInstance.h"

class CCommonTime : public CCommonSingleInstance<CCommonTime>
{
    friend class CCommonSingleInstance<CCommonTime>;

public:
    time_t
        GetCurrentUnixTimestampMillisecond();

    time_t
        SystemTimeToUnixTimeMillisecond(
        _In_ SYSTEMTIME st
        );

    SYSTEMTIME
        UnixTimeMillisecondToSystemTime(
        _In_ time_t time
        );

private:
    CCommonTime();

    ~CCommonTime();
};
