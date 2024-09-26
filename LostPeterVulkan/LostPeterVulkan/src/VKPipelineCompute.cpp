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

#include "../include/VKPipelineCompute.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKPipelineCompute::VKPipelineCompute(const String& namePipelineCompute)
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
    VKPipelineCompute::~VKPipelineCompute()
    {
        Destroy();
    }
    void VKPipelineCompute::Destroy()
    {
        CleanupSwapChain();
    }
    void VKPipelineCompute::CleanupSwapChain()
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
    void VKPipelineCompute::CreateTextureCopy()
    {
        DestroyTextureCopy();
        this->pTextureCopy = new TextureCopyConstants();
        VkDeviceSize bufferSize = sizeof(TextureCopyConstants);
        Base::GetWindowPtr()->createVkBuffer("TextureCopyConstants-" + this->name, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
    }
    void VKPipelineCompute::DestroyTextureCopy()
    {
        F_DELETE(this->pTextureCopy)
        if (this->poBuffer_TextureCopy != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
        }
        this->poBuffer_TextureCopy = VK_NULL_HANDLE;
        this->poBufferMemory_TextureCopy = VK_NULL_HANDLE;
    }

}; //LostPeterVulkan