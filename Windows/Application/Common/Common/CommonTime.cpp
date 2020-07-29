
#include "CommonTime.h"

CCommonTime::CCommonTime()
{
    ;
}

CCommonTime::~CCommonTime()
{
    ;
}

time_t
    CCommonTime::GetCurrentUnixTimestampMillisecond()
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
    boost::posix_time::time_duration time_from_epoch = boost::posix_time::microsec_clock::universal_time() - epoch;
    return time_from_epoch.total_milliseconds();
}

time_t
    CCommonTime::SystemTimeToUnixTimeMillisecond(
    _In_ SYSTEMTIME st
    )
{
    FILETIME ft = { 0 };
    SystemTimeToFileTime(&st, &ft);

    time_t ret = 0;
    LONGLONG ll = 0;
    ULARGE_INTEGER ui = { 0 };
    ui.LowPart = ft.dwLowDateTime;
    ui.HighPart = ft.dwHighDateTime;
#pragma warning(push)
#pragma warning(disable : 4293)
    ll = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;
#pragma warning(pop)
    ret = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000);

    time_t timeUtc = 0;
    time(&timeUtc);

    tm tmLocal = {0};
    localtime_s(&tmLocal, &timeUtc);

    tm tmGm = {0};
    gmtime_s(&tmGm, &timeUtc);

    ret -= ((tmLocal.tm_hour - tmGm.tm_hour) * 60 * 60 * 1000);

    return ret;
}

SYSTEMTIME
    CCommonTime::UnixTimeMillisecondToSystemTime(
    _In_ time_t time
    )
{
    time_t tSeconds = time / 1000;
    tm timeinfo;
    localtime_s(&timeinfo, &tSeconds);

    SYSTEMTIME st = { (WORD)(1900 + timeinfo.tm_year),
        (WORD)(1 + timeinfo.tm_mon),
        (WORD)timeinfo.tm_wday,
        (WORD)timeinfo.tm_mday,
        (WORD)timeinfo.tm_hour,
        (WORD)timeinfo.tm_min,
        (WORD)timeinfo.tm_sec,
        (WORD)0 };

    return st;
}
