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

template<> LostPeterVulkan::CullObjectStatic* LostPeterFoundation::FSingleton<LostPeterVulkan::CullObjectStatic>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
    ////////////////////// CullUnitObjectStatic //////////////////////
    CullObjectStatic::CullUnitObjectStatic::CullUnitObjectStatic(const String& nameUnit, CullObjectStatic* pCullOS)
        : CullUnitObject(nameUnit)
        , pCullObjectStatic(pCullOS)
    {
        F_Assert(pCullObjectStatic != nullptr && "CullUnitObjectStatic::CullUnitObjectStatic")
    }

    CullObjectStatic::CullUnitObjectStatic::~CullUnitObjectStatic()
    {
        Destroy();
    }

    void CullObjectStatic::CullUnitObjectStatic::Destroy()
    {
        
    }
    
    void CullObjectStatic::CullUnitObjectStatic::Init()
    {

    }

    bool CullObjectStatic::CullUnitObjectStatic::IsCulling()
    {
        return true;
    }

    int CullObjectStatic::CullUnitObjectStatic::GetRenderCount()
    {
        return 0;
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetRenderArgsCB()
    {
        return nullptr; 
    }

    int CullObjectStatic::CullUnitObjectStatic::GetRenderDataCount()
    {
        return 0;
    }

    CullRenderData* CullObjectStatic::CullUnitObjectStatic::GetRenderData()
    {
        return nullptr; 
    }

    int CullObjectStatic::CullUnitObjectStatic::GetClusterDataCount(int index)
    {
        return 0;
    }

    CullObjectConstantsVector* CullObjectStatic::CullUnitObjectStatic::GetClusterDatas()
    {
        return nullptr; 
    }

    int CullObjectStatic::CullUnitObjectStatic::GetLodCount()
    {
        return 0;
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetClusterDataCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetLodCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetResultCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullObjectStatic::CullUnitObjectStatic::GetClipCB() 
    { 
        return nullptr; 
    }

    void CullObjectStatic::CullUnitObjectStatic::UpdateBuffer()
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
    int CullObjectStatic::s_nMaxRenderCount = 200;
    int CullObjectStatic::s_nStepRenderCount = 20;
    int CullObjectStatic::s_nMaxInstanceCount = 100000;
    
    CullObjectStatic::CullObjectStatic()
        : Base(s_nameCullObjectStatic)
    {
        pCullUnitObjectStatic = new CullUnitObjectStatic(s_nameCullUnitObjectStatic, this);
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
        
    }

}; //LostPeterVulkan