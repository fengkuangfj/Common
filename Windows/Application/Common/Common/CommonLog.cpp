
#include "CommonLog.h"

COMMON_ERROR
    CCommonLog::Init(
    _In_ CONST HMODULE & hModule,
    _In_ CONST std::wstring & wstrLogDir ,
    _In_ CONST BOOL & bDrivenByError ,
    _In_ CONST BOOL & bOutputDebugString
    )
{
    COMMON_ERROR CommonError = COMMON_ERROR_SUCCESS;

    std::wstring wstrTempPath = L"";


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                InitializeCriticalSection(&m_WriteLock);
                m_bInitializeCriticalSection = TRUE;

                CommonError = CCommonService::GetInstance()->Init();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"Init failed. CommonError(%d)", CommonError);
                    break;
                }

                m_bDrivenByError = bDrivenByError;

                m_strOutputDebugStringPattern = "[%04d-%02d-%02d %02d:%02d:%02d.%03d][%s][%d][%d] ";
                m_wstrOutputDebugStringPattern = CCommonStringConvert::GetInstance()->AnsiToWideChar(m_strOutputDebugStringPattern);
                m_strPattrenSourceCode = "%s(%d): %s ";
                m_wstrPattrenSourceCode = CCommonStringConvert::GetInstance()->AnsiToWideChar(m_strPattrenSourceCode);
                m_strPattrenNewLine = "\r\n";
                m_wstrPattrenNewLine = CCommonStringConvert::GetInstance()->AnsiToWideChar(m_strPattrenNewLine);
                m_llMaxFileSizeB = 5 * 1024 * 1024;
                m_nMaxFile = 5;

                m_bOutputDebugString = bOutputDebugString;

                m_bService = CCommonService::GetInstance()->CheckCurrentProcessIsService();
                if (m_bService)
                {
                    m_bCanInteractWithDesktop = CCommonService::GetInstance()->CheckCurrentServiceCanInteractWithDesktop();
                }

                m_wstrProcessName = CCommonProcess::GetInstance()->GetName();

                if (NULL != hModule)
                {
                    m_wstrModuleName = CCommonModule::GetInstance()->GetName(hModule);
                }

                m_wstrLogDir = wstrLogDir;

                m_wstrLogPath = GetLogPath();
                if (m_wstrLogPath.length())
                {
                    if (!m_bDrivenByError)
                    {
                        CommonError = CreateFile(m_wstrLogPath);
                    }
                }

                SetInitFlag(TRUE);

                if (!m_bDrivenByError)
                {
#ifdef _WIN64
                    COMMON_LOGW(COMMON_LOG_LEVEL_INFO, L"[%s][%s][%s][%s]",
                        L"x64",
                        m_bService ? L"服务" : L"非服务",
                        m_bCanInteractWithDesktop ? L"可交互" : L"非交互",
                        CCommonProcess::GetInstance()->GetPath().c_str()
                        );
#else
                    COMMON_LOGW(COMMON_LOG_LEVEL_INFO, L"[%s][%s][%s][%s]",
                        L"x86",
                        m_bService ? L"服务" : L"非服务",
                        m_bCanInteractWithDesktop ? L"可交互" : L"非交互",
                        CCommonProcess::GetInstance()->GetPath().c_str()
                        );
#endif
                }
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    if (COMMON_ERROR_SUCCESS != CommonError)
    {
        CommonError = Unload();
    }

    return CommonError;
}

