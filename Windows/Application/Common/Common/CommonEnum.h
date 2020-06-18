
#pragma once

typedef enum _COMMON_LOG_CREATE_FILE_STATUS
{
    COMMON_LOG_CREATE_FILE_STATUS_UNKNOWN,
    COMMON_LOG_CREATE_FILE_STATUS_UN_CREATED,
    COMMON_LOG_CREATE_FILE_STATUS_CREATED,
    COMMON_LOG_CREATE_FILE_STATUS_FAILED
}COMMON_LOG_CREATE_FILE_STATUS, *PCOMMON_LOG_CREATE_FILE_STATUS;

typedef enum _COMMON_LOG_LEVEL
{
    COMMON_LOG_LEVEL_UNKNOWN,
    COMMON_LOG_LEVEL_INFO,
    COMMON_LOG_LEVEL_WARNING,
    COMMON_LOG_LEVEL_ERROR
}COMMON_LOG_LEVEL, *PCOMMON_LOG_LEVEL;

typedef enum _COMMON_PATH_TYPE
{
    COMMON_PATH_TYPE_UNKNOWN,
    COMMON_PATH_TYPE_LOG_DIR
} COMMON_PATH_TYPE, *PCOMMON_PATH_TYPE;