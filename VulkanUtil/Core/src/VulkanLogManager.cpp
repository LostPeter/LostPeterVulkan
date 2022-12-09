#include "../include/PreInclude.h"
#include "../include/VulkanLogManager.h"

namespace LostPeter
{
    const std::string VulkanLogManager::ms_strLogDefault_Console = "console";
    const std::string VulkanLogManager::ms_strLogDefault_File = "file";
    VulkanLogManager* VulkanLogManager::ms_pLogManager = nullptr;
    VulkanLogManager::VulkanLogManager()
    {

    }

    VulkanLogManager::~VulkanLogManager()
    {
        Destroy();
    }

    void VulkanLogManager::Destroy()
    {
        DestroyAllLogPtr();
    }


////VulkanLog
    VulkanLog* VulkanLogManager::GetLogPtr(const std::string& strName)
    {
        VulkanLogPtrMap::iterator itFind = m_mapLogs.find(strName);
        if (itFind == m_mapLogs.end())
            return nullptr;
        return itFind->second;
    }
    VulkanLog* VulkanLogManager::CreateLogPtr(const std::string& strName, VulkanLogType eLog)
    {
        VulkanLog* pLog = GetLogPtr(strName);
        if (pLog != nullptr)
        {
            return pLog;
        }

        if (eLog == Vulkan_Log_Console)
        {
            pLog = new VulkanLogConsole(strName);
        }
        else if (eLog == Vulkan_Log_File)
        {
            pLog = new VulkanLogFile(strName);
        }
        else
        {
            assert(false && "VulkanLogManager::CreateLogPtr");
            return nullptr;
        }

        InsertLogPtr(pLog);
        return pLog;
    }
    void VulkanLogManager::InsertLogPtr(VulkanLog* pLog)
    {
        VulkanLogPtrMap::iterator itFind = m_mapLogs.find(pLog->GetName());
        if (itFind != m_mapLogs.end())
            return;

        m_aLogs.push_back(pLog);
        m_mapLogs[pLog->GetName()] = pLog;
    }
    void VulkanLogManager::RemoveLogPtr(VulkanLog* pLog)
    {
        VulkanLogPtrMap::iterator itFind = m_mapLogs.find(pLog->GetName());
        if (itFind == m_mapLogs.end())
            return;
        
        VulkanLogPtrVector::iterator itV = std::find(m_aLogs.begin(), m_aLogs.end(), pLog);
        if (itV != m_aLogs.end())
            m_aLogs.erase(itV);
        m_mapLogs.erase(itFind);
    }
    void VulkanLogManager::RemoveAllLogPtr()
    {
        m_aLogs.clear();
        m_mapLogs.clear();
    }
    void VulkanLogManager::DestroyLogPtr(VulkanLog* pLog)
    {
        RemoveLogPtr(pLog);
        UTIL_DELETE(pLog)
    }
    void VulkanLogManager::DestroyAllLogPtr()
    {
        size_t count = m_aLogs.size();
        for (size_t i = 0; i < count; i++)
        {
            UTIL_DELETE(m_aLogs[i])
        }
        RemoveAllLogPtr();
    }

}; //LostPeter