/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SceneDataSerializer.h"
#include "../include/VulkanWindow.h"
#include "../include/SceneDataManager.h"
#include "../include/SceneManager.h"
#include "../include/Scene.h"

namespace LostPeter
{
#define SCENE_DATA_FILE_XML_EXT								".scene"
#define SCENE_DATA_FILE_BINARY_EXT							".bscene"

#define SCENE_DATA_TAG_SCENE_CFG								            "cfg_scene_data"
    #define	SCENE_DATA_TAG_SCENE									            "scene_data"
        #define	SCENE_DATA_TAG_SCENE_SETTINGS									    "scene_settings"
        #define	SCENE_DATA_TAG_SCENE_OBJECTS									    "scene_objects"
            #define	SCENE_DATA_TAG_SCENE_OBJECTS_OBJECT							        "object"
                #define	SCENE_DATA_TAG_SCENE_OBJECTS_OBJECT_OBJ							    "obj"
        #define	SCENE_DATA_TAG_SCENE_NODES									        "scene_nodes"


#define SCENE_DATA_TAG_ATTRIBUTE_GROUP			            "group"
#define SCENE_DATA_TAG_ATTRIBUTE_NAME		                "name"
#define	SCENE_DATA_TAG_ATTRIBUTE_TYPE				        "type"
#define	SCENE_DATA_TAG_ATTRIBUTE_ID				            "id"
#define	SCENE_DATA_TAG_ATTRIBUTE_ID_MAX				        "id_max"
#define	SCENE_DATA_TAG_ATTRIBUTE_POS				        "pos"
#define	SCENE_DATA_TAG_ATTRIBUTE_ROT				        "rot"
#define	SCENE_DATA_TAG_ATTRIBUTE_SCALE				        "scale"

    //Object Mesh
    //Object SkinMesh
    //Object Camera
    //Object Light
    //Object Terrain
    //Object Water
    //Object Sky
    //Object Cloud
    //Object Particle



    static bool	s_parserXML_SceneData(FXMLElement* pElementSceneData, Scene* pScene)
    {
        F_Assert(pElementSceneData && pScene && "s_parserXML_SceneData")

        FXMLAttribute* pAttr = nullptr;
        int count_scene_data = pElementSceneData->GetElementChildrenCount();
		for (int i = 0; i < count_scene_data; i++)
        {
            FXMLElement* pElementItem = pElementSceneData->GetElementChild(i);


        }

        return true;
    }



    SceneDataSerializer::SceneDataSerializer(SceneDataManager* pSceneDataManager)
        : m_pSceneDataManager(pSceneDataManager)
    {
        
    }
    SceneDataSerializer::~SceneDataSerializer()
    {

    }
    
    bool SceneDataSerializer::Parser(uint32 nGroup, const String& strName, Scene* pScene, ScenePtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
		if (strPath.empty())
			return false;
        
		if (strPath.find(SCENE_DATA_FILE_BINARY_EXT) != String::npos)
		{
			if (!ParserBinary(strPath.c_str(), pScene, pRet))
			{
                F_LogError("*********************** SceneDataSerializer::Parser: Parser scene binary file failed, path: [%s] !", strPath.c_str());
				return false;
			}
			return true;
		}
		else if(strPath.find(SCENE_DATA_FILE_XML_EXT) != String::npos)
		{
			if (!ParserXML(strPath.c_str(), pScene, pRet))
			{
                F_LogError("*********************** SceneDataSerializer::Parser: Parser scene xml file failed, path: [%s] !", strPath.c_str());
				return false;
			}
			return true;
		}
		else
		{	
            F_LogError("*********************** SceneDataSerializer::Parser: Not valid scene file, path: [%s] !", strPath.c_str());
		}
		
		return false;
    }

