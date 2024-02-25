/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SceneDataSerializer.h"
#include "../include/SceneDataManager.h"
#include "../include/SceneData.h"
#include "../include/SceneConfig.h"
#include "../include/SceneManager.h"
#include "../include/Scene.h"

namespace LostPeterEngine
{
#define SCENE_DATA_FILE_XML_EXT								".scene"
#define SCENE_DATA_FILE_BINARY_EXT							".bscene"

#define SCENE_DATA_TAG_SCENE_CFG								            "cfg_scene_data"
    #define	SCENE_DATA_TAG_SCENE									            "scene_data"
        #define	SCENE_DATA_TAG_SCENE_SETTING									    "scene_setting"
        #define	SCENE_DATA_TAG_SCENE_OBJECT									        "scene_object"
            #define	SCENE_DATA_TAG_SCENE_OBJECT_OBJECT							        "object"
                #define	SCENE_DATA_TAG_SCENE_OBJECT_OBJECT_OBJ							    "obj"
        #define	SCENE_DATA_TAG_SCENE_NODE									        "scene_node"
            #define	SCENE_DATA_TAG_SCENE_NODE_NODE									    "node"


#define SCENE_DATA_TAG_ATTRIBUTE_GROUP			            "group"
#define SCENE_DATA_TAG_ATTRIBUTE_NAME		                "name"
#define	SCENE_DATA_TAG_ATTRIBUTE_TYPE				        "type"
#define	SCENE_DATA_TAG_ATTRIBUTE_ID				            "id"
#define	SCENE_DATA_TAG_ATTRIBUTE_ID_MAX				        "id_max"
#define	SCENE_DATA_TAG_ATTRIBUTE_POS				        "pos"
#define	SCENE_DATA_TAG_ATTRIBUTE_ANGLE				        "angle"
#define	SCENE_DATA_TAG_ATTRIBUTE_SCALE				        "scale"

    //Object Mesh
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_MESH		            "param_mesh"
    //Object SkinMesh
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_SKIN_MESH	        "param_skin_mesh"
    //Object Camera
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_CAMERA		        "param_camera"
    //Object Light
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_LIGHT		        "param_light"
    //Object Terrain
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_TERRAIN		        "param_terrain"
    //Object Water
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_WATER		        "param_water"
    //Object Sky
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_SKY		            "param_sky"
    //Object Cloud
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_CLOUD		        "param_cloud"
    //Object Particle
    #define	SCENE_DATA_TAG_ATTRIBUTE_PARAM_PARTICLE		        "param_particle"


        static bool s_parserXML_SceneSetting(FXMLElement* pElementSceneSetting, SceneData* pSceneData)
        {
            F_Assert(pElementSceneSetting && pSceneData && "s_parserXML_SceneSetting")


            return true;
        }


                static bool s_parserXML_Object(FXMLElement* pElementObject, SceneData* pSceneData)
                {
                    F_Assert(pElementObject && pSceneData && "s_parserXML_Object")

                    
                    String nameObject;
                    if (!pElementObject->ParserAttribute_String(SCENE_DATA_TAG_ATTRIBUTE_NAME, nameObject))
                    {
                        F_LogError("*********************** s_parserXML_Object: Can not find attribute: 'name', from obj !");     
                        return false;
                    }
                    uint32 id;
                    if (!pElementObject->ParserAttribute_UInt(SCENE_DATA_TAG_ATTRIBUTE_ID, id))
                    {
                        F_LogError("*********************** s_parserXML_Object: Can not find attribute: 'id', from obj !"); 
                        return false;
                    }


                    F_LogInfo("s_parserXML_Object: Parser object success, name: [%s], id: [%u] !", nameObject.c_str(), id);
                    return true;
                }


            static bool s_parserXML_Objects(FXMLElement* pElementObjects, SceneData* pSceneData)
            {
                F_Assert(pElementObjects && pSceneData && "s_parserXML_Objects")

                String nameObjectType;
                if (!pElementObjects->ParserAttribute_String(SCENE_DATA_TAG_ATTRIBUTE_TYPE, nameObjectType))
                {
                    F_LogError("*********************** s_parserXML_Objects: Can not find attribute: 'type', from object !");
                    return false;
                }

                int count_obj = pElementObjects->GetElementChildrenCount();
                for (int i = 0; i < count_obj; i++)
                {
                    FXMLElement* pElementItem = pElementObjects->GetElementChild(i);

                    if (!s_parserXML_Object(pElementItem, pSceneData))
                    {
                        F_LogError("*********************** s_parserXML_Objects: Parser object failed, index: [%d], type: [%s] !", i, nameObjectType.c_str());
                        return false;
                    }
                }

                F_LogInfo("##### s_parserXML_Objects: Parser objects success, type: [%s] !", nameObjectType.c_str());
                return true;
            }

