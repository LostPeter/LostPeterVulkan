#include "../include/preinclude.h"
#include "../include/vulkanqueue.h"
#include "../include/vulkandevice.h"

namespace LostPeter
{
    VulkanQueue::VulkanQueue()
        : m_vkQueue(VK_NULL_HANDLE)
        , m_nFamilyIndex(-1)
        , m_pDevice(nullptr)
    {

    }

    VulkanQueue::~VulkanQueue()
    {
        Destroy();
    }

    void VulkanQueue::Destroy()
    {
        if (m_vkQueue != VK_NULL_HANDLE)
        {

        }
        m_vkQueue = VK_NULL_HANDLE;
    }

    bool VulkanQueue::Init(VulkanDevice* pDevice, uint32 nFamilyIndex)
    {
        // m_pDevice = pDevice;
        // m_nFamilyIndex = nFamilyIndex;
        // vkGetDeviceQueue(m_pDevice->GetVkDevice(), m_nFamilyIndex, 0, &m_vkQueue);
        // if (m_vkQueue == VK_NULL_HANDLE)
        // {
        //     Util_LogError("*********************** VulkanQueue::Init: vkGetDeviceQueue failed !");
        //     return false;
        // }

        return true;
    }

}; //LostPeter