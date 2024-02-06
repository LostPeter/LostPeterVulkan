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

#include "../include/ShaderProgramSerializer.h"

namespace LostPeterEngine
{
    #define SHADER_TAG_SHADER_PROGRAM_CFG						"cfg_shader_program"
    #define	SHADER_TAG_SHADER_PROGRAM								"shader_program"

#define SHADER_TAG_ATTRIBUTE_NAME		            "name"
#define SHADER_TAG_ATTRIBUTE_PATH			        "path"
#define SHADER_TAG_ATTRIBUTE_TYPE			        "type"


    ShaderProgramSerializer::ShaderProgramSerializer()
        : Base("ShaderProgramSerializer")
    {
        
    }
    ShaderProgramSerializer::~ShaderProgramSerializer()
    {
        DeleteShaderProgramInfoAll();
    }
    
    bool ShaderProgramSerializer::HasShaderProgramInfo(uint nGroup, const String& strName)
    {
        return GetShaderProgramInfo(nGroup, strName) != nullptr;
    }
    ShaderProgramInfo* ShaderProgramSerializer::GetShaderProgramInfo(uint nGroup, const String& strName)
    {
        ShaderProgramInfoGroupPtrMap::iterator itFind = m_mapShaderProgramInfoGroup.find(nGroup);
        if (itFind == m_mapShaderProgramInfoGroup.end())
            return nullptr;
        ShaderProgramInfoPtrMap::iterator itFindSPI = itFind->second.find(strName);
        if (itFindSPI == itFind->second.end())
            return nullptr;
        return itFindSPI->second;
    }
    bool ShaderProgramSerializer::AddShaderProgramInfo(ShaderProgramInfo* pShaderProgramInfo)
    {
        uint32 nGroup = 0;
        if (!FPathManager::GetSingleton().GetGroup(FPathManager::PathGroup_Shader, pShaderProgramInfo->pathShaderProgram, nGroup))
        {   
            F_LogError("*********************** ShaderProgramSerializer::AddShaderProgramInfo: Can not find group by shader path: [%s] !", pShaderProgramInfo->pathShaderProgram.c_str());
            F_DELETE(pShaderProgramInfo)
            return false;
        }
        
        ShaderProgramInfoGroupPtrMap::iterator itFind = m_mapShaderProgramInfoGroup.find(nGroup);
        if (itFind == m_mapShaderProgramInfoGroup.end())
        {
            ShaderProgramInfoPtrMap mapSPIs;
            m_mapShaderProgramInfoGroup[nGroup] = mapSPIs;
            itFind = m_mapShaderProgramInfoGroup.find(nGroup);
        }
        ShaderProgramInfoPtrMap::iterator itFindSPI = itFind->second.find(pShaderProgramInfo->nameShaderProgram);
        if (itFindSPI != itFind->second.end())
        {
            F_LogError("*********************** ShaderProgramSerializer::AddShaderProgramInfo: Shader program name already exist: [%s] !", pShaderProgramInfo->nameShaderProgram.c_str());
            F_DELETE(pShaderProgramInfo)
            return false;
        }

        pShaderProgramInfo->group = nGroup;
        itFind->second.insert(ShaderProgramInfoPtrMap::value_type(pShaderProgramInfo->nameShaderProgram, pShaderProgramInfo));
        m_aShaderProgramInfo.push_back(pShaderProgramInfo);
        return true;
    }
    void ShaderProgramSerializer::DeleteShaderProgramInfo(uint nGroup, const String& strName)
    {
        ShaderProgramInfoGroupPtrMap::iterator itFind = m_mapShaderProgramInfoGroup.find(nGroup);
        if (itFind == m_mapShaderProgramInfoGroup.end())
        {
            return;
        }

        ShaderProgramInfoPtrMap::iterator itFindSPI = itFind->second.find(strName);
        if (itFindSPI != itFind->second.end())
        {
            ShaderProgramInfoPtrVector::iterator itFindA = std::find(m_aShaderProgramInfo.begin(), m_aShaderProgramInfo.end(), itFindSPI->second);
            if (itFindA != m_aShaderProgramInfo.end())
                m_aShaderProgramInfo.erase(itFindA);
            F_DELETE(itFindSPI->second)
            itFind->second.erase(itFindSPI);
        }
    }
    void ShaderProgramSerializer::DeleteShaderProgramInfoAll()
    {
        for (ShaderProgramInfoGroupPtrMap::iterator it = m_mapShaderProgramInfoGroup.begin();
             it != m_mapShaderProgramInfoGroup.end(); ++it)
        {
            ShaderProgramInfoPtrMap& mapSPIs = it->second;
            for (ShaderProgramInfoPtrMap::iterator itSPI = mapSPIs.begin(); 
                 itSPI != mapSPIs.end(); ++itSPI)
            {
                F_DELETE(itSPI->second)
            }
        }
        m_aShaderProgramInfo.clear();
        m_mapShaderProgramInfoGroup.clear();
    }

    //File Content XML
    bool ShaderProgramSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "ShaderProgramSerializer::serializeXML")


        return false;
    }
    bool ShaderProgramSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "ShaderProgramSerializer::deserializeXML")
        
        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);

            //name
            String strNameShaderProgram;
            if (!pChild->ParserAttribute_String(SHADER_TAG_ATTRIBUTE_NAME, strNameShaderProgram))
            {
                F_LogError("*********************** ShaderProgramSerializer::deserializeXML: Can not find attribute: 'name', shader program index: [%d] !", i);
                continue;
            }
            //path
            String strPathShaderProgram;
            if (!pChild->ParserAttribute_String(SHADER_TAG_ATTRIBUTE_PATH, strPathShaderProgram))
            {
                F_LogError("*********************** ShaderProgramSerializer::deserializeXML: Can not find attribute: 'path', shader program index: [%d] !", i);
                continue;
            }
            //type
            String strType;
            if (!pChild->ParserAttribute_String(SHADER_TAG_ATTRIBUTE_TYPE, strType))
            {
                F_LogError("*********************** ShaderProgramSerializer::deserializeXML: Can not find attribute: 'type', shader program index: [%d] !", i);
                continue;
            }
            FShaderType typeShader = F_ParseShaderType(strType);

            ShaderProgramInfo* pShaderProgramInfo = new ShaderProgramInfo(strNameShaderProgram,
                                                                          strPathShaderProgram,
                                                                          typeShader);
            if (AddShaderProgramInfo(pShaderProgramInfo))
            {
                F_LogInfo("ShaderProgramSerializer::deserializeXML: Add shader program info success, [%s]-[%s]-[%s] !",
                          strNameShaderProgram.c_str(), 
                          strPathShaderProgram.c_str(), 
                          strType.c_str());
            }
        }

        return true;
    }

    //File Content Binary
    bool ShaderProgramSerializer::serializeBinary()
    {
        return false;
    }
    bool ShaderProgramSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeterEngine