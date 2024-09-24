/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKDebug.h"
#include "../include/VulkanWindow.h"

template<> LostPeterVulkan::VKDebug* LostPeterFoundation::FSingleton<LostPeterVulkan::VKDebug>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
    VKDebug* VKDebug::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VKDebug& VKDebug::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VKDebug::GetSingleton")
		return (*ms_Singleton);     
	}


    VKDebug::VKDebug()
        : Base("VKDebug")
        , bIsActive(false)

        , pfnDebugMarkerSetObjectTag(VK_NULL_HANDLE)
        , pfnDebugMarkerSetObjectName(VK_NULL_HANDLE)
        , pfnCmdDebugMarkerBegin(VK_NULL_HANDLE)
        , pfnCmdDebugMarkerInsert(VK_NULL_HANDLE)
        , pfnCmdDebugMarkerEnd(VK_NULL_HANDLE)

        , pfnQueueBeginDebugUtilsLabelEXT(VK_NULL_HANDLE)
        , pfnQueueInsertDebugUtilsLabelEXT(VK_NULL_HANDLE)
        , pfnQueueEndDebugUtilsLabelEXT(VK_NULL_HANDLE)
       
        , pfnCmdBeginDebugUtilsLabelEXT(VK_NULL_HANDLE)
        , pfnCmdInsertDebugUtilsLabelEXT(VK_NULL_HANDLE)
        , pfnCmdEndDebugUtilsLabelEXT(VK_NULL_HANDLE)
        
    {

    }
    VKDebug::~VKDebug()
    {
        Destroy();
    }   

    void VKDebug::Destroy()
    {
        
    } 

    bool VKDebug::Init(VkDevice device)
    {
        this->pfnDebugMarkerSetObjectTag = reinterpret_cast<PFN_vkDebugMarkerSetObjectTagEXT>(vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectTagEXT"));
        this->pfnDebugMarkerSetObjectName = reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectNameEXT"));
        this->pfnCmdDebugMarkerBegin = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>(vkGetDeviceProcAddr(device, "vkCmdDebugMarkerBeginEXT"));
        this->pfnCmdDebugMarkerInsert = reinterpret_cast<PFN_vkCmdDebugMarkerInsertEXT>(vkGetDeviceProcAddr(device, "vkCmdDebugMarkerInsertEXT"));
        this->pfnCmdDebugMarkerEnd = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>(vkGetDeviceProcAddr(device, "vkCmdDebugMarkerEndEXT"));

        this->pfnQueueBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueBeginDebugUtilsLabelEXT>(vkGetDeviceProcAddr(device, "vkQueueBeginDebugUtilsLabelEXT"));
        this->pfnQueueInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueInsertDebugUtilsLabelEXT>(vkGetDeviceProcAddr(device, "vkQueueInsertDebugUtilsLabelEXT"));
        this->pfnQueueEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueEndDebugUtilsLabelEXT>(vkGetDeviceProcAddr(device, "vkQueueEndDebugUtilsLabelEXT"));

        this->pfnCmdBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(vkGetDeviceProcAddr(device, "vkCmdBeginDebugUtilsLabelEXT"));
        this->pfnCmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(vkGetDeviceProcAddr(device, "vkCmdInsertDebugUtilsLabelEXT"));
        this->pfnCmdEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(vkGetDeviceProcAddr(device, "vkCmdEndDebugUtilsLabelEXT"));

        this->bIsActive = (this->pfnDebugMarkerSetObjectName != VK_NULL_HANDLE);
        
        return this->bIsActive;
    }

    void VKDebug::SetVkObjectName(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, const char *name)
    {
        if (this->pfnDebugMarkerSetObjectName)
        {
            VkDebugMarkerObjectNameInfoEXT nameInfo = {};
            nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            nameInfo.objectType = objectType;
            nameInfo.object = object;
            nameInfo.pObjectName = name;
            this->pfnDebugMarkerSetObjectName(device, &nameInfo);
        }
    }

    void VKDebug::SetVkObjectTag(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, uint64_t name, size_t tagSize, const void* tag)
    {
        if (this->pfnDebugMarkerSetObjectTag)
        {
            VkDebugMarkerObjectTagInfoEXT tagInfo = {};
            tagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            tagInfo.objectType = objectType;
            tagInfo.object = object;
            tagInfo.tagName = name;
            tagInfo.tagSize = tagSize;
            tagInfo.pTag = tag;
            this->pfnDebugMarkerSetObjectTag(device, &tagInfo);
        }
    }

    void VKDebug::BeginRegion(VkCommandBuffer cmdbuffer, const char* pMarkerName, FVector4 color)
    {
        if (this->pfnCmdDebugMarkerBegin)
        {
            VkDebugMarkerMarkerInfoEXT markerInfo = {};
            markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
            memcpy(markerInfo.color, &color[0], sizeof(float) * 4);
            markerInfo.pMarkerName = pMarkerName;
            this->pfnCmdDebugMarkerBegin(cmdbuffer, &markerInfo);
        }
    }

    void VKDebug::Insert(VkCommandBuffer cmdbuffer, std::string markerName, FVector4 color)
    {
        if (this->pfnCmdDebugMarkerInsert)
        {
            VkDebugMarkerMarkerInfoEXT markerInfo = {};
            markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
            memcpy(markerInfo.color, &color[0], sizeof(float) * 4);
            markerInfo.pMarkerName = markerName.c_str();
            this->pfnCmdDebugMarkerInsert(cmdbuffer, &markerInfo);
        }
    }

    void VKDebug::EndRegion(VkCommandBuffer cmdBuffer)
    {
        if (this->pfnCmdDebugMarkerEnd)
        {
            this->pfnCmdDebugMarkerEnd(cmdBuffer);
        }
    }

    void VKDebug::BeginQueueLabel(VkQueue queue, const VkDebugUtilsLabelEXT* pDebugLabel)
    {
        if (this->pfnQueueBeginDebugUtilsLabelEXT)
        {
            this->pfnQueueBeginDebugUtilsLabelEXT(queue, pDebugLabel);
        }
    }
    void VKDebug::InsertQueueLabel(VkQueue queue, const VkDebugUtilsLabelEXT* pDebugLabel)
    {
        if (this->pfnQueueInsertDebugUtilsLabelEXT)
        {
            this->pfnQueueInsertDebugUtilsLabelEXT(queue, pDebugLabel);
        }
    }
    void VKDebug::EndQueueLabel(VkQueue queue)
    {
        if (this->pfnQueueEndDebugUtilsLabelEXT)
        {
            this->pfnQueueEndDebugUtilsLabelEXT(queue);
        }
    }

    void VKDebug::BeginCmdLabel(VkCommandBuffer cmdbuffer, const VkDebugUtilsLabelEXT* pDebugLabel)
    {
        if (this->pfnCmdBeginDebugUtilsLabelEXT)
        {
            this->pfnCmdBeginDebugUtilsLabelEXT(cmdbuffer, pDebugLabel);
        }
    }
    void VKDebug::InsertCmdLabel(VkCommandBuffer cmdbuffer, const VkDebugUtilsLabelEXT* pDebugLabel)
    {
        if (this->pfnCmdInsertDebugUtilsLabelEXT)
        {
            this->pfnCmdInsertDebugUtilsLabelEXT(cmdbuffer, pDebugLabel);
        }
    }
    void VKDebug::EndCmdLabel(VkCommandBuffer cmdbuffer)
    {
        if (this->pfnCmdEndDebugUtilsLabelEXT)
        {
            this->pfnCmdEndDebugUtilsLabelEXT(cmdbuffer);
        }
    }

    void VKDebug::SetVkCommandBufferName(VkDevice device, VkCommandBuffer cmdBuffer, const char * name)
    {
        SetVkObjectName(device, (uint64_t)cmdBuffer, VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT, name);
    }

    void VKDebug::SetVkQueueName(VkDevice device, VkQueue queue, const char * name)
    {
        SetVkObjectName(device, (uint64_t)queue, VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT, name);
    }

    void VKDebug::SetVkImageName(VkDevice device, VkImage image, const char * name)
    {
        SetVkObjectName(device, (uint64_t)image, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, name);
    }

    void VKDebug::SetVkSamplerName(VkDevice device, VkSampler sampler, const char * name)
    {
        SetVkObjectName(device, (uint64_t)sampler, VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, name);
    }

    void VKDebug::SetVkBufferName(VkDevice device, VkBuffer buffer, const char * name)
    {
        SetVkObjectName(device, (uint64_t)buffer, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, name);
    }

    void VKDebug::SetVkDeviceMemoryName(VkDevice device, VkDeviceMemory memory, const char * name)
    {
        SetVkObjectName(device, (uint64_t)memory, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, name);
    }

    void VKDebug::SetVkShaderModuleName(VkDevice device, VkShaderModule shaderModule, const char * name)
    {
        SetVkObjectName(device, (uint64_t)shaderModule, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, name);
    }

    void VKDebug::SetVkPipelineName(VkDevice device, VkPipeline pipeline, const char * name)
    {
        SetVkObjectName(device, (uint64_t)pipeline, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, name);
    }

    void VKDebug::SetVkPipelineLayoutName(VkDevice device, VkPipelineLayout pipelineLayout, const char * name)
    {
        SetVkObjectName(device, (uint64_t)pipelineLayout, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT, name);
    }

    void VKDebug::SetVkRenderPassName(VkDevice device, VkRenderPass renderPass, const char * name)
    {
        SetVkObjectName(device, (uint64_t)renderPass, VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, name);
    }

    void VKDebug::SetVkFramebufferName(VkDevice device, VkFramebuffer framebuffer, const char * name)
    {
        SetVkObjectName(device, (uint64_t)framebuffer, VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT, name);
    }

    void VKDebug::SetVkDescriptorSetLayoutName(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const char * name)
    {
        SetVkObjectName(device, (uint64_t)descriptorSetLayout, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, name);
    }

    void VKDebug::SetVkDescriptorSetName(VkDevice device, VkDescriptorSet descriptorSet, const char * name)
    {
        SetVkObjectName(device, (uint64_t)descriptorSet, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, name);
    }

    void VKDebug::SetVkSemaphoreName(VkDevice device, VkSemaphore semaphore, const char * name)
    {
        SetVkObjectName(device, (uint64_t)semaphore, VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT, name);
    }

    void VKDebug::SetVkFenceName(VkDevice device, VkFence fence, const char * name)
    {
        SetVkObjectName(device, (uint64_t)fence, VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT, name);
    }

    void VKDebug::SetVkEventName(VkDevice device, VkEvent _event, const char * name)
    {
        SetVkObjectName(device, (uint64_t)_event, VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT, name);
    }

}; //LostPeterVulkan