// author: LostPeter
// time:   2022-12-03

#ifndef _VULKAN_OBJECT_H_
#define _VULKAN_OBJECT_H_

#include "predefine.h"

namespace LostPeter
{
    class utilExport VulkanObject
    {
    public:
        VulkanObject(const std::string& strName);
        virtual ~VulkanObject();

    public:
    protected:
        static VulkanManager* ms_pVulkanManager;

    public:
        static VulkanManager* GetVulkanManager() { return ms_pVulkanManager; }
        static void SetVulkanManager(VulkanManager* pVulkanManager) { ms_pVulkanManager = pVulkanManager; }

    public:
    protected:
        std::string m_strName;

        int m_nRefCount;

        bool m_bIsInit;
        bool m_bIsShowSphere;
		bool m_bIsShowAABB;

    public:
        const std::string& GetName() const { return m_strName; }
        void SetName(const std::string& strName) { m_strName = strName; }

        int GetRef() const { return m_nRefCount; }
        bool HasRef() const { return m_nRefCount <= 0 ? false : true; }

        int AddRef() { return ++m_nRefCount; }
        int DelRef() { return --m_nRefCount; }

        bool IsInit() const { return m_bIsInit; }
        void SetIsInit(bool b) { m_bIsInit = b; }
        bool IsShowSphere() const { return m_bIsShowSphere; }
        void SetIsShowSphere(bool bIsShowSphere) { m_bIsShowSphere = bIsShowSphere; }
        bool IsShowAABB() const { return m_bIsShowAABB; }
        void SetIsShowAABB(bool bIsShowAABB) { m_bIsShowAABB = bIsShowAABB; }
    };

}; //LostPeter

#endif