COMMON_ERROR CCommonLog::Unload()
{
    COMMON_ERROR CommonError = COMMON_ERROR_SUCCESS;


    do
    {
        if (GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (GetInitFlag())
            {
                if (INVALID_HANDLE_VALUE != m_hFile)
                {
                    CloseHandle(m_hFile);
                    m_hFile = INVALID_HANDLE_VALUE;
                }

                CommonError = CCommonService::GetInstance()->Unload();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"Unload failed. CommonError(%d)", CommonError);
                }

                if (m_bInitializeCriticalSection)
                {
                    m_bInitializeCriticalSection = FALSE;

                    DeleteCriticalSection(&m_WriteLock);
                }

                SetInitFlag(FALSE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

VOID
    CCommonLog::Log(
    _In_ CONST COMMON_LOG_LEVEL & LogLevel,
    _In_ CONST PCHAR & pFile,
    _In_ CONST ULONG & ulLine,
    _In_ CONST PCHAR & pFunction,
    _In_ _Printf_format_string_ CONST PCHAR & pFormat,
    ...
    )
{
    va_list ArgList;
    size_t nPreBufferLengthCh = 0;
    size_t nPostBufferLengthCh = 0;
    PCHAR pOutputDebugStringBuffer = NULL;
    PCHAR pSpdlogBuffer = NULL;
    SYSTEMTIME SystemTime = { 0 };
    std::wstring wstrText = L"";
    COMMON_ERROR CommonError = COMMON_ERROR_SUCCESS;


    do
    {
        // CommonReadLock lock(ms_InitFlagLock);

        va_start(ArgList, pFormat);

        nPreBufferLengthCh = COMMON_MAX_PATH * 2;

        nPostBufferLengthCh = _vscprintf(pFormat, ArgList) + 1;

        GetLocalTime(&SystemTime);

        pOutputDebugStringBuffer = (PCHAR)calloc(1, (nPreBufferLengthCh + nPostBufferLengthCh) * sizeof(CHAR));
        if (NULL == pOutputDebugStringBuffer)
        {
            break;
        }

        StringCchPrintfA(pOutputDebugStringBuffer, nPreBufferLengthCh + nPostBufferLengthCh, (m_strOutputDebugStringPattern + m_strPattrenSourceCode + m_strPattrenNewLine).c_str(),
            SystemTime.wYear,
            SystemTime.wMonth,
            SystemTime.wDay,
            SystemTime.wHour,
            SystemTime.wMinute,
            SystemTime.wSecond,
            SystemTime.wMilliseconds,
            ConvertLogLevelToString(LogLevel).c_str(),
            GetCurrentProcessId(),
            GetCurrentThreadId(),
            pFile,
            ulLine,
            pFunction
            );

        vsprintf_s(pOutputDebugStringBuffer + strlen(pOutputDebugStringBuffer), nPostBufferLengthCh, pFormat, ArgList);

        strcat_s(pOutputDebugStringBuffer, nPreBufferLengthCh + nPostBufferLengthCh, m_strPattrenNewLine.c_str());

        if (!GetInitFlag() || m_bOutputDebugString)
        {
            OutputDebugStringA(pOutputDebugStringBuffer);
        }

        if (COMMON_LOG_CREATE_FILE_STATUS_UN_CREATED == m_LogCreateFileStatus
            && m_bDrivenByError
            && COMMON_LOG_LEVEL_ERROR == LogLevel )
        {
            CommonError = CreateFile(m_wstrLogPath);
            if (COMMON_ERROR_SUCCESS == CommonError)
            {
#ifdef _WIN64
                COMMON_LOGW(COMMON_LOG_LEVEL_INFO, L"[%s][%s][%s][%s]",
                    L"x64",
                    m_bService ? L"服务" : L"非服务",
                    m_bCanInteractWithDesktop ? L"可交互" : L"非交互",
                    CCommonProcess::GetInstance()->GetPath().c_str()
                    );
#else
                COMMON_LOGW(COMMON_LOG_LEVEL_INFO, L"[%s][%s][%s][%s]",
                    L"x86",
                    m_bService ? L"服务" : L"非服务",
                    m_bCanInteractWithDesktop ? L"可交互" : L"非交互",
                    CCommonProcess::GetInstance()->GetPath().c_str()
                    );
#endif
            }
        }

        Write(CCommonStringConvert::GetInstance()->AnsiToUtf8(pOutputDebugStringBuffer));

        if (COMMON_LOG_LEVEL_ERROR == LogLevel)
        {
            if (IsDebuggerPresent())
            {
                __debugbreak();
            }
            else
            {
#ifdef _DEBUG
                if (m_bCanInteractWithDesktop)
                {
                    if (m_bService)
                    {
                        wstrText = CCommonIp::GetInstance()->GetCurrentIp();
                        wstrText += L"\r\n";
                        wstrText += m_wstrProcessName;
                        wstrText += L"\r\n";
                        wstrText += CCommonTool::GetInstance()->ToWstring(GetCurrentProcessId());

                        CCommonService::GetInstance()->MessageBoxForServiceW(wstrText, L"Common_LOG_LEVEL_ERROR", MB_OK);
                    }
                    else
                    {
                        assert(FALSE);
                    }
                }
#endif
            }
        }
    } while (FALSE);

    if (NULL != pSpdlogBuffer)
    {
        free(pSpdlogBuffer);
        pSpdlogBuffer = NULL;
    }

    if (NULL != pOutputDebugStringBuffer)
    {
        free(pOutputDebugStringBuffer);
        pOutputDebugStringBuffer = NULL;
    }

    va_end(ArgList);

    return;
}

VOID
    CCommonLog::Log(
    _In_ CONST COMMON_LOG_LEVEL & LogLevel,
    _In_ CONST PWCHAR & pFile,
    _In_ CONST ULONG & ulLine,
    _In_ CONST PWCHAR & pFunction,
    _In_ _Printf_format_string_ CONST PWCHAR & pFormat,
    ...
    )
{
    va_list ArgList;
    size_t nPreBufferLengthCh = 0;
    size_t nPostBufferLengthCh = 0;
    PWCHAR pOutputDebugStringBuffer = NULL;
    PWCHAR pSpdlogBuffer = NULL;
    SYSTEMTIME SystemTime = { 0 };
    std::wstring wstrText = L"";
    COMMON_ERROR CommonError = COMMON_ERROR_SUCCESS;


    do
    {
        // CommonReadLock lock(ms_InitFlagLock);

        va_start(ArgList, pFormat);

        nPreBufferLengthCh = COMMON_MAX_PATH * 2;

        nPostBufferLengthCh = _vscwprintf(pFormat, ArgList) + 1;

        GetLocalTime(&SystemTime);

        pOutputDebugStringBuffer = (PWCHAR)calloc(1, (nPreBufferLengthCh + nPostBufferLengthCh) * sizeof(WCHAR));
        if (NULL == pOutputDebugStringBuffer)
        {
            break;
        }

        StringCchPrintfW(pOutputDebugStringBuffer, nPreBufferLengthCh + nPostBufferLengthCh, (m_wstrOutputDebugStringPattern + m_wstrPattrenSourceCode + m_wstrPattrenNewLine).c_str(),
            SystemTime.wYear,
            SystemTime.wMonth,
            SystemTime.wDay,
            SystemTime.wHour,
            SystemTime.wMinute,
            SystemTime.wSecond,
            SystemTime.wMilliseconds,
            ConvertLogLevelToWstring(LogLevel).c_str(),
            GetCurrentProcessId(),
            GetCurrentThreadId(),
            pFile,
            ulLine,
            pFunction
            );

        vswprintf_s(pOutputDebugStringBuffer + wcslen(pOutputDebugStringBuffer), nPostBufferLengthCh, pFormat, ArgList);

        wcscat_s(pOutputDebugStringBuffer, nPreBufferLengthCh + nPostBufferLengthCh, m_wstrPattrenNewLine.c_str());

        if (!GetInitFlag() || m_bOutputDebugString)
        {
            OutputDebugStringW(pOutputDebugStringBuffer);
        }

        if (COMMON_LOG_CREATE_FILE_STATUS_UN_CREATED == m_LogCreateFileStatus
            && m_bDrivenByError
            && COMMON_LOG_LEVEL_ERROR == LogLevel )
        {
            CommonError = CreateFile(m_wstrLogPath);
            if (COMMON_ERROR_SUCCESS == CommonError)
            {
#ifdef _WIN64
                COMMON_LOGW(COMMON_LOG_LEVEL_INFO, L"[%s][%s][%s][%s]",
                    L"x64",
                    m_bService ? L"服务" : L"非服务",
                    m_bCanInteractWithDesktop ? L"可交互" : L"非交互",
                    CCommonProcess::GetInstance()->GetPath().c_str()
                    );
#else
                COMMON_LOGW(COMMON_LOG_LEVEL_INFO, L"[%s][%s][%s][%s]",
                    L"x86",
                    m_bService ? L"服务" : L"非服务",
                    m_bCanInteractWithDesktop ? L"可交互" : L"非交互",
                    CCommonProcess::GetInstance()->GetPath().c_str()
                    );
#endif
            }
        }

        Write(CCommonStringConvert::GetInstance()->WideCharToUtf8(pOutputDebugStringBuffer));

        if (COMMON_LOG_LEVEL_ERROR == LogLevel)
        {
            if (IsDebuggerPresent())
            {
                __debugbreak();
            }
            else
            {
#ifdef _DEBUG
                if (m_bCanInteractWithDesktop)
                {
                    if (m_bService)
                    {
                        wstrText = CCommonIp::GetInstance()->GetCurrentIp();
                        wstrText += L"\r\n";
                        wstrText += m_wstrProcessName;
                        wstrText += L"\r\n";
                        wstrText += CCommonTool::GetInstance()->ToWstring(GetCurrentProcessId());

                        CCommonService::GetInstance()->MessageBoxForServiceW(wstrText, L"Common_LOG_LEVEL_ERROR", MB_OK);
                    }
                    else
                    {
                        assert(FALSE);
                    }
                }
#endif
            }
        }
    } while (FALSE);

    if (NULL != pSpdlogBuffer)
    {
        free(pSpdlogBuffer);
        pSpdlogBuffer = NULL;
    }

    if (NULL != pOutputDebugStringBuffer)
    {
        free(pOutputDebugStringBuffer);
        pOutputDebugStringBuffer = NULL;
    }

    va_end(ArgList);

    return;
}

CCommonLog::CCommonLog()
{
    m_bOutputDebugString = FALSE;
    m_bCanInteractWithDesktop = TRUE;
    m_bService = FALSE;
    m_strOutputDebugStringPattern = "";
    m_wstrOutputDebugStringPattern = L"";
    m_strPattrenSourceCode = "";
    m_wstrPattrenSourceCode = L"";
    m_strPattrenNewLine = "";
    m_wstrPattrenNewLine = L"";
    m_llMaxFileSizeB = 0;
    m_nMaxFile = 0;
    m_wstrProcessName = L"";
    m_wstrModuleName = L"";
    m_wstrLogDir = L"";
    m_hFile = INVALID_HANDLE_VALUE;
    m_liFileSizeB.QuadPart = 0;
    m_wstrLogPath = L"";
    m_bDrivenByError = TRUE;
    m_LogCreateFileStatus = COMMON_LOG_CREATE_FILE_STATUS_UN_CREATED;
    m_bInitializeCriticalSection = FALSE;
}

CCommonLog::~CCommonLog()
{
    m_bOutputDebugString = FALSE;
    m_bCanInteractWithDesktop = TRUE;
    m_bService = FALSE;
    m_strOutputDebugStringPattern = "";
    m_wstrOutputDebugStringPattern = L"";
    m_strPattrenSourceCode = "";
    m_wstrPattrenSourceCode = L"";
    m_strPattrenNewLine = "";
    m_wstrPattrenNewLine = L"";
    m_llMaxFileSizeB = 0;
    m_nMaxFile = 0;
    m_wstrProcessName = L"";
    m_wstrModuleName = L"";
    m_wstrLogDir = L"";
    m_hFile = INVALID_HANDLE_VALUE;
    m_liFileSizeB.QuadPart = 0;
    m_wstrLogPath = L"";
    m_bDrivenByError = TRUE;
    m_LogCreateFileStatus = COMMON_LOG_CREATE_FILE_STATUS_UN_CREATED;
    m_bInitializeCriticalSection = FALSE;
}

std::string
    CCommonLog::ConvertLogLevelToString(
    _In_ CONST COMMON_LOG_LEVEL & LogLevel
    )
{
    std::string strRet = "";


    do
    {
        switch (LogLevel)
        {
        case COMMON_LOG_LEVEL_INFO:
            {
                strRet = "INFO";
                break;
            }
        case COMMON_LOG_LEVEL_WARNING:
            {
                strRet = "WARNING";
                break;
            }
        case COMMON_LOG_LEVEL_ERROR:
            {
                strRet = "ERROR";
                break;
            }
        default:
            {
                strRet = "UNKNOWN";
                break;
            }
        }
    } while (FALSE);

    return strRet;
}

std::wstring
    CCommonLog::ConvertLogLevelToWstring(
    _In_ CONST COMMON_LOG_LEVEL & LogLevel
    )
{
    std::wstring wstrRet = L"";


    do
    {
        switch (LogLevel)
        {
        case COMMON_LOG_LEVEL_INFO:
            {
                wstrRet = L"INFO";
                break;
            }
        case COMMON_LOG_LEVEL_WARNING:
            {
                wstrRet = L"WARNING";
                break;
            }
        case COMMON_LOG_LEVEL_ERROR:
            {
                wstrRet = L"ERROR";
                break;
            }
        default:
            {
                wstrRet = L"UNKNOWN";
                break;
            }
        }
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonLog::GetLogPath()
{
    std::wstring wstrRet = L"";

    std::wstring wstrFileName = L"";


    do
    {
        if (!m_wstrLogDir.length())
        {
            m_wstrLogDir = GetLogDir();
            if (!m_wstrLogDir.length())
            {
                break;
            }
        }

        wstrFileName = GetLogFileName();
        if (!wstrFileName.length())
        {
            break;
        }

        wstrRet = m_wstrLogDir;
        wstrRet += L"\\";
        wstrRet += wstrFileName;
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonLog::GetLogDir()
{
    std::wstring wstrRet = L"";


    do
    {
        wstrRet = CSsDedsPath::GetInstance()->GetPath(COMMON_PATH_TYPE_LOG_DIR);
        wstrRet += L"\\";
        wstrRet += m_wstrProcessName;

        if (m_wstrModuleName.length())
        {
            wstrRet += L"\\";
            wstrRet += m_wstrModuleName;
        }
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonLog::GetLogFileName()
{
    std::wstring wstrRet = L"";


    do
    {
        wstrRet = m_wstrProcessName;
        wstrRet += L"_";
        wstrRet += CCommonTool::GetInstance()->ToWstring(GetCurrentProcessId());
        wstrRet += L"_";

        if (m_wstrModuleName.length())
        {
            wstrRet += m_wstrModuleName;
        }
        else
        {
#ifdef _DEBUG
#ifdef _WIN64
            wstrRet += L"Debug_x64";
#else
            wstrRet += L"Debug_x86";
#endif
#else
#ifdef _WIN64
            wstrRet += L"Release_x64";
#else
            wstrRet += L"Release_x86";
#endif
#endif
        }

        wstrRet += L"_";
        wstrRet += CCommonTool::GetInstance()->GenGuidRegistryFormat();

        wstrRet += L".log";
    } while (FALSE);

    return wstrRet;
}

VOID
    CCommonLog::Write(
    _In_ CONST std::string & strLog
    )
{
    WCHAR wchTempPath[COMMON_MAX_PATH] = {0};
    std::wstring wstrBasePath = L"";
    DWORD dwNumberOfBytesWritten = 0;
    COMMON_ERROR CommonError = COMMON_ERROR_SUCCESS;
    std::wstring wstrTempPath = L"";
    BOOL bEnterCriticalSection = FALSE;


    do
    {
        if (m_bInitializeCriticalSection)
        {
            if (0 == m_mapThreadList[GetCurrentThreadId()])
            {
                EnterCriticalSection(&m_WriteLock);
                bEnterCriticalSection = TRUE;
            }
        }

        m_mapThreadList[GetCurrentThreadId()]++;

        if (INVALID_HANDLE_VALUE == m_hFile)
        {
            break;
        }

        if (! WriteFile(
            m_hFile,
            strLog .c_str(),
            (DWORD)strLog.length(),
            &dwNumberOfBytesWritten,
            NULL
            ))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"WriteFile (%s) failed. msdn(%d)", m_wstrLogPath.c_str(), GetLastError());
            break;
        }

        if (! SetEndOfFile(m_hFile))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"SetEndOfFile (%s) failed. msdn(%d)", m_wstrLogPath.c_str(), GetLastError());
            break;
        }

        if (! FlushFileBuffers(m_hFile))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"FlushFileBuffers (%s) failed. msdn(%d)", m_wstrLogPath.c_str(), GetLastError());
            break;
        }

        if (! GetFileSizeEx(m_hFile, &m_liFileSizeB))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetFileSizeEx (%s) failed. msdn(%d)", m_wstrLogPath.c_str(), GetLastError());
            break;
        }

        if (m_llMaxFileSizeB <= m_liFileSizeB.QuadPart)
        {
            CloseHandle(m_hFile);
            m_hFile = INVALID_HANDLE_VALUE;

            wstrBasePath = m_wstrLogPath;
            m_wstrLogPath = L"";

            do
            {
                if (!PathYetAnotherMakeUniqueName(wchTempPath, wstrBasePath.c_str(), NULL, PathFindFileName(wstrBasePath.c_str())))
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"PathYetAnotherMakeUniqueName (%s) failed. msdn(%d)", wstrBasePath.c_str(), GetLastError());
                    continue;
                }

                CommonError = CreateFile(wchTempPath);
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    if (COMMON_ERROR_CREATE_DIRECTORY_FAILED == CommonError)
                    {
                        COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"(%s) CSsDedsPath::GetInstance()->MakeSureParentExist failed. CommonError(%d)", wchTempPath, CommonError);
                        break;
                    }
                    else
                    {
                        COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"(%s) CSsDedsPath::GetInstance()->MakeSureParentExist failed. CommonError(%d)", wchTempPath, CommonError);
                        continue;
                    }
                }

                break;
            } while (TRUE);

            m_wstrLogPath = wchTempPath;
        }
    } while (FALSE);

    m_mapThreadList[GetCurrentThreadId()]--;

    if (bEnterCriticalSection)
    {
        LeaveCriticalSection(&m_WriteLock);
    }

    return ;
}

