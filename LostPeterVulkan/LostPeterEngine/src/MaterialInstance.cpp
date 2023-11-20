/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MaterialInstance.h"
#include "../include/Material.h"
#include "../include/MaterialManager.h"

namespace LostPeterEngine
{
    MaterialInstance::MaterialInstance(const String& nameMaterialInstance, Material* pMaterial)
        : Base(nameMaterialInstance)
        , m_pMaterial(pMaterial)
    {
        
    }
    MaterialInstance::~MaterialInstance()
    {
        Destroy();
    }
    void MaterialInstance::Destroy()
    {
        UnloadMaterialInstance();
    }

    bool MaterialInstance::LoadMaterialInstance()
    {


        return true;
    }
	void MaterialInstance::UnloadMaterialInstance()
    {
        
    }

}; //LostPeterEngine