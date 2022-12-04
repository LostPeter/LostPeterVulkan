// author: LostPeter
// time:   2022-12-03

#ifndef _VULKAN_MANAGER_H_
#define _VULKAN_MANAGER_H_

#include "predefine.h"

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