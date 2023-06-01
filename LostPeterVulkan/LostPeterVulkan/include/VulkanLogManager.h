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

#ifndef _VULKAN_LOG_MANAGER_H_
#define _VULKAN_LOG_MANAGER_H_

#include "PreDefine.h"
#include "VulkanLog.h"

namespace LostPeter
{
    class utilExport VulkanLogManager
    {
    private:
        VulkanLogManager();
    public:
        ~VulkanLogManager();

    public:
    protected:
        VulkanLogPtrVector m_aLogs;
        VulkanLogPtrMap m_mapLogs;

    public:
        static const String ms_strLogDefault_Console;
        static const String ms_strLogDefault_File;

        static VulkanLogManager* GetInstance()
        {
            if (ms_pLogManager == nullptr)
            {
                ms_pLogManager = new VulkanLogManager();
            }
            return ms_pLogManager;
        }

    protected:
        static VulkanLogManager* ms_pLogManager;

    public:
        void Destroy();

        

    public:
    ////VulkanLog
        const VulkanLogPtrVector& GetLogPtrVector() const { return m_aLogs; }
        VulkanLogPtrVector& GetLogPtrVector() { return m_aLogs; }
        const VulkanLogPtrMap& GetLogPtrMap() const { return m_mapLogs; }
        VulkanLogPtrMap& GetLogPtrMap() { return m_mapLogs; }

        VulkanLog* GetLogPtr(const String& strName);
        VulkanLog* CreateLogPtr(const String& strName, VulkanLogType eLog);
        void InsertLogPtr(VulkanLog* pLog);
        void RemoveLogPtr(VulkanLog* pLog);
        void RemoveAllLogPtr();
        void DestroyLogPtr(VulkanLog* pLog);
        void DestroyAllLogPtr();
    };

}; //LostPeter

#endif