/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SceneData.h"
#include "../include/SceneConfig.h"
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
	const String SceneData::ms_nameSceneData = "SceneData";
    SceneData::SceneData(ResourceManager* pResourceManager,
                               uint32 nGroup, 
                               const String& strName,
                               const String& strGroupName,
                               ResourceHandle nHandle,
                               bool bIsManualLoad /*= false*/,
                               ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : Resource(pResourceManager,
				   nGroup, 
				   strName,
				   strGroupName,
				   nHandle,
				   bIsManualLoad,
				   pResourceManualLoader)
		, m_strPath("")

		, m_pSceneConfigSetting(nullptr)
        
        , m_bInternalResourcesCreated(false)
    {
		if (createParameterDictionary(ms_nameSceneData))
		{
			addParameterBase();
            addParameterInherit();
		}
    }
		void SceneData::addParameterBase()
        {
            FParameterDictionary* pDictionary = GetParameterDictionary();

        }
        void SceneData::addParameterInherit()
        {

        }

    SceneData::~SceneData()
    {
        Destroy();
    }

    void SceneData::Destroy()
    {
		F_DELETE(m_pSceneConfigSetting)
		DeleteObjectConfigAll();
		DeleteSceneConfigNodeAll();

		Resource::Destroy();
    }


////ObjectConfig
	bool SceneData::HasObjectConfig(EObjectType eObject, const String& strName)
	{	
		return GetObjectConfig(eObject, strName) != nullptr;
	}
	ObjectConfig* SceneData::GetObjectConfig(EObjectType eObject, const String& strName)
	{
		ObjectConfigPtrGroupMap::iterator itFind = m_mapObjectGroupConfig.find((int)eObject);
		if (itFind == m_mapObjectGroupConfig.end())
			return nullptr;
		ObjectConfigPtrMap::iterator itObject = itFind->second.find(strName);
		if (itObject == itFind->second.end())
			return nullptr;
		return itObject->second;
	}
	bool SceneData::AddObjectConfig(EObjectType eObject, ObjectConfig* pObjectConfig)
	{
		ObjectConfigPtrGroupMap::iterator itFind = m_mapObjectGroupConfig.find((int)eObject);
		if (itFind == m_mapObjectGroupConfig.end())
		{
			ObjectConfigPtrMap mapObjectConfig;
			mapObjectConfig.insert(ObjectConfigPtrMap::value_type(pObjectConfig->GetName(), pObjectConfig));
			m_mapObjectGroupConfig.insert(ObjectConfigPtrGroupMap::value_type((int)eObject, mapObjectConfig));
		}
		else
		{
			itFind->second.insert(ObjectConfigPtrMap::value_type(pObjectConfig->GetName(), pObjectConfig));
		}
		return true;
	}
	void SceneData::DeleteObjectConfig(ObjectConfig* pObjectConfig)
	{
		if (!pObjectConfig)
			return;
		DeleteObjectConfig(pObjectConfig->GetObjectType(), pObjectConfig->GetName());
	}
	void SceneData::DeleteObjectConfig(EObjectType eObject, const String& strName)
	{
		ObjectConfigPtrGroupMap::iterator itFind = m_mapObjectGroupConfig.find((int)eObject);
		if (itFind == m_mapObjectGroupConfig.end())
			return;
		ObjectConfigPtrMap::iterator itObject = itFind->second.find(strName);
		if (itObject == itFind->second.end())
			return;
		F_DELETE(itObject->second)
		itFind->second.erase(itObject);
	}
	void SceneData::DeleteObjectConfigAll()
	{
		for (ObjectConfigPtrGroupMap::iterator it = m_mapObjectGroupConfig.begin();
			 it != m_mapObjectGroupConfig.end(); ++it)
		{
			ObjectConfigPtrMap& mapObjectConfig = it->second;
			for (ObjectConfigPtrMap::iterator itObject = mapObjectConfig.begin();
			 	 itObject != mapObjectConfig.end(); ++itObject)
			{
				F_DELETE(itObject->second)
			}
		}
		m_mapObjectGroupConfig.clear();
	}

////SceneConfigNode
	bool SceneData::HasSceneConfigNode(const String& strName)
	{
		return GetSceneConfigNode(strName) != nullptr;
	}
	SceneConfigNode* SceneData::GetSceneConfigNode(const String& strName)
	{
		SceneConfigNodePtrMap::iterator itFind = m_mapSceneConfigNode.find(strName);
		if (itFind == m_mapSceneConfigNode.end())
			return nullptr;
		return itFind->second;
	}
	bool SceneData::AddSceneConfigNode(SceneConfigNode* pSceneConfigNode)
	{
		const String& strName = pSceneConfigNode->GetName();
        SceneConfigNodePtrMap::iterator itFind = m_mapSceneConfigNode.find(strName);
        if (itFind != m_mapSceneConfigNode.end())
        {
            F_LogError("*********************** SceneData::AddSceneConfigNode: SceneNode name already exist: [%s] !", strName.c_str());
            F_DELETE(pSceneConfigNode)
            return false;
        }

        m_aSceneConfigNode.push_back(pSceneConfigNode);
        m_mapSceneConfigNode.insert(SceneConfigNodePtrMap::value_type(strName, pSceneConfigNode));
        return true;
	}
	void SceneData::DeleteSceneConfigNode(SceneConfigNode* pSceneConfigNode)
	{
		if (!pSceneConfigNode)
			return;
		DeleteSceneConfigNode(pSceneConfigNode->GetName());
	}
	void SceneData::DeleteSceneConfigNode(const String& strName)
	{
		SceneConfigNodePtrMap::iterator itFind = m_mapSceneConfigNode.find(strName);
        if (itFind == m_mapSceneConfigNode.end())
        {
            return;
        }

        SceneConfigNodePtrVector::iterator itFindA = std::find(m_aSceneConfigNode.begin(), m_aSceneConfigNode.end(), itFind->second);
        if (itFindA != m_aSceneConfigNode.end())
            m_aSceneConfigNode.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapSceneConfigNode.erase(itFind);
	}
	void SceneData::DeleteSceneConfigNodeAll()
	{
		for (SceneConfigNodePtrMap::iterator it = m_mapSceneConfigNode.begin();
             it != m_mapSceneConfigNode.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aSceneConfigNode.clear();
        m_mapSceneConfigNode.clear();
	}

	void SceneData::SerializerFrom(const SceneData* pSceneData)
	{
		
	}

	void SceneData::loadImpl()
    {
        createInternalResources();
    }

    void SceneData::unloadImpl()
    {
        destroyInternalResources();
    }

    uint32 SceneData::calculateSize() const
    {
		uint32 nMemSize = sizeof(*this);
		nMemSize += (uint32)(m_strPath.size() * sizeof(char));
        nMemSize += (uint32)(GetName().size() * sizeof(char));

        

        return nMemSize;
    }

    void SceneData::destroyInternalResources()
    {
        if (m_bInternalResourcesCreated)
		{
			destroyInternalResourcesImpl();
			m_bInternalResourcesCreated = false;

			if (m_eResourceLoading.load() != E_ResourceLoading_Unloading)
            {
                m_eResourceLoading.store(E_ResourceLoading_Unloaded);
                _FireUnloadingComplete();
            }
		}
    }
        void SceneData::destroyInternalResourcesImpl()
        {

        }

    bool SceneData::createInternalResources()
    {
        if (!m_bInternalResourcesCreated)
		{
			createInternalResourcesImpl();
			m_bInternalResourcesCreated = true;

			if (!IsLoading())
            {
                m_eResourceLoading.store(E_ResourceLoading_Loaded);
                _FireLoadingComplete(false);
            }
		}
		return true;
    }
        void SceneData::createInternalResourcesImpl()
        {

        }


}; //LostPeterEngine