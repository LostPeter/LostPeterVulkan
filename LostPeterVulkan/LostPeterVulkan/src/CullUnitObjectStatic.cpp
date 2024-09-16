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

#include "../include/CullUnitObjectStatic.h"
#include "../include/VulkanWindow.h"
#include "../include/CullObjectStatic.h"

namespace LostPeterVulkan
{
    

    CullUnitObjectStatic::CullUnitObjectStatic(const String& nameUnit, CullObjectStatic* pCullOS)
        : CullUnitObject(nameUnit)
        , pCullObjectStatic(pCullOS)
    {
        F_Assert(pCullObjectStatic != nullptr && "CullUnitObjectStatic::CullUnitObjectStatic")
    }

    CullUnitObjectStatic::~CullUnitObjectStatic()
    {
        Destroy();
    }

    void CullUnitObjectStatic::Destroy()
    {
        
    }
    
    void CullUnitObjectStatic::Init()
    {

    }

    bool CullUnitObjectStatic::IsCulling()
    {
        return true;
    }

    int CullUnitObjectStatic::GetRenderCount()
    {
        return 0;
    }

    ComputeBuffer* CullUnitObjectStatic::GetRenderArgsCB()
    {
        return nullptr; 
    }

    int CullUnitObjectStatic::GetRenderDataCount()
    {
        return 0;
    }

    CullRenderData* CullUnitObjectStatic::GetRenderData()
    {
        return nullptr; 
    }

    int CullUnitObjectStatic::GetClusterDataCount(int index)
    {
        return 0;
    }

    CullObjectConstantsVector* CullUnitObjectStatic::GetClusterDatas()
    {
        return nullptr; 
    }

    int CullUnitObjectStatic::GetLodCount()
    {
        return 0;
    }

    ComputeBuffer* CullUnitObjectStatic::GetClusterDataCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullUnitObjectStatic::GetLodCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullUnitObjectStatic::GetResultCB()
    {
        return nullptr; 
    }

    ComputeBuffer* CullUnitObjectStatic::GetClipCB() 
    { 
        return nullptr; 
    }

    void CullUnitObjectStatic::UpdateBuffer()
    {

    }

}; //LostPeterVulkan