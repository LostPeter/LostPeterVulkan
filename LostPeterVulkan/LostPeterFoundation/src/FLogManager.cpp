/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FPreInclude.h"
#include "../include/FLogManager.h"
#include "../include/FLog.h"

namespace LostPeterFoundation
{
    const String FLogManager::ms_strLogDefault_Console = "console";
    const String FLogManager::ms_strLogDefault_File = "file";
    FLogManager* FLogManager::ms_pLogManager = nullptr;
    FLogManager::FLogManager()
    {

    }

    FLogManager::~FLogManager()
    {
        Destroy();
    }

    void FLogManager::Destroy()
    {
        DestroyAllLogPtr();
    }


////FLog
    FLog* FLogManager::GetLogPtr(const String& strName)
    {
        VFLogPtrMap::iterator itFind = m_mapLogs.find(strName);
        if (itFind == m_mapLogs.end())
            return nullptr;
        return itFind->second;
    }
    FLog* FLogManager::CreateLogPtr(const String& strName, FLogType eLog)
    {
        FLog* pLog = GetLogPtr(strName);
        if (pLog != nullptr)
        {
            return pLog;
        }

        if (eLog == F_Log_Console)
        {
            pLog = new FLogConsole(strName);
        }
        else if (eLog == F_Log_File)
        {
            pLog = new FLogFile(strName);
        }
        else
        {
            assert(false && "FLogManager::CreateLogPtr");
            return nullptr;
        }

        InsertLogPtr(pLog);
        return pLog;
    }
    void FLogManager::InsertLogPtr(FLog* pLog)
    {
        VFLogPtrMap::iterator itFind = m_mapLogs.find(pLog->GetName());
        if (itFind != m_mapLogs.end())
            return;

        m_aLogs.push_back(pLog);
        m_mapLogs[pLog->GetName()] = pLog;
    }
    void FLogManager::RemoveLogPtr(FLog* pLog)
    {
        VFLogPtrMap::iterator itFind = m_mapLogs.find(pLog->GetName());
        if (itFind == m_mapLogs.end())
            return;
        
        FLogPtrVector::iterator itV = std::find(m_aLogs.begin(), m_aLogs.end(), pLog);
        if (itV != m_aLogs.end())
            m_aLogs.erase(itV);
        m_mapLogs.erase(itFind);
    }
    void FLogManager::RemoveAllLogPtr()
    {
        m_aLogs.clear();
        m_mapLogs.clear();
    }
    void FLogManager::DestroyLogPtr(FLog* pLog)
    {
        RemoveLogPtr(pLog);
        F_DELETE(pLog)
    }
    void FLogManager::DestroyAllLogPtr()
    {
        size_t count = m_aLogs.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(m_aLogs[i])
        }
        RemoveAllLogPtr();
    }

}; //LostPeterFoundation