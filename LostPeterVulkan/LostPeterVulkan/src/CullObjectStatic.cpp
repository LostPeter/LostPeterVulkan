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
#include "../include/CullUnitObjectStatic.h"

template<> LostPeterVulkan::CullObjectStatic* LostPeterFoundation::FSingleton<LostPeterVulkan::CullObjectStatic>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
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