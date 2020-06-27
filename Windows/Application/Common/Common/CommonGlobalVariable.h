
#pragma once

#ifndef MAX_PATH
#define MAX_PATH (260)
#endif

#ifndef COMMON_MAX_PATH
#define COMMON_MAX_PATH (MAX_PATH * 3 / 2)
#endif

#define COMMON_IP_MAX_LENGTH (15)

#ifndef EPSILON
#define EPSILON (1e-6)
#endif

#ifndef COMMON_LONG_PATH_PRE
#define COMMON_LONG_PATH_PRE L"\\\\?"
#endif

#ifndef COMMON_PATH_FILE_NAME_LOG_DIR
#define COMMON_PATH_FILE_NAME_LOG_DIR L"{B1000E46-8566-430F-AF71-4309938D15CA}"
#endif

#ifndef COMMON_PENDING_WAIT_MILLISECONDS
#define COMMON_PENDING_WAIT_MILLISECONDS (1000 * 10)
#endif
