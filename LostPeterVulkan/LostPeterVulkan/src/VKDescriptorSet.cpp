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

#include "../include/VKDescriptorSet.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    VKDescriptorSet::VKDescriptorSet(const String& _nameDescriptorSet,
                                     VulkanDescriptorSetType _typeDescriptorSet,
                                     VulkanDescriptorSetUsageType _typeDescriptorSetUsage,
                                     const Uint32Vector& _aShaderTypes)
        : Base(_nameDescriptorSet)
        , typeDescriptorSet(_typeDescriptorSet)
        , typeDescriptorSetUsage(_typeDescriptorSetUsage)
        , aShaderTypes(_aShaderTypes)
    {

    }
    VKDescriptorSet::~VKDescriptorSet()
    {

    }

    bool VKDescriptorSet::LoadDescriptorSet()
    {
        if (this->aShaderTypes.empty())
        {
            F_LogError("*********************** VKDescriptorSet::LoadDescriptorSet: Shader types is empty !");
            return false;
        }

        this->vkShaderStageFlags = Util_Transform2VkShaderStageFlagBits(this->aShaderTypes);
        this->vkDescriptorSetLayoutBinding = {};
        this->vkDescriptorSetLayoutBinding.binding = (uint32_t)0;
        this->vkDescriptorSetLayoutBinding.descriptorType = Util_Transform2VkDescriptorType(this->typeDescriptorSetUsage);
        this->vkDescriptorSetLayoutBinding.descriptorCount = (uint32_t)1;
        this->vkDescriptorSetLayoutBinding.stageFlags = this->vkShaderStageFlags;
        this->vkDescriptorSetLayoutBinding.pImmutableSamplers = nullptr;

        return true;
    }

    VkDescriptorSetLayoutBinding VKDescriptorSet::NewDescriptorSetLayoutBinding(uint32_t binding, uint32_t descriptorCount, VkSampler* pSampler) const
    {
        VkDescriptorSetLayoutBinding newBinding = this->vkDescriptorSetLayoutBinding;
        newBinding.binding = binding;
        newBinding.descriptorCount = descriptorCount;
        newBinding.pImmutableSamplers = pSampler;
        return newBinding;
    }

}; //LostPeter