/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_H_
#define _VK_PIPELINE_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKPipeline : public Base
    {
    public:
        VKPipeline(const String& namePipeline);
        virtual ~VKPipeline();

    public:
    protected:
        VkPipeline vkPipeline;

    public:
        LP_FORCEINLINE VkPipeline GetVkPipeline() const { return this->vkPipeline; }

    public:
        void Destroy();

    public:
        bool LoadPipeline();
    };

}; //LostPeter

#endif