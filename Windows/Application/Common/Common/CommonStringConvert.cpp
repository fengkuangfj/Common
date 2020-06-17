
#include "CommonStringConvert.h"
// #pragma warning(push)
// #pragma warning(disable : 4005)
// #pragma warning(disable : 4100)
// #pragma warning(disable : 4189)
// #pragma warning(disable : 6387)
// #include <atlenc.h>
// #pragma warning(pop)
//
// #include <boost/regex.hpp>

std::wstring
    CCommonStringConvert::AnsiToWideChar(
    _In_ CONST std::string & strSrc
    )
{
    std::wstring wstrRet = L"";

    int nLengthB = 0;
    PWCHAR pwchBuffer = NULL;


    do
    {
        if (strSrc.empty())
        {
            break;
        }

        nLengthB = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)strSrc.length(), NULL, 0);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"MultiByteToWideChar failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB += 1;

        pwchBuffer = (PWCHAR)calloc(1, nLengthB * sizeof(WCHAR));
        if (NULL == pwchBuffer)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)strSrc.length(), pwchBuffer, nLengthB);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"MultiByteToWideChar failed. msdn(%d)", GetLastError());
            break;
        }

        wstrRet = pwchBuffer;
    } while (FALSE);

    if (NULL != pwchBuffer)
    {
        free(pwchBuffer);
        pwchBuffer = NULL;
    }

    return wstrRet;
}

std::string
    CCommonStringConvert::AnsiToUtf8(
    _In_ CONST std::string & strSrc
    )
{
    std::string strRet = "";

    std::wstring wstrTemp = L"";


    do
    {
        wstrTemp = AnsiToWideChar(strSrc);

        strRet = WideCharToUtf8(wstrTemp);
    } while (FALSE);

    return strRet;
}

std::string
    CCommonStringConvert::WideCharToAnsi(
    _In_ CONST std::wstring & strSrc
    )
{
    std::string strRet = "";

    int nLengthB = 0;
    PCHAR pchBuffer = NULL;


    do
    {
        if (strSrc.empty())
        {
            break;
        }

        nLengthB = WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), (int)strSrc.length(), NULL, 0, NULL, NULL);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"WideCharToMultiByte failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB += 1;

        pchBuffer = (PCHAR)calloc(1, nLengthB * sizeof(CHAR));
        if (NULL == pchBuffer)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB = WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), (int)strSrc.length(), pchBuffer, nLengthB, NULL, NULL);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"WideCharToMultiByte failed. msdn(%d)", GetLastError());
            break;
        }

        strRet = pchBuffer;
    } while (FALSE);

    if (NULL != pchBuffer)
    {
        free(pchBuffer);
        pchBuffer = NULL;
    }

    return strRet;
}

std::string
    CCommonStringConvert::WideCharToUtf8(
    _In_ CONST std::wstring & strSrc
    )
{
    std::string strRet = "";

    int nLengthB = 0;
    PCHAR pchBuffer = NULL;


    do
    {
        if (strSrc.empty())
        {
            break;
        }

        nLengthB = WideCharToMultiByte(CP_UTF8, 0, strSrc.c_str(), (int)strSrc.length(), NULL, 0, NULL, NULL);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"WideCharToMultiByte failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB += 1;

        pchBuffer = (PCHAR)calloc(1, nLengthB * sizeof(CHAR));
        if (NULL == pchBuffer)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB = WideCharToMultiByte(CP_UTF8, 0, strSrc.c_str(), (int)strSrc.length(), pchBuffer, nLengthB, NULL, NULL);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"WideCharToMultiByte failed. msdn(%d)", GetLastError());
            break;
        }

        strRet = pchBuffer;
    } while (FALSE);

    if (NULL != pchBuffer)
    {
        free(pchBuffer);
        pchBuffer = NULL;
    }

    return strRet;
}

std::string
    CCommonStringConvert::Utf8ToAnsi(
    _In_ CONST std::string & strSrc
    )
{
    std::string strRet = "";

    std::wstring wstrTemp = L"";


    do
    {
        wstrTemp = Utf8ToWideChar(strSrc);

        strRet = WideCharToAnsi(wstrTemp);
    } while (FALSE);

    return strRet;
}

std::wstring
    CCommonStringConvert::Utf8ToWideChar(
    _In_ CONST std::string & strSrc
    )
{
    std::wstring wstrRet = L"";

    int nLengthB = 0;
    PWCHAR pwchBuffer = NULL;


    do
    {
        if (strSrc.empty())
        {
            break;
        }

        nLengthB = MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), (int)strSrc.length(), NULL, 0);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"MultiByteToWideChar failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB += 1;

        pwchBuffer = (PWCHAR)calloc(1, nLengthB * sizeof(WCHAR));
        if (NULL == pwchBuffer)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        nLengthB = MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), (int)strSrc.length(), pwchBuffer, nLengthB);
        if (0 == nLengthB)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"MultiByteToWideChar failed. msdn(%d)", GetLastError());
            break;
        }

        wstrRet = pwchBuffer;
    } while (FALSE);

    if (NULL != pwchBuffer)
    {
        free(pwchBuffer);
        pwchBuffer = NULL;
    }

    return wstrRet;
}

VOID
    CCommonStringConvert::ToLower(
    _Inout_ std::wstring & wstrSrc
    )
{
    do
    {
        if (0 == wstrSrc.length())
        {
            break;
        }

        for (int i = 0; i < (int)wstrSrc.length(); i++)
        {
            if (L'A' <= wstrSrc.c_str()[i] && L'Z' >= wstrSrc.c_str()[i])
            {
                wstrSrc.replace(i, 1, 1, wstrSrc.c_str()[i] + (L'a' - L'A'));
            }
        }
    } while (FALSE);

    return ;
}

CCommonStringConvert::CCommonStringConvert()
{
    ;
}

CCommonStringConvert::~CCommonStringConvert()
{
    ;
}
