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

#include "../include/Scene.h"
#include "../include/SceneDataManager.h"
#include "../include/SceneManager.h"
#include "../include/SceneNode.h"
#include "../include/ObjectMesh.h"
#include "../include/ObjectSkinMesh.h"
#include "../include/ObjectCamera.h"
#include "../include/ObjectLight.h"
#include "../include/ObjectTerrain.h"
#include "../include/ObjectWater.h"
#include "../include/ObjectSky.h"
#include "../include/ObjectCloud.h"
#include "../include/ObjectParticle.h"

namespace LostPeterEngine
{
	String Scene::ms_nameRootSceneNode = "RootSceneNode";
    Scene::Scene(uint nGroup, const String& strNameScene)
        : Base(nGroup, strNameScene)
	////SceneData
		, m_pSceneData(nullptr)
    ////SceneManager/SceneNode
        , m_pSceneManager(nullptr)
        , m_pRootSceneNode(nullptr)
    ////Viewport
        , m_pViewportMain(nullptr)
    ////Object
		, m_pMainObjectCamera(nullptr)
		, m_pMainObjectLight(nullptr)
    {

    }

    Scene::~Scene()
    {
        Destroy();
    }

    void Scene::Destroy()
    {
        UnloadScene();
    }

    bool Scene::LoadScene(bool bIsFromFile /*= true*/)
    {
		uint32 nGroup = GetGroup();
        const String& strName = GetName();
        if (!IsGroupNameValid())
		{
            F_LogError("*********************** Scene::LoadScene: Group, Name is not valid: group: [%u], name: [%s] !", nGroup, strName.c_str());
            return false;
        }

		if (!m_pSceneData)
		{
			m_pSceneData = SceneDataManager::GetSingleton().LoadSceneData(nGroup, strName, bIsFromFile);
            if (m_pSceneData == nullptr)
            {
                F_LogError("*********************** Scene::LoadScene: LoadMaterialData failed, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return false;
            }
		}



        return true;
    }
	void Scene::UnloadScene()
	{
		ClearScene();

		if (m_pSceneData)
		{
			SceneDataManager::GetSingleton().UnloadSceneData(m_pSceneData);
		}
        m_pSceneData = nullptr;
	}

	void Scene::ClearScene()
    {
		if (!m_pRootSceneNode)
			return;

		m_pRootSceneNode->RemoveAllChildren();
		m_pRootSceneNode->DetachAllObjects();
		for (SceneNodePtrMap::iterator it = m_mapSceneNodes.begin();
			 it != m_mapSceneNodes.end();++it)
		{
			F_DELETE(it->second)
		}
		m_mapSceneNodes.clear();
		m_setAutoTrackingSceneNodes.clear();
		m_pRootSceneNode = nullptr;

		DestroyObjectAll();
    }


////SceneManager/SceneNode
	SceneNode* Scene::CreateRootSceneNode(const String& strName,
										  const FVector3& vTrans /*= FMath::ms_v3Zero*/, 
										  const FQuaternion& qRot /*= FMath::ms_qUnit*/,
									      const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		if (!m_pRootSceneNode)
		{
			m_pRootSceneNode = createSceneNodeImpl(strName);
			m_pRootSceneNode->SetPositionLocalOnly(vTrans);
			m_pRootSceneNode->SetRotationLocalOnly(qRot);
			m_pRootSceneNode->SetScaleLocalOnly(vScale);
			m_pRootSceneNode->NeedUpdate();
			m_pRootSceneNode->NotifyRootNode();
		}	
		return m_pRootSceneNode;
	}
	SceneNode* Scene::CreateRootSceneNode(const String& strName, 
										  const FVector3& vTrans /*= FMath::ms_v3Zero*/, 
										  const FVector3& vAngle /*= FMath::ms_v3Zero*/,
										  const FVector3& vScale /*= FMath::ms_v3One*/)
	{
		if (!m_pRootSceneNode)
		{
			m_pRootSceneNode = createSceneNodeImpl(strName);
			m_pRootSceneNode->SetPositionLocalOnly(vTrans);
			m_pRootSceneNode->SetAngleLocalOnly(vAngle);
			m_pRootSceneNode->SetScaleLocalOnly(vScale);
			m_pRootSceneNode->NeedUpdate();
			m_pRootSceneNode->NotifyRootNode();
		}	
		return m_pRootSceneNode;
	}
    SceneNode* Scene::GetRootSceneNode()
	{
		if (!m_pRootSceneNode)
		{
			CreateRootSceneNode(ms_nameRootSceneNode,
								FMath::ms_v3Zero,
								FMath::ms_qUnit,
								FMath::ms_v3One);
		}
		return m_pRootSceneNode;
	}

	SceneNode* Scene::GetSceneNode(const String& strName) const
	{
		SceneNodePtrMap::const_iterator itFind = m_mapSceneNodes.find(strName);
		if (itFind == m_mapSceneNodes.end())
		{
			return nullptr;
		}
		return itFind->second;
	}
	bool Scene::HasSceneNode(const String& strName) const
	{
		return (m_mapSceneNodes.find(strName) != m_mapSceneNodes.end());
	}
	bool Scene::AddSceneNode(SceneNode* pSceneNode)
	{
		if (!pSceneNode)
		{
			return false;
		}

		const String& strName = pSceneNode->GetName();
		SceneNodePtrMap::const_iterator itFind = m_mapSceneNodes.find(strName);
		if (itFind != m_mapSceneNodes.end())
		{
			F_LogError("*********************** Scene::AddSceneNode: Scene node with the name: [%s] already exists !", strName.c_str());
			return false;
		}

		m_mapSceneNodes[strName] = pSceneNode;
		return true;
	}

	SceneNode* Scene::CreateSceneNode()
	{
		SceneNode* pSceneNode = createSceneNodeImpl();
		if (!AddSceneNode(pSceneNode))
		{
			F_LogError("*********************** Scene::CreateSceneNode: Scene node with the name: [%s] already exists !", pSceneNode->GetName().c_str());
			return nullptr;
		}
		return pSceneNode;
	}
	SceneNode* Scene::CreateSceneNode(const String& strName)
	{
		return CreateSceneNode(strName, nullptr);
	}
	SceneNode* Scene::CreateSceneNode(const String& strName, SceneNode* pParent)
	{
		if (m_mapSceneNodes.find(strName) != m_mapSceneNodes.end())
		{
			F_LogError("*********************** Scene::CreateSceneNode: Scene node with the name: [%s] already exists !", strName.c_str());
			return nullptr;
		}
		
		SceneNode* pSceneNode = createSceneNodeImpl(strName);
		if (pParent != nullptr)
		{
			pParent->AddChild(pSceneNode);
		}
		AddSceneNode(pSceneNode);
		return pSceneNode;
	}

	void Scene::DestroySceneNode(const String& strName)
	{
		SceneNodePtrMap::iterator itFind = m_mapSceneNodes.find(strName);
		if (itFind == m_mapSceneNodes.end())
		{
			F_LogError("*********************** SceneManager::DestroySceneNode: SceneNode: [%s] not found !", strName.c_str());
			return;
		}

		for (SceneNodePtrSet::iterator it = m_setAutoTrackingSceneNodes.begin();
			 it != m_setAutoTrackingSceneNodes.end(); )
		{
			SceneNodePtrSet::iterator itCur = it++;
			SceneNode* pSceneNode = *itCur;

			if (pSceneNode->GetAutoTrackTarget() == itFind->second)
			{
				pSceneNode->SetAutoTracking(false);
			}
			else if (pSceneNode == itFind->second)
			{
				m_setAutoTrackingSceneNodes.erase(itCur);
			}
		}

		Node* pNodeParent = itFind->second->GetParent();
		if (pNodeParent)
		{
			pNodeParent->RemoveChild(itFind->second);
		}
		F_DELETE(itFind->second);
		m_mapSceneNodes.erase(itFind);
	}

	void Scene::DestroySceneNode(SceneNode* pSceneNode)
	{
		DestroySceneNode(pSceneNode->GetName());
	}

	void Scene::NotifyAutoTrackingSceneNode(SceneNode* pSceneNode, bool bAutoTrack)
	{
		if (bAutoTrack)
		{
			m_setAutoTrackingSceneNodes.insert(pSceneNode);
		}
		else
		{
			m_setAutoTrackingSceneNodes.erase(pSceneNode);
		}
	}

    SceneNode* Scene::createSceneNodeImpl()
	{
		return new SceneNode(this);
	}

	SceneNode* Scene::createSceneNodeImpl(const String& strName)
	{
		return new SceneNode(strName, this);
	}


////Viewport



////Object
    ObjectPtrMap* Scene::GetObjectPtrMapByType(EObjectType type)
	{
		return GetObjectPtrMapByType((int)type);
	}
	ObjectPtrMap* Scene::GetObjectPtrMapByType(int type)
	{
		ObjectPtrGroupMap::iterator itFind = m_mapObjectGroups.find(type);
		if (itFind == m_mapObjectGroups.end())
		{
			ObjectPtrMap mapObject;
			m_mapObjectGroups[type] = mapObject;
			itFind = m_mapObjectGroups.find(type);
		}
		return &itFind->second;
	}

	Object* Scene::GetObject(EObjectType type, const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(type);
		ObjectPtrMap::iterator itFind = pObjectMap->find(strName);
		if (itFind == pObjectMap->end())
		{
			F_LogError("*********************** Scene::GetObject: Can not find object named: [%s], type: [%s] !", strName.c_str(), E_GetObjectTypeName(type).c_str());
			return nullptr;
		}
		return itFind->second;
	}
	bool Scene::HasObject(EObjectType type, const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(type);
		return pObjectMap->find(strName) != pObjectMap->end();
	}
	Object* Scene::DestroyObject(EObjectType type, const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(type);
		ObjectPtrMap::iterator itFind = pObjectMap->find(strName);
		if (itFind != pObjectMap->end())
		{
			Object* pObject = itFind->second;
			pObjectMap->erase(itFind);
			return pObject;
		}
		return nullptr;
	}
	void Scene::DestroyObjectAll(EObjectType type)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(type);
		for (ObjectPtrMap::iterator it = pObjectMap->begin();
			 it != pObjectMap->end(); ++it)
		{
			F_DELETE(it->second)
		}
		pObjectMap->clear();
	}
	void Scene::DestroyObjectAll()
	{
		for (ObjectPtrGroupMap::iterator it = m_mapObjectGroups.begin();
			 it != m_mapObjectGroups.end(); ++it)
		{
			ObjectPtrMap& mapObject = it->second;
			for (ObjectPtrMap::iterator itO = mapObject.begin();
				itO != mapObject.end(); ++itO)
			{
				F_DELETE(itO->second)
			}
			mapObject.clear();
		}
		m_mapObjectGroups.clear();
		m_pMainObjectCamera = nullptr;
		m_pMainObjectLight = nullptr;
	}


