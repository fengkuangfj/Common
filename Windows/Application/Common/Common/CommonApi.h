
#pragma once

#include "CommonPublic.h"

#include "CommonAvoidReentry.h"
#include "CommonDebug.h"
#include "CommonEnum.h"
#include "CommonError.h"
#include "CommonEvent.h"
#include "CommonFile.h"
#include "CommonGlobalVariable.h"
#include "CommonHookEnv.h"
#include "CommonIp.h"
#include "CommonLog.h"
#include "CommonMacro.h"
#include "CommonModule.h"
#include "CommonMultipleInstance.h"
#include "CommonNickname.h"
#include "CommonNtDefine.h"
#include "CommonNtHelper.h"
#include "CommonOperationSystem.h"
#include "CommonPath.h"
#include "CommonProcess.h"
#include "CommonService.h"
#include "CommonSession.h"
#include "CommonSingleInstance.h"
#include "CommonStringConvert.h"
#include "CommonStruct.h"
#include "CommonThread.h"
#include "CommonTime.h"
#include "CommonTool.h"

#ifndef Common_lib
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptDecrypt/x64/Debug/Common.lib")
#else
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptDecrypt/Debug/Common.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptDecrypt/x64/Release/Common.lib")
#else
#pragma comment(lib, "../../../../../Custom/Windows/Application/TransparentEncryptDecrypt/Release/Common.lib")
#endif
#endif
#endif
