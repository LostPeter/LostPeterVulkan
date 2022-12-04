#include "../include/preinclude.h"
#include "../include/vulkanobject.h"
#include "../include/vulkanmanager.h"

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