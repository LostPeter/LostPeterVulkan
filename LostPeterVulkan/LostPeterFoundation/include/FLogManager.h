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

#ifndef _F_LOG_MANAGER_H_
#define _F_LOG_MANAGER_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FLogManager
    {
    private:
        FLogManager();
    public:
        ~FLogManager();

    public:
    protected:
        FLogPtrVector m_aLogs;
        FLogPtrMap m_mapLogs;

    public:
        static const String ms_strLogDefault_Console;
        static const String ms_strLogDefault_File;

        static FLogManager* GetInstance()
        {
            if (ms_pLogManager == nullptr)
            {
                ms_pLogManager = new FLogManager();
            }
            return ms_pLogManager;
        }

    protected:
        static FLogManager* ms_pLogManager;

    public:
        void Destroy();
    
    public:
    ////FLog
        LP_FORCEINLINE const FLogPtrVector& GetLogPtrVector() const { return m_aLogs; }
        LP_FORCEINLINE FLogPtrVector& GetLogPtrVector() { return m_aLogs; }
        LP_FORCEINLINE const FLogPtrMap& GetLogPtrMap() const { return m_mapLogs; }
        LP_FORCEINLINE FLogPtrMap& GetLogPtrMap() { return m_mapLogs; }

        FLog* GetLogPtr(const String& strName);
        FLog* CreateLogPtr(const String& strName, FLogType eLog);
        void InsertLogPtr(FLog* pLog);
        void RemoveLogPtr(FLog* pLog);
        void RemoveAllLogPtr();
        void DestroyLogPtr(FLog* pLog);
        void DestroyAllLogPtr();
    };

}; //LostPeterFoundation

#endif