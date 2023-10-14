/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_COMPUTE_H_
#define _VK_PIPELINE_COMPUTE_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKPipelineCompute : public Base
    {
    public:
        VKPipelineCompute(const String& namePipelineCompute);
        virtual ~VKPipelineCompute();

    public:
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline;
        VkDescriptorSet poDescriptorSet;
        
        Texture* pTextureSource;
        Texture* pTextureTarget;
        TextureCopyConstants* pTextureCopy;
        VkBuffer poBuffer_TextureCopy;  
        VkDeviceMemory poBufferMemory_TextureCopy;
        int frameRand;

    public:
        void Destroy();

        virtual void CleanupSwapChain();

    public:
        void CreateTextureCopy();
        void DestroyTextureCopy();
    };

}; //LostPeter

#endif