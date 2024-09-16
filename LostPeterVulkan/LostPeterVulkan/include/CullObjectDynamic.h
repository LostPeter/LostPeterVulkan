/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _CULL_OBJECT_DYNAMIC_H_
#define _CULL_OBJECT_DYNAMIC_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullObjectDynamic : public FSingleton<CullObjectDynamic>
                                         , public Base
    {
    public:
        CullObjectDynamic();
        virtual ~CullObjectDynamic();

    public:
        static const String s_nameCullObjectDynamic;
        static const String s_nameCullUnitObjectDynamic;
        

    public:
        CullUnitObjectDynamic* pCullUnitObjectDynamic;

    public:
        static CullObjectDynamic& GetSingleton();
		static CullObjectDynamic* GetSingletonPtr();

    public:
        void Destroy();
        void Init();
    };

}; //LostPeterVulkan

#endif