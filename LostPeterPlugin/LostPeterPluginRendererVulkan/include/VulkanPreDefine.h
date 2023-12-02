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


#if E_PLATFORM == E_PLATFORM_WIN32
	#if defined (VULKAN_EXPORTS)
		#define vulkanExport            __declspec(dllexport)
	#else
		#define vulkanExport            __declspec(dllimport)
	#endif
#elif E_PLATFORM == E_PLATFORM_ANDROID
	#define vulkanExport				__attribute__ ((visibility("default")))
#else
	#define vulkanExport
#endif


namespace LostPeter
{
	/////////////////////////////////////// Typedef //////////////////////////////////////
	String E_VkResult2String(VkResult result);

    #define E_VK_CHECK(vkcall) \
    { \
        VkResult result = vkcall; \
        if (result != VK_SUCCESS) \
        { \
            String vkfunc = #vkcall; \
            vkfunc = vkfunc.substr(0, vkfunc.find('(')); \
            F_LogError("*********************** E_VK_CHECK: [%s] failed with: %s", vkfunc.c_str(), E_VkResult2String(result).c_str()); \
        } \
    }

    bool E_CheckVkResult(VkResult result, const String& nameFunc);

    template<class T>
    void E_ZeroStruct(T& vkStruct, VkStructureType vkType)
    {
        vkStruct.sType = vkType;
        memset(((uint8*)&vkStruct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
    }


    /////////////////////////////////////// Vulkan Define ////////////////////////////////
	#define E_CPU_ALLOCATOR              nullptr

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