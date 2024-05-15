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

#include "volk.h"

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;

#include "RHIPreDefine.h"
#include "RHIPreInclude.h"
using namespace LostPeterRHI;

//GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


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
	////////////////////////////// Define //////////////////////////////
	String RHI_VkResult2String(VkResult result);

    #define RHI_VK_CHECK(vkcall) \
    { \
        VkResult result = vkcall; \
        if (result != VK_SUCCESS) \
        { \
            String vkfunc = #vkcall; \
            vkfunc = vkfunc.substr(0, vkfunc.find('(')); \
            F_LogError("*********************** RHI_VK_CHECK: [%s] failed with: %s", vkfunc.c_str(), RHI_VkResult2String(result).c_str()); \
        } \
    }

    bool RHI_CheckVkResult(VkResult result, const String& nameFunc);

    template<class T>
    void RHI_ZeroStruct(T& vkStruct, VkStructureType vkType)
    {
        vkStruct.sType = vkType;
        memset(((uint8*)&vkStruct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
    }
	

	////////////////////////////// Typedef /////////////////////////////

	////////////////////////////// Class ///////////////////////////////
	class RHIVulkanBindGroup;
	class RHIVulkanBindGroupLayout;
	class RHIVulkanBuffer;
	class RHIVulkanBufferView;
	class RHIVulkanCommandBuffer;
	class RHIVulkanCommandDecoder;
	class RHIVulkanCommandEncoder;
	class RHIVulkanDebug;
	class RHIVulkanDevice;
	class RHIVulkanFence;
	class RHIVulkanFrameBuffer;
	class RHIVulkanGraphicsState;
	class RHIVulkanInstance;
	class RHIVulkanPhysicalDevice;
	class RHIVulkanPipeline;
	class RHIVulkanPipelineCompute;
	class RHIVulkanPipelineGraphics;
	class RHIVulkanPipelineLayout;
	class RHIVulkanQueue;
	class RHIVulkanSampler;
	class RHIVulkanShaderModule;
	class RHIVulkanSurface;
	class RHIVulkanSwapChain;
	class RHIVulkanBarrier;
	class RHIVulkanTexture;
	class RHIVulkanTextureView;
	class RHIVulkanUtil;
	class RHIVulkanVolk;

	typedef std::vector<RHIVulkanPhysicalDevice*> RHIVulkanPhysicalDevicePtrVector;	
	typedef std::vector<RHIVulkanTexture*> RHIVulkanTexturePtrVector;
    

}; //LostPeterPluginRHIVulkan

#endif