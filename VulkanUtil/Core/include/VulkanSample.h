/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_SAMPLE_H_
#define _VULKAN_SAMPLE_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanSample
    {
    public:
        VulkanSample();
        virtual ~VulkanSample();

    public:
    protected:
        VulkanInstance* m_pInstance;
        VulkanDevice* m_pDevice;

    public:
        void Destroy();
        bool Init();

    public:


    protected:
        

    };

}; //LostPeter

#endif