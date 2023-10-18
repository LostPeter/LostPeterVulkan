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

#include "../include/MaterialManager.h"
#include "../include/VulkanWindow.h"
#include "../include/MaterialSerializer.h"
#include "../include/Material.h"
#include "../include/MaterialInstance.h"

template<> LostPeter::MaterialManager* LostPeterFoundation::FSingleton<LostPeter::MaterialManager>::ms_Singleton = nullptr;

namespace LostPeter
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

    uint32 MaterialManager::ms_nInstanceID = 0;
	String MaterialManager::ms_strMaterialName_Default = "Assets/Material/Default.material";
    String MaterialManager::ms_strMaterialName_DefaultOpaque = "Assets/Material/Default_Opaque.material";
    String MaterialManager::ms_strMaterialName_DefaultTransparent = "Assets/Material/Default_Transparent.material";
    MaterialManager::MaterialManager()
        : Base("MaterialManager")
        , m_pMaterialSerializer(nullptr)
        , m_pMaterial_Default(nullptr)
        , m_pMaterialInstance_Default(nullptr)
        , m_pMaterial_DefaultOpaque(nullptr)
        , m_pMaterialInstance_DefaultOpaque(nullptr)
        , m_pMaterial_DefaultTransparent(nullptr)
        , m_pMaterialInstance_DefaultTransparent(nullptr)
    {
        m_pMaterialSerializer = new MaterialSerializer();
    }
    MaterialManager::~MaterialManager()
    {
        F_DELETE(m_pMaterialSerializer)
        Destroy();
    }

    void MaterialManager::Destroy()
    {
        DeleteMaterialAll();
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
        if (!initMaterialDefault())
        {
            F_LogError("*********************** MaterialManager::Init: initMaterialDefault failed !");
            return false;
        }

        return true;
    }
    bool MaterialManager::initMaterialDefault()
    {
        //1> Default

        //2> Default_Opaque

        //3> Default_Transparent

        return true;
    }

    bool MaterialManager::LoadMaterialAll()
    {
        if (m_pMaterialSerializer == nullptr)
            return false;

        DeleteMaterialAll();
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
                                           pMI->nameMaterial,
                                           pMI->pathMaterial);
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
            F_LogError("*********************** MaterialManager::GetMaterial: Can not find group: [%u], name: [%s]", nGroup, strName.c_str());
            return nullptr;
        }

        MaterialPtrMap::iterator itFindMaterial = itFindGroup->second.find(strName);
        if (itFindMaterial == itFindGroup->second.end())
        {
            F_LogError("*********************** MaterialManager::GetMaterial: Can not find material from group: [%u], name: [%s]", nGroup, strName.c_str());
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
    
}; //LostPeter