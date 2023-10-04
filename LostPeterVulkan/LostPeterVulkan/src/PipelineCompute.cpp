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

#include "../include/PipelineCompute.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    PipelineCompute::PipelineCompute(const String& namePipelineCompute)
        : Base(namePipelineCompute)
        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        , pTextureSource(nullptr)
        , pTextureTarget(nullptr)
        
        , pTextureCopy(nullptr)
        , poBuffer_TextureCopy(VK_NULL_HANDLE)
        , poBufferMemory_TextureCopy(VK_NULL_HANDLE)
        , frameRand(0)
    {

    }
    PipelineCompute::~PipelineCompute()
    {
        Destroy();
    }
    void PipelineCompute::Destroy()
    {
        CleanupSwapChain();
    }
    void PipelineCompute::CleanupSwapChain()
    {
        this->poDescriptorSetLayoutNames = nullptr;
        this->poDescriptorSetLayout = VK_NULL_HANDLE;
        this->poPipelineLayout = VK_NULL_HANDLE;
        if (this->poPipeline != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline);
        }       
        this->poPipeline = VK_NULL_HANDLE;
        this->poDescriptorSet = VK_NULL_HANDLE;

        DestroyTextureCopy();
    }  
    void PipelineCompute::CreateTextureCopy()
    {
        DestroyTextureCopy();
        this->pTextureCopy = new TextureCopyConstants();
        VkDeviceSize bufferSize = sizeof(TextureCopyConstants);
        Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
    }
    void PipelineCompute::DestroyTextureCopy()
    {
        F_DELETE(this->pTextureCopy)
        if (this->poBuffer_TextureCopy != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
        }
        this->poBuffer_TextureCopy = VK_NULL_HANDLE;
        this->poBufferMemory_TextureCopy = VK_NULL_HANDLE;
    }

}; //LostPeter