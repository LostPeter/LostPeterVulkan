/****************************************************************************
* LostPeterUI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _UI_PRE_MACRO_H_
#define _UI_PRE_MACRO_H_

#include "FPreMacro.h"

#if LP_PLATFORM == LP_PLATFORM_WIN32
	#if defined(LPUI_EXPORTS)
		#define LPUI_Export        		__declspec(dllexport) 
	#else
		#define LPUI_Export      		__declspec(dllimport) 
	#endif
#elif LP_PLATFORM == LP_PLATFORM_ANDROID
	#define LPUI_Export					__attribute__ ((visibility("default")))
#else
	#define LPUI_Export
#endif


#endif