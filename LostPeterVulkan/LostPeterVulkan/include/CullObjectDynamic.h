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
#include "CullUnitObject.h"

namespace LostPeterVulkan
{
    ////////////////////// CullObjectDynamic /////////////////////////
    class vulkanExport CullObjectDynamic : public FSingleton<CullObjectDynamic>
                                         , public Base
    {
    public:
        CullObjectDynamic();
        virtual ~CullObjectDynamic();

    public:
        ////////////////////// CullUnitObjectDynamic /////////////////////
        class vulkanExport CullUnitObjectDynamic : public CullUnitObject
        {
        public:
            CullUnitObjectDynamic(const String& nameUnit, CullObjectDynamic* pCullOD);
            virtual ~CullUnitObjectDynamic();

        public:
            CullObjectDynamic* pCullObjectDynamic;
            
            int nLodCount;
            CullRenderDataPtrVector aCullRenderData;
            CullRenderDataPtr2IndexMap mapCullRenderData2Index;

            int nObjectCount;
            CullObjectConstantsVector aCullObjectConstants;

            int nRenderArgsCount;

            BufferCompute* pCB_CullObjects;
            BufferCompute* pCB_LodArgs;
            BufferCompute* pCB_RenderArgs;
            BufferCompute* pCB_Result;

            bool isRender;

        public:
            void Destroy();
            virtual void Init();

        protected:
            void destroyComputeBuffers();
            void destroyDatas();
            
            void createComputeBuffers();

        public:
            virtual bool IsCulling();
            virtual int GetRenderCount();  
            virtual BufferCompute* GetRenderArgsCB();

        public:
            virtual int GetRenderDataCount();
            virtual CullRenderData* GetRenderData(int index);
            
            virtual int GetObjectDataCount();
            virtual CullObjectConstantsVector* GetObjectDatas();

            virtual int GetLodCount();

            virtual BufferCompute* GetObjectDataCB();
            virtual BufferCompute* GetLodArgsCB();
            virtual BufferCompute* GetResultCB();

            virtual void UpdateBuffer();

        public:
            bool HasCullRenderData(CullRenderData* pCullRenderData);
            void AddCullRenderData(CullRenderData* pCullRenderData);
            void RemoveCullRenderData(CullRenderData* pCullRenderData);
            void RefreshCullRenderData();
        };
        typedef std::vector<CullUnitObjectDynamic*> CullUnitObjectDynamicPtrVector;
        typedef std::map<String, CullUnitObjectDynamic*> CullUnitObjectDynamicPtrMap;


    public:
        static const String s_nameCullObjectDynamic;
        static const String s_nameCullUnitObjectDynamic;
        
        static float s_aLodMaxDistance[6];
        
        static int s_nRenderCountMax;
        static int s_nInstanceCountMax;

    public:
        CullUnitObjectDynamic* pCullUnitObjectDynamic;

    public:
        static CullObjectDynamic& GetSingleton();
		static CullObjectDynamic* GetSingletonPtr();

    public:
        void Destroy();
        void Init();

    public: 
        CullRenderData* AddDynamicCullRenderData(CullLodData* pCullLodData);
        bool AddDynamicCullRenderDatas(const CullLodDataPtrVector& aLodDatas, CullRenderDataPtrVector& aCullRenderData);
        void RemoveDynamicCullRenderData(CullRenderData* pCullRenderData);

    };

}; //LostPeterVulkan

#endif