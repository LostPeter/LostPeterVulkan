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

#include "../include/Scene.h"
#include "../include/VulkanWindow.h"
#include "../include/SceneDataManager.h"
#include "../include/SceneManager.h"
#include "../include/SceneNode.h"

namespace LostPeter
{
	String Scene::ms_nameRootSceneNode = "RootSceneNode";

    Scene::Scene(uint _group, 
                 const String& nameScene)
        : Base(_group, nameScene)
    ////SceneManager/SceneNode
        , m_pSceneManager(nullptr)
        , m_pRootSceneNode(nullptr)
    ////Viewport
        , m_pViewportMain(nullptr)
    ////Object
        
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

    bool Scene::LoadScene()
    {
        if (!IsGroupNameValid())
		{
            F_LogError("*********************** Scene::LoadScene: Group, Name is not valid, group: [%u, name: [%s] !", this->group, this->name.c_str());
            return false;
        }

        if (!SceneDataManager::GetSingleton().Parser(this->group, this->name, this))
        {
            F_LogError("*********************** Scene::LoadScene: Group, Parse scene file failed, group: [%u, name: [%s] !", this->group, this->name.c_str());
            return false;
        }

        return true;
    }

	void Scene::UnloadScene()
    {

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
			F_LogError("Scene::AddSceneNode: Scene node with the name: [%s] already exists !", strName.c_str());
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
			F_LogError("Scene::CreateSceneNode: Scene node with the name: [%s] already exists !", pSceneNode->GetName().c_str());
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
			F_LogError("Scene::CreateSceneNode: Scene node with the name: [%s] already exists !", strName.c_str());
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
			F_LogError("SceneManager::DestroySceneNode: SceneNode: [%s] not found !", strName.c_str());
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




}; //LostPeter