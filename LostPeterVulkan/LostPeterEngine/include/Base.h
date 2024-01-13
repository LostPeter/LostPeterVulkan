/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _BASE_H_
#define _BASE_H_

#include "PreDefine.h"
#include "EngineDefine.h"

namespace LostPeterEngine
{
    class engineExport Base
    {
    public:
        Base(const String& strName);
        Base(uint32 nGroup, const String& strName);
        virtual ~Base();

    public:
    protected:
        uint32 m_nGroup;
        String m_strName;
        int m_nRefCount;

        bool m_bIsInit;

    public:
        F_FORCEINLINE uint32 GetGroup() const { return this->m_nGroup; }
        F_FORCEINLINE void SetGroup(uint32 nGroup) { this->m_nGroup = nGroup; }
        F_FORCEINLINE const String& GetName() const { return this->m_strName; }
        F_FORCEINLINE void SetName(const String& strName) { this->m_strName = strName; }

        F_FORCEINLINE int GetRef() const { return this->m_nRefCount; }
        F_FORCEINLINE bool HasRef() const { return this->m_nRefCount <= 0 ? false : true; }
        F_FORCEINLINE int AddRef() { return ++this->m_nRefCount; }
        F_FORCEINLINE int DelRef() { return --this->m_nRefCount; }
        
        F_FORCEINLINE int CanDel() { return !HasRef(); }

        F_FORCEINLINE bool IsInit() const { return this->m_bIsInit; }
        F_FORCEINLINE void SetIsInit(bool b) { this->m_bIsInit = b; }

    public:
        bool IsGroupNameValid() const;
    };

}; //LostPeterEngine

#endif