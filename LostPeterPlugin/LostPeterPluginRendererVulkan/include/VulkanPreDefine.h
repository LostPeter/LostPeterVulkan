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

#include "volk.h"

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;

#include "PreDefine.h"
#include "PreInclude.h"
using namespace LostPeterEngine;


#if F_PLATFORM == F_PLATFORM_WINDOW
	#if defined (VULKAN_EXPORTS)
		#define vulkanExport            __declspec(dllexport)
	#else
		#define vulkanExport            __declspec(dllimport)
	#endif
#elif F_PLATFORM == F_PLATFORM_ANDROID
	#define vulkanExport				__attribute__ ((visibility("default")))
#else
	#define vulkanExport
#endif


namespace LostPeterPluginRendererVulkan
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
    void E_SetObjectName(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, const char* szName);

    template<class T>
    void E_ZeroStruct(T& vkStruct, VkStructureType vkType)
    {
        vkStruct.sType = vkType;
        memset(((uint8*)&vkStruct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
    }


    typedef std::vector<FPixelFormatType> PixelFormatTypeVector;
    typedef std::map<FPixelFormatType, VkFormat> PixelFormat2VkFormatMap;
    typedef std::map<VkFormat, FPixelFormatType> VkFormat2PixelFormatMap;
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


    /////////////////////////////////////// Vulkan Define ////////////////////////////////
	#define E_CPU_ALLOCATOR              nullptr

	enum VulkanSwapStatusType
    {
        Vulkan_SwapStatus_Normal = 0,               //0:    Normal
        Vulkan_SwapStatus_OutOfDate,                //1:    OutOfDate
        Vulkan_SwapStatus_Lost,                     //2:    Lost
        Vulkan_SwapStatus_Error,                    //3:    Error
    };


    enum VulkanSubmitType
    {
        Vulkan_Submit_FlushOnly = 0,                //0:    FlushOnly
        Vulkan_Submit_NewFrame,                     //1:    NewFrame
        Vulkan_Submit_EndFrameAndSwap,              //2:    EndFrameAndSwap
    };


    enum VulkanFenceStateType
    {
        Vulkan_FenceState_NotReady = 0,             //0:    NotReady
        Vulkan_FenceState_Signaled,                 //1:    Signaled
    };


    enum VulkanResourceAccessType
    {
        Vulkan_ResourceAccess_Undefined = 0x00,	    //0: Undefined
        Vulkan_ResourceAccess_Read = 0x01,			//1: Read
        Vulkan_ResourceAccess_Write = 0x02,		    //2: Write
        Vulkan_ResourceAccess_ReadWrite = Vulkan_ResourceAccess_Read | Vulkan_ResourceAccess_Write //3: ReadWrite
    };


    /////////////////////////////////////// Class Define /////////////////////////////////
	class VulkanBuffer;
	class VulkanBufferIndex;
	class VulkanBufferUniform;
	class VulkanBufferVertex;
    class VulkanConverter;
    class VulkanDebug;
	class VulkanDevice;
	class VulkanDeviceMemoryAllocation;
	class VulkanDeviceMemoryManager;
	class VulkanFence;
	class VulkanFenceManager;
    class VulkanFrameBuffer;
    class VulkanFrameBufferAttachment;
    class VulkanFrameBufferManager;
	class VulkanInstance;
	class VulkanPlugin;
	class VulkanQueue;
	class VulkanRenderer;
    class VulkanRenderFrameBufferDescriptor;
    class VulkanRenderPass;
    class VulkanRenderPassManager;
    class VulkanRenderPassDescriptor;
    class VulkanRenderPassDescriptorManager;
    class VulkanRenderTexture;
    class VulkanRenderWindow;
	class VulkanSemaphore;
    class VulkanShader;
    class VulkanShaderManager;
    class VulkanStreamIndex;
    class VulkanStreamManager;
    class VulkanStreamTexture;
    class VulkanStreamVertex;
	class VulkanSwapChain;
    class VulkanTexture;
    class VulkanTextureManager;
    class VulkanViewport;
    class VulkanVolk;

	typedef std::vector<VulkanDeviceMemoryAllocation*> VulkanDeviceMemoryAllocationPtrVector;

	typedef std::vector<VulkanFence*> VulkanFencePtrVector;
    typedef std::map<String, VulkanFence*> VulkanFencePtrMap;

    typedef std::vector<VulkanSemaphore*> VulkanSemaphorePtrVector;
    typedef std::map<String, VulkanSemaphore*> VulkanSemaphorePtrMap;

    typedef std::vector<VulkanQueue*> VulkanQueuePtrVector;
    typedef std::map<String, VulkanQueue*> VulkanQueuePtrMap;

    typedef std::vector<VulkanFrameBuffer*> VulkanFrameBufferPtrVector;
    typedef std::map<String, VulkanFrameBuffer*> VulkanFrameBufferPtrMap;
    typedef std::vector<VulkanFrameBufferAttachment*> VulkanFrameBufferAttachmentPtrVector;
    typedef std::map<String, VulkanFrameBufferAttachment*> VulkanFrameBufferAttachmentPtrMap;
    
    typedef std::vector<VulkanRenderPass*> VulkanRenderPassPtrVector;
    typedef std::map<String, VulkanRenderPass*> VulkanRenderPassPtrMap;
    typedef std::vector<VulkanRenderPassDescriptor*> VulkanRenderPassDescriptorPtrVector;
    typedef std::map<String, VulkanRenderPassDescriptor*> VulkanRenderPassDescriptorPtrMap;

    typedef std::vector<VulkanViewport*> VulkanViewportPtrVector;
    
    
}; //LostPetLostPeterPluginRendererVulkaner

#endif