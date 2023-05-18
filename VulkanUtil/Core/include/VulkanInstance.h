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

#ifndef _VULKAN_INSTANCE_H_
#define _VULKAN_INSTANCE_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanInstance
    {
    private:
        VulkanInstance();
    public:
        ~VulkanInstance();

    public:
    protected:
        VkInstance m_vkInstance;
        VkDebugReportCallbackEXT m_vkDebugReport;
        std::vector<const char*> m_aInstanceLayers;
        std::vector<const char*> m_aInstanceExtensions;
        std::vector<const char*> m_aAppInstanceExtensions;
        std::vector<const char*> m_aAppDeviceExtensions;
        int32 m_nPreferredVendorID;
        VkPhysicalDeviceFeatures2* m_pVkPhysicalDeviceFeatures2;
        uint32 m_nDesiredNumSwapChainImages;

        bool m_bIsEnableValidationLayers;
        VulkanDevice* m_pDevice;

        VulkanPixelFormatType m_eSwapChainImagePixelFormat;


    public:
        void Destroy();
        bool Init();

    public:
    protected:
        bool createInstance();
        bool createDebugReport();
        bool createDevice();

    private:
        void getInstanceLayersAndExtensions(bool bIsEnableValidationLayers,
                                            std::vector<const char*>& outInstanceLayers, 
                                            std::vector<const char*>& outInstanceExtensions);

        VkDebugReportCallbackCreateInfoEXT createReportCallbackInfo();
        void destroyReportCallbackInfo();
    };

}; //LostPeter

#endif