/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#if F_PLATFORM == F_PLATFORM_MAC

#ifndef _F_UTIL_OC_H_
#define _F_UTIL_OC_H_

#include "FPreDefine.h"

std::string F_OCGetPathExecute();
std::string F_OCGetMacBundlePath();
std::string F_OCGetMacPluginPath();
std::string F_OCGetMacFrameworksPath();
std::string F_OCGetMacCachePath();
std::string F_OCGetMacTempFileName();
std::string F_OCGetMacResourcesPath();
std::string F_OCGetMacLogPath();

CFBundleRef F_OCMac_LoadExeBundle(const char* path);
void* F_OCMac_GetBundleSym(CFBundleRef bundle, const char* name);
bool F_OCMac_UnloadExeBundle(CFBundleRef bundle);
void* F_OCMac_LoadDynamicLib(const char* name);
void* F_OCMac_LoadFramework(std::string name);

#endif

#endif

