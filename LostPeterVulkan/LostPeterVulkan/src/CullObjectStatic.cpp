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

#include "../include/CullObjectStatic.h"
#include "../include/VulkanWindow.h"
#include "../include/CullManager.h"
#include "../include/ComputeBuffer.h"
#include "../include/CullRenderData.h"

template<> LostPeterVulkan::CullObjectStatic* LostPeterFoundation::FSingleton<LostPeterVulkan::CullObjectStatic>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
    ////////////////////// CullUnitObjectStatic //////////////////////
    CullObjectStatic::CullUnitObjectStatic::CullUnitObjectStatic(const String& nameUnit, CullObjectStatic* pCullOS)
        : CullUnitObject(nameUnit)
        , pCullObjectStatic(pCullOS)
        , nLodCount((int)Vulkan_Lod_Count)
        , nObjectCount(0)
        , nRenderArgsCount(0)
        , pCB_CullObjects(nullptr)
        , pCB_LodArgs(nullptr)
        , pCB_RenderArgs(nullptr)
        , pCB_Result(nullptr)
        , pLodArgs(nullptr)
        , pRenderArgs(nullptr)
        , pResult(nullptr)
        , isRender(true)
    {
        F_Assert(pCullObjectStatic != nullptr && "CullUnitObjectStatic::CullUnitObjectStatic")
    }

    CullObjectStatic::CullUnitObjectStatic::~CullUnitObjectStatic()
    {
        Destroy();
    }

    void CullObjectStatic::CullUnitObjectStatic::Destroy()
    {
        destroyComputeBuffers();
        destroyDatas();
    }
        void CullObjectStatic::CullUnitObjectStatic::destroyComputeBuffers()
        {
            F_DELETE(this->pCB_CullObjects)
            F_DELETE(this->pCB_LodArgs)
            F_DELETE(this->pCB_RenderArgs)
            F_DELETE(this->pCB_Result)
            F_DELETE_T(this->pLodArgs)
            F_DELETE_T(this->pRenderArgs)
            F_DELETE_T(this->pResult)
        }
        void CullObjectStatic::CullUnitObjectStatic::destroyDatas()
        {
            size_t count = this->aCullRenderData.size();
            for (size_t i = 0; i < count; i++)
            {
                CullRenderData* pData = this->aCullRenderData[i];
                CullManager::GetSingleton().BackCullRenderData(pData);
            }
            this->aCullRenderData.clear();
            this->mapCullRenderData2Index.clear();

            this->nObjectCount = 0;
            this->aCullObjectConstants.clear();

            this->nRenderArgsCount = 0;
        }       

    void CullObjectStatic::CullUnitObjectStatic::Init()
    {
        createComputeBuffers();

    }
        void CullObjectStatic::CullUnitObjectStatic::createComputeBuffers()
        {
            //pCB_CullObjects
            {
                this->pCB_CullObjects = new ComputeBuffer("ComputeBuffer-Static-CullObjects", CullObjectStatic::s_nInstanceCountMax, sizeof(CullObjectConstants));
            }
            //pCB_LodArgs
            {
                int count_lodArgs = this->nLodCount * CullObjectStatic::s_nRenderCountMax * 2;
                this->pCB_LodArgs = new ComputeBuffer("ComputeBuffer-Static-LodArgs", count_lodArgs, sizeof(float));
                this->pLodArgs = new float[count_lodArgs];
            }
            //pCB_RenderArgs
            {
                int count_renderArgs = this->nLodCount * CullObjectStatic::s_nRenderCountMax * 5;
                this->pCB_RenderArgs = new ComputeBuffer("ComputeBuffer-Static-RenderArgs", count_renderArgs, sizeof(uint));
                this->pRenderArgs = new uint[count_renderArgs];
            }
            //pCB_Result
            {
                int count_result = this->nLodCount * CullObjectStatic::s_nInstanceCountMax * 5;
                this->pCB_Result = new ComputeBuffer("ComputeBuffer-Static-Result", count_result, sizeof(uint));
                this->pResult = new uint[count_result];
            }
        }

    bool CullObjectStatic::CullUnitObjectStatic::IsCulling()
    {
        if (this->isRender)
        {
            return true;
        }
        return false;
    }

    int CullObjectStatic::CullUnitObjectStatic::GetRenderCount()
    {
        return this->nRenderArgsCount;
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetRenderArgsCB()
    {
        return this->pCB_RenderArgs; 
    }

    int CullObjectStatic::CullUnitObjectStatic::GetRenderDataCount()
    {
        return (int)this->aCullRenderData.size();
    }

    CullRenderData* CullObjectStatic::CullUnitObjectStatic::GetRenderData(int index)
    {
        if (index < 0 || index >= this->aCullRenderData.size())
            return nullptr;
        return this->aCullRenderData[index];
    }

    int CullObjectStatic::CullUnitObjectStatic::GetObjectDataCount()
    {
        return (int)this->aCullObjectConstants.size();
    }

    CullObjectConstantsVector* CullObjectStatic::CullUnitObjectStatic::GetObjectDatas()
    {
        return &this->aCullObjectConstants; 
    }

    int CullObjectStatic::CullUnitObjectStatic::GetLodCount()
    {
        return this->nLodCount;
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetObjectDataCB()
    {
        return this->pCB_CullObjects; 
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetLodArgsCB()
    {
        return this->pCB_LodArgs; 
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetResultCB()
    {
        return this->pCB_Result;
    }

    void CullObjectStatic::CullUnitObjectStatic::UpdateBuffer()
    {

    }

    void CullObjectStatic::CullUnitObjectStatic::AddCullRenderData(CullRenderData* pData)
    {

    }
    void CullObjectStatic::CullUnitObjectStatic::RemoveCullRenderData(CullRenderData* pData)
    {

    }
    void CullObjectStatic::CullUnitObjectStatic::RefreshCullRenderData()
    {

    }


    ////////////////////// CullObjectStatic //////////////////////////
    CullObjectStatic* CullObjectStatic::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	CullObjectStatic& CullObjectStatic::GetSingleton()
	{  
		F_Assert(ms_Singleton && "CullObjectStatic::GetSingleton")
		return (*ms_Singleton);     
	}

    const String CullObjectStatic::s_nameCullObjectStatic = "CullObjectStatic";
    const String CullObjectStatic::s_nameCullUnitObjectStatic = "CullUnitObjectStatic";
    float CullObjectStatic::s_aLodMaxDistance[6] = 
    {
          0.0f,   128.0f,
		128.0f,   256.0f,
		256.0f,   512.0f
    };
    int CullObjectStatic::s_nRenderCountMax = 200;
    int CullObjectStatic::s_nInstanceCountMax = 100000;
    
    CullObjectStatic::CullObjectStatic()
        : Base(s_nameCullObjectStatic)
        , pCullUnitObjectStatic(nullptr)
        
    {
        
    }

    CullObjectStatic::~CullObjectStatic()
    {
        Destroy();
    }

    void CullObjectStatic::Destroy()
    {
        F_DELETE(pCullUnitObjectStatic)
        
    }
    
    void CullObjectStatic::Init()
    {
        this->pCullUnitObjectStatic = new CullUnitObjectStatic(s_nameCullUnitObjectStatic, this);
        this->pCullUnitObjectStatic->Init();
    }

}; //LostPeterVulkan