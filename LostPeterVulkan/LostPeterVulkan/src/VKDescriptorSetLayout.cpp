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

#include "../include/VKDescriptorSetLayout.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    VKDescriptorSetLayout::VKDescriptorSetLayout(const String& _nameDescriptorSetLayout)
        : Base(_nameDescriptorSetLayout)
        , vkDescriptorSetLayout(VK_NULL_HANDLE)
    {

    }
    VKDescriptorSetLayout::~VKDescriptorSetLayout()
    {
        Destroy();
    }

    void VKDescriptorSetLayout::Destroy()
    {
        if (this->vkDescriptorSetLayout != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkDescriptorSetLayout(this->vkDescriptorSetLayout);
        }
        this->vkDescriptorSetLayout = VK_NULL_HANDLE;
    }

    bool VKDescriptorSetLayout::LoadDescriptorSetLayout()
    {
        if (this->name.empty())
        {
            F_LogError("*********************** VKDescriptorSetLayout::LoadDescriptorSetLayout: DescriptorSetLayout is empty !");
            return false;
        }
        this->aNamesDescriptorSet = FUtilString::Split(this->name, "-");
        if (this->aNamesDescriptorSet.empty())
        {
            F_LogError("*********************** VKDescriptorSetLayout::LoadDescriptorSetLayout: DescriptorSetLayout's descriptor set is empty !");
            return false;
        }

        Destroy();
        this->vkDescriptorSetLayout = Base::GetWindowPtr()->CreateDescriptorSetLayout(this->name, &this->aNamesDescriptorSet);
        if (this->vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** VKDescriptorSetLayout::LoadDescriptorSetLayout: Failed to create descriptor set layout, name: [%s] !", this->name.c_str());
            return false;
        }

        return true;
    }

}; //LostPeter