COMMON_ERROR
    CCommonLog::CreateFile(
    _In_ CONST std::wstring & wstrPath
    )
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;

    std::wstring wstrTempPath = L"";


    do
    {
        CommonError = CSsDedsPath::GetInstance()->MakeSureParentExist(wstrPath);
        if (COMMON_ERROR_SUCCESS != CommonError)
        {
            if (COMMON_ERROR_CREATE_DIRECTORY_FAILED == CommonError)
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"(%s) CSsDedsPath::GetInstance()->MakeSureParentExist failed. CommonError(%d)", m_wstrLogPath.c_str(), CommonError);
            }
            else
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"(%s) CSsDedsPath::GetInstance()->MakeSureParentExist failed. CommonError(%d)", m_wstrLogPath.c_str(), CommonError);
            }

            m_LogCreateFileStatus = COMMON_LOG_CREATE_FILE_STATUS_FAILED;

            break;
        }

        if (MAX_PATH < wstrPath.length())
        {
            if (wstrPath.at(0) == '\\')
            {
                wstrTempPath = COMMON_LONG_PATH_PRE + wstrPath;
            }
            else
            {
                wstrTempPath = COMMON_LONG_PATH_PRE;
                wstrTempPath += L"\\";
                wstrTempPath += wstrPath;
            }
        }
        else
        {
            wstrTempPath = wstrPath;
        }

        m_hFile = ::CreateFile(
            wstrTempPath.c_str(),
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );
        if (INVALID_HANDLE_VALUE == m_hFile)
        {
            if (ERROR_ACCESS_DENIED != GetLastError())
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CreateFile (%s) failed. msdn(%d)", m_wstrLogPath.c_str(), GetLastError());
            }
            else
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"CreateFile (%s) failed. msdn(%d)", m_wstrLogPath.c_str(), GetLastError());
            }

            m_LogCreateFileStatus = COMMON_LOG_CREATE_FILE_STATUS_FAILED;

            break;
        }

        m_LogCreateFileStatus = COMMON_LOG_CREATE_FILE_STATUS_CREATED;

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}
