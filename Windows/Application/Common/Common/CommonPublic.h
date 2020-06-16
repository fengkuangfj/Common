
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <winsock2.h>
#include <windows.h>

#if 0
COMMON_ERROR CommonError = COMMON_ERROR_DEFAULT;


do
{
    CommonError = COMMON_ERROR_SUCCESS;
} while (FALSE);

return CommonError;
#endif
