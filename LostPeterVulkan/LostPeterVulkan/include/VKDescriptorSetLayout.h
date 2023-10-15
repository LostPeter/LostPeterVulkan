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

#ifndef _VK_DESCRIPTOR_SET_LAYOUT_H_
#define _VK_DESCRIPTOR_SET_LAYOUT_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKDescriptorSetLayout : public Base
    {
    public:
        VKDescriptorSetLayout(const String& _nameDescriptorSetLayout);
        virtual ~VKDescriptorSetLayout();

    public:
    protected:
        StringVector aNamesDescriptorSet;

        VkDescriptorSetLayout vkDescriptorSetLayout;

    public:
        LP_FORCEINLINE const StringVector& GetNamesDescriptorSet() const { return this->aNamesDescriptorSet; }
        LP_FORCEINLINE StringVector& GetNamesDescriptorSet() { return this->aNamesDescriptorSet; }
        LP_FORCEINLINE VkDescriptorSetLayout GetVKDescriptorSetLayout() const { return this->vkDescriptorSetLayout; }

    public:
        void Destroy();
        

    public:
        bool LoadDescriptorSetLayout();
    };

}; //LostPeter

#endif