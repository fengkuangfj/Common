
#pragma once

#include "CommonPublic.h"

#include "CommonAvoidReentry.h"
#include "CommonError.h"
#include "CommonHookEnv.h"
#include "CommonMultipleInstance.h"
#include "CommonNickname.h"
#include "CommonNtDefine.h"
#include "CommonSingleInstance.h"
#include "CommonStruct.h"

#ifndef Common_lib
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptionDecryption/x64/Debug/Common.lib")
#else
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptionDecryption/Debug/Common.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptionDecryption/x64/Release/Common.lib")
#else
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptionDecryption/Release/Common.lib")
#endif
#endif
#endif
