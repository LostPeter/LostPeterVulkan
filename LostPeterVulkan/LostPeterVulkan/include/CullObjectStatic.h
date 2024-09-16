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

#ifndef _CULL_OBJECT_STATIC_H_
#define _CULL_OBJECT_STATIC_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullObjectStatic : public FSingleton<CullObjectStatic>
                                        , public Base
    {
    public:
        CullObjectStatic();
        virtual ~CullObjectStatic();

    public:
        static const String s_nameCullObjectStatic;
        static const String s_nameCullUnitObjectStatic;

        static float s_aLodMaxDistance[6];
        static int s_nMaxRenderCount;
        static int s_nStepRenderCount;
        static int s_nMaxInstanceCount;


    public:
        CullUnitObjectStatic* pCullUnitObjectStatic;

    public:
        static CullObjectStatic& GetSingleton();
		static CullObjectStatic* GetSingletonPtr();

    public:
        void Destroy();
        void Init();

    };

}; //LostPeterVulkan

#endif