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

#ifndef _VK_DEBUG_H_
#define _VK_DEBUG_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKDebug : public FSingleton<VKDebug>
                               , public Base
    {
    public:
        VKDebug();
        ~VKDebug();

    public:
        bool bIsActive;

        PFN_vkDebugMarkerSetObjectTagEXT pfnDebugMarkerSetObjectTag;
		PFN_vkDebugMarkerSetObjectNameEXT pfnDebugMarkerSetObjectName;
		PFN_vkCmdDebugMarkerBeginEXT pfnCmdDebugMarkerBegin;
		PFN_vkCmdDebugMarkerInsertEXT pfnCmdDebugMarkerInsert;
        PFN_vkCmdDebugMarkerEndEXT pfnCmdDebugMarkerEnd;

        PFN_vkQueueBeginDebugUtilsLabelEXT pfnQueueBeginDebugUtilsLabelEXT;
        PFN_vkQueueInsertDebugUtilsLabelEXT pfnQueueInsertDebugUtilsLabelEXT;
        PFN_vkQueueEndDebugUtilsLabelEXT pfnQueueEndDebugUtilsLabelEXT;

        PFN_vkCmdBeginDebugUtilsLabelEXT pfnCmdBeginDebugUtilsLabelEXT;
        PFN_vkCmdInsertDebugUtilsLabelEXT pfnCmdInsertDebugUtilsLabelEXT;
        PFN_vkCmdEndDebugUtilsLabelEXT pfnCmdEndDebugUtilsLabelEXT;

    public:
        static VKDebug& GetSingleton();
		static VKDebug* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(VkDevice device);

    public:
		void SetVkObjectName(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, const char *name);
		void SetVkObjectTag(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, uint64_t name, size_t tagSize, const void* tag);

		void BeginRegion(VkCommandBuffer cmdbuffer, const char* pMarkerName, FVector4 color);
		void Insert(VkCommandBuffer cmdbuffer, std::string markerName, FVector4 color);
		void EndRegion(VkCommandBuffer cmdBuffer);

        void BeginQueueLabel(VkQueue queue, const VkDebugUtilsLabelEXT* pDebugLabel);
        void InsertQueueLabel(VkQueue queue, const VkDebugUtilsLabelEXT* pDebugLabel);
        void EndQueueLabel(VkQueue queue);

        void BeginCmdLabel(VkCommandBuffer cmdbuffer, const VkDebugUtilsLabelEXT* pDebugLabel);
        void InsertCmdLabel(VkCommandBuffer cmdbuffer, const VkDebugUtilsLabelEXT* pDebugLabel);
        void EndCmdLabel(VkCommandBuffer cmdbuffer);

		void SetVkCommandBufferName(VkDevice device, VkCommandBuffer cmdBuffer, const char * name);
		void SetVkQueueName(VkDevice device, VkQueue queue, const char * name);
		void SetVkImageName(VkDevice device, VkImage image, const char * name);
		void SetVkSamplerName(VkDevice device, VkSampler sampler, const char * name);
		void SetVkBufferName(VkDevice device, VkBuffer buffer, const char * name);
		void SetVkDeviceMemoryName(VkDevice device, VkDeviceMemory memory, const char * name);
		void SetVkShaderModuleName(VkDevice device, VkShaderModule shaderModule, const char * name);
		void SetVkPipelineName(VkDevice device, VkPipeline pipeline, const char * name);
		void SetVkPipelineLayoutName(VkDevice device, VkPipelineLayout pipelineLayout, const char * name);
		void SetVkRenderPassName(VkDevice device, VkRenderPass renderPass, const char * name);
		void SetVkFramebufferName(VkDevice device, VkFramebuffer framebuffer, const char * name);
		void SetVkDescriptorSetLayoutName(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const char * name);
		void SetVkDescriptorSetName(VkDevice device, VkDescriptorSet descriptorSet, const char * name);
		void SetVkSemaphoreName(VkDevice device, VkSemaphore semaphore, const char * name);
		void SetVkFenceName(VkDevice device, VkFence fence, const char * name);
		void SetVkEventName(VkDevice device, VkEvent _event, const char * name);

    };

}; //LostPeterVulkan

#endif