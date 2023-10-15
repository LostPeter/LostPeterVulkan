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

#ifndef _VK_PIPELINE_LAYOUT_H_
#define _VK_PIPELINE_LAYOUT_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKPipelineLayout : public Base
    {
    public:
        VKPipelineLayout(const String& _namePipelineLayout);
        virtual ~VKPipelineLayout();

    public:
    protected:
        VkPipelineLayout vkPipelineLayout;

    public:
        LP_FORCEINLINE VkPipelineLayout GetVkPipelineLayout() const { return this->vkPipelineLayout; }

    public:
        void Destroy();

    public:
        bool LoadPipelineLayout();
    };

}; //LostPeter

#endif