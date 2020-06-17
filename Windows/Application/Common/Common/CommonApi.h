
#pragma once

#include "CommonPublic.h"

#include "CommonAvoidReentry.h"
#include "CommonEnum.h"
#include "CommonError.h"
#include "CommonGlobalVariable.h"
#include "CommonHookEnv.h"
#include "CommonIp.h"
#include "CommonLog.h"
#include "CommonMacro.h"
#include "CommonModule.h"
#include "CommonMultipleInstance.h"
#include "CommonNickname.h"
#include "CommonNtDefine.h"
#include "CommonOperationSystem.h"
#include "CommonProcess.h"
#include "CommonService.h"
#include "CommonSession.h"
#include "CommonSingleInstance.h"
#include "CommonStringConvert.h"
#include "CommonStruct.h"
#include "CommonTool.h"
#include "CommonNtHelper.h"

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
