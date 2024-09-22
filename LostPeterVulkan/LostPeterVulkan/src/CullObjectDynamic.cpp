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

#include "../include/CullObjectDynamic.h"
#include "../include/VulkanWindow.h"

template<> LostPeterVulkan::CullObjectDynamic* LostPeterFoundation::FSingleton<LostPeterVulkan::CullObjectDynamic>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
    ////////////////////// CullUnitObjectDynamic /////////////////////
    CullObjectDynamic::CullUnitObjectDynamic::CullUnitObjectDynamic(const String& nameUnit, CullObjectDynamic* pCullOD)
        : CullUnitObject(nameUnit)
        , pCullObjectDynamic(pCullOD)
    {
        F_Assert(pCullObjectDynamic != nullptr && "CullUnitObjectDynamic::CullUnitObjectDynamic")
    }

    CullObjectDynamic::CullUnitObjectDynamic::~CullUnitObjectDynamic()
    {
        Destroy();
    }

    void CullObjectDynamic::CullUnitObjectDynamic::Destroy()
    {
        
    }

    void CullObjectDynamic::CullUnitObjectDynamic::Init()
    {

    }

    bool CullObjectDynamic::CullUnitObjectDynamic::IsCulling()
    {
        return true;
    }

    int CullObjectDynamic::CullUnitObjectDynamic::GetRenderCount()
    {
        return 0;
    }

    ComputeBuffer* CullObjectDynamic::CullUnitObjectDynamic::GetRenderArgsCB()
    {
        return nullptr; 
    }

    int CullObjectDynamic::CullUnitObjectDynamic::GetRenderDataCount()
    {
        return 0;
    }

    CullRenderData* CullObjectDynamic::CullUnitObjectDynamic::GetRenderData()
    {
        return nullptr; 
    }

    int CullObjectDynamic::CullUnitObjectDynamic::GetClusterDataCount(int index)
    {
        return 0;
    }

    CullObjectConstantsVector* CullObjectDynamic::CullUnitObjectDynamic::GetClusterDatas()
    {
        return nullptr; 
    }

    int CullObjectDynamic::CullUnitObjectDynamic::GetLodCount()
    {
        return 0;
    }

    ComputeBuffer* CullObjectDynamic::CullUnitObjectDynamic::GetClusterDataCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullObjectDynamic::CullUnitObjectDynamic::GetLodCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullObjectDynamic::CullUnitObjectDynamic::GetResultCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullObjectDynamic::CullUnitObjectDynamic::GetClipCB() 
    { 
        return nullptr; 
    }

    void CullObjectDynamic::CullUnitObjectDynamic::UpdateBuffer()
    {

    }


    ////////////////////// CullObjectDynamic /////////////////////////
    CullObjectDynamic* CullObjectDynamic::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	CullObjectDynamic& CullObjectDynamic::GetSingleton()
	{  
		F_Assert(ms_Singleton && "CullObjectDynamic::GetSingleton")
		return (*ms_Singleton);     
	}

    const String CullObjectDynamic::s_nameCullObjectDynamic = "CullObjectDynamic";
    const String CullObjectDynamic::s_nameCullUnitObjectDynamic = "CullUnitObjectDynamic";
    
    CullObjectDynamic::CullObjectDynamic()
        : Base(s_nameCullObjectDynamic)
    {
        pCullUnitObjectDynamic = new CullUnitObjectDynamic(s_nameCullUnitObjectDynamic, this);
    }

    CullObjectDynamic::~CullObjectDynamic()
    {
        Destroy();
    }

    void CullObjectDynamic::Destroy()
    {
        F_DELETE(pCullUnitObjectDynamic)
    }

    void CullObjectDynamic::Init()
    {
        
    }
    
}; //LostPeterVulkan