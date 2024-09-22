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

#include "../include/CullTerrainStatic.h"
#include "../include/VulkanWindow.h"

template<> LostPeterVulkan::CullTerrainStatic* LostPeterFoundation::FSingleton<LostPeterVulkan::CullTerrainStatic>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
    ////////////////////// CullUnitTerrainStatic /////////////////////
    CullTerrainStatic::CullUnitTerrainStatic::CullUnitTerrainStatic(const String& nameUnit, CullTerrainStatic* pCullTS)
        : CullUnitTerrain(nameUnit)
        , pCullTerrainStatic(pCullTS)
    {
        F_Assert(pCullTerrainStatic != nullptr && "CullUnitTerrainStatic::CullUnitTerrainStatic")
    }

    CullTerrainStatic::CullUnitTerrainStatic::~CullUnitTerrainStatic()
    {
        Destroy();
    }

    void CullTerrainStatic::CullUnitTerrainStatic::Destroy()
    {
        
    }

    void CullTerrainStatic::CullUnitTerrainStatic::Init()
    {

    }

    bool CullTerrainStatic::CullUnitTerrainStatic::IsCulling()
    {
        return true;
    }

    int CullTerrainStatic::CullUnitTerrainStatic::GetRenderCount()
    {
        return 0;
    }

    ComputeBuffer* CullTerrainStatic::CullUnitTerrainStatic::GetRenderArgsCB()
    {
        return nullptr; 
    }
    

    ////////////////////// CullTerrainStatic /////////////////////////
    CullTerrainStatic* CullTerrainStatic::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	CullTerrainStatic& CullTerrainStatic::GetSingleton()
	{  
		F_Assert(ms_Singleton && "CullTerrainStatic::GetSingleton")
		return (*ms_Singleton);     
	}

    const String CullTerrainStatic::s_nameCullTerrainStatic = "CullTerrainStatic";
    const String CullTerrainStatic::s_nameCullUnitTerrainStatic = "CullUnitTerrainStatic";


    CullTerrainStatic::CullTerrainStatic()
        : Base(s_nameCullTerrainStatic)
    {
        pCullUnitTerrainStatic = new CullUnitTerrainStatic(s_nameCullUnitTerrainStatic, this);
    }
    CullTerrainStatic::~CullTerrainStatic()
    {
        Destroy();
    }

    void CullTerrainStatic::Destroy()
    {
        F_DELETE(pCullUnitTerrainStatic)
    }

    void CullTerrainStatic::Init()
    {
        
    }
    
}; //LostPeterVulkan