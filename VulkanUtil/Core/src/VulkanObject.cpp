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