
#pragma once

#include "CommonPublic.h"

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <Objbase.h>

typedef boost::shared_mutex CommonMutex;
typedef boost::shared_lock<CommonMutex> CommonReadLock;
typedef boost::unique_lock<CommonMutex> CommonWriteLock;

typedef
    VOID
    (WINAPI * Kernel32_dll_ExitProcess_Proc_Type)(
    __in UINT uExitCode
    );

typedef
    VOID
    (WINAPI * Kernelbase_dll_ExitProcess_Proc_Type)(
    __in UINT uExitCode
    );

typedef
    HRESULT
    ( WINAPI * Kernel32_dll_CopyFile2_Proc_Type)(
    PCWSTR pwszExistingFileName,
    PCWSTR pwszNewFileName,
    VOID *pExtendedParameters
    );

typedef
    BOOL
    ( WINAPI * Kernel32_dll_CopyFileW_Proc_Type)(
    __in LPCWSTR lpExistingFileName,
    __in LPCWSTR lpNewFileName,
    __in BOOL bFailIfExists
    );

typedef
    BOOL
    ( WINAPI * Kernel32_dll_CopyFileExW_Proc_Type)(
    __in LPCWSTR lpExistingFileName,
    __in LPCWSTR lpNewFileName,
    __in_opt LPPROGRESS_ROUTINE lpProgressRoutine,
    __in_opt LPVOID lpData,
    __in_opt LPBOOL pbCancel,
    __in DWORD dwCopyFlags
    );

typedef
    BOOL
    ( WINAPI * Kernel32_dll_CopyFileTransactedW_Proc_Type)(
    __in LPCWSTR lpExistingFileName,
    __in LPCWSTR lpNewFileName,
    __in_opt LPPROGRESS_ROUTINE lpProgressRoutine,
    __in_opt LPVOID lpData,
    __in_opt LPBOOL pbCancel,
    __in DWORD dwCopyFlags,
    __in HANDLE hTransaction
    );

typedef
    HRESULT
    ( WINAPI * KernelBase_dll_CopyFile2_Proc_Type)(
    PCWSTR pwszExistingFileName,
    PCWSTR pwszNewFileName,
    VOID *pExtendedParameters
    );

typedef
    BOOL
    ( WINAPI * KernelBase_dll_CopyFileW_Proc_Type)(
    __in LPCWSTR lpExistingFileName,
    __in LPCWSTR lpNewFileName,
    __in BOOL bFailIfExists
    );

typedef
    BOOL
    ( WINAPI * KernelBase_dll_CopyFileExW_Proc_Type)(
    __in LPCWSTR lpExistingFileName,
    __in LPCWSTR lpNewFileName,
    __in_opt LPPROGRESS_ROUTINE lpProgressRoutine,
    __in_opt LPVOID lpData,
    __in_opt LPBOOL pbCancel,
    __in DWORD dwCopyFlags
    );

typedef
    unsigned
    (__stdcall* _beginthreadex_proc_type)(
    void*
    );

typedef
    unsigned
    (__stdcall* _endthreadex_proc_type)(
    void*
    );

typedef
    HRESULT
    (STDAPICALLTYPE * Ole32_dll_StgCreateStorageEx_Proc_Type) (
    __in_opt __nullterminated const WCHAR* pwcsName,
    __in DWORD grfMode,
    __in DWORD stgfmt, // enum
    __in DWORD grfAttrs,
    __inout_opt STGOPTIONS* pStgOptions,
    __in_opt PSECURITY_DESCRIPTOR pSecurityDescriptor,
    __in REFIID riid,
    __deref_out void** ppObjectOpen
    );
