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

#if F_PLATFORM == F_PLATFORM_WINDOW
	#if defined(UI_EXPORTS)
		#define uiExport        		__declspec(dllexport) 
	#else
		#define uiExport      			__declspec(dllimport) 
	#endif
#elif F_PLATFORM == F_PLATFORM_ANDROID
	#define uiExport					__attribute__ ((visibility("default")))
#else
	#define uiExport
#endif


#endif