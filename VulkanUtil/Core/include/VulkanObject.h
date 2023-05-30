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

#ifndef _VULKAN_OBJECT_H_
#define _VULKAN_OBJECT_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanObject
    {
    public:
        VulkanObject(const String& strName);
        virtual ~VulkanObject();

    public:
    protected:
        static VulkanManager* ms_pVulkanManager;

    public:
        static VulkanManager* GetVulkanManager() { return ms_pVulkanManager; }
        static void SetVulkanManager(VulkanManager* pVulkanManager) { ms_pVulkanManager = pVulkanManager; }

    public:
    protected:
        String m_strName;
        int m_nRefCount;

        // bool m_bIsInit;
        // bool m_bIsShowSphere;
		// bool m_bIsShowAABB;

    public:
        UTIL_FORCEINLINE const String& GetName() const { return m_strName; }
        UTIL_FORCEINLINE void SetName(const String& strName) { m_strName = strName; }

        UTIL_FORCEINLINE int GetRef() const { return m_nRefCount; }
        UTIL_FORCEINLINE bool HasRef() const { return m_nRefCount <= 0 ? false : true; }
        UTIL_FORCEINLINE int AddRef() { return ++m_nRefCount; }
        UTIL_FORCEINLINE int DelRef() { return --m_nRefCount; }

    public:
        virtual bool IsInit() const = 0;
        virtual void SetIsInit(bool b) = 0;
        virtual bool IsShowSphere() const = 0;
        virtual void SetIsShowSphere(bool bIsShowSphere) = 0;
        virtual bool IsShowAABB() const = 0;
        virtual void SetIsShowAABB(bool bIsShowAABB) = 0;
    };

}; //LostPeter

#endif