        static bool s_parserXML_SceneObject(FXMLElement* pElementSceneObject, SceneData* pSceneData)
        {
            F_Assert(pElementSceneObject && pSceneData && "s_parserXML_SceneObject")

            int count_object = pElementSceneObject->GetElementChildrenCount();
            for (int i = 0; i < count_object; i++)
            {
                FXMLElement* pElementItem = pElementSceneObject->GetElementChild(i);

                if (!s_parserXML_Objects(pElementItem, pSceneData))
                {
                    F_LogError("*********************** s_parserXML_SceneObject: Parse 'object' failed, index: [%d] !", i);
                    return false;
                }
            }

            return true;
        }

            static SceneConfigNode* s_parserXML_Node(FXMLElement* pElementNode, SceneData* pSceneData, SceneConfigNode* pParent)
            {
                F_Assert(pElementNode && pSceneData && "s_parserXML_Node")

                String nameSceneNode;
                if (!pElementNode->ParserAttribute_String(SCENE_DATA_TAG_ATTRIBUTE_NAME, nameSceneNode))
                {
                    F_LogError("*********************** s_parserXML_Node: Can not find attribute: 'name', from node !");     
                    return nullptr;
                }
                FVector3 vPos;
                if (!pElementNode->ParserAttribute_Vector3(SCENE_DATA_TAG_ATTRIBUTE_POS, vPos))
                {
                    F_LogError("*********************** s_parserXML_Node: Can not find attribute: 'pos', from node !");     
                    return nullptr;
                }
                FVector3 vAngle;
                if (!pElementNode->ParserAttribute_Vector3(SCENE_DATA_TAG_ATTRIBUTE_ANGLE, vAngle))
                {
                    F_LogError("*********************** s_parserXML_Node: Can not find attribute: 'angle', from node !");     
                    return nullptr;
                }
                FVector3 vScale;
                if (!pElementNode->ParserAttribute_Vector3(SCENE_DATA_TAG_ATTRIBUTE_SCALE, vScale))
                {
                    F_LogError("*********************** s_parserXML_Node: Can not find attribute: 'scale', from node !");     
                    return nullptr;
                }

                SceneConfigNode* pSceneConfigNode = new SceneConfigNode(nameSceneNode);
                if (pParent != nullptr)
                    pSceneConfigNode->SetNameParent(pParent->GetName());
                pSceneConfigNode->SetPos(vPos);
                pSceneConfigNode->SetAngle(vAngle);
                pSceneConfigNode->SetScale(vScale);
                pSceneData->AddSceneConfigNode(pSceneConfigNode);

                F_LogInfo("s_parserXML_Node: Parser scene node success, node: [%s], parent: [%s] !", nameSceneNode.c_str(), (pParent == nullptr ? "nullptr" : pParent->GetName().c_str()));
                return pSceneConfigNode;
            }

        static bool s_parserXML_SceneNode(FXMLElement* pElementSceneNode, SceneData* pSceneData, SceneConfigNode* pParent)
        {
            F_Assert(pElementSceneNode && pSceneData && "s_parserXML_SceneNode")

            int count_node = pElementSceneNode->GetElementChildrenCount();
            for (int i = 0; i < count_node; i++)
            {
                FXMLElement* pElementItem = pElementSceneNode->GetElementChild(i);
                
                SceneConfigNode* pSceneNode = s_parserXML_Node(pElementItem, pSceneData, pParent);
                if (!pSceneNode)
                {
                    F_LogError("*********************** s_parserXML_SceneNode: Parse 'node' failed, index: [%d] !", i);
                    return false;
                }

                if (!s_parserXML_SceneNode(pElementItem, pSceneData, pSceneNode))
                {
                    F_LogError("*********************** s_parserXML_SceneNode: Parse 'node' child failed, index: [%d], name: [%s] !", i, pSceneNode->GetName().c_str());
                    return false;
                }
            }

            return true;
        }

