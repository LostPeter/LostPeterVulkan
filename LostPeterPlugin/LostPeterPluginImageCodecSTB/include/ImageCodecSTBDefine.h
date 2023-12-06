/****************************************************************************
* LostPeterPluginImageCodecSTB - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _IMAGE_CODEC_STB_DEFINE_H_
#define _IMAGE_CODEC_STB_DEFINE_H_

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;

#include "PreDefine.h"
#include "PreInclude.h"
using namespace LostPeterEngine;


#if E_PLATFORM == E_PLATFORM_WINDOW
	#if defined (IMAGE_CODEC_STB_EXPORTS)
		#define imageCodecSTBExport            	__declspec(dllexport)
	#else
		#define imageCodecSTBExport            	__declspec(dllimport)
	#endif
#elif E_PLATFORM == E_PLATFORM_ANDROID
	#define imageCodecSTBExport			    	__attribute__ ((visibility("default")))
#else
	#define imageCodecSTBExport
#endif


namespace LostPeter
{
    

}; //LostPeter

#endif