/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/CullUnitObjectDynamic.h"
#include "../include/VulkanWindow.h"
#include "../include/CullObjectDynamic.h"

namespace LostPeterVulkan
{
    CullUnitObjectDynamic::CullUnitObjectDynamic(const String& nameUnit, CullObjectDynamic* pCullOD)
        : CullUnitObject(nameUnit)
        , pCullObjectDynamic(pCullOD)
    {
        F_Assert(pCullObjectDynamic != nullptr && "CullUnitObjectDynamic::CullUnitObjectDynamic")
    }

    CullUnitObjectDynamic::~CullUnitObjectDynamic()
    {
        Destroy();
    }

    void CullUnitObjectDynamic::Destroy()
    {
        
    }

    void CullUnitObjectDynamic::Init()
    {

    }

    bool CullUnitObjectDynamic::IsCulling()
    {
        return true;
    }

    int CullUnitObjectDynamic::GetRenderCount()
    {
        return 0;
    }

    ComputeBuffer* CullUnitObjectDynamic::GetRenderArgsCB()
    {
        return nullptr; 
    }

    int CullUnitObjectDynamic::GetRenderDataCount()
    {
        return 0;
    }

    CullRenderData* CullUnitObjectDynamic::GetRenderData()
    {
        return nullptr; 
    }

    int CullUnitObjectDynamic::GetClusterDataCount(int index)
    {
        return 0;
    }

    CullObjectConstantsVector* CullUnitObjectDynamic::GetClusterDatas()
    {
        return nullptr; 
    }

    int CullUnitObjectDynamic::GetLodCount()
    {
        return 0;
    }

    ComputeBuffer* CullUnitObjectDynamic::GetClusterDataCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullUnitObjectDynamic::GetLodCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullUnitObjectDynamic::GetResultCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullUnitObjectDynamic::GetClipCB() 
    { 
        return nullptr; 
    }

    void CullUnitObjectDynamic::UpdateBuffer()
    {

    }
    
}; //LostPeterVulkan