/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-07-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_GRAPHICS_COPY_BLIT_H_
#define _VK_PIPELINE_GRAPHICS_COPY_BLIT_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineGraphicsCopyBlit : public Base
    {
    public:
        VKPipelineGraphicsCopyBlit(const String& namePipelineGraphics);
        virtual ~VKPipelineGraphicsCopyBlit();

    public:
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline_WireFrame;
        VkPipeline poPipeline;
        VkDescriptorSetVector poDescriptorSets;

        CopyBlitObjectConstants m_objectCB_CopyBlit;
        VkBuffer m_vkBuffer_CopyBlit;
        VkDeviceMemory m_vkBuffersMemory_CopyBlit;

        Mesh* pMeshBlit;

    public:
        void Destroy();
        virtual bool Init(Mesh* pMesh,
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

        virtual void UpdateDescriptorSets(const VkDescriptorImageInfo& imageInfo);
        virtual void UpdateBuffer(const CopyBlitObjectConstants& object);
    };  

}; //LostPeterVulkan

#endif