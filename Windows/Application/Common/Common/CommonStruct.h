
#pragma once

#include "CommonPublic.h"

typedef struct _COMMON_THUMB_FILE_MAPPING_INFO
{
    WCHAR wchPath[COMMON_MAX_PATH];

    _COMMON_THUMB_FILE_MAPPING_INFO()
    {
        ZeroMemory(wchPath, sizeof(wchPath));
    }
} COMMON_THUMB_FILE_MAPPING_INFO, *PCOMMON_THUMB_FILE_MAPPING_INFO;
