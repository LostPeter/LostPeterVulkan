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

#ifndef _VK_DESCRIPTOR_SET_H_
#define _VK_DESCRIPTOR_SET_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKDescriptorSet : public Base
    {
    public:
        VKDescriptorSet(const String& _nameDescriptorSet,
                        VulkanDescriptorSetType _typeDescriptorSet,
                        VulkanDescriptorSetUsageType _typeDescriptorSetUsage,
                        const Uint32Vector& _aShaderTypes);
        virtual ~VKDescriptorSet();

    public:
        VulkanDescriptorSetType typeDescriptorSet;
        VulkanDescriptorSetUsageType typeDescriptorSetUsage;
        Uint32Vector aShaderTypes;

        VkShaderStageFlags vkShaderStageFlags;
        VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding;

    public:
        bool LoadDescriptorSet();

        VkDescriptorSetLayoutBinding NewDescriptorSetLayoutBinding(uint32_t binding, uint32_t descriptorCount, VkSampler* pSampler) const;
    };

}; //LostPeter

#endif