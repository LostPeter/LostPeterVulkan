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

#ifndef _VK_PIPELINE_GRAPHICS_DEPTH_HIZ_H_
#define _VK_PIPELINE_GRAPHICS_DEPTH_HIZ_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineGraphicsDepthHiz : public Base
    {
    public:
        VKPipelineGraphicsDepthHiz(const String& namePipelineGraphics, VKRenderPassCull* pVKRenderPassCull);
        virtual ~VKPipelineGraphicsDepthHiz();

    public:
        VKRenderPassCull* m_pVKRenderPassCull;

        //PipelineGraphics-HizDepth
		DescriptorSetLayout* pDescriptorSetLayout_HizDepth;
		VKStatePipelineGraphics* poStatePipelineGraphics_HizDepth;
        
        VkDescriptorImageInfo imageInfo;

        Mesh* pMesh;

    public:
        void Destroy();

        virtual bool Init(Mesh* pMesh);

        virtual bool InitHizDepth(DescriptorSetLayout* pDSL,
                                  VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    protected:
        VKStatePipelineGraphics* createGraphicsPipeline(const String& nameGraphicsPipeline,
														DescriptorSetLayout* pDSL,
														VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSet_HizDepth();

    protected:
        virtual void updateDescriptorSets(VKStatePipelineGraphics* pStatePipelineGraphics,
										  VkDescriptorSetVector& vkDescriptorSets,
                                          VKBufferUniform* pBuffer_HizDepthCB,
                                          const VkImageView& vkImageView,
                                          const VkSampler& vkSampler);
    };  

}; //LostPeterVulkan

#endif