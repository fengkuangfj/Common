
#pragma once

#include "CommonPublic.h"

#include "CommonEnum.h"
#include "CommonError.h"
#include "CommonGlobalVariable.h"
#include "CommonIp.h"
#include "CommonPath.h"
#include "CommonProcess.h"
#include "CommonService.h"
#include "CommonSingleInstance.h"
#include "CommonStringConvert.h"
#include "CommonTool.h"

#pragma warning(push)
#pragma warning(disable : 6387)
#include <Shlobj.h>
#pragma warning(pop)
#include <shlwapi.h>
#include <strsafe.h>

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Shlwapi.lib")

#define COMMON_LOGA(LogLevel, Format, ...) CCommonLog::GetInstance()->Log(LogLevel, __FILE__, __LINE__, __FUNCTION__, Format, __VA_ARGS__)
#define COMMON_LOGW(LogLevel, Format, ...) CCommonLog::GetInstance()->Log(LogLevel, __FILEW__, __LINE__, _CRT_WIDE(__FUNCTION__), Format, __VA_ARGS__)

class CCommonLog : public CCommonSingleInstance<CCommonLog>
{
    friend class CCommonSingleInstance<CCommonLog>;

public:
    COMMON_ERROR
        Init(
        _In_ CONST HMODULE & hModule,
        _In_ CONST std::wstring & wstrLogDir ,
        _In_ CONST BOOL & bDrivenByError ,
        _In_ CONST BOOL & bOutputDebugString
        );

    COMMON_ERROR
        Unload();

    VOID
        Log(
        _In_ CONST COMMON_LOG_LEVEL LogLevel,
        _In_ CONST PCHAR pFile,
        _In_ CONST ULONG ulLine,
        _In_ CONST PCHAR pFunction,
        _In_ _Printf_format_string_ CONST PCHAR pFormat,
        ...
        );

    VOID
        Log(
        _In_ CONST COMMON_LOG_LEVEL LogLevel,
        _In_ CONST PWCHAR pFile,
        _In_ CONST ULONG ulLine,
        _In_ CONST PWCHAR pFunction,
        _In_ _Printf_format_string_ CONST PWCHAR pFormat,
        ...
        );

private:
    BOOL m_bOutputDebugString;
    BOOL m_bCanInteractWithDesktop;
    BOOL m_bService;
    std::string m_strOutputDebugStringPattern;
    std::wstring m_wstrOutputDebugStringPattern;
    std::string m_strPattrenSourceCode;
    std::wstring m_wstrPattrenSourceCode;
    std::string m_strPattrenNewLine;
    std::wstring m_wstrPattrenNewLine;
    LONGLONG m_llMaxFileSizeB;
    size_t m_nMaxFile;
    std::wstring m_wstrProcessName;
    std::wstring m_wstrModuleName;
    std::wstring m_wstrLogDir;
    HANDLE m_hFile;
    LARGE_INTEGER m_liFileSizeB;
    std::wstring m_wstrLogPath ;
    BOOL m_bDrivenByError;
    COMMON_LOG_CREATE_FILE_STATUS m_LogCreateFileStatus;
    CRITICAL_SECTION m_WriteLock;
    std::map <int, int> m_mapThreadList;
    BOOL m_bInitializeCriticalSection ;

    CCommonLog();

    ~CCommonLog();

    std::string
        ConvertLogLevelToString(
        _In_ CONST COMMON_LOG_LEVEL & LogLevel
        );

    std::wstring
        ConvertLogLevelToWstring(
        _In_ CONST COMMON_LOG_LEVEL & LogLevel
        );

    std::wstring
        GetLogPath();

    std::wstring
        GetLogDir();

    std::wstring
        GetLogFileName();

    VOID
        Write(
        _In_ CONST std::string & strLog
        );

    COMMON_ERROR
        CreateFile(
        _In_ CONST std::wstring & wstrPath
        );
};
