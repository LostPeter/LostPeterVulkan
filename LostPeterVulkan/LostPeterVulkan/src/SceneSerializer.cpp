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

#include "../include/SceneSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    #define SCENE_TAG_SCENE_CFG								"cfg_scene"
    #define	SCENE_TAG_SCENE									"scene"

#define SCENE_TAG_ATTRIBUTE_NAME		            "name"
#define SCENE_TAG_ATTRIBUTE_PATH			        "path"


    SceneSerializer::SceneSerializer()
        : Base("SceneSerializer")
    {
        
    }
    SceneSerializer::~SceneSerializer()
    {
        DeleteSceneInfoAll();
    }

    bool SceneSerializer::HasSceneInfo(uint nGroup, const String& strName)
    {
        return GetSceneInfo(nGroup, strName) != nullptr;
    }
    SceneInfo* SceneSerializer::GetSceneInfo(uint nGroup, const String& strName)
    {
        SceneInfoGroupPtrMap::iterator itFind = m_mapSceneInfoGroup.find(nGroup);
        if (itFind == m_mapSceneInfoGroup.end())
            return nullptr;
        SceneInfoPtrMap::iterator itFindSI = itFind->second.find(strName);
        if (itFindSI == itFind->second.end())
            return nullptr;
        return itFindSI->second;
    }
    bool SceneSerializer::AddSceneInfo(SceneInfo* pSceneInfo)
    {
        uint32 nGroup = 0;
        if (!FPathManager::GetSingleton().GetGroup(FPathManager::PathGroup_Scene, pSceneInfo->pathScene, nGroup))
        {   
            F_LogError("*********************** SceneSerializer::AddSceneInfo: Can not find group by scene path: [%s] !", pSceneInfo->pathScene.c_str());
            F_DELETE(pSceneInfo)
            return false;
        }
        
        SceneInfoGroupPtrMap::iterator itFind = m_mapSceneInfoGroup.find(nGroup);
        if (itFind == m_mapSceneInfoGroup.end())
        {
            SceneInfoPtrMap mapSIs;
            m_mapSceneInfoGroup[nGroup] = mapSIs;
            itFind = m_mapSceneInfoGroup.find(nGroup);
        }
        SceneInfoPtrMap::iterator itFindSI = itFind->second.find(pSceneInfo->nameScene);
        if (itFindSI != itFind->second.end())
        {
            F_LogError("*********************** SceneSerializer::AddSceneInfo: Scene name already exist: [%s] !", pSceneInfo->nameScene.c_str());
            F_DELETE(pSceneInfo)
            return false;
        }

        pSceneInfo->group = nGroup;
        itFind->second.insert(SceneInfoPtrMap::value_type(pSceneInfo->nameScene, pSceneInfo));
        m_aSceneInfo.push_back(pSceneInfo);
        return true;
    }
    void SceneSerializer::DeleteSceneInfo(uint nGroup, const String& strName)
    {
        SceneInfoGroupPtrMap::iterator itFind = m_mapSceneInfoGroup.find(nGroup);
        if (itFind == m_mapSceneInfoGroup.end())
        {
            return;
        }

        SceneInfoPtrMap::iterator itFindSI = itFind->second.find(strName);
        if (itFindSI != itFind->second.end())
        {
            SceneInfoPtrVector::iterator itFindA = std::find(m_aSceneInfo.begin(), m_aSceneInfo.end(), itFindSI->second);
            if (itFindA != m_aSceneInfo.end())
                m_aSceneInfo.erase(itFindA);
            F_DELETE(itFindSI->second)
            itFind->second.erase(itFindSI);
        }
    }
    void SceneSerializer::DeleteSceneInfoAll()
    {
        for (SceneInfoGroupPtrMap::iterator it = m_mapSceneInfoGroup.begin();
             it != m_mapSceneInfoGroup.end(); ++it)
        {
            SceneInfoPtrMap& mapSIs = it->second;
            for (SceneInfoPtrMap::iterator itSI = mapSIs.begin(); 
                 itSI != mapSIs.end(); ++itSI)
            {
                F_DELETE(itSI->second)
            }
        }
        m_aSceneInfo.clear();
        m_mapSceneInfoGroup.clear();
    }
    
    //File Content XML
    bool SceneSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "SceneSerializer::serializeXML")


        return false;
    }
    bool SceneSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "SceneSerializer::deserializeXML")
        
        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);

            //name
            String strNameScene;
            if (!pChild->ParserAttribute_String(SCENE_TAG_ATTRIBUTE_NAME, strNameScene))
            {
                F_LogError("*********************** SceneSerializer::deserializeXML: Can not find attribute: 'name', scene index: [%d] !", i);
                continue;
            }
            //path
            String strPathScene;
            if (!pChild->ParserAttribute_String(SCENE_TAG_ATTRIBUTE_PATH, strPathScene))
            {
                F_LogError("*********************** SceneSerializer::deserializeXML: Can not find attribute: 'path', scene index: [%d] !", i);
                continue;
            }

            SceneInfo* pSceneInfo = new SceneInfo(strNameScene,
                                                  strPathScene);
            if (AddSceneInfo(pSceneInfo))
            {
                F_LogInfo("SceneSerializer::deserializeXML: Add scene info success, [%u]-[%s]-[%s] !",
                          pSceneInfo->group,
                          strNameScene.c_str(), 
                          strPathScene.c_str());
            }
        }

        return true;
    }

    //File Content Binary
    bool SceneSerializer::serializeBinary()
    {
        return false;
    }
    bool SceneSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter