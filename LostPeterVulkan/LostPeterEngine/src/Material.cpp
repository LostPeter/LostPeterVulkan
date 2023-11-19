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

#include "../include/Material.h"
#include "../include/MaterialData.h"
#include "../include/MaterialDataManager.h"

namespace LostPeter
{
    Material::Material(uint32 _group,
                       const String& _nameMaterial)
        : Base(_group, _nameMaterial)
        , m_pMaterialData(nullptr)
    {
        
    }
    Material::~Material()
    {
        Destroy();
    }

    void Material::Destroy()
    {
        UnloadMaterial();
    }

    bool Material::LoadMaterial(bool bIsFromFile /*= true*/)
    {
        if (!IsGroupNameValid())
		{
            F_LogError("*********************** Material::LoadMaterial: Group, Name is not valid: group: [%u, name: [%s] !", this->group, this->name.c_str());
            return false;
        }
        
        if (!m_pMaterialData)
		{
			m_pMaterialData = MaterialDataManager::GetSingleton().CreateMaterialData(this->group, this->name, bIsFromFile);
            if (m_pMaterialData == nullptr)
            {
                F_LogError("*********************** Material::LoadMaterial: CreateMaterialData failed, group: [%u, name: [%s] !", this->group, this->name.c_str());
                return false;
            }
		}

        return true;
    }
	void Material::UnloadMaterial()
    {
        if (m_pMaterialData)
		{
			MaterialDataManager::GetSingleton().DestroyMaterialData(m_pMaterialData);
		}
        m_pMaterialData = nullptr;
    }

	void Material::init()
    {
        
    }
    
}; //LostPeter