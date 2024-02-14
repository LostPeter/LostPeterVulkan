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

namespace LostPeterEngine
{
    Material::Material(uint32 nGroup, const String& strName)
        : Base(nGroup, strName)
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
        uint32 nGroup = GetGroup();
        const String& strName = GetName();
        if (!IsGroupNameValid())
		{
            F_LogError("*********************** Material::LoadMaterial: Group, Name is not valid: group: [%u], name: [%s] !", nGroup, strName.c_str());
            return false;
        }
        
        if (!m_pMaterialData)
		{
			m_pMaterialData = MaterialDataManager::GetSingleton().LoadMaterialData(nGroup, strName, bIsFromFile);
            if (m_pMaterialData == nullptr)
            {
                F_LogError("*********************** Material::LoadMaterial: LoadMaterialData failed, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return false;
            }
		}

        return true;
    }
	void Material::UnloadMaterial()
    {
        if (m_pMaterialData)
		{
			MaterialDataManager::GetSingleton().UnloadMaterialData(m_pMaterialData);
		}
        m_pMaterialData = nullptr;
    }

	void Material::init()
    {
        
    }
    
}; //LostPeterEngine