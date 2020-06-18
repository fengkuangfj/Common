
#include "CommonNtHelper.h"

COMMON_ERROR
    CCommonNtHelper::Init()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (!GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (!GetInitFlag())
            {
                m_hNtdll_dll = GetModuleHandle(L"ntdll.dll");
                if (NULL == m_hNtdll_dll)
                {
                    CommonError = COMMON_ERROR_GET_MODULE_HANDLE_FAILED;
                    COMMON_LOGW(COMMON_LOG_LEVEL_ERROR, L"GetModuleHandle (%s) failed. msdn(%d)", L"ntdll.dll", GetLastError());
                    break;
                }

                m_Ntdll_dll_NtQueryObject = (Ntdll_dll_NtQueryObject_Proc_Type)GetProcAddress(m_hNtdll_dll, "NtQueryObject");
                if (NULL == m_Ntdll_dll_NtQueryObject)
                {
                    CommonError = COMMON_ERROR_GET_PROC_ADDRESS_FAILED;
                    COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"GetProcAddress (%s) failed. msdn(%d)", L"NtQueryObject", GetLastError());
                }

                m_Ntdll_dll_NtQueryInformationFile = (Ntdll_dll_NtQueryInformationFile_Proc_Type)GetProcAddress(m_hNtdll_dll, "NtQueryInformationFile");
                if (NULL == m_Ntdll_dll_NtQueryInformationFile)
                {
                    CommonError = COMMON_ERROR_GET_PROC_ADDRESS_FAILED;
                    COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"GetProcAddress (%s) failed. msdn(%d)", L"NtQueryInformationFile", GetLastError());
                }

                m_Ntdll_dll_RtlGetNtVersionNumbers = (Ntdll_dll_RtlGetNtVersionNumbers_Proc_Type)GetProcAddress(m_hNtdll_dll, "RtlGetNtVersionNumbers");
                if (NULL == m_Ntdll_dll_RtlGetNtVersionNumbers)
                {
                    CommonError = COMMON_ERROR_GET_PROC_ADDRESS_FAILED;
                    COMMON_LOGW(COMMON_LOG_LEVEL_WARNING, L"GetProcAddress (%s) failed. msdn(%d)", L"RtlGetNtVersionNumbers", GetLastError());
                }

                SetInitFlag(TRUE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

COMMON_ERROR
    CCommonNtHelper::Unload()
{
    COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


    do
    {
        if (GetInitFlag())
        {
            CommonWriteLock lock(ms_InitFlagLock);
            if (GetInitFlag())
            {
                m_Ntdll_dll_NtQueryObject = NULL;
                m_Ntdll_dll_NtQueryInformationFile = NULL;
                m_Ntdll_dll_RtlGetNtVersionNumbers = NULL;

                m_hNtdll_dll = NULL;

                SetInitFlag(FALSE);
            }
        }

        CommonError = COMMON_ERROR_SUCCESS;
    } while (FALSE);

    return CommonError;
}

NTSTATUS
    CCommonNtHelper::NtQueryObject(
    _In_opt_ HANDLE Handle,
    _In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
    _Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
    _In_ ULONG ObjectInformationLength,
    _Out_opt_ PULONG ReturnLength
    )
{
    if (NULL != m_Ntdll_dll_NtQueryObject)
    {
        return m_Ntdll_dll_NtQueryObject(
            Handle,
            ObjectInformationClass,
            ObjectInformation,
            ObjectInformationLength,
            ReturnLength
            );
    }
    else
    {
        return STATUS_UNSUCCESSFUL;
    }
}

NTSTATUS
    CCommonNtHelper::NtQueryInformationFile(
    _In_ HANDLE FileHandle,
    _Out_ PIO_STATUS_BLOCK IoStatusBlock,
    _Out_writes_bytes_(Length) PVOID FileInformation,
    _In_ ULONG Length,
    _In_ FILE_INFORMATION_CLASS FileInformationClass
    )
{
    if (NULL != m_Ntdll_dll_NtQueryInformationFile)
    {
        return m_Ntdll_dll_NtQueryInformationFile(
            FileHandle,
            IoStatusBlock,
            FileInformation,
            Length,
            FileInformationClass
            );
    }
    else
    {
        return STATUS_UNSUCCESSFUL;
    }
}

VOID
    CCommonNtHelper::RtlGetNtVersionNumbers(
    DWORD * dwMajorVersion,
    DWORD * dwMinorVersion,
    DWORD * dwBuildNumber
    )
{
    if (NULL != m_Ntdll_dll_RtlGetNtVersionNumbers)
    {
        m_Ntdll_dll_RtlGetNtVersionNumbers(
            dwMajorVersion,
            dwMinorVersion,
            dwBuildNumber
            );
    }

    return;
}

CCommonNtHelper::CCommonNtHelper()
{
    m_hNtdll_dll = NULL;
    m_Ntdll_dll_NtQueryObject = NULL;
    m_Ntdll_dll_NtQueryInformationFile = NULL;
    m_Ntdll_dll_RtlGetNtVersionNumbers = NULL;
}

CCommonNtHelper::~CCommonNtHelper()
{
    m_hNtdll_dll = NULL;
    m_Ntdll_dll_NtQueryObject = NULL;
    m_Ntdll_dll_NtQueryInformationFile = NULL;
    m_Ntdll_dll_RtlGetNtVersionNumbers = NULL;
}
