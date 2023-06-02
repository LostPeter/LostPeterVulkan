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


//Platform
#if LP_PLATFORM == LP_PLATFORM_WIN32
	#undef min
	#undef max
	#if defined(__MINGW32__)
		#include <unistd.h>
	#endif
	#include <io.h>
	#include <process.h>
    #include <Windows.h>
    #include <shlwapi.h>

#elif LP_PLATFORM == LP_PLATFORM_MAC

#elif LP_PLATFORM == LP_PLATFORM_LINUX

#elif LP_PLATFORM == LP_PLATFORM_ANDROID

#elif LP_PLATFORM == LP_PLATFORM_IOS
    

#endif

namespace LostPeterFoundation
{
    

}; //LostPeterFoundation

#endif