/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_GRAPHICS_COPY_BLIT_FROM_FRAME_H_
#define _VK_PIPELINE_GRAPHICS_COPY_BLIT_FROM_FRAME_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineGraphicsCopyBlitFromFrame : public Base
    {
    public:
        VKPipelineGraphicsCopyBlitFromFrame(const String& namePipelineGraphics);
        virtual ~VKPipelineGraphicsCopyBlitFromFrame();

    public:
        VKRenderPassCopyBlitFromFrame* pVKRenderPassCopyBlitFromFrame;

        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline;
        VkDescriptorSetVector poDescriptorSets;

        CopyBlitObjectConstants m_objectCB_CopyBlit;
        VkBuffer m_vkBuffer_CopyBlit;
        VkDeviceMemory m_vkBuffersMemory_CopyBlit;

        Mesh* pMeshBlit;

    public:
        void Destroy();

        virtual bool Init(uint32_t width, 
                          uint32_t height,
                          VkFormat format,
                          bool isDepth,
                          Mesh* pMesh,
                          const String& descriptorSetLayout,
                          StringVector* pDescriptorSetLayoutNames,
                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                          const VkPipelineLayout& vkPipelineLayout,
                          const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    protected:
        void destroyBufferCopyBlitObject();
        
        bool createBufferCopyBlitObject();

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSets(const VkImageView& imageView);
        virtual void UpdateDescriptorSets(const VkDescriptorImageInfo& imageInfo);
        virtual void UpdateBuffer(const CopyBlitObjectConstants& object);
    };

}; //LostPeterVulkan

#endif