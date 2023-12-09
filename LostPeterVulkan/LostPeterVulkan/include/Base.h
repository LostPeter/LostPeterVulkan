/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
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
#include "VulkanDefine.h"

namespace LostPeterVulkan
{
    class vulkanExport Base
    {
    public:
        Base(const String& _name);
        Base(uint32 _group, const String& _name);
        virtual ~Base();

    public:
        static VulkanWindow* ms_pVulkanWindow;

    public:
        static VulkanWindow* GetWindowPtr() { return ms_pVulkanWindow; }

    protected:
        uint32 group;
        String name;
        int refCount;

        bool isInit;

    public:
        F_FORCEINLINE uint32 GetGroup() const { return this->group; }
        F_FORCEINLINE void SetGroup(uint32 _group) { this->group = _group; }
        F_FORCEINLINE const String& GetName() const { return this->name; }
        F_FORCEINLINE void SetName(const String& _name) { this->name = _name; }

        F_FORCEINLINE int GetRef() const { return this->refCount; }
        F_FORCEINLINE bool HasRef() const { return this->refCount <= 0 ? false : true; }
        F_FORCEINLINE int AddRef() { return ++this->refCount; }
        F_FORCEINLINE int DelRef() { return --this->refCount; }

        F_FORCEINLINE bool IsInit() const { return this->isInit; }
        F_FORCEINLINE void SetIsInit(bool b) { this->isInit = b; }

    public:
        bool IsGroupNameValid() const;
    };

}; //LostPeterVulkan

#endif