    static bool	s_parserXML_SceneData(FXMLElement* pElementSceneData, SceneData* pSceneData)
    {
        F_Assert(pElementSceneData && pSceneData && "s_parserXML_SceneData")

        int count_scene_data = pElementSceneData->GetElementChildrenCount();
		for (int i = 0; i < count_scene_data; i++)
        {
            FXMLElement* pElementItem = pElementSceneData->GetElementChild(i);
            const String& nameTag = pElementItem->GetName();

            //1> scene_setting
            if (nameTag == SCENE_DATA_TAG_SCENE_SETTING)
            {
                if (!s_parserXML_SceneSetting(pElementItem, pSceneData))
                {
                    F_LogError("*********************** s_parserXML_SceneData: Parse 'scene_setting' failed, index: [%d] !", i);
                    return false;
                }
            }
            //2> scene_object
            else if (nameTag == SCENE_DATA_TAG_SCENE_OBJECT)
            {
                if (!s_parserXML_SceneObject(pElementItem, pSceneData))
                {
                    F_LogError("*********************** s_parserXML_SceneData: Parse 'scene_object' failed, index: [%d] !", i);
                    return false;
                }
            }
            //3> scene_node
            else if (nameTag == SCENE_DATA_TAG_SCENE_NODE)
            {
                if (!s_parserXML_SceneNode(pElementItem, pSceneData, nullptr))
                {
                    F_LogError("*********************** s_parserXML_SceneData: Parse 'scene_node' failed, index: [%d] !", i);
                    return false;
                }
            }   
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
    
    bool SceneDataSerializer::Parser(uint32 nGroup, const String& strName, SceneData* pSceneData, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
		if (strPath.empty())
			return false;
        
		if (strPath.find(SCENE_DATA_FILE_BINARY_EXT) != String::npos)
		{
			if (!ParserBinary(nGroup, strName, strPath, pSceneData, pRet))
			{
                F_LogError("*********************** SceneDataSerializer::Parser: Parser scene binary file failed, group: [%u], path: [%s] !", nGroup, strPath.c_str());
				return false;
			}
			return true;
		}
		else if(strPath.find(SCENE_DATA_FILE_XML_EXT) != String::npos)
		{
			if (!ParserXML(nGroup, strName, strPath, pSceneData, pRet))
			{
                F_LogError("*********************** SceneDataSerializer::Parser: Parser scene xml file failed, group: [%u], path: [%s] !", nGroup, strPath.c_str());
				return false;
			}
			return true;
		}
		else
		{	
            F_LogError("*********************** SceneDataSerializer::Parser: Not valid scene file, group: [%u], path: [%s] !", nGroup, strPath.c_str());
		}
		
		return false;
    }

    bool SceneDataSerializer::ParserXML(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
        if (strPath.empty())
		{
			F_LogError("*********************** SceneDataSerializer::ParserXML: Can not find scene file by group: [%u], name: [%s] !", nGroup, strName.c_str());
			return false;
		}
		
		return ParserXML(nGroup, strName, strPath, nullptr, pRet);
    }
    bool SceneDataSerializer::ParserXML(uint32 nGroup, const String& strName, const String& strPath, SceneData* pSceneData /*= nullptr*/, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        FFileXML xml;
		if (!xml.LoadXMLIndirect(strPath))
        {
            F_LogError("*********************** SceneDataSerializer::ParserXML: Load scene file: [%s] failed !", strPath.c_str());
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

            SceneData* pSD = m_pSceneDataManager->GetSceneData(strNameScene);
            if (pSD != nullptr)
            {
                F_LogWarning("####################### SceneDataSerializer::ParserXML: Scene data [%s] already exist !", strNameScene.c_str());
            }
            else
            {
                bool bExtern = pSceneData ? true : false;
				if (!bExtern)
				{
					pSceneData = m_pSceneDataManager->NewSceneData(nGroup, strNameScene);
				}
                pSceneData->SetPath(strPath);

                if (!s_parserXML_SceneData(pElementSceneData, pSceneData))
				{
					if (!bExtern)
                        delete pSceneData;
					F_LogError("*********************** SceneDataSerializer::ParserXML: Parser scene data: [%s] failed !", strNameScene.c_str());
					return false;
				}

                if (!bExtern)
				{
					if (m_pSceneDataManager->AddSceneData(pSceneData))
					{
						pSD = pSceneData;
						F_LogInfo("SceneDataSerializer::ParserXML: Parser Scene data [%s] success !", strNameScene.c_str());
						pSceneData = nullptr;
					}
					else
					{
                        F_DELETE(pSceneData)
                        F_LogError("*********************** SceneDataSerializer::ParserXML: AddScene: [%s] failed !", strNameScene.c_str());
					    return false;
					}
				}
				else
				{
					if (pRet && pSceneData)
					{
						pRet->push_back(pSceneData);
					}
					F_LogInfo("SceneDataSerializer::ParserXML: Parser Scene data: [%s] success !", strNameScene.c_str());
					break;
				}
            }

            if (pRet && pSD)
			{
				pRet->push_back(pSD);
			}
        }

        return true;
    }

    bool SceneDataSerializer::ParserBinary(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
        if (strPath.empty())
		{
			F_LogError("*********************** SceneDataSerializer::ParserBinary: Can not find scene file by group: [%u], name: [%s] !", nGroup, strName.c_str());
			return false;
		}
		
		return ParserBinary(nGroup, strName, strPath, nullptr, pRet);
    }
    bool SceneDataSerializer::ParserBinary(uint32 nGroup, const String& strName, const String& strPath, SceneData* pSceneData /*= nullptr*/, SceneDataPtrVector* pRet /*= nullptr*/)
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
    bool SceneDataSerializer::SaveXML(const String& strPath, ScenePtrVector& aSA)
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
    bool SceneDataSerializer::SaveBinary(const String& strPath, ScenePtrVector& aSA)
    {

        return true;
    }

}; //LostPeterEngine