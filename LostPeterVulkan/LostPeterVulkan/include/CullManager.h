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
        static int s_nRenderCount_Init;
        static int s_nRenderCount_Max;
        static int s_nRenderCount_Step;
        static int s_nInstanceCount_Init;
        static int s_nInstanceCount_Max;
        static int s_nInstanceCount_Step;

        ObjectPool<CullRenderData>* pCullRenderDataPool;
        ObjectPool<CullObjectConstants>* pCullObjectConstantsPool;
        ObjectPool<CullObjectInstanceConstants>* pCullObjectInstanceConstantsPool;

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

    protected:  
        void destroyCullObjects();
        void destroyPools();

        void createPools();
        void createCullObjects();

    public:
        void AddCullUnit(CullUnit* pCullUnit);
        void RemoveCullUnit(CullUnit* pCullUnit);
        void RemoveAllCullUnit();

    public:
        CullRenderData* GetCullRenderData();
        void BackCullRenderData(CullRenderData* pCullRenderData);

        CullObjectConstants* GetCullObjectConstants();
        void BackCullObjectConstants(CullObjectConstants* pCullObject);

        CullObjectInstanceConstants* GetCullObjectInstanceConstants();
        void BackCullObjectInstanceConstants(CullObjectInstanceConstants* pCullObjectInstance);
    };

}; //LostPeterVulkan

#endif