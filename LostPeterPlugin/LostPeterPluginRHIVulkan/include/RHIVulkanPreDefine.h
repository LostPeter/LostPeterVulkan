/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_PRE_DEFINE_H_
#define _RHI_VULKAN_PRE_DEFINE_H_

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;

#include "RHIPreDefine.h"
#include "RHIPreInclude.h"
using namespace LostPeterRHI;


#if F_PLATFORM == F_PLATFORM_WINDOW
	#if defined (RHI_VULKAN_EXPORTS)
		#define rhiVulkanExport            	__declspec(dllexport)
	#else
		#define rhiVulkanExport            	__declspec(dllimport)
	#endif
#elif F_PLATFORM == F_PLATFORM_ANDROID
	#define rhiVulkanExport			    	__attribute__ ((visibility("default")))
#else
	#define rhiVulkanExport
#endif


namespace LostPeterPluginRHIVulkan
{
    
    

}; //LostPeterPluginRHIVulkan

#endif