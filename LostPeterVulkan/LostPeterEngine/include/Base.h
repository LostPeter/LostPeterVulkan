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
        Base(const String& _name);
        Base(uint32 _group, const String& _name);
        virtual ~Base();

    public:
    public:
        

    protected:
        uint32 group;
        String name;
        int refCount;

        bool isInit;

    public:
        E_FORCEINLINE uint32 GetGroup() const { return this->group; }
        E_FORCEINLINE void SetGroup(uint32 _group) { this->group = _group; }
        E_FORCEINLINE const String& GetName() const { return this->name; }
        E_FORCEINLINE void SetName(const String& _name) { this->name = _name; }

        E_FORCEINLINE int GetRef() const { return this->refCount; }
        E_FORCEINLINE bool HasRef() const { return this->refCount <= 0 ? false : true; }
        E_FORCEINLINE int AddRef() { return ++this->refCount; }
        E_FORCEINLINE int DelRef() { return --this->refCount; }

        E_FORCEINLINE bool IsInit() const { return this->isInit; }
        E_FORCEINLINE void SetIsInit(bool b) { this->isInit = b; }

    public:
        bool IsGroupNameValid() const;
    };

}; //LostPeterEngine

#endif