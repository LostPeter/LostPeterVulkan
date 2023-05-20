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

#include "../include/PreInclude.h"
#include "../include/VulkanSample.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanSample::VulkanSample()
        : m_pInstance(nullptr)
        , m_pDevice(nullptr)
    {

    }

    VulkanSample::~VulkanSample()
    {
        
    }

    void VulkanSample::Destroy()
    {
        m_pDevice = nullptr;
        UTIL_DELETE(m_pInstance)
    }

    bool VulkanSample::Init()
    {


        return true;
    }



}; //LostPeter