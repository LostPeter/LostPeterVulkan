/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _CULL_MANAGER_H_
#define _CULL_MANAGER_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport CullManager : public FSingleton<CullManager>
                                   , public Base
    {
    public:
        CullManager();
        virtual ~CullManager();

    public:


    public:
        VKPipelineComputeCull* pVKPipelineComputeCull;

        CullUnitPtrVector aCullUnits;
        CullUnitObjectPtrVector aCullUnitObjects;
        CullUnitTerrainPtrVector aCullUnitTerrains;

        CullObjectStatic* pCullObjectStatic;
        CullObjectDynamic* pCullObjectDynamic;

        bool isInit;

    public:
        static CullManager& GetSingleton();
		static CullManager* GetSingletonPtr();

    public:
        F_FORCEINLINE bool IsInit() const { return isInit; }

    public:
        void Destroy();
        void Init(VKPipelineComputeCull* pPipelineComputeCull);

    public:
        void AddCullUnit(CullUnit* pCullUnit);
        void RemoveCullUnit(CullUnit* pCullUnit);
        void RemoveAllCullUnit();

    public:

    };

}; //LostPeterVulkan

#endif