    bool SceneDataSerializer::ParserXML(uint32 nGroup, const String& strName, ScenePtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
        if (strPath.empty())
		{
			F_LogError("*********************** SceneDataSerializer::ParserXML: Can not find scene file by group: [%u], name: [%s] !", nGroup, strName.c_str());
			return false;
		}
		
		return ParserXML(strPath.c_str(), nullptr, pRet);
    }
    bool SceneDataSerializer::ParserXML(const char* szFilePath, Scene* pScene /*= nullptr*/, ScenePtrVector* pRet /*= nullptr*/)
    {
        FFileXML xml;
		if (!xml.LoadXMLIndirect(szFilePath))
        {
            F_LogError("*********************** SceneDataSerializer::ParserXML: Load scene file: [%s] failed !", szFilePath);
			return false;
        }

        FXMLDocument* pXMLDocument = xml.GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_scene_datas = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_scene_datas; i++)
        {
            FXMLElement* pElementSceneData = pRoot->GetElementChild(i);

            //group
            uint nGroup;
            if (!pElementSceneData->ParserAttribute_UInt(SCENE_DATA_TAG_ATTRIBUTE_GROUP, nGroup))
            {
                F_LogError("*********************** SceneDataSerializer::ParserXML: Can not find attribute: 'group', scene data index: [%d] !", i);           
                continue;
            }

            //name
            String strNameScene;
            if (!pElementSceneData->ParserAttribute_String(SCENE_DATA_TAG_ATTRIBUTE_NAME, strNameScene))
            {
                F_LogError("*********************** SceneDataSerializer::ParserXML: Can not find attribute: 'name', scene data index: [%d] !", i);           
                continue;
            }
            F_LogInfo("SceneDataSerializer::ParserXML: Start to parser scene data: [%s] !", strNameScene.c_str());

            Scene* pSceneFind = m_pSceneDataManager->GetScene(nGroup, strNameScene);
            if (pSceneFind != nullptr)
            {
                F_LogWarning("####################### SceneDataSerializer::ParserXML: Scene data [%s] already exist !", strNameScene.c_str());
            }
            else
            {
                bool bExtern = pScene ? true : false;
				if (!bExtern)
				{
					pScene = new Scene(nGroup, strNameScene);
				}

                if (!s_parserXML_SceneData(pElementSceneData, pScene))
				{
					if (!bExtern)
						delete pScene;
					F_LogError("*********************** SceneDataSerializer::ParserXML: Parser scene data: [%s] failed !", strNameScene.c_str());
					return false;
				}

                if (!bExtern)
				{
					if (m_pSceneDataManager->AddScene(nGroup, pScene))
					{
						pSceneFind = pScene;
						F_LogInfo("SceneDataSerializer::ParserXML: Parser Scene data [%s] success !", strNameScene.c_str());
						pScene = nullptr;
					}
					else
					{
                        F_DELETE(pScene)
                        F_LogError("*********************** SceneDataSerializer::ParserXML: AddScene: [%s] failed !", strNameScene.c_str());
					    return false;
					}
				}
				else
				{
					if (pRet && pScene)
					{
						pRet->push_back(pScene);
					}
					F_LogInfo("SceneDataSerializer::ParserXML: Parser Scene data: [%s] success !", strNameScene.c_str());
					break;
				}
            }

            if (pRet && pSceneFind)
			{
				pRet->push_back(pSceneFind);
			}
        }

        return true;
    }

    bool SceneDataSerializer::ParserBinary(uint32 nGroup, const String& strName, ScenePtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
        if (strPath.empty())
		{
			F_LogError("*********************** SceneDataSerializer::ParserBinary: Can not find scene file by group: [%u], name: [%s] !", nGroup, strName.c_str());
			return false;
		}
		
		return ParserBinary(strPath.c_str(), nullptr, pRet);
    }
    bool SceneDataSerializer::ParserBinary(const char* szFilePath, Scene* pScene /*= nullptr*/, ScenePtrVector* pRet /*= nullptr*/)
    {

        return true;
    }

    bool SceneDataSerializer::SaveXML(Scene* pScene)
    {

        return true;
    }
    bool SceneDataSerializer::SaveXML(uint32 nGroup, Scene* pScene)
    {

        return true;
    }
    bool SceneDataSerializer::SaveXML(const char* szFilePath, ScenePtrVector& aSA)
    {

        return true;
    }

    bool SceneDataSerializer::SaveBinary(Scene* pScene)
    {

        return true;
    }
    bool SceneDataSerializer::SaveBinary(uint32 nGroup, Scene* pScene)
    {

        return true;
    }
    bool SceneDataSerializer::SaveBinary(const char* szFilePath, ScenePtrVector& aSA)
    {

        return true;
    }

}; //LostPeter