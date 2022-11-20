// author: LostPeter
// time:   2022-10-30

#ifndef _PRE_INCLUDE_STD_H_
#define _PRE_INCLUDE_STD_H_


#include "premacro.h"

//C
#include <float.h>
#include <assert.h>
#include <string.h>


//C++
#include <stdexcept>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <memory>

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
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


#endif