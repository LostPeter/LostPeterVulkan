/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Material.h"
#include "../include/VulkanWindow.h"
#include "../include/MaterialData.h"

namespace LostPeter
{
    Material::Material(uint32 _group,
                       const String& _nameMaterial,
                       const String& _pathMaterial)
        : Base(_group, _nameMaterial)
        , pathMaterial(_pathMaterial)
        , pMaterialData(nullptr)
    {
        
    }
    Material::~Material()
    {
        Destroy();
    }

    void Material::Destroy()
    {

    }

    bool Material::LoadMaterial(bool bNew /*= false*/)
    {
        
        return true;
    }
	bool Material::UnloadMaterial()
    {
        return true;
    }

	void Material::init()
    {
        
    }
    
}; //LostPeter