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

#include "../include/PreInclude.h"
#include "../include/VulkanManager.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanManager::VulkanManager()
        : m_pInstance(nullptr)
        , m_pDevice(nullptr)
    {

    }

    VulkanManager::~VulkanManager()
    {

    }

    void VulkanManager::Destroy()
    {
        
    }
    bool VulkanManager::Init()
    {
        
        return true;
    } 

}; //LostPeter