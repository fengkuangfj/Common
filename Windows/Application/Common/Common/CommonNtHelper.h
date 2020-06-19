
#pragma once

#include "CommonPublic.h"

#include "CommonLog.h"
#include "CommonNtDefine.h"
#include "CommonSingleInstance.h"

class CCommonNtHelper : public CCommonSingleInstance<CCommonNtHelper>
{
    friend class CCommonSingleInstance<CCommonNtHelper>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    NTSTATUS
        NtQueryObject (
        _In_opt_ HANDLE Handle,
        _In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
        _Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
        _In_ ULONG ObjectInformationLength,
        _Out_opt_ PULONG ReturnLength
        );

    NTSTATUS
        NtQueryInformationFile(
        _In_ HANDLE FileHandle,
        _Out_ PIO_STATUS_BLOCK IoStatusBlock,
        _Out_writes_bytes_(Length) PVOID FileInformation,
        _In_ ULONG Length,
        _In_ FILE_INFORMATION_CLASS FileInformationClass
        );

    NTSTATUS
        NtQueryInformationProcess(
        __in HANDLE ProcessHandle,
        __in PROCESSINFOCLASS ProcessInformationClass,
        __out PVOID ProcessInformation,
        __in ULONG ProcessInformationLength,
        __out_opt PULONG ReturnLength
        );

    VOID
        RtlGetNtVersionNumbers(
        DWORD * dwMajorVersion,
        DWORD * dwMinorVersion,
        DWORD * dwBuildNumber
        );

private:
    HMODULE m_hNtdll_dll;
    Ntdll_dll_NtQueryObject_Proc_Type m_Ntdll_dll_NtQueryObject;
    Ntdll_dll_NtQueryInformationFile_Proc_Type m_Ntdll_dll_NtQueryInformationFile;
    Ntdll_dll_NtQueryInformationProcess_Proc_Type m_Ntdll_dll_NtQueryInformationProcess;
    Ntdll_dll_RtlGetNtVersionNumbers_Proc_Type m_Ntdll_dll_RtlGetNtVersionNumbers;

    CCommonNtHelper();

    ~CCommonNtHelper();
};