	//ObjectMesh
	ObjectMesh* Scene::GetObjectMesh(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Mesh, strName);
		if (!pObject)
			return nullptr;
		return (ObjectMesh*)pObject;
	}
	bool Scene::HasObjectMesh(const String& strName)
	{
		return HasObject(E_Object_Mesh, strName);
	}

	ObjectMesh* Scene::CreateObjectMesh(const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Mesh);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectMesh: Object mesh named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectMesh* pObjectMesh = new ObjectMesh(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectMesh));
		return pObjectMesh;
	}

	void Scene::DestroyObjectMesh(ObjectMesh* pObjectMesh)
	{
		DestroyObjectMesh(pObjectMesh->GetName());
	}
	void Scene::DestroyObjectMesh(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Mesh, strName);
		F_DELETE(pObject)
	}
	void Scene::DestroyObjectMeshAll()
	{
		DestroyObjectAll(E_Object_Mesh);
	}


	//ObjectSkinMesh
	ObjectSkinMesh* Scene::GetObjectSkinMesh(const String& strName)
	{
		Object* pObject = GetObject(E_Object_SkinMesh, strName);
		if (!pObject)
			return nullptr;
		return (ObjectSkinMesh*)pObject;
	}
	bool Scene::HasObjectSkinMesh(const String& strName)
	{
		return HasObject(E_Object_SkinMesh, strName);
	}

	ObjectSkinMesh* Scene::CreateObjectSkinMesh(const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_SkinMesh);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectSkinMesh: Object skin mesh named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectSkinMesh* pObjectSkinMesh = new ObjectSkinMesh(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectSkinMesh));
		return pObjectSkinMesh;
	}

	void Scene::DestroyObjectSkinMesh(ObjectSkinMesh* pObjectSkinMesh)
	{
		DestroyObjectSkinMesh(pObjectSkinMesh->GetName());
	}
	void Scene::DestroyObjectSkinMesh(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_SkinMesh, strName);
		F_DELETE(pObject)
	}
	void Scene::DestroyObjectSkinMeshAll()
	{
		DestroyObjectAll(E_Object_SkinMesh);
	}


	//ObjectCamera
	ObjectCamera* Scene::GetObjectCamera(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Camera, strName);
		if (!pObject)
			return nullptr;
		return (ObjectCamera*)pObject;
	}
	bool Scene::HasObjectCamera(const String& strName)
	{
		return HasObject(E_Object_Camera, strName);
	}

	ObjectCamera* Scene::CreateObjectCamera(const String& strName, SceneManager* pSceneManager, bool bMainCamera /*= false*/)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Camera);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectCamera: Object camera named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectCamera* pObjectCamera = nullptr;

		pObjectCamera = new ObjectCamera(strName, pSceneManager);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectCamera));
		if (bMainCamera && !m_pMainObjectCamera)
		{
			m_pMainObjectCamera = pObjectCamera;
		}
		return pObjectCamera;
	}		

	void Scene::DestroyObjectCamera(ObjectCamera* pObjectCamera)
	{
		DestroyObjectCamera(pObjectCamera->GetName());
	}
	void Scene::DestroyObjectCamera(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Camera, strName);
		if (pObject != nullptr)
		{
			if (pObject == m_pMainObjectCamera)
			{
				m_pMainObjectCamera = nullptr;
			}
			F_DELETE(pObject)
		}
	}
	void Scene::DestroyObjectCameraAll()
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Camera);
		for (ObjectPtrMap::iterator it = pObjectMap->begin();
			 it != pObjectMap->end(); ++it)
		{
			ObjectCamera* pObjectCamera = (ObjectCamera*)it->second;
			F_DELETE(pObjectCamera)
		}
		pObjectMap->clear();
		m_pMainObjectCamera = nullptr;
	}


	//ObjectLight
	ObjectLight* Scene::GetObjectLight(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Light, strName);
		if (!pObject)
			return nullptr;
		return (ObjectLight*)pObject;
	}
	bool Scene::HasObjectLight(const String& strName)
	{
		return HasObject(E_Object_Light, strName);
	}

	ObjectLight* Scene::CreateObjectLight(const String& strName, bool bMainLight /*= false*/)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Light);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectLight: Object light named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectLight* pObjectLight = nullptr;

		pObjectLight = new ObjectLight(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectLight));
		if (bMainLight && !m_pMainObjectLight)
		{
			m_pMainObjectLight = pObjectLight;
		}
		return pObjectLight;
	}

	void Scene::DestroyObjectLight(ObjectLight* pObjectLight)
	{
		DestroyObjectLight(pObjectLight->GetName());
	}
	void Scene::DestroyObjectLight(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Light, strName);
		if (pObject != nullptr)
		{
			if (pObject == m_pMainObjectLight)
			{
				m_pMainObjectLight = nullptr;
			}
			F_DELETE(pObject)
		}
	}
	void Scene::DestroyObjectLightAll()
	{
		DestroyObjectAll(E_Object_Light);
		m_pMainObjectLight = nullptr;
	}


	//ObjectTerrain
	ObjectTerrain* Scene::GetObjectTerrain(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Terrain, strName);
		if (!pObject)
			return nullptr;
		return (ObjectTerrain*)pObject;
	}
	bool Scene::HasObjectTerrain(const String& strName)
	{
		return HasObject(E_Object_Terrain, strName);
	}

	ObjectTerrain* Scene::CreateObjectTerrain(const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Terrain);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectTerrain: Object terrain named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectTerrain* pObjectTerrain = new ObjectTerrain(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectTerrain));
		return pObjectTerrain;
	}

	void Scene::DestroyObjectTerrain(ObjectTerrain* pObjectTerrain)
	{
		DestroyObjectTerrain(pObjectTerrain->GetName());
	}
	void Scene::DestroyObjectTerrain(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Terrain, strName);
		F_DELETE(pObject)
	}
	void Scene::DestroyObjectTerrainAll()
	{
		DestroyObjectAll(E_Object_Terrain);
	}


	//ObjectWater
	ObjectWater* Scene::GetObjectWater(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Water, strName);
		if (!pObject)
			return nullptr;
		return (ObjectWater*)pObject;
	}
	bool Scene::HasObjectWater(const String& strName)
	{
		return HasObject(E_Object_Water, strName);
	}

	ObjectWater* Scene::CreateObjectWater(const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Water);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectWater: Object water named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectWater* pObjectWater = new ObjectWater(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectWater));
		return pObjectWater;
	}

	void Scene::DestroyObjectWater(ObjectWater* pObjectWater)
	{
		DestroyObjectWater(pObjectWater->GetName());
	}
	void Scene::DestroyObjectWater(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Water, strName);
		F_DELETE(pObject)
	}
	void Scene::DestroyObjectWaterAll()
	{
		DestroyObjectAll(E_Object_Water);
	}


	//ObjectSky
	ObjectSky* Scene::GetObjectSky(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Sky, strName);
		if (!pObject)
			return nullptr;
		return (ObjectSky*)pObject;
	}
	bool Scene::HasObjectSky(const String& strName)
	{
		return HasObject(E_Object_Sky, strName);
	}

	ObjectSky* Scene::CreateObjectSky(const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Sky);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectSky: Object sky named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectSky* pObjectSky = new ObjectSky(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectSky));
		return pObjectSky;
	}

	void Scene::DestroyObjectSky(ObjectSky* pObjectSky)
	{
		DestroyObjectSky(pObjectSky->GetName());
	}
	void Scene::DestroyObjectSky(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Sky, strName);
		F_DELETE(pObject)
	}
	void Scene::DestroyObjectSkyAll()
	{
		DestroyObjectAll(E_Object_Sky);
	}


	//ObjectCloud
	ObjectCloud* Scene::GetObjectCloud(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Cloud, strName);
		if (!pObject)
			return nullptr;
		return (ObjectCloud*)pObject;
	}
	bool Scene::HasObjectCloud(const String& strName)
	{
		return HasObject(E_Object_Cloud, strName);
	}

	ObjectCloud* Scene::CreateObjectCloud(const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Cloud);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectCloud: Object cloud named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectCloud* pObjectCloud = new ObjectCloud(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectCloud));
		return pObjectCloud;
	}

	void Scene::DestroyObjectCloud(ObjectCloud* pObjectCloud)
	{
		DestroyObjectCloud(pObjectCloud->GetName());
	}
	void Scene::DestroyObjectCloud(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Cloud, strName);
		F_DELETE(pObject)
	}
	void Scene::DestroyObjectCloudAll()
	{
		DestroyObjectAll(E_Object_Cloud);
	}


	//ObjectParticle
	ObjectParticle* Scene::GetObjectParticle(const String& strName)
	{
		Object* pObject = GetObject(E_Object_Particle, strName);
		if (!pObject)
			return nullptr;
		return (ObjectParticle*)pObject;
	}
	bool Scene::HasObjectParticle(const String& strName)
	{
		return HasObject(E_Object_Particle, strName);
	}

	ObjectParticle* Scene::CreateObjectParticle(const String& strName)
	{
		ObjectPtrMap* pObjectMap = GetObjectPtrMapByType(E_Object_Particle);
		if (pObjectMap->find(strName) != pObjectMap->end())
		{
			F_LogError("*********************** Scene::CreateObjectParticle: Object particle named: [%s] already exists !", strName.c_str());
			return nullptr;
		}

		ObjectParticle* pObjectParticle = new ObjectParticle(strName, this);
		pObjectMap->insert(ObjectPtrMap::value_type(strName, pObjectParticle));
		return pObjectParticle;
	}

	void Scene::DestroyObjectParticle(ObjectParticle* pObjectParticle)
	{
		DestroyObjectParticle(pObjectParticle->GetName());
	}
	void Scene::DestroyObjectParticle(const String& strName)
	{
		Object* pObject = DestroyObject(E_Object_Particle, strName);
		F_DELETE(pObject)
	}
	void Scene::DestroyObjectParticleAll()
	{
		DestroyObjectAll(E_Object_Particle);
	}


}; //LostPeterEngine