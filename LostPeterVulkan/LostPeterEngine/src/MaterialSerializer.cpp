/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MaterialSerializer.h"

namespace LostPeter
{
    #define MATERIAL_TAG_MATERIAL_CFG								"cfg_material"
    #define	MATERIAL_TAG_MATERIAL									"material"

#define MATERIAL_TAG_ATTRIBUTE_NAME		            "name"
#define MATERIAL_TAG_ATTRIBUTE_PATH			        "path"


    MaterialSerializer::MaterialSerializer()
        : Base("MaterialSerializer")
    {
        
    }
    MaterialSerializer::~MaterialSerializer()
    {
        DeleteMaterialInfoAll();
    }
    
    bool MaterialSerializer::HasMaterialInfo(uint nGroup, const String& strName)
    {
        return GetMaterialInfo(nGroup, strName) != nullptr;
    }
    MaterialInfo* MaterialSerializer::GetMaterialInfo(uint nGroup, const String& strName)
    {
        MaterialInfoGroupPtrMap::iterator itFind = m_mapMaterialInfoGroup.find(nGroup);
        if (itFind == m_mapMaterialInfoGroup.end())
            return nullptr;
        MaterialInfoPtrMap::iterator itFindMI = itFind->second.find(strName);
        if (itFindMI == itFind->second.end())
            return nullptr;
        return itFindMI->second;
    }
    bool MaterialSerializer::AddMaterialInfo(MaterialInfo* pMaterialInfo)
    {
        uint32 nGroup = 0;
        if (!FPathManager::GetSingleton().GetGroup(FPathManager::PathGroup_Material, pMaterialInfo->pathMaterial, nGroup))
        {   
            F_LogError("*********************** MaterialSerializer::AddMaterialInfo: Can not find group by material path: [%s] !", pMaterialInfo->pathMaterial.c_str());
            F_DELETE(pMaterialInfo)
            return false;
        }
        
        MaterialInfoGroupPtrMap::iterator itFind = m_mapMaterialInfoGroup.find(nGroup);
        if (itFind == m_mapMaterialInfoGroup.end())
        {
            MaterialInfoPtrMap mapMIs;
            m_mapMaterialInfoGroup[nGroup] = mapMIs;
            itFind = m_mapMaterialInfoGroup.find(nGroup);
        }
        MaterialInfoPtrMap::iterator itFindMI = itFind->second.find(pMaterialInfo->nameMaterial);
        if (itFindMI != itFind->second.end())
        {
            F_LogError("*********************** MaterialSerializer::AddMaterialInfo: Material name already exist: [%s] !", pMaterialInfo->nameMaterial.c_str());
            F_DELETE(pMaterialInfo)
            return false;
        }

        pMaterialInfo->group = nGroup;
        itFind->second.insert(MaterialInfoPtrMap::value_type(pMaterialInfo->nameMaterial, pMaterialInfo));
        m_aMaterialInfo.push_back(pMaterialInfo);
        return true;
    }
    void MaterialSerializer::DeleteMaterialInfo(uint nGroup, const String& strName)
    {
        MaterialInfoGroupPtrMap::iterator itFind = m_mapMaterialInfoGroup.find(nGroup);
        if (itFind == m_mapMaterialInfoGroup.end())
        {
            return;
        }

        MaterialInfoPtrMap::iterator itFindMI = itFind->second.find(strName);
        if (itFindMI != itFind->second.end())
        {
            MaterialInfoPtrVector::iterator itFindA = std::find(m_aMaterialInfo.begin(), m_aMaterialInfo.end(), itFindMI->second);
            if (itFindA != m_aMaterialInfo.end())
                m_aMaterialInfo.erase(itFindA);
            F_DELETE(itFindMI->second)
            itFind->second.erase(itFindMI);
        }
    }
    void MaterialSerializer::DeleteMaterialInfoAll()
    {
        for (MaterialInfoGroupPtrMap::iterator it = m_mapMaterialInfoGroup.begin();
             it != m_mapMaterialInfoGroup.end(); ++it)
        {
            MaterialInfoPtrMap& mapMIs = it->second;
            for (MaterialInfoPtrMap::iterator itMI = mapMIs.begin(); 
                 itMI != mapMIs.end(); ++itMI)
            {
                F_DELETE(itMI->second)
            }
        }
        m_aMaterialInfo.clear();
        m_mapMaterialInfoGroup.clear();
    }

    //File Content XML
    bool MaterialSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "MaterialSerializer::serializeXML")


        return false;
    }
    bool MaterialSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "MaterialSerializer::deserializeXML")
        
        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);

            //name
            String strNameMaterial;
            if (!pChild->ParserAttribute_String(MATERIAL_TAG_ATTRIBUTE_NAME, strNameMaterial))
            {
                F_LogError("*********************** MaterialSerializer::deserializeXML: Can not find attribute: 'name', material index: [%d] !", i);
                continue;
            }
            //path
            String strPathMaterial;
            if (!pChild->ParserAttribute_String(MATERIAL_TAG_ATTRIBUTE_PATH, strPathMaterial))
            {
                F_LogError("*********************** MaterialSerializer::deserializeXML: Can not find attribute: 'path', material index: [%d] !", i);
                continue;
            }

            MaterialInfo* pMaterialInfo = new MaterialInfo(strNameMaterial,
                                                           strPathMaterial);
            if (AddMaterialInfo(pMaterialInfo))
            {
                F_LogInfo("MaterialSerializer::deserializeXML: Add material info success, [%u]-[%s]-[%s] !",
                          pMaterialInfo->group,
                          strNameMaterial.c_str(), 
                          strPathMaterial.c_str());
            }
        }

        return true;
    }

    //File Content Binary
    bool MaterialSerializer::serializeBinary()
    {
        return false;
    }
    bool MaterialSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter