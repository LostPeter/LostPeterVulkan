/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_MANAGER_H_
#define _VULKAN_MANAGER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanManager
    {
    public:
        VulkanManager();
        ~VulkanManager();

    public:
        VulkanInstance* m_pInstance;
        VulkanDevice* m_pDevice;

    public:
        UTIL_FORCEINLINE VulkanInstance* GetInstance() const { return m_pInstance; }
        UTIL_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

    public:
        void Destroy();
        bool Init();

    protected:



    };

}; //LostPeter

#endif