/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_PRE_DEFINE_H_
#define _VULKAN_PRE_DEFINE_H_

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;

#include "PreDefine.h"
#include "PreInclude.h"
using namespace LostPeterEngine;


#if LP_PLATFORM == LP_PLATFORM_WIN32
	#if defined (LP_VULKAN_EXPORTS)
		#define lpVulkanExport          __declspec(dllexport)
	#else
		#define lpVulkanExport          __declspec(dllimport)
	#endif
#elif LP_PLATFORM == LP_PLATFORM_ANDROID
	#define lpVulkanExport				__attribute__ ((visibility("default")))
#else
	#define lpVulkanExport
#endif


namespace LostPeter
{


    /////////////////////////////////////// Vulkan Define ////////////////////////////////
	enum VulkanSwapStatusType
    {
        Vulkan_SwapStatus_Normal = 0,
        Vulkan_SwapStatus_OutOfDate = -1,
        Vulkan_SwapStatus_Lost = -2,
        Vulkan_SwapStatus_Error = -3,
    };


    enum VulkanFenceStateType
    {
        Vulkan_FenceState_NotReady = 0,
        Vulkan_FenceState_Signaled,
    };


    /////////////////////////////////////// Class Define /////////////////////////////////
	class VulkanBuffer;
	class VulkanBufferIndex;
	class VulkanBufferUniform;
	class VulkanBufferVertex;
	class VulkanDevice;
	class VulkanDeviceMemoryAllocation;
	class VulkanDeviceMemoryManager;
	class VulkanFence;
	class VulkanFenceManager;
	class VulkanInstance;
	class VulkanPlugin;
	class VulkanQueue;
	class VulkanRenderer;
	class VulkanSemaphore;
	class VulkanSwapChain;

	typedef std::vector<VulkanDeviceMemoryAllocation*> VulkanDeviceMemoryAllocationPtrVector;

	typedef std::vector<VulkanFence*> VulkanFencePtrVector;
    typedef std::vector<VulkanSemaphore*> VulkanSemaphorePtrVector;
    typedef std::vector<VulkanQueue*> VulkanQueuePtrVector;
    typedef std::map<String, VulkanQueue*> VulkanQueuePtrMap;

}; //LostPeter

#endif