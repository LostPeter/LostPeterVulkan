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