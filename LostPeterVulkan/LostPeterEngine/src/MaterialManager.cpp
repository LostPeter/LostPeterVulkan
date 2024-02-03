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

#include "../include/MaterialManager.h"
#include "../include/MaterialSerializer.h"
#include "../include/Material.h"
#include "../include/MaterialInstance.h"

template<> LostPeterEngine::MaterialManager* LostPeterFoundation::FSingleton<LostPeterEngine::MaterialManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    MaterialManager* MaterialManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	MaterialManager& MaterialManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "MaterialManager::GetSingleton")
		return (*ms_Singleton);     
	}

    const String MaterialManager::ms_strMaterialConfigName = "Cfg_Material.xml";
    uint32 MaterialManager::ms_nInstanceID = 0;

    static const int s_nCountDefaults = 3;
    static String s_aNameDefaults[s_nCountDefaults] = 
    {
        "Default",
        "Default_Opaque",
        "Default_Transparent",
    };
    const String& MaterialManager::GetMaterialName_Default()
    {
        return s_aNameDefaults[0];
    }
    const String& MaterialManager::GetMaterialName_DefaultOpaque()
    {
        return s_aNameDefaults[1];
    }
    const String& MaterialManager::GetMaterialName_DefaultTransparent()
    {
        return s_aNameDefaults[2];
    }   


    MaterialManager::MaterialManager()
        : Base("MaterialManager")
        , m_pMaterialSerializer(nullptr)
    {
        m_pMaterialSerializer = new MaterialSerializer();
    }
    MaterialManager::~MaterialManager()
    {
        F_DELETE(m_pMaterialSerializer)
        Destroy();
    }

    bool MaterialManager::IsMaterialDefault(Material* pMaterial)
    {
        for (MaterialPtrMap::iterator it = m_mapMaterialDefaults.begin(); 
             it != m_mapMaterialDefaults.end(); ++it)
        {
            if (it->second == pMaterial)
                return true;
        }
        return false;
    }
    Material* MaterialManager::GetMaterialDefault(const String& strName)
    {
        MaterialPtrMap::iterator itFind = m_mapMaterialDefaults.find(strName);
        if (itFind == m_mapMaterialDefaults.end())
            return nullptr;
        return itFind->second;
    }
    Material* MaterialManager::GetMaterial_Default()
    {
        return GetMaterialDefault(s_aNameDefaults[0]);
    }
    Material* MaterialManager::GetMaterial_DefaultOpaque()
    {
        return GetMaterialDefault(s_aNameDefaults[1]);
    }
    Material* MaterialManager::GetMaterial_DefaultTransparent()
    {
        return GetMaterialDefault(s_aNameDefaults[2]);
    }

    bool MaterialManager::IsMaterialInstanceDefault(MaterialInstance* pMaterialInstance)
    {
        for (MaterialInstancePtrMap::iterator it = m_mapMaterialInstanceDefaults.begin(); 
            it != m_mapMaterialInstanceDefaults.end(); ++it)
        {
            if (it->second == pMaterialInstance)
                return true;
        }
        return false;
    }
    MaterialInstance* MaterialManager::GetMaterialInstanceDefault(const String& strName)
    {
        MaterialInstancePtrMap::iterator itFind = m_mapMaterialInstanceDefaults.find(strName);
        if (itFind == m_mapMaterialInstanceDefaults.end())
            return nullptr;
        return itFind->second;
    }
    MaterialInstance* MaterialManager::GetMaterialInstance_Default()
    {
        return GetMaterialInstanceDefault(s_aNameDefaults[0]);
    }
    MaterialInstance* MaterialManager::GetMaterialInstance_DefaultOpaque()
    {
        return GetMaterialInstanceDefault(s_aNameDefaults[1]);
    }
    MaterialInstance* MaterialManager::GetMaterialInstance_DefaultTransparent()
    {
        return GetMaterialInstanceDefault(s_aNameDefaults[2]);
    }

    void MaterialManager::Destroy()
    {
        DestroyMaterialInstanceAll();
        DeleteMaterialAll();
        
        destroyMaterialInstanceDefaults();
        destroyMaterialDefaults();
    }
    bool MaterialManager::Init(uint nGroup, const String& strNameCfg)
    {
        Destroy();

        //1> Material Cfg Path 
        String strPathCfgMaterial = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgMaterial.empty())
        {
            F_LogError("*********************** MaterialManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> Material Serializer
        if (!m_pMaterialSerializer->LoadFile(strPathCfgMaterial))
        {
            F_LogError("*********************** MaterialManager::Init: Load file material cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }
        
        //3> Material Default
        if (!initMaterialDefaults())
        {
            F_LogError("*********************** MaterialManager::Init: initMaterialDefaults failed !");
            return false;
        }

        //4> Material Instance Default
        if (!initMaterialInstanceDefaults())
        {
            F_LogError("*********************** MaterialManager::Init: initMaterialInstanceDefaults failed !");
            return false;
        }

        return true;
    }
    bool MaterialManager::initMaterialDefaults()
    {
        int count = s_nCountDefaults;
        for (int i = 0; i < count; i++)
        {
            String& nameMaterial = s_aNameDefaults[i];
            Material* pMaterial = loadMaterial(FPathManager::PathGroup_Material, nameMaterial, true);
            if (pMaterial == nullptr)
            {
                F_LogError("*********************** MaterialManager::initMaterialDefaults: Load default material: [%s] failed !", nameMaterial.c_str());
                return false;
            }
            m_mapMaterialDefaults[nameMaterial] = pMaterial;
            F_LogInfo("MaterialManager::initMaterialDefaults: Load default material: [%s] success !", nameMaterial.c_str());
        }
        return true;
    }
    Material* MaterialManager::loadMaterial(uint nGroup, const String& strName, bool bIsFromFile /*= true*/)
    {   
        Material* pMaterial = new Material(nGroup, strName);
        if (!pMaterial->LoadMaterial(bIsFromFile))
        {
            F_LogError("*********************** MaterialManager::loadMaterial: Load material failed, group: [%u] name: [%s] !", nGroup, strName.c_str());
            F_DELETE(pMaterial)
            return nullptr;
        }
        return pMaterial;
    }
    void MaterialManager::destroyMaterialDefaults()
    {
        for (MaterialPtrMap::iterator it = m_mapMaterialDefaults.begin(); 
            it != m_mapMaterialDefaults.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_mapMaterialDefaults.clear();
    }

    bool MaterialManager::initMaterialInstanceDefaults()
    {
        for (MaterialPtrMap::iterator it = m_mapMaterialDefaults.begin(); 
             it != m_mapMaterialDefaults.end(); ++it)
        {
            const String& nameMaterial = it->first;
            Material* pMaterial = it->second;
            MaterialInstance* pMaterialInstance = CreateMaterialInstance(pMaterial, false);     
            if (pMaterialInstance == nullptr)
            {
                F_LogError("*********************** MaterialManager::initMaterialInstanceDefaults: Create material instance from material: [%s] failed !", nameMaterial.c_str());
                return false;
            }
            m_mapMaterialInstanceDefaults[nameMaterial] = pMaterialInstance;
            F_LogInfo("MaterialManager::initMaterialInstanceDefaults: Create material instance from material: [%s] success !", nameMaterial.c_str());
        }
        return true;
    }
    void MaterialManager::destroyMaterialInstanceDefaults()
    {
        for (MaterialInstancePtrMap::iterator it = m_mapMaterialInstanceDefaults.begin(); 
            it != m_mapMaterialInstanceDefaults.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_mapMaterialInstanceDefaults.clear();
    }
    

    bool MaterialManager::LoadMaterialAll()
    {
        if (m_pMaterialSerializer == nullptr)
            return false;

        MaterialInfoPtrVector& aMaterialInfos = m_pMaterialSerializer->GetMaterialInfoPtrVector();
        for (MaterialInfoPtrVector::iterator it = aMaterialInfos.begin();
             it != aMaterialInfos.end(); ++it)
        {
            if (!loadMaterial(*it))
                continue;
        }

        return true;
    }
    Material* MaterialManager::LoadMaterial(uint nGroup, const String& strName)
    {
        if (m_pMaterialSerializer == nullptr)
            return nullptr;

        Material* pMaterial = GetMaterial(nGroup, strName);
        if (pMaterial == nullptr)
        {
            MaterialInfo* pMaterialInfo = m_pMaterialSerializer->GetMaterialInfo(nGroup, strName);
            if (pMaterialInfo == nullptr)
            {
                F_LogError("*********************** MaterialManager::LoadMaterial: Can not find material info, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return nullptr;
            }
            if (!loadMaterial(pMaterialInfo))
            {
                return nullptr;
            }
        }
        return pMaterial;
    }
    Material* MaterialManager::loadMaterial(MaterialInfo* pMI)
    {
        Material* pMaterial = new Material(pMI->group,
                                           pMI->nameMaterial);
        if (!pMaterial->LoadMaterial(true))
        {
            F_LogError("*********************** MaterialManager::loadMaterial: Load material failed, name: [%s], path: [%s] !", pMI->nameMaterial.c_str(), pMI->pathMaterial.c_str());
            F_DELETE(pMaterial)
            return nullptr;
        }

        if (AddMaterial(pMI->group, pMaterial))
        {
            F_LogInfo("MaterialManager::loadMaterial: Load material success, [%u]-[%s]-[%s] !", 
                      pMI->group, 
                      pMI->nameMaterial.c_str(), 
                      pMI->pathMaterial.c_str());
        }
        return pMaterial;
    }

    bool MaterialManager::HasMaterial(uint nGroup, const String& strName)
    {
        return GetMaterial(nGroup, strName) != nullptr;
    }

    Material* MaterialManager::GetMaterial(uint nGroup, const String& strName)
    {
        MaterialGroupPtrMap::iterator itFindGroup = m_mapMaterialGroup.find(nGroup);
        if (itFindGroup == m_mapMaterialGroup.end())
        {
            return nullptr;
        }

        MaterialPtrMap::iterator itFindMaterial = itFindGroup->second.find(strName);
        if (itFindMaterial == itFindGroup->second.end())
        {
            return nullptr;
        }
        return itFindMaterial->second;
    }

    bool MaterialManager::AddMaterial(uint nGroup, Material* pMaterial)
    {
        MaterialGroupPtrMap::iterator itFind = m_mapMaterialGroup.find(nGroup);
        if (itFind == m_mapMaterialGroup.end())
        {
            MaterialPtrMap mapMaterial;
            m_mapMaterialGroup[nGroup] = mapMaterial;
            itFind = m_mapMaterialGroup.find(nGroup);
        }
        const String& strName = pMaterial->GetName();
        MaterialPtrMap::iterator itFindMaterial = itFind->second.find(strName);
        if (itFindMaterial != itFind->second.end())
        {
            F_LogError("*********************** MaterialManager::AddMaterial: Material name already exist: [%s] !", strName.c_str());
            F_DELETE(pMaterial)
            return false;
        }

        itFind->second.insert(MaterialPtrMap::value_type(strName, pMaterial));
        m_aMaterial.push_back(pMaterial);
        return true;
    }

    void MaterialManager::DeleteMaterial(uint nGroup, const String& strName)
    {
        MaterialGroupPtrMap::iterator itFind = m_mapMaterialGroup.find(nGroup);
        if (itFind == m_mapMaterialGroup.end())
        {
            return;
        }

        MaterialPtrMap::iterator itFindMaterial = itFind->second.find(strName);
        if (itFindMaterial != itFind->second.end())
        {
            MaterialPtrVector::iterator itFindA = std::find(m_aMaterial.begin(), m_aMaterial.end(), itFindMaterial->second);
            if (itFindA != m_aMaterial.end())
                m_aMaterial.erase(itFindA);
            F_DELETE(itFindMaterial->second)
            itFind->second.erase(itFindMaterial);
        }
    }

    void MaterialManager::DeleteMaterialAll()
    {
        for (MaterialGroupPtrMap::iterator it = m_mapMaterialGroup.begin();
             it != m_mapMaterialGroup.end(); ++it)
        {
            MaterialPtrMap& mapMaterial = it->second;
            for (MaterialPtrMap::iterator itMaterial = mapMaterial.begin(); 
                 itMaterial != mapMaterial.end(); ++itMaterial)
            {
                F_DELETE(itMaterial->second)
            }
        }
        m_aMaterial.clear();
        m_mapMaterialGroup.clear();
    }
    
    MaterialInstance* MaterialManager::CreateMaterialInstance_Default(bool bIsUnique)
    {
        Material* pMaterial = GetMaterial_Default();
        F_Assert(pMaterial && "MaterialManager::CreateMaterialInstance_Default")
		return CreateMaterialInstance(pMaterial, bIsUnique);
    }
    MaterialInstance* MaterialManager::CreateMaterialInstance_DefaultOpaque(bool bIsUnique)
    {
        Material* pMaterial = GetMaterial_DefaultOpaque();
        F_Assert(pMaterial && "MaterialManager::CreateMaterialInstance_DefaultOpaque")
		return CreateMaterialInstance(pMaterial, bIsUnique);
    }
    MaterialInstance* MaterialManager::CreateMaterialInstance_DefaultTransparent(bool bIsUnique)
    {
        Material* pMaterial = GetMaterial_DefaultTransparent();
        F_Assert(pMaterial && "MaterialManager::CreateMaterialInstance_DefaultTransparent")
		return CreateMaterialInstance(pMaterial, bIsUnique);
    }

    MaterialInstance* MaterialManager::CreateMaterialInstance(Material* pMaterial, bool bIsUnique)
    {
        String strName = pMaterial->GetName();
        if (bIsUnique)
        {
            strName += "_" + FUtilString::SaveUInt(GetNextInstanceID());
        }
		MaterialInstance* pMaterialInstance = new MaterialInstance(strName, pMaterial);
		if (!pMaterialInstance->LoadMaterialInstance())
        {
            F_DELETE(pMaterialInstance)
            F_LogError("*********************** MaterialManager::CreateMaterialInstance: Create material instance from material failed, group: [%u], name: [%s] !", pMaterial->GetGroup(), pMaterial->GetName().c_str());
            return nullptr;
        }
        if (bIsUnique)
        {
            m_mapMaterialInstance[strName] = pMaterialInstance;
        }
		return pMaterialInstance;
    }
    MaterialInstance* MaterialManager::CreateMaterialInstance(uint32 nGroup, const String& strMaterialName, bool bIsUnique)
    {
        Material* pMaterial = GetMaterial(nGroup, strMaterialName);
		if (pMaterial == nullptr)
		{
			pMaterial = LoadMaterial(nGroup, strMaterialName);
			if (pMaterial == nullptr)
			{
				F_LogError("*********************** MaterialManager::CreateMaterialInstance: Create material failed, group: [%u], name: [%s] !", nGroup, strMaterialName.c_str());
				return nullptr;
			}
		}
        return CreateMaterialInstance(pMaterial, bIsUnique);
    }
    void MaterialManager::DestroyMaterialInstance(MaterialInstance* pMaterialInstance)
    {
        if (IsMaterialInstanceDefault(pMaterialInstance))
			return;

		MaterialInstancePtrMap::iterator itFind = m_mapMaterialInstance.find(pMaterialInstance->GetName());
		if (itFind == m_mapMaterialInstance.end())
			return;
        F_DELETE(itFind->second)
		m_mapMaterialInstance.erase(itFind);
    }
    void MaterialManager::DestroyMaterialInstanceAll()
    {
        for (MaterialInstancePtrMap::iterator it = m_mapMaterialInstance.begin();
			 it != m_mapMaterialInstance.end(); ++it)
		{
            F_DELETE(it->second)
		}
		m_mapMaterialInstance.clear();
    }

}; //LostPeterEngine