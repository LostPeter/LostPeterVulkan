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
#include "../include/SceneManager.h"
#include "../include/Scene.h"
#include "../include/SceneNode.h"

namespace LostPeter
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


        static bool s_parserXML_SceneSetting(FXMLElement* pElementSceneSetting, Scene* pScene)
        {
            F_Assert(pElementSceneSetting && pScene && "s_parserXML_SceneSetting")


            return true;
        }


                static bool s_parserXML_Object(FXMLElement* pElementObject, Scene* pScene)
                {
                    F_Assert(pElementObject && pScene && "s_parserXML_Object")

                    
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


            static bool s_parserXML_Objects(FXMLElement* pElementObjects, Scene* pScene)
            {
                F_Assert(pElementObjects && pScene && "s_parserXML_Objects")

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

                    if (!s_parserXML_Object(pElementItem, pScene))
                    {
                        F_LogError("*********************** s_parserXML_Objects: Parser object failed, index: [%d], type: [%s] !", i, nameObjectType.c_str());
                        return false;
                    }
                }

                F_LogInfo("##### s_parserXML_Objects: Parser objects success, type: [%s] !", nameObjectType.c_str());
                return true;
            }

        static bool s_parserXML_SceneObject(FXMLElement* pElementSceneObject, Scene* pScene)
        {
            F_Assert(pElementSceneObject && pScene && "s_parserXML_SceneObject")

            int count_object = pElementSceneObject->GetElementChildrenCount();
            for (int i = 0; i < count_object; i++)
            {
                FXMLElement* pElementItem = pElementSceneObject->GetElementChild(i);

                if (!s_parserXML_Objects(pElementItem, pScene))
                {
                    F_LogError("*********************** s_parserXML_SceneObject: Parse 'object' failed, index: [%d] !", i);
                    return false;
                }
            }

            return true;
        }

            static SceneNode* s_parserXML_Node(FXMLElement* pElementNode, Scene* pScene, SceneNode* pParent)
            {
                F_Assert(pElementNode && pScene && "s_parserXML_Node")

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

                SceneNode* pSceneNode = nullptr;
                if (pParent == nullptr)
                {
                    pSceneNode = pScene->CreateRootSceneNode(nameSceneNode, vPos, vAngle, vScale);
                }
                else
                {
                    pSceneNode = pParent->CreateChildSceneNodeByAngle(nameSceneNode, vPos, vAngle, vScale);
                }

                F_LogInfo("s_parserXML_Node: Parser scene node success, node: [%s], parent: [%s] !", nameSceneNode.c_str(), (pParent == nullptr ? "nullptr" : pParent->GetName().c_str()));
                return pSceneNode;
            }

        static bool s_parserXML_SceneNode(FXMLElement* pElementSceneNode, Scene* pScene, SceneNode* pParent)
        {
            F_Assert(pElementSceneNode && pScene && "s_parserXML_SceneNode")

            SceneNode* pSceneNode_Parent = nullptr;
            int count_node = pElementSceneNode->GetElementChildrenCount();
            for (int i = 0; i < count_node; i++)
            {
                FXMLElement* pElementItem = pElementSceneNode->GetElementChild(i);
                
                SceneNode* pSceneNode = s_parserXML_Node(pElementItem, pScene, pParent);
                if (!pSceneNode)
                {
                    F_LogError("*********************** s_parserXML_SceneNode: Parse 'node' failed, index: [%d] !", i);
                    return false;
                }

                if (!s_parserXML_SceneNode(pElementItem, pScene, pSceneNode))
                {
                    F_LogError("*********************** s_parserXML_SceneNode: Parse 'node' child failed, index: [%d], name: [%s] !", i, pSceneNode->GetName().c_str());
                    return false;
                }
            }

            return true;
        }

    static bool	s_parserXML_SceneData(FXMLElement* pElementSceneData, Scene* pScene)
    {
        F_Assert(pElementSceneData && pScene && "s_parserXML_SceneData")

        int count_scene_data = pElementSceneData->GetElementChildrenCount();
		for (int i = 0; i < count_scene_data; i++)
        {
            FXMLElement* pElementItem = pElementSceneData->GetElementChild(i);
            const String& nameTag = pElementItem->GetName();

            //1> scene_setting
            if (nameTag == SCENE_DATA_TAG_SCENE_SETTING)
            {
                F_LogInfo("########## 11111 s_parserXML_SceneData: Start to parser 'scene_setting' !");
                if (!s_parserXML_SceneSetting(pElementItem, pScene))
                {
                    F_LogError("*********************** s_parserXML_SceneData: Parse 'scene_setting' failed, index: [%d] !", i);
                    return false;
                }
                F_LogInfo("########## 11111 s_parserXML_SceneData: End to parser 'scene_setting' !");
            }
            //2> scene_object
            else if (nameTag == SCENE_DATA_TAG_SCENE_OBJECT)
            {
                F_LogInfo("########## 22222 s_parserXML_SceneData: Start to parser 'scene_object' !");
                if (!s_parserXML_SceneObject(pElementItem, pScene))
                {
                    F_LogError("*********************** s_parserXML_SceneData: Parse 'scene_object' failed, index: [%d] !", i);
                    return false;
                }
                F_LogInfo("########## 22222 s_parserXML_SceneData: End to parser 'scene_object' !");
            }
            //3> scene_node
            else if (nameTag == SCENE_DATA_TAG_SCENE_NODE)
            {
                F_LogInfo("########## 33333 s_parserXML_SceneData: Start to parser 'scene_node' !");
                if (!s_parserXML_SceneNode(pElementItem, pScene, nullptr))
                {
                    F_LogError("*********************** s_parserXML_SceneData: Parse 'scene_node' failed, index: [%d] !", i);
                    return false;
                }
                F_LogInfo("########## 33333 s_parserXML_SceneData: End to parser 'scene_node' !");
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
					{
                        delete pScene;
                    }
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