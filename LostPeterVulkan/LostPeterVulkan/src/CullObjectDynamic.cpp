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
#include "../include/CullUnitObjectDynamic.h"

template<> LostPeterVulkan::CullObjectDynamic* LostPeterFoundation::FSingleton<LostPeterVulkan::CullObjectDynamic>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
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