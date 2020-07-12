
#pragma once

#include "CommonPublic.h"

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

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
