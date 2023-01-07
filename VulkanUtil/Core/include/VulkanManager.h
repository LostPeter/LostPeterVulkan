/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-12-03
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


    public:
        void Destroy();
        bool Init();

    protected:



    };

}; //LostPeter

#endif