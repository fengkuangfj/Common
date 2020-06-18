
#pragma once

#include "CommonPublic.h"

typedef struct _COMMON_HOOK_INFO
{
    LPSTR lpProcName;
    PVOID pNewAddress;
    PVOID * ppOldAddress;

    _COMMON_HOOK_INFO()
    {
        lpProcName = NULL;
        pNewAddress = NULL;
        ppOldAddress = NULL;
    }
} COMMON_HOOK_INFO, *PCOMMON_HOOK_INFO;
