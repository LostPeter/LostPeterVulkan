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
#include "CullUnitObject.h"

namespace LostPeterVulkan
{
    ////////////////////// CullObjectStatic //////////////////////////
    class vulkanExport CullObjectStatic : public FSingleton<CullObjectStatic>
                                        , public Base
    {
    public:
        CullObjectStatic();
        virtual ~CullObjectStatic();

    public:
        ////////////////////// CullUnitObjectStatic //////////////////////
        class vulkanExport CullUnitObjectStatic : public CullUnitObject
        {
        public:
            CullUnitObjectStatic(const String& nameUnit, CullObjectStatic* pCullOS);
            virtual ~CullUnitObjectStatic();

        public:
            CullObjectStatic* pCullObjectStatic;

            int nLodCount;
            CullRenderDataPtrVector aCullRenderData;
            CullRenderDataPtr2IndexMap mapCullRenderData2Index;

            int nObjectCount;
            CullObjectConstantsVector aCullObjectConstants;

            int nRenderArgsCount;

            ComputeBuffer* pCB_CullObjects;
            ComputeBuffer* pCB_LodArgs;
            ComputeBuffer* pCB_RenderArgs;
            ComputeBuffer* pCB_Result;
            float* pLodArgs;
			uint* pRenderArgs;
			uint* pResult;

            bool isRender;

        public:
            virtual void Destroy();
            virtual void Init();

        protected:
            void destroyComputeBuffers();
            void destroyDatas();
            
            void createComputeBuffers();

        public:
            virtual bool IsCulling();
            virtual int GetRenderCount();  
            virtual ComputeBuffer* GetRenderArgsCB();

        public:
            virtual int GetRenderDataCount();
            virtual CullRenderData* GetRenderData(int index);
            
            virtual int GetObjectDataCount();
            virtual CullObjectConstantsVector* GetObjectDatas();

            virtual int GetLodCount();

            virtual ComputeBuffer* GetObjectDataCB();
            virtual ComputeBuffer* GetLodArgsCB();
            virtual ComputeBuffer* GetResultCB();

            virtual void UpdateBuffer();

        public:
            void AddCullRenderData(CullRenderData* pData);
            void RemoveCullRenderData(CullRenderData* pData);
            void RefreshCullRenderData();
        };
        typedef std::vector<CullUnitObjectStatic*> CullUnitObjectStaticPtrVector;
        typedef std::map<String, CullUnitObjectStatic*> CullUnitObjectStaticPtrMap;

    public:
        static const String s_nameCullObjectStatic;
        static const String s_nameCullUnitObjectStatic;

        static float s_aLodMaxDistance[6];
        
        static int s_nRenderCountMax;
        static int s_nInstanceCountMax;

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