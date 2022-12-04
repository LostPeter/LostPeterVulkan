#include "../include/preinclude.h"
#include "../include/vulkanmanager.h"
#include "../include/vulkaninstance.h"
#include "../include/vulkandevice.h"

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