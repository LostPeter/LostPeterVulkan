// author: LostPeter
// time:   2022-10-30

#ifndef _PRE_INCLUDE_STD_H_
#define _PRE_INCLUDE_STD_H_


#include "PreMacro.h"

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


//Platform
#if UTIL_PLATFORM == UTIL_PLATFORM_WIN32
	#undef min
	#undef max
	#if defined(__MINGW32__)
		#include <unistd.h>
	#endif
	#include <io.h>
	#include <process.h>
    #include <Windows.h>
    #include <shlwapi.h>

#elif UTIL_PLATFORM == UTIL_PLATFORM_MAC

#elif UTIL_PLATFORM == UTIL_PLATFORM_LINUX

#elif UTIL_PLATFORM == UTIL_PLATFORM_ANDROID

#elif UTIL_PLATFORM == UTIL_PLATFORM_IOS
    

#endif


namespace LostPeter
{
//Log
#if UTIL_PLATFORM == UTIL_PLATFORM_ANDROID
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
    #define Util_LogError(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
    #define Util_LogInfo(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define Util_LogWarning(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#else
	utilExport void Util_LogError(const char* fmt, ...);
	utilExport void Util_LogWarning(const char* fmt, ...);
	utilExport void Util_LogInfo(const char* fmt, ...);

#endif
}; //LostPeter



#endif