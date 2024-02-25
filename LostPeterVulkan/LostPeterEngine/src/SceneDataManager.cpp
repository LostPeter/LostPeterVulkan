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

#include "../include/SceneDataManager.h"
#include "../include/SceneDataSerializer.h"
#include "../include/SceneData.h"
#include "../include/SceneManagerEnumerator.h"
#include "../include/SceneManager.h"
#include "../include/Scene.h"

template<> LostPeterEngine::SceneDataManager* LostPeterFoundation::FSingleton<LostPeterEngine::SceneDataManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    SceneDataManager* SceneDataManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	SceneDataManager& SceneDataManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "SceneDataManager::GetSingleton")
		return (*ms_Singleton);     
	}

    SceneDataManager::SceneDataManager()
        : ResourceManager(E_GetResourceTypeName(E_Resource_Scene), E_Resource_Scene)
    {
        m_pSceneDataSerializer = new SceneDataSerializer(this);
    }

    SceneDataManager::~SceneDataManager()
    {
        Destroy();
    }

    void SceneDataManager::Destroy()
    {
        F_DELETE(m_pSceneDataSerializer)

        ResourceManager::Destroy();
    }

    SceneData* SceneDataManager::NewSceneData(uint32 nGroup, const String& strName, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
	{
		SceneData* pSceneData = (SceneData*)createImpl(nGroup,
                                                       strName,
                                                       strGroupName,
                                                       getNextHandle(),
                                                       false,
                                                       nullptr,
                                                       nullptr);
		return pSceneData;
	}
	bool SceneDataManager::AddSceneData(SceneData* pSceneData)
	{
		if (GetSceneData(pSceneData->GetName(), pSceneData->GetGroupName()))
			return false;

		addImpl(pSceneData);
		ResourceGroupManager::GetSingleton()._NotifyResourceCreated(pSceneData);
		return true;
	}
	SceneData* SceneDataManager::LoadSceneData(uint32 nGroup, const String& strName, bool bIsFromFile, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
	{
		if (m_pSceneDataSerializer == nullptr)
            return nullptr;

        SceneData* pSceneData = GetSceneData(strName, strGroupName);
        if (pSceneData != nullptr)
        {
            pSceneData->AddRef();
        }
        else
        {
			pSceneData = NewSceneData(nGroup, strName, strGroupName);
			if (bIsFromFile)
			{
				if (!Parser(nGroup, strName, pSceneData))
				{
					delete pSceneData;
					return nullptr;
				}
				AddSceneData(pSceneData);
			}
			else
			{
				Scene* pSceneDefault = SceneManagerEnumerator::GetSingleton().GetScene_Default();
				pSceneData->SerializerFrom(pSceneDefault->GetSceneData());
				pSceneData->SetPath(pSceneDefault->GetSceneData()->GetPath());
			}
        }
        return pSceneData;
	}
	void SceneDataManager::UnloadSceneData(SceneData* pSceneData)
	{
		if (!pSceneData)
            return;
        Delete(pSceneData);
	}

	bool SceneDataManager::HasSceneData(const String& strName)
	{
		return GetResourceByName(strName) != nullptr;
	}	
	bool SceneDataManager::HasSceneData(const String& strName, const String& strGroupName)
	{
		return GetResourceByName(strName, strGroupName) != nullptr;
	}
	SceneData* SceneDataManager::GetSceneData(const String& strName)
	{
		Resource* pResource = GetResourceByName(strName);
        if (pResource == nullptr)
            return nullptr;
        return (SceneData*)pResource;
	}
	SceneData* SceneDataManager::GetSceneData(const String& strName, const String& strGroupName)
	{
		Resource* pResource = GetResourceByName(strName, strGroupName);
        if (pResource == nullptr)
            return nullptr;
        return (SceneData*)pResource;
	}

	ResourceCreateOrRetrieveResult SceneDataManager::CreateOrRetrieveSceneData(const String& strPath,
                                                                               uint32 nGroup, 
                                                                               const String& strName, 
                                                                               const String& strGroupName, 
                                                                               bool bIsManualLoad /*= false*/,
                                                                               ResourceManualLoader* pManualLoader /*= nullptr*/, 
                                                                               const NameValuePairMap* pLoadParams /*= nullptr*/)
    {
        NameValuePairMap mapMeshParam;
        if (bIsManualLoad && pLoadParams)
        {
            FUtil::CopyNameValuePairMapTo(pLoadParams, &mapMeshParam);
        }

        ResourceCreateOrRetrieveResult result = ResourceManager::CreateOrRetrieve(nGroup,
                                                                                  strName,
                                                                                  strGroupName,
                                                                                  bIsManualLoad,
                                                                                  pManualLoader,
                                                                                  &mapMeshParam);
		if (!result.first || !result.second)
		{
            F_LogError("*********************** SceneDataManager::CreateOrRetrieveSceneData: CreateOrRetrieve resource failed, group: [%d], name: [%s]", nGroup, strName.c_str());
			return result;
		}

        SceneData* pSceneData = (SceneData*)result.first;
        pSceneData->SetPath(strPath);

		return result;
    }

    SceneData* SceneDataManager::Prepare(const String& strPath,
                                         uint32 nGroup, 
                                         const String& strName, 
                                         const String& strGroupName)
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieveSceneData(strPath,
                                                                          nGroup,
                                                                          strName,
                                                                          strGroupName,
                                                                          false,
                                                                          nullptr,
                                                                          nullptr);
		SceneData* pSceneData = (SceneData*)result.first;
        if (!pSceneData)
            return nullptr;
		pSceneData->Prepare();

		return pSceneData;
    }

    SceneData* SceneDataManager::CreateSceneData(const String& strPath,
                                                 uint32 nGroup, 
                                                 const String& strName, 
                                                 const String& strGroupName)         
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieveSceneData(strPath,
                                                                          nGroup,
                                                                          strName,
                                                                          strGroupName,
                                                                          false,
                                                                          nullptr,
                                                                          nullptr);
		SceneData* pSceneData = (SceneData*)result.first;
        if (!pSceneData)
            return nullptr;
		pSceneData->Load();

        pSceneData->AddRef();
		return pSceneData;
    }       

	Resource* SceneDataManager::createImpl(uint32 nGroup,
                                           const String& strName,
                                           const String& strGroupName,
                                           ResourceHandle nHandle, 
                                           bool bIsManualLoad,
                                           ResourceManualLoader* pManualLoader, 
                                           const NameValuePairMap* pLoadParams)
    {
        return new SceneData(this,
                             nGroup, 
                             strName,
                             strGroupName,
                             nHandle,
                             bIsManualLoad,
                             pManualLoader);
    }


    bool SceneDataManager::Parser(uint32 nGroup, const String& strName, SceneData* pSceneData)
    {
        return m_pSceneDataSerializer->Parser(nGroup, strName, pSceneData, nullptr);
    }
    bool SceneDataManager::Parser(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->Parser(nGroup, strName, nullptr, pRet);
    }

    bool SceneDataManager::ParserXML(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserXML(nGroup, strName, pRet);
    }
    bool SceneDataManager::ParserXML(uint32 nGroup, const String& strName, const String& strPath, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserXML(nGroup, strName, strPath, nullptr, pRet);
    }

    bool SceneDataManager::ParserBinary(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserBinary(nGroup, strName, pRet);
    }
    bool SceneDataManager::ParserBinary(uint32 nGroup, const String& strName, const String& strPath, SceneDataPtrVector* pRet /*= nullptr*/)
    {
        return m_pSceneDataSerializer->ParserBinary(nGroup, strName, strPath, nullptr, pRet);
    }

    bool SceneDataManager::SaveXML(Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveXML(pScene);
    }
    bool SceneDataManager::SaveXML(uint32 nGroup, Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveXML(nGroup, pScene);
    }
    bool SceneDataManager::SaveXML(const String& strPath, ScenePtrVector& aSA)
    {
        return m_pSceneDataSerializer->SaveXML(strPath, aSA);
    }

    bool SceneDataManager::SaveBinary(Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveBinary(pScene);
    }
    bool SceneDataManager::SaveBinary(uint32 nGroup, Scene* pScene)
    {
        return m_pSceneDataSerializer->SaveBinary(nGroup, pScene);
    }
    bool SceneDataManager::SaveBinary(const String& strPath, ScenePtrVector& aSA)
    {
        return m_pSceneDataSerializer->SaveBinary(strPath, aSA);
    }


}; //LostPeterEngine