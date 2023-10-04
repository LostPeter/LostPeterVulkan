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

namespace LostPeter
{
    class utilExport Base
    {
    public:
        Base(const String& _name);
        virtual ~Base();

    public:
        static VulkanWindow* ms_pVulkanWindow;

    public:
        static VulkanWindow* GetWindowPtr() { return ms_pVulkanWindow; }

    protected:
        String name;
        int refCount;

        bool isInit;

    public:
        UTIL_FORCEINLINE const String& GetName() const { return name; }
        UTIL_FORCEINLINE void SetName(const String& _name) { name = _name; }

        UTIL_FORCEINLINE int GetRef() const { return refCount; }
        UTIL_FORCEINLINE bool HasRef() const { return refCount <= 0 ? false : true; }
        UTIL_FORCEINLINE int AddRef() { return ++refCount; }
        UTIL_FORCEINLINE int DelRef() { return --refCount; }

        UTIL_FORCEINLINE bool IsInit() const { return isInit; }
        UTIL_FORCEINLINE void SetIsInit(bool b) { isInit = b; }
    };

}; //LostPeter

#endif