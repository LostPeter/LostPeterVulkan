/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineLayout.h"
#include "../include/VulkanWindow.h"
#include "../include/VKDescriptorSetLayoutManager.h"
#include "../include/VKDescriptorSetLayout.h"

namespace LostPeter
{
    VKPipelineLayout::VKPipelineLayout(const String& _namePipelineLayout)
        : Base(_namePipelineLayout)
        , vkPipelineLayout(VK_NULL_HANDLE)
    {

    }
    VKPipelineLayout::~VKPipelineLayout()
    {
        Destroy();
    }
    
    void VKPipelineLayout::Destroy()
    {
        if (this->vkPipelineLayout != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipelineLayout(this->vkPipelineLayout);
        }
        this->vkPipelineLayout = VK_NULL_HANDLE;
    }

    bool VKPipelineLayout::LoadPipelineLayout()
    {   
        if (this->name.empty())
        {
            F_LogError("*********************** VKPipelineLayout::LoadPipelineLayout: PipelineLayout name is empty !");
            return false;
        }

        Destroy();

        VKDescriptorSetLayout* pVKDescriptorSetLayout = VKDescriptorSetLayoutManager::GetSingleton().GetVKDescriptorSetLayout(this->name);
        if (pVKDescriptorSetLayout == nullptr)
        {
            F_LogError("*********************** VKPipelineLayout::LoadPipelineLayout: Can not find DescriptorSetLayout by name: [%s]", this->name.c_str());
            return false;
        }
        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(pVKDescriptorSetLayout->GetVKDescriptorSetLayout());
        this->vkPipelineLayout = Base::GetWindowPtr()->createVkPipelineLayout(aDescriptorSetLayout);
        if (this->vkPipelineLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** VKPipelineLayout::LoadPipelineLayout: Failed to create pipeline layout, name: [%s] !", this->name.c_str());
            return false;
        }

        return true;
    }

}; //LostPeter