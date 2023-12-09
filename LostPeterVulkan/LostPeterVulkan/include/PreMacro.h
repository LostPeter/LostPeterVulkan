/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _PRE_MACRO_H_
#define _PRE_MACRO_H_

#include "FPreMacro.h"

#if F_PLATFORM == F_PLATFORM_WINDOW
	#if defined(VULKAN_EXPORTS)
		#define vulkanExport        	__declspec(dllexport) 
	#else
		#define vulkanExport      		__declspec(dllimport)
	#endif
#elif F_PLATFORM == F_PLATFORM_ANDROID
	#define vulkanExport				__attribute__ ((visibility("default")))
#else
	#define vulkanExport
#endif


#endif