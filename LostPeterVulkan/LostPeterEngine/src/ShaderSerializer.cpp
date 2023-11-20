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

#include "../include/ShaderSerializer.h"

namespace LostPeterEngine
{
    #define SHADER_TAG_SHADER_CFG								"cfg_shader"
    #define	SHADER_TAG_SHADER									"shader"

#define SHADER_TAG_ATTRIBUTE_NAME		            "name"
#define SHADER_TAG_ATTRIBUTE_PATH			        "path"
#define SHADER_TAG_ATTRIBUTE_TYPE			        "type"


    ShaderSerializer::ShaderSerializer()
        : Base("ShaderSerializer")
    {
        
    }
    ShaderSerializer::~ShaderSerializer()
    {
        DeleteShaderInfoAll();
    }
    
    bool ShaderSerializer::HasShaderInfo(uint nGroup, const String& strName)
    {
        return GetShaderInfo(nGroup, strName) != nullptr;
    }
    ShaderInfo* ShaderSerializer::GetShaderInfo(uint nGroup, const String& strName)
    {
        ShaderInfoGroupPtrMap::iterator itFind = m_mapShaderInfoGroup.find(nGroup);
        if (itFind == m_mapShaderInfoGroup.end())
            return nullptr;
        ShaderInfoPtrMap::iterator itFindSI = itFind->second.find(strName);
        if (itFindSI == itFind->second.end())
            return nullptr;
        return itFindSI->second;
    }
    bool ShaderSerializer::AddShaderInfo(ShaderInfo* pShaderInfo)
    {
        uint32 nGroup = 0;
        if (!FPathManager::GetSingleton().GetGroup(FPathManager::PathGroup_Shader, pShaderInfo->pathShader, nGroup))
        {   
            F_LogError("*********************** ShaderSerializer::AddShaderInfo: Can not find group by shader path: [%s] !", pShaderInfo->pathShader.c_str());
            F_DELETE(pShaderInfo)
            return false;
        }
        
        ShaderInfoGroupPtrMap::iterator itFind = m_mapShaderInfoGroup.find(nGroup);
        if (itFind == m_mapShaderInfoGroup.end())
        {
            ShaderInfoPtrMap mapSIs;
            m_mapShaderInfoGroup[nGroup] = mapSIs;
            itFind = m_mapShaderInfoGroup.find(nGroup);
        }
        ShaderInfoPtrMap::iterator itFindSI = itFind->second.find(pShaderInfo->nameShader);
        if (itFindSI != itFind->second.end())
        {
            F_LogError("*********************** ShaderSerializer::AddShaderInfo: Shader name already exist: [%s] !", pShaderInfo->nameShader.c_str());
            F_DELETE(pShaderInfo)
            return false;
        }

        pShaderInfo->group = nGroup;
        itFind->second.insert(ShaderInfoPtrMap::value_type(pShaderInfo->nameShader, pShaderInfo));
        m_aShaderInfo.push_back(pShaderInfo);
        return true;
    }
    void ShaderSerializer::DeleteShaderInfo(uint nGroup, const String& strName)
    {
        ShaderInfoGroupPtrMap::iterator itFind = m_mapShaderInfoGroup.find(nGroup);
        if (itFind == m_mapShaderInfoGroup.end())
        {
            return;
        }

        ShaderInfoPtrMap::iterator itFindSI = itFind->second.find(strName);
        if (itFindSI != itFind->second.end())
        {
            ShaderInfoPtrVector::iterator itFindA = std::find(m_aShaderInfo.begin(), m_aShaderInfo.end(), itFindSI->second);
            if (itFindA != m_aShaderInfo.end())
                m_aShaderInfo.erase(itFindA);
            F_DELETE(itFindSI->second)
            itFind->second.erase(itFindSI);
        }
    }
    void ShaderSerializer::DeleteShaderInfoAll()
    {
        for (ShaderInfoGroupPtrMap::iterator it = m_mapShaderInfoGroup.begin();
             it != m_mapShaderInfoGroup.end(); ++it)
        {
            ShaderInfoPtrMap& mapSIs = it->second;
            for (ShaderInfoPtrMap::iterator itMI = mapSIs.begin(); 
                 itMI != mapSIs.end(); ++itMI)
            {
                F_DELETE(itMI->second)
            }
        }
        m_aShaderInfo.clear();
        m_mapShaderInfoGroup.clear();
    }

    //File Content XML
    bool ShaderSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "ShaderSerializer::serializeXML")


        return false;
    }
    bool ShaderSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "ShaderSerializer::deserializeXML")
        
        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);

            //name
            String strNameShader;
            if (!pChild->ParserAttribute_String(SHADER_TAG_ATTRIBUTE_NAME, strNameShader))
            {
                F_LogError("*********************** ShaderSerializer::deserializeXML: Can not find attribute: 'name', shader index: [%d] !", i);
                continue;
            }
            //path
            String strPathShader;
            if (!pChild->ParserAttribute_String(SHADER_TAG_ATTRIBUTE_PATH, strPathShader))
            {
                F_LogError("*********************** ShaderSerializer::deserializeXML: Can not find attribute: 'path', shader index: [%d] !", i);
                continue;
            }
            //type
            String strType;
            if (!pChild->ParserAttribute_String(SHADER_TAG_ATTRIBUTE_TYPE, strType))
            {
                F_LogError("*********************** ShaderSerializer::deserializeXML: Can not find attribute: 'type', shader index: [%d] !", i);
                continue;
            }
            FShaderType typeShader = F_ParseShaderType(strType);

            ShaderInfo* pShaderInfo = new ShaderInfo(strNameShader,
                                                     strPathShader,
                                                     typeShader);
            if (AddShaderInfo(pShaderInfo))
            {
                F_LogInfo("ShaderSerializer::deserializeXML: Add shader info success, [%s]-[%s]-[%s] !",
                          strNameShader.c_str(), 
                          strPathShader.c_str(), 
                          strType.c_str());
            }
        }

        return true;
    }

    //File Content Binary
    bool ShaderSerializer::serializeBinary()
    {
        return false;
    }
    bool ShaderSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeterEngine