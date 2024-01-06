/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_PRE_INCLUDE_STD_H_
#define _F_PRE_INCLUDE_STD_H_

#include "FPreMacro.h"

//C
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

//C++
#include <memory>
#include <new>
#include <limits>
#include <type_traits>
#include <chrono>

//C++ Stream
#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>  
#include <iomanip>
#include <sstream>

//STL containers
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <deque>
#include <queue>
#include <bitset>
#include <string>
#include <unordered_map>

#include <sys/types.h>
#include <sys/stat.h>

//Platform
#if F_PLATFORM == F_PLATFORM_WINDOW
	#undef min
	#undef max
	#if defined(__MINGW32__)
		#include <unistd.h>
	#endif
	#include <io.h>
	#include <process.h>
    #include <Windows.h>
    #include <shlwapi.h>

#elif F_PLATFORM == F_PLATFORM_MAC
    extern "C" 
	{
		#include <unistd.h>
		#include <sys/param.h>
		#include <CoreFoundation/CoreFoundation.h>
	}
	#include <dirent.h>
    #include <dlfcn.h>
    #include <filesystem>
    #include <sys/param.h>
	#define MAX_PATH	MAXPATHLEN
    
#elif F_PLATFORM == F_PLATFORM_LINUX
    #include <sys/param.h>
	#define MAX_PATH	MAXPATHLEN

#elif F_PLATFORM == F_PLATFORM_ANDROID

#elif F_PLATFORM == F_PLATFORM_iOS
    

#endif

namespace LostPeterFoundation
{
   //Log
#if F_PLATFORM == F_PLATFORM_ANDROID
    #include <jni.h>
    #include <android/log.h>
    #include <android/native_window.h>
    #include <android/native_activity.h>
    #include <android/configuration.h>
    #include <android/asset_manager.h>
    #include <android/sensor.h>
	#include <sys/system_properties.h>

    #ifndef LOG_TAG
        #define LOG_TAG "NDK-LIB"
    #endif

	#include <android_native_app_glue.h>
    #define F_LogError(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
    #define F_LogInfo(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define F_LogWarning(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#else
	foundationExport void F_LogError(const char* fmt, ...);
	foundationExport void F_LogWarning(const char* fmt, ...);
	foundationExport void F_LogInfo(const char* fmt, ...);

#endif
}; //LostPeterFoundation

#endif