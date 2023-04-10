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
#include "../include/VulkanObject.h"
#include "../include/VulkanManager.h"

namespace LostPeter
{
    VulkanManager* VulkanObject::ms_pVulkanManager = nullptr;
    VulkanObject::VulkanObject(const std::string& strName)
        : m_strName(strName)
        , m_nRefCount(0)
        , m_bIsInit(false)
        , m_bIsShowSphere(false)
		, m_bIsShowAABB(false)
    {
        
    }

    VulkanObject::~VulkanObject()
    {
        
    }

}; //LostPeter