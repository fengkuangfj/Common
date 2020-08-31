
#include "CommonProcess.h"

COMMON_ERROR
    CCommonProcess::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_SUCCESS;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                CommonError = CCommonNtHelper::GetInstance()->Init();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonNtHelper::GetInstance()->Init failed. CommonError(%d)", CommonError);
                    break;
                }

                SetInitFlag(TRUE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

COMMON_ERROR
    CCommonProcess::Unload()
{
    COMMON_ERROR CommonError = COMMON_ERROR_SUCCESS;


    do
    {
        if (GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (GetInitFlag())
            {
                CommonError = CCommonNtHelper::GetInstance()->Unload();
                if (COMMON_ERROR_SUCCESS != CommonError)
                {
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonNtHelper::GetInstance()->Unload failed. CommonError(%d)", CommonError);
                }

                SetInitFlag(FALSE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

std::wstring
    CCommonProcess::GetPath()
{
    std::wstring wstrRet = L"";


    do
    {
        wstrRet = CCommonModule::GetInstance()->GetPath(NULL);
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonProcess::GetPath(
    _In_ CONST DWORD & dwPid
    )
{
    std::wstring wstrRet = L"";

    PROCESS_BASIC_INFORMATION pbi = { 0 };
    ULONG ulReturnLength = 0;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    PEB Peb = { 0 };
    SIZE_T NumberOfBytesRead = 0;
    WCHAR * pwchPath = NULL;
    HANDLE hProcess = NULL;
    std::wstring wstrTemp = L"";
    RTL_USER_PROCESS_PARAMETERS RtlUserProcessParameters = { 0 };


    do
    {
        if (0 == dwPid || 4 == dwPid)
        {
            break;
        }

        hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, dwPid);
        if (NULL == hProcess)
        {
            if (ERROR_ACCESS_DENIED != GetLastError()
                && ERROR_INVALID_PARAMETER != GetLastError())
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"OpenProcess failed. msdn(%d)", GetLastError());
            }

            break;
        }

        ntStatus = CCommonNtHelper::GetInstance()->NtQueryInformationProcess(
            hProcess,
            ProcessBasicInformation,
            &pbi,
            sizeof(pbi),
            &ulReturnLength
            );
        if (!NT_SUCCESS(ntStatus))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonNtHelper::GetInstance()->NtQueryInformationProcess failed. ntStatus(0x%08x)", ntStatus);
            break;
        }

        if (NULL == pbi.PebBaseAddress)
        {
            // COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"m_NtQueryInformationProcess failed. msdn(%d)", GetLastError());
            break;
        }

        if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &Peb, sizeof(Peb), &NumberOfBytesRead))
        {
            if (ERROR_PARTIAL_COPY == GetLastError()
                || ERROR_ACCESS_DENIED == GetLastError())
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"ReadProcessMemory failed. msdn(%d)", GetLastError());
            }
            else
            {
                COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"ReadProcessMemory failed. msdn(%d)", GetLastError());
            }

            break;
        }

        if (NULL == Peb.ProcessParameters)
        {
            // COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"ReadProcessMemory failed. msdn(%d)", GetLastError());
            break;
        }

        if (!ReadProcessMemory(hProcess, Peb.ProcessParameters, &RtlUserProcessParameters, sizeof(RtlUserProcessParameters), &NumberOfBytesRead))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"ReadProcessMemory failed. msdn(%d)", GetLastError());
            break;
        }

        if (NULL == RtlUserProcessParameters.ImagePathName.Buffer)
        {
            // COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"ReadProcessMemory failed. msdn(%d)", GetLastError());
            break;
        }

        pwchPath = (WCHAR *)calloc(1, RtlUserProcessParameters.ImagePathName.Length + sizeof(WCHAR));
        if (NULL == pwchPath)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"calloc failed. msdn(%d)", GetLastError());
            break;
        }

        if (!ReadProcessMemory(hProcess, RtlUserProcessParameters.ImagePathName.Buffer, pwchPath, RtlUserProcessParameters.ImagePathName.Length + sizeof(WCHAR), &NumberOfBytesRead))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"ReadProcessMemory failed. msdn(%d)", GetLastError());
            break;
        }

        wstrRet = CCommonPath::GetInstance()->ToLong(pwchPath);
    } while (FALSE);

    if (NULL != pwchPath)
    {
        free(pwchPath);
        pwchPath = NULL;
    }

    if (NULL != hProcess)
    {
        CloseHandle(hProcess);
        hProcess = NULL;
    }

    return wstrRet;
}

DWORD
    CCommonProcess::GetParentPid()
{
    DWORD dwRet = (DWORD)-1;

    HANDLE hProcess = NULL;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    PROCESS_BASIC_INFORMATION pbi = { 0 };
    ULONG ulReturnLength = 0;


    do
    {
        hProcess = GetCurrentProcess();
        if (NULL == hProcess)
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetCurrentProcess failed. msdn(%d)", GetLastError());
            break;
        }

        ntStatus = CCommonNtHelper::GetInstance()->NtQueryInformationProcess(
            hProcess,
            ProcessBasicInformation,
            &pbi,
            sizeof(pbi),
            &ulReturnLength
            );
        if (!NT_SUCCESS(ntStatus))
        {
            COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"CCommonNtHelper::GetInstance()->NtQueryInformationProcess failed. ntStatus(0x%08x)", ntStatus);
            break;
        }

        dwRet = (DWORD)pbi.Reserved3;
    } while (FALSE);

    return dwRet;
}

std::wstring
    CCommonProcess::GetParentPath()
{
    std::wstring wstrRet = L"";

    DWORD dwParentPid = (DWORD)-1;


    do
    {
        dwParentPid = GetParentPid();
        if (0 > dwParentPid)
        {
            break;
        }

        wstrRet = GetPath(dwParentPid);
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonProcess::GetName()
{
    std::wstring wstrRet = L"";


    do
    {
        wstrRet = CCommonModule::GetInstance()->GetName(NULL);
    } while (FALSE);

    return wstrRet;
}

std::wstring
    CCommonProcess::GetName(
    _In_ CONST DWORD & dwPid
    )
{
    std::wstring wstrRet = L"";

    std::wstring wstrPath = L"";
    WCHAR * pName = NULL;


    do
    {
        wstrPath = GetPath(dwPid);
        if (!wstrPath.length())
        {
            break;
        }

        pName = PathFindFileName(wstrPath.c_str());
        if (NULL == pName)
        {
            break;
        }

        wstrRet = pName;
    } while (FALSE);

    return wstrRet;
}

CCommonProcess::CCommonProcess()
{
    ;
}

CCommonProcess::~CCommonProcess()
{
    ;
}
