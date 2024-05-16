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

//VMA
#include "vk_mem_alloc.h"


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
    #define RHI_CPU_ALLOCATOR              nullptr

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
	typedef std::vector<RHIPixelFormatType> PixelFormatTypeVector;
    typedef std::map<RHIPixelFormatType, VkFormat> PixelFormat2VkFormatMap;
    typedef std::map<VkFormat, RHIPixelFormatType> VkFormat2PixelFormatMap;

	typedef std::vector<VkFormat> VkFormatVector;
    typedef std::map<VkFormat, VkFormatProperties> VkFormat2PropertiesMap;
    typedef std::vector<VkClearValue> VkClearValueVector;

	typedef std::vector<VkQueueFamilyProperties> VkQueueFamilyPropertiesVector;

    typedef std::vector<VkBuffer> VkBufferVector;
    typedef std::vector<VkDeviceMemory> VkDeviceMemoryVector;

	typedef std::vector<VkAttachmentDescription> VkAttachmentDescriptionVector;
    typedef std::vector<VkSubpassDescription> VkSubpassDescriptionVector;
    typedef std::vector<VkSubpassDependency> VkSubpassDependencyVector;

    typedef std::vector<VkImage> VkImageVector;
    typedef std::vector<VkImageView> VkImageViewVector;
    typedef std::vector<VkFramebuffer> VkFramebufferVector;

    typedef std::vector<VkViewport> VkViewportVector;
    typedef std::vector<VkRect2D> VkRect2DVector;

    typedef std::vector<VkCommandBuffer> VkCommandBufferVector;

    typedef std::vector<VkSwapchainKHR> VkSwapchainKHRVector;

    typedef std::vector<VkSemaphore> VkSemaphoreVector;
    typedef std::vector<VkFence> VkFenceVector;
        
    typedef std::vector<VkPipelineShaderStageCreateInfo> VkPipelineShaderStageCreateInfoVector;
    typedef std::map<String, VkPipelineShaderStageCreateInfo> VkPipelineShaderStageCreateInfoMap;

    typedef std::vector<VkDescriptorSetLayoutBinding> VkDescriptorSetLayoutBindingVector;

    typedef std::vector<VkDescriptorSetLayout> VkDescriptorSetLayoutVector;
    typedef std::map<String, VkDescriptorSetLayout> VkDescriptorSetLayoutMap;

    typedef std::vector<VkShaderModule> VkShaderModuleVector;
    typedef std::map<String, VkShaderModule> VkShaderModuleMap;

    typedef std::vector<VkPipelineLayout> VkPipelineLayoutVector;
    typedef std::map<String, VkPipelineLayout> VkPipelineLayoutMap;

    typedef std::vector<VkPipeline> VkPipelineVector;
    typedef std::map<String, VkPipeline> VkPipelineMap;
    
    typedef std::vector<VkDescriptorSet> VkDescriptorSetVector;
    typedef std::vector<VkWriteDescriptorSet> VkWriteDescriptorSetVector;

    typedef std::vector<VkVertexInputBindingDescription> VkVertexInputBindingDescriptionVector;
    typedef std::vector<VkVertexInputAttributeDescription> VkVertexInputAttributeDescriptionVector;


	////////////////////////////// Class ///////////////////////////////
	class RHIVulkanBindGroup;
	class RHIVulkanBindGroupLayout;
	class RHIVulkanBuffer;
	class RHIVulkanBufferView;
	class RHIVulkanCommandBuffer;
	class RHIVulkanCommandDecoder;
	class RHIVulkanCommandEncoder;
	class RHIVulkanConverter;
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