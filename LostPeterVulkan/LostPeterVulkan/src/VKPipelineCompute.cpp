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
#include "../include/VKBufferUniform.h"

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
        , poBuffer_TextureCopy(nullptr)
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
		String nameBuffer = "TextureCopyConstants-" + this->name;
		this->poBuffer_TextureCopy = Base::GetWindowPtr()->createBufferUniform(nameBuffer,
																			   sizeof(TextureCopyConstants),
																			   (uint8*)this->pTextureCopy,
																			   false);
		if (!this->poBuffer_TextureCopy)
		{
			String msg = "*********************** VKPipelineCompute::CreateTextureCopy: create buffer uniform: [" + nameBuffer + "] failed !";
			F_LogError(msg.c_str());
			throw std::runtime_error(msg);
		}
    }
    void VKPipelineCompute::DestroyTextureCopy()
    {
        F_DELETE(this->pTextureCopy)
		F_DELETE(this->poBuffer_TextureCopy)
    }

}; //LostPeterVulkan