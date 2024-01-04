/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-31
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ResourceGroupManager.h"
#include "../include/ResourceManager.h"
#include "../include/Resource.h"
#include "../include/ResourceGroupListener.h"
#include "../include/ResourceLoadingListener.h"
#include "../include/SceneManager.h"

template<> LostPeterEngine::ResourceGroupManager* LostPeterFoundation::FSingleton<LostPeterEngine::ResourceGroupManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    ////////////////////////// ResourceDeclaration /////////////////


    ////////////////////////// ResourceLocation ////////////////////


    ////////////////////////// ResourceGroup ///////////////////////
    void ResourceGroup::AddToIndex(const String& strResourceName, FArchive* pArchive)
	{
		this->mapIndexCaseSensitive[strResourceName] = pArchive;
		if (!pArchive->IsCaseSensitive())
		{
			String lcase = strResourceName;
			FUtilString::ToLowerCase(lcase);
			this->mapIndexCaseInsensitive[lcase] = pArchive;
		}
	}

	void ResourceGroup::RemoveFromIndex(const String& strResourceName, FArchive* pArchive)
	{
		ResourceLocationPtrMap::iterator itFind = this->mapIndexCaseSensitive.find(strResourceName);
		if (itFind != this->mapIndexCaseSensitive.end() && itFind->second == pArchive)
			this->mapIndexCaseSensitive.erase(itFind);

		if (!pArchive->IsCaseSensitive())
		{
			String lcase = strResourceName;
			FUtilString::ToLowerCase(lcase);
			itFind = this->mapIndexCaseInsensitive.find(strResourceName);
			if (itFind != this->mapIndexCaseInsensitive.end() && itFind->second == pArchive)
				this->mapIndexCaseInsensitive.erase(itFind);
		}
	}

	void ResourceGroup::RemoveFromIndex(FArchive* pArchive)
	{
		ResourceLocationPtrMap::iterator it, itEnd;
		itEnd = this->mapIndexCaseInsensitive.end();
		for (it = this->mapIndexCaseInsensitive.begin(); it != itEnd;)
		{
			if (it->second == pArchive)
			{
				ResourceLocationPtrMap::iterator itDel = it++;
				this->mapIndexCaseInsensitive.erase(itDel);
			}
			else
			{
				++it;
			}
		}
		itEnd = this->mapIndexCaseSensitive.end();
		for (it = this->mapIndexCaseSensitive.begin(); it != itEnd;)
		{
			if (it->second == pArchive)
			{
				ResourceLocationPtrMap::iterator itDel = it++;
				this->mapIndexCaseSensitive.erase(itDel);
			}
			else
			{
				++it;
			}
		}
	}


    ////////////////////////// ResourceGroupManager ////////////////
    ResourceGroupManager* ResourceGroupManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ResourceGroupManager& ResourceGroupManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ResourceGroupManager::GetSingleton")
		return (*ms_Singleton);     
	}

    String	ResourceGroupManager::ms_strNameResourceGroup_Default = "Default";	
	String	ResourceGroupManager::ms_strNameResourceGroup_Internal	= "Internal";	
	String	ResourceGroupManager::ms_strNameResourceGroup_Bootstrap	= "Bootstrap";		
	String	ResourceGroupManager::ms_strNameResourceGroup_AutoDetect = "Autodetect";
	size_t	ResourceGroupManager::ms_nResourceSysRefCounts = 3;
    ResourceGroupManager::ResourceGroupManager()
        : Base("ResourceGroupManager")
        , m_pResourceLoadingListener(nullptr)
        , m_pResourceGroupCurrent(nullptr)
    {
		CreateResourceGroup(ms_strNameResourceGroup_Default);
		CreateResourceGroup(ms_strNameResourceGroup_Internal);
		CreateResourceGroup(ms_strNameResourceGroup_AutoDetect);    

		m_strWorldGroupName = ms_strNameResourceGroup_Default;
    }

    ResourceGroupManager::~ResourceGroupManager()
    {
		Destroy();
    }

	void ResourceGroupManager::Destroy()
	{
        for (ResourceGroupPtrMap::iterator it = m_mapResourceGroup.begin(); 
			 it != m_mapResourceGroup.end(); ++it)
		{
			DeleteGroup(it->second);
		}
		m_mapResourceGroup.clear();
	}

    ResourceGroup* ResourceGroupManager::GetResourceGroup(const String& strGroupName)
	{
		ResourceGroupPtrMap::iterator itFind = m_mapResourceGroup.find(strGroupName);
		if (itFind != m_mapResourceGroup.end())
		{
			return itFind->second;
		}
		return nullptr;
	}

	bool ResourceGroupManager::CreateResourceGroup(const String& strGroupName, const bool bIsInGlobalPool /*= true*/)
	{
        ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (pResourceGroup != nullptr)
		{
            F_LogError("*********************** ResourceGroupManager::CreateResourceGroup: ResourceGroup with the nameGroup: [%s] already exists !", strGroupName.c_str());
            return false;
		}
		pResourceGroup = new ResourceGroup;
		pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Uninitialized;
		pResourceGroup->strGroupName = strGroupName;
		pResourceGroup->bIsInGlobalPool = bIsInGlobalPool;
//		pResourceGroup->pWorldGeometrySceneManager = nullptr;
		m_mapResourceGroup.insert(ResourceGroupPtrMap::value_type(strGroupName, pResourceGroup));

        F_LogInfo("ResourceGroupManager::CreateResourceGroup: ResourceGroup with the nameGroup: [%s] created success !", strGroupName.c_str());
        return true;
    }

	bool ResourceGroupManager::InitializeResourceGroup(const String& strGroupName)
	{
        ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (pResourceGroup == nullptr)
		{
            F_LogError("*********************** ResourceGroupManager::InitializeResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}   

		if (pResourceGroup->eResourceGroupStatus == E_ResourceGroupStatus_Uninitialized)
		{
			pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Initializing;
			ParseResourceGroupScripts(pResourceGroup);
            {
                m_pResourceGroupCurrent = pResourceGroup;
            }
			CreateDeclaredResources(pResourceGroup);
			pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Initialized;
			m_pResourceGroupCurrent = nullptr;
		}

        F_LogInfo("ResourceGroupManager::InitializeResourceGroup: ResourceGroup with the nameGroup: [%s] initialize success !", strGroupName.c_str());
        return true;
	}

	void ResourceGroupManager::InitializeAllResourceGroups()
	{
		for (ResourceGroupPtrMap::iterator it = m_mapResourceGroup.begin(); 
             it != m_mapResourceGroup.end(); ++it)
		{
			ResourceGroup* pResourceGroup = it->second;
			if (pResourceGroup->eResourceGroupStatus == E_ResourceGroupStatus_Uninitialized)
			{
				pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Initializing;
				m_pResourceGroupCurrent = pResourceGroup;
				ParseResourceGroupScripts(pResourceGroup);
				CreateDeclaredResources(pResourceGroup);
				pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Initialized;
				m_pResourceGroupCurrent = nullptr;
			}
		}
	}

	bool ResourceGroupManager::PrepareResourceGroup(const String& strGroupName,
                                                    bool bIsPrepareMainResources /*= true*/, 
													bool bIsPrepareWorldGeometry /*= true*/)
	{
        F_LogInfo("ResourceGroupManager::PrepareResourceGroup: Start to preparing resource group: [%s], bIsPrepareMainResources: [%s], bIsPrepareWorldGeometry: [%s] !", 
                  strGroupName.c_str(),
                  bIsPrepareMainResources ? "true" : "false",
                  bIsPrepareWorldGeometry ? "true" : "false");
        {
            ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
            if (!pResourceGroup)
            {
                F_LogError("*********************** ResourceGroupManager::PrepareResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
                return false;
            }

            m_pResourceGroupCurrent = pResourceGroup;

            size_t nResourceCount = 0;
            if (bIsPrepareMainResources)
            {
                for (ResourceLoadOrderMap::iterator it = pResourceGroup->mapResourceLoadOrder.begin(); 
                    it != pResourceGroup->mapResourceLoadOrder.end(); ++it)
                {
                    nResourceCount += it->second->size();
                }
            }
            
    //		if (pResourceGroup->pWorldGeometrySceneManager && bIsPrepareWorldGeometry)
    //		{
    //			nResourceCount += pResourceGroup->pWorldGeometrySceneManager->estimateWorldGeometry(pResourceGroup->strWorldGeometry);
    //		}

            FireResourceGroupPrepareStarted(strGroupName, nResourceCount);
            {
                if (bIsPrepareMainResources)
                {
                    for (ResourceLoadOrderMap::iterator it = pResourceGroup->mapResourceLoadOrder.begin();
                         it != pResourceGroup->mapResourceLoadOrder.end(); ++it)
                    {
                        size_t n = 0;
                        ResourcePtrList* pListResource = it->second;
                        for (ResourcePtrList::iterator itList = pListResource->begin();
                             itList != pListResource->end(); ++itList, ++n)
                        {
                            Resource* pResource = *itList;
                            FireResourceLoadStarted(pResource);
                            {
                                pResource->Prepare();
                                if (pResource->GetGroupName() != strGroupName)
                                {
                                    itList = pListResource->begin();
                                    std::advance(itList, n);
                                }
                            }
                            FireResourceLoadEnded();
                        }
                    }
                }
                
        //		if (pResourceGroup->pWorldGeometrySceneManager && bIsPrepareWorldGeometry)
        //		{
        //			pResourceGroup->pWorldGeometrySceneManager->PrepareWorldGeometry(pResourceGroup->strWorldGeometry);
        //		}
            }
            FireResourceGroupPrepareEnded(strGroupName);
            m_pResourceGroupCurrent = nullptr;
        }
		F_LogInfo("ResourceGroupManager::PrepareResourceGroup: End to preparing resource group: [%s], bIsPrepareMainResources: [%s], bIsPrepareWorldGeometry: [%s] !", 
                  strGroupName.c_str(),
                  bIsPrepareMainResources ? "true" : "false",
                  bIsPrepareWorldGeometry ? "true" : "false");
        return true;
	}

	bool ResourceGroupManager::LoadResourceGroup(const String& strGroupName,
                                                 bool bIsLoadMainResources /*= true*/, 
												 bool bIsLoadWorldGeometry /*= true*/)
	{
        F_LogInfo("ResourceGroupManager::LoadResourceGroup: Start to load resource group: [%s], bIsLoadMainResources: [%s], bIsLoadWorldGeometry: [%s] !", 
                  strGroupName.c_str(),
                  bIsLoadMainResources ? "true" : "false",
                  bIsLoadWorldGeometry ? "true" : "false");
        {
            ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
            if (!pResourceGroup)
            {
                F_LogError("*********************** ResourceGroupManager::LoadResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
                return false;
            }

            m_pResourceGroupCurrent = pResourceGroup;

            size_t nResourceCount = 0;
            if (bIsLoadMainResources)
            {
                for (ResourceLoadOrderMap::iterator it = pResourceGroup->mapResourceLoadOrder.begin(); 
                     it != pResourceGroup->mapResourceLoadOrder.end(); ++it)
                {
                    nResourceCount += it->second->size();
                }
            }   

    //		if (pResourceGroup->pWorldGeometrySceneManager && bIsLoadWorldGeometry)
    //		{
    //			nResourceCount += pResourceGroup->pWorldGeometrySceneManager->EstimateWorldGeometry(pResourceGroup->strWorldGeometry);
    //		}

            FireResourceGroupLoadStarted(strGroupName, nResourceCount);
            {
                if (bIsLoadMainResources)
                {
                    for (ResourceLoadOrderMap::iterator it = pResourceGroup->mapResourceLoadOrder.begin(); 
                         it != pResourceGroup->mapResourceLoadOrder.end(); ++it)
                    {
                        size_t n = 0;
                        ResourcePtrList* pListResource = it->second;
                        for (ResourcePtrList::iterator itList = pListResource->begin();
                             itList != pListResource->end(); ++itList, ++n)
                        {
                            Resource* pResource = *itList;
                            FireResourceLoadStarted(pResource);
                            {
                                 pResource->Load();
                                if (pResource->GetGroupName() != strGroupName)
                                {
                                    itList = pListResource->begin();
                                    std::advance(itList, n);
                                }
                            }
                            FireResourceLoadEnded();
                        }
                    }
                }
                
        //		if (pResourceGroup->pWorldGeometrySceneManager && bIsLoadWorldGeometry)
        //		{
        //			pResourceGroup->pWorldGeometrySceneManager->SetWorldGeometry(pResourceGroup->strWorldGeometry);
        //		}
            }
            FireResourceGroupLoadEnded(strGroupName);
            pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Loaded;
            m_pResourceGroupCurrent = nullptr;
        }
        F_LogInfo("ResourceGroupManager::LoadResourceGroup: End to load resource group: [%s], bIsLoadMainResources: [%s], bIsLoadWorldGeometry: [%s] !", 
                  strGroupName.c_str(),
                  bIsLoadMainResources ? "true" : "false",
                  bIsLoadWorldGeometry ? "true" : "false");
        return true;
	}

	bool ResourceGroupManager::UnloadResourceGroup(const String& strGroupName, bool bIsReloadableOnly /*= true*/)
	{
        F_LogInfo("ResourceGroupManager::UnloadResourceGroup: Start to unload resource group: [%s], bIsReloadableOnly: [%s] !", 
                  strGroupName.c_str(),
                  bIsReloadableOnly ? "true" : "false");
        {
            ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
            if (!pResourceGroup)
            {
                F_LogError("*********************** ResourceGroupManager::UnloadResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
                return false;
            }
            
            m_pResourceGroupCurrent = pResourceGroup;
            for (ResourceLoadOrderMap::reverse_iterator it = pResourceGroup->mapResourceLoadOrder.rbegin(); 
                 it != pResourceGroup->mapResourceLoadOrder.rend(); ++it)
            {
                ResourcePtrList* pListResource = it->second;
                for (ResourcePtrList::iterator itList = pListResource->begin();
                     itList != pListResource->end(); ++itList)
                {
                    Resource* pResource = *itList;
                    if (!bIsReloadableOnly || pResource->IsReloadable())
                    {
                        pResource->Unload();
                    }
                }
            }
            pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Initialized;
            m_pResourceGroupCurrent = nullptr;
        }
        F_LogInfo("ResourceGroupManager::UnloadResourceGroup: End to unload resource group: [%s], bIsReloadableOnly: [%s] !", 
                  strGroupName.c_str(),
                  bIsReloadableOnly ? "true" : "false");
		return true;
	}

	bool ResourceGroupManager::UnloadUnreferencedResourcesInGroup(const String& strGroupName, bool bIsReloadableOnly /*= true*/)
	{
        F_LogInfo("ResourceGroupManager::UnloadUnreferencedResourcesInGroup: Start to unload unreferenced resources in group: [%s], bIsReloadableOnly: [%s] !", 
                  strGroupName.c_str(),
                  bIsReloadableOnly ? "true" : "false");
        {
            ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
            if (!pResourceGroup)
            {
                F_LogError("*********************** ResourceGroupManager::UnloadUnreferencedResourcesInGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
                return false;
            }
            
            m_pResourceGroupCurrent = pResourceGroup;
            for (ResourceLoadOrderMap::reverse_iterator it = pResourceGroup->mapResourceLoadOrder.rbegin(); 
                 it != pResourceGroup->mapResourceLoadOrder.rend(); ++it)
            {
                ResourcePtrList* pListResource = it->second;
                for (ResourcePtrList::iterator itList = pListResource->begin();
                     itList != pListResource->end(); ++itList)
                {
                    Resource* pResource = *itList;
                    if (pResource->GetRef() == ms_nResourceSysRefCounts)
                    {
                        if (!bIsReloadableOnly || pResource->IsReloadable())
                        {
                            pResource->Unload();
                        }
                    }
                }
            }
            pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Initialized;
            m_pResourceGroupCurrent = nullptr;
        }
        F_LogInfo("ResourceGroupManager::UnloadUnreferencedResourcesInGroup: End to unload unreferenced resources in group: [%s], bIsReloadableOnly: [%s] !", 
                  strGroupName.c_str(),
                  bIsReloadableOnly ? "true" : "false");
        return true;
	}

	bool ResourceGroupManager::ClearResourceGroup(const String& strGroupName)
	{
        F_LogInfo("ResourceGroupManager::ClearResourceGroup: Start to clear resource group: [%s] !", 
                  strGroupName.c_str());
        {
            ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
            if (!pResourceGroup)
            {
                F_LogError("*********************** ResourceGroupManager::ClearResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
                return false;
            }

            m_pResourceGroupCurrent = pResourceGroup;
            DropGroupContents(pResourceGroup);
            pResourceGroup->eResourceGroupStatus = E_ResourceGroupStatus_Uninitialized;
            m_pResourceGroupCurrent = nullptr;
        }
         F_LogInfo("ResourceGroupManager::ClearResourceGroup: End to clear resource group: [%s] !", 
                   strGroupName.c_str());
        return true;
	}

	bool ResourceGroupManager::DestroyResourceGroup(const String& strGroupName)
	{
        F_LogInfo("ResourceGroupManager::DestroyResourceGroup: Start to destroy resource group: [%s] !", 
                  strGroupName.c_str());
        {
            ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
            if (!pResourceGroup)
            {
                F_LogError("*********************** ResourceGroupManager::DestroyResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
                return false;
            }

            m_pResourceGroupCurrent = pResourceGroup;
            UnloadResourceGroup(strGroupName, false);
            DropGroupContents(pResourceGroup);
            DeleteGroup(pResourceGroup);
            m_mapResourceGroup.erase(m_mapResourceGroup.find(strGroupName));
            m_pResourceGroupCurrent = nullptr;
        }
        F_LogInfo("ResourceGroupManager::DestroyResourceGroup: End to destroy resource group: [%s] !", 
                  strGroupName.c_str());
        return true;
	}

	bool ResourceGroupManager::IsResourceGroupInitialized(const String& strGroupName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::IsResourceGroupInitialized: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}
		return (pResourceGroup->eResourceGroupStatus != E_ResourceGroupStatus_Uninitialized &&
			    pResourceGroup->eResourceGroupStatus != E_ResourceGroupStatus_Initializing);
	}

	bool ResourceGroupManager::IsResourceGroupLoaded(const String& strGroupName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
			F_LogError("*********************** ResourceGroupManager::IsResourceGroupLoaded: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}
		return (pResourceGroup->eResourceGroupStatus == E_ResourceGroupStatus_Loaded);
	}

	void ResourceGroupManager::AddResourceLocation(const String& strResourceName,
                                                   const String& strLocationType, 
									               const String& strGroupName /*= ms_strNameResourceGroup_Default*/,
                                                   bool bIsRecursive /*= false*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
			CreateResourceGroup(strGroupName);
			pResourceGroup = GetResourceGroup(strGroupName);
		}

		FArchive* pArchive = FArchiveManager::GetSingleton().Load(strResourceName, strLocationType);
		
		ResourceLocation* pResourceLocation = new ResourceLocation;
		pResourceLocation->pArchive	= pArchive;
		pResourceLocation->bRecursive = bIsRecursive;
		pResourceGroup->listResourceLocation.push_back(pResourceLocation);
		
		StringVector* pStringVector = pArchive->Find("*", bIsRecursive);
		for (StringVector::iterator it = pStringVector->begin();
             it != pStringVector->end(); ++it)
		{
			pResourceGroup->mapIndexCaseSensitive[(*it)] = pArchive;
			if (!pArchive->IsCaseSensitive())
			{
				String indexName = (*it);
				FUtilString::ToLowerCase(indexName);
				pResourceGroup->mapIndexCaseInsensitive[indexName] = pArchive;
			}
		}

        F_LogInfo("ResourceGroupManager::AddResourceLocation: Added resource location: file: [%s], location: [%s], bIsRecursive: [%s] to resource group: [%s] success !", 
                  strResourceName.c_str(),
                  strLocationType.c_str(),
                  bIsRecursive ? "true" : "false",
                  strGroupName.c_str());
	}

	bool ResourceGroupManager::RemoveResourceLocation(const String& strResourceName,
                                                      const String& strGroupName /*= ms_strNameResourceGroup_Default*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::RemoveResourceLocation: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}

		ResourceLocationPtrList::iterator itLocation, itLocationEnd;
		itLocationEnd = pResourceGroup->listResourceLocation.end();
		for (itLocation = pResourceGroup->listResourceLocation.begin(); 
             itLocation != itLocationEnd; ++itLocation)
		{
			FArchive* pArchive = (*itLocation)->pArchive;
			if (pArchive->GetName() == strResourceName)
			{
				ResourceLocationPtrMap::iterator it, itEnd;
				itEnd = pResourceGroup->mapIndexCaseInsensitive.end();
				for (it = pResourceGroup->mapIndexCaseInsensitive.begin(); 
                     it != itEnd;)
				{
					if (it->second == pArchive)
					{
						ResourceLocationPtrMap::iterator itDel = it++;
						pResourceGroup->mapIndexCaseSensitive.erase(itDel);
					}
					else
					{
						++it;
					}
				}
				itEnd = pResourceGroup->mapIndexCaseSensitive.end();
				for (it = pResourceGroup->mapIndexCaseSensitive.begin(); it != itEnd;)
				{
					if (it->second == pArchive)
					{
						ResourceLocationPtrMap::iterator itDel = it++;
						pResourceGroup->mapIndexCaseSensitive.erase(itDel);
					}
					else
					{
						++it;
					}
				}
				
				delete (*itLocation);
				pResourceGroup->listResourceLocation.erase(itLocation);
				break;
			}
		}
        
        F_LogInfo("ResourceGroupManager::AddResourceLocation: Removed resource location: file: [%s] from resource group: [%s] success !", 
                  strResourceName.c_str(),
                  strGroupName.c_str());
        return true;
	}

	void ResourceGroupManager::DeclareResource(const String& strResourceName,
                                               const String& strResourceType,
											   const String& strGroupName /*= ms_strNameResourceGroup_Default*/,
											   const NameValuePairMap& mapParameters /*= NameValuePairMap()*/)
	{
		DeclareResource(strResourceName,
                        strResourceType,
                        strGroupName,
                        nullptr,
                        mapParameters);
	}

	void ResourceGroupManager::DeclareResource(const String& strResourceName,
                                               const String& strResourceType,
											   const String& strGroupName,
                                               ResourceManualLoader* pManualLoader,
											   const NameValuePairMap& mapParameters /*= NameValuePairMap()*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::DeclareResource: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return;
		}

		ResourceDeclaration resourceDecl;
        resourceDecl.strResourceName = strResourceName;
		resourceDecl.strResourceType = strResourceType;
		resourceDecl.pManualLoader = pManualLoader;
		resourceDecl.mapParameters = mapParameters;
		pResourceGroup->listDeclaration.push_back(resourceDecl);
	}

	void ResourceGroupManager::UndeclareResource(const String& strResourceName,
                                                 const String& strGroupName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
			F_LogError("*********************** ResourceGroupManager::UndeclareResource: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return;
		}

		for (ResourceDeclarationList::iterator it = pResourceGroup->listDeclaration.begin();
			 it != pResourceGroup->listDeclaration.end(); ++it)
		{
			if (it->strResourceName == strResourceName)
			{
				pResourceGroup->listDeclaration.erase(it);
				break;
			}
		}
	}

	FStreamData* ResourceGroupManager::OpenResource(const String& strResourceName, 
												    const String& strGroupName /*= ms_strNameResourceGroup_Default*/,
												    bool bIsSearchGroupsIfNotFound /*= true*/,
                                                    Resource* pResourceBeingLoaded /*= nullptr*/)
	{
        FStreamData* pStream = nullptr;
		if (m_pResourceLoadingListener)
		{
			pStream = m_pResourceLoadingListener->ResourceLoading(strResourceName, 
                                                                  strGroupName, 
                                                                  pResourceBeingLoaded);
			if (pStream != nullptr)
				return pStream;
		}

		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::OpenResource: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return nullptr;
		}

		FArchive* pArchive = nullptr;
		ResourceLocationPtrMap::iterator itFind = pResourceGroup->mapIndexCaseSensitive.find(strResourceName);
		if (itFind != pResourceGroup->mapIndexCaseSensitive.end())
		{
			pArchive = itFind->second;
			pStream = pArchive->Open(strResourceName);
			if (m_pResourceLoadingListener)
				m_pResourceLoadingListener->ResourceStreamOpened(strResourceName,
                                                                 strGroupName,
                                                                 pResourceBeingLoaded,
                                                                 pStream);
			return pStream;
		}
		else 
		{
			String strResourceNameLower = strResourceName;
			FUtilString::ToLowerCase(strResourceNameLower);
			itFind = pResourceGroup->mapIndexCaseInsensitive.find(strResourceNameLower);
			if (itFind != pResourceGroup->mapIndexCaseInsensitive.end())
			{
				pArchive = itFind->second;
				pStream = pArchive->Open(strResourceName);
				if (m_pResourceLoadingListener)
					m_pResourceLoadingListener->ResourceStreamOpened(strResourceName,
                                                                     strGroupName,
                                                                     pResourceBeingLoaded,
                                                                     pStream);
				return pStream;
			}
			else
			{
				ResourceLocationPtrList::iterator itLocation, itLocationEnd;
				itLocationEnd = pResourceGroup->listResourceLocation.end();
				for (itLocation = pResourceGroup->listResourceLocation.begin(); itLocation != itLocationEnd; ++itLocation)
				{
					FArchive* pArchive = (*itLocation)->pArchive;
					if (pArchive->Exists(strResourceName))
					{
						pStream = pArchive->Open(strResourceName);
						if (m_pResourceLoadingListener)
							m_pResourceLoadingListener->ResourceStreamOpened(strResourceName,
                                                                             strGroupName,
                                                                             pResourceBeingLoaded,
                                                                             pStream);
						return pStream;
					}
				}
			}
		}

		if (bIsSearchGroupsIfNotFound)
		{
			ResourceGroup* pResourceGroup = FindGroupContainingResourceImpl(strResourceName); 
			if (pResourceGroup)
			{
				if (pResourceBeingLoaded)
				{
					pResourceBeingLoaded->ChangeGroupOwnership(pResourceGroup->strGroupName);
				}
				return OpenResource(strResourceName,
                                    pResourceGroup->strGroupName,
                                    false);
			}
		}

        F_LogError("*********************** ResourceGroupManager::OpenResource: ResourceGroup has no resource: [%s], group: [%s] not exists !", 
                   strResourceName.c_str(),
                   strGroupName.c_str());
		return nullptr;
	}

	FStreamDataPtrList* ResourceGroupManager::OpenResources(const String& strPattern,
                                                            const String& strGroupName /*= ms_strNameResourceGroup_Default*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::OpenResources: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return nullptr;
		}

		FStreamDataPtrList* pListStream = new FStreamDataPtrList;
		ResourceLocationPtrList::iterator itLocation, itLocationEnd;
		itLocationEnd = pResourceGroup->listResourceLocation.end();
		for (itLocation = pResourceGroup->listResourceLocation.begin(); itLocation != itLocationEnd; ++itLocation)
		{
			FArchive* pArchive = (*itLocation)->pArchive;
			StringVector* pNames = pArchive->Find(strPattern, (*itLocation)->bRecursive);
			for (StringVector::iterator it = pNames->begin(); 
                 it != pNames->end(); ++it)
			{
				FStreamData* pStream = pArchive->Open(*it);
				if (pStream != nullptr)
				{
					pListStream->push_back(pStream);
				}
			}
		}
		return pListStream;
	}

	StringVector* ResourceGroupManager::ListResourceNames(const String& strGroupName,
                                                          bool bDirs /*= false*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::ListResourceNames: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return nullptr;
		}

        StringVector* pResourceNames = new StringVector;
		ResourceLocationPtrList::iterator it, itEnd;
		itEnd = pResourceGroup->listResourceLocation.end();
		for (it = pResourceGroup->listResourceLocation.begin(); 
             it != itEnd; ++it)
		{
			StringVector* pNames = (*it)->pArchive->ListFile((*it)->bRecursive, bDirs);
			pResourceNames->insert(pResourceNames->end(), pNames->begin(), pNames->end());
		}
		return pResourceNames;
	}

	FFileInfoVector* ResourceGroupManager::ListResourceFileInfo(const String& strGroupName,
                                                                bool bDirs /*= false*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::ListResourceFileInfo: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return nullptr;
		}

        FFileInfoVector* pFileInfoVector = new FFileInfoVector;
		ResourceLocationPtrList::iterator it, itEnd;
		itEnd = pResourceGroup->listResourceLocation.end();
		for (it = pResourceGroup->listResourceLocation.begin(); 
             it != itEnd; ++it)
		{
			FFileInfoVector* p = (*it)->pArchive->ListFileInfo((*it)->bRecursive, bDirs);
			pFileInfoVector->insert(pFileInfoVector->end(), p->begin(), p->end());
		}
		return pFileInfoVector;
	}

	StringVector* ResourceGroupManager::FindResourceNames(const String& strGroupName,
                                                          const String& strPattern,
														  bool bDirs /*= false*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::FindResourceNames: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return nullptr;
		}

        StringVector* pResourceNames = new StringVector;
		ResourceLocationPtrList::iterator it, itEnd;
		itEnd = pResourceGroup->listResourceLocation.end();
		for (it = pResourceGroup->listResourceLocation.begin(); 
             it != itEnd; ++it)
		{
			StringVector* p = (*it)->pArchive->Find(strPattern, (*it)->bRecursive, bDirs);
			pResourceNames->insert(pResourceNames->end(), p->begin(), p->end());
		}
		return pResourceNames;
	}

	FFileInfoVector* ResourceGroupManager::FindResourceFileInfo(const String& strGroupName,
                                                                const String& strPattern,
																bool bDirs /*= false*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::FindResourceFileInfo: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return nullptr;
		}

		FFileInfoVector* pFileInfoVector = new FFileInfoVector;
		ResourceLocationPtrList::iterator it, itEnd;
		itEnd = pResourceGroup->listResourceLocation.end();
		for (it = pResourceGroup->listResourceLocation.begin(); 
             it != itEnd; ++it)
		{
			FFileInfoVector* p = (*it)->pArchive->FindFileInfo(strPattern, (*it)->bRecursive, bDirs);
			pFileInfoVector->insert(pFileInfoVector->end(), p->begin(), p->end());
		}
		return pFileInfoVector;
	}

	bool ResourceGroupManager::ResourceExists(const String& strGroupName, const String& strResourceName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
			F_LogError("*********************** ResourceGroupManager::ResourceExists: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}
		return ResourceExists(pResourceGroup, strResourceName);
	}

	bool ResourceGroupManager::ResourceExists(ResourceGroup* pResourceGroup, const String& strResourceName)
	{
		ResourceLocationPtrMap::iterator itFind = pResourceGroup->mapIndexCaseSensitive.find(strResourceName);
		if (itFind != pResourceGroup->mapIndexCaseSensitive.end())
		{
			return true;
		}
		else 
		{
			String strResourceNameLower = strResourceName;
			FUtilString::ToLowerCase(strResourceNameLower);
			itFind = pResourceGroup->mapIndexCaseInsensitive.find(strResourceNameLower);
			if (itFind != pResourceGroup->mapIndexCaseInsensitive.end())
			{
				return true;
			}
			else
			{
				ResourceLocationPtrList::iterator itLocation, itLocationEnd;
				itLocationEnd = pResourceGroup->listResourceLocation.end();
				for (itLocation = pResourceGroup->listResourceLocation.begin(); 
                     itLocation != itLocationEnd; ++itLocation)
				{
					FArchive* pArchive = (*itLocation)->pArchive;
					if (pArchive->Exists(strResourceName))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	const String& ResourceGroupManager::FindGroupContainingResource(const String& strResourceName)
	{
		ResourceGroup* pResourceGroup = FindGroupContainingResourceImpl(strResourceName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::FindGroupContainingResource: Can not find resource with the name: [%s] !", strResourceName.c_str());
            return FUtilString::BLANK;
		}
		return pResourceGroup->strGroupName;
	}

	time_t ResourceGroupManager::ResourceModifiedTime(const String& strGroupName, const String& strResourceName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::ResourceModifiedTime: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return 0;
		}
		return ResourceModifiedTime(pResourceGroup, strResourceName);
	}

	time_t ResourceGroupManager::ResourceModifiedTime(ResourceGroup* pResourceGroup, const String& strResourceName)
	{
		ResourceLocationPtrMap::iterator itFind = pResourceGroup->mapIndexCaseSensitive.find(strResourceName);
		if (itFind != pResourceGroup->mapIndexCaseSensitive.end())
		{
			return itFind->second->GetModifiedTime(strResourceName);
		}
		else 
		{
			String strResourceNameLower = strResourceName;
			FUtilString::ToLowerCase(strResourceNameLower);
			itFind = pResourceGroup->mapIndexCaseInsensitive.find(strResourceNameLower);
			if (itFind != pResourceGroup->mapIndexCaseInsensitive.end())
			{
				return itFind->second->GetModifiedTime(strResourceName);
			}
			else
			{
				ResourceLocationPtrList::iterator itLocation, itLocationEnd;
				itLocationEnd = pResourceGroup->listResourceLocation.end();
				for (itLocation = pResourceGroup->listResourceLocation.begin(); 
                     itLocation != itLocationEnd; ++itLocation)
				{
					FArchive* pArchive = (*itLocation)->pArchive;
					time_t testTime = pArchive->GetModifiedTime(strResourceName);
                        
					if (testTime > 0)
					{
						return testTime;
					}
				}
			}
		}
		return 0;
	}
	
	FStreamData* ResourceGroupManager::CreateResource(const String& strResourceName, 
                                                      const String& strGroupName /*= ms_strNameResourceGroup_Default*/, 
                                                      bool bIsOverwrite /*= false*/, 
                                                      const String& strLocationPattern /*= FUtilString::BLANK*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
			F_LogError("*********************** ResourceGroupManager::CreateResource: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return nullptr;
		}

		for (ResourceLocationPtrList::iterator itLocation = pResourceGroup->listResourceLocation.begin(); 
			 itLocation != pResourceGroup->listResourceLocation.end(); ++itLocation)
		{
			FArchive* pArchive = (*itLocation)->pArchive;
			if (!pArchive->IsReadOnly() && 
				(strLocationPattern.empty() || FUtilString::Match(pArchive->GetName(), strLocationPattern, false)))
			{
				if (!bIsOverwrite && pArchive->Exists(strResourceName))
				{
                    F_LogError("*********************** ResourceGroupManager::CreateResource: Can not overwrite existing file: [%s] !", strResourceName.c_str());
                    return nullptr;
				}

				FStreamData* pStream = pArchive->Create(strResourceName);
				pResourceGroup->AddToIndex(strResourceName, pArchive);
				return pStream;
			}
		}

        F_LogError("*********************** ResourceGroupManager::CreateResource: Can not find resource file: [%s] from grou: [%s] !", strResourceName.c_str(), strGroupName.c_str());
        return nullptr;
	}

	bool ResourceGroupManager::DeleteResource(const String& strResourceName, 
											  const String& strGroupName /*= ms_strNameResourceGroup_Default*/, 
											  const String& strLocationPattern /*= FUtilString::BLANK*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::DeleteResource: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}

		for (ResourceLocationPtrList::iterator itLocation = pResourceGroup->listResourceLocation.begin(); 
			 itLocation != pResourceGroup->listResourceLocation.end(); ++itLocation)
		{
			FArchive* pArchive = (*itLocation)->pArchive;
			if (!pArchive->IsReadOnly() && 
				(strLocationPattern.empty() || FUtilString::Match(pArchive->GetName(), strLocationPattern, false)))
			{
				if (pArchive->Exists(strResourceName))
				{
					pArchive->Remove(strResourceName);
					pResourceGroup->RemoveFromIndex(strResourceName, pArchive);
					return true;
				}
			}
		}
        return false;
	}

	bool ResourceGroupManager::DeleteMatchingResources(const String& strFilePattern, 
													   const String& strGroupName /*= ms_strNameResourceGroup_Default*/, 
													   const String& strLocationPattern /*= FUtilString::BLANK*/)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::DeleteMatchingResources: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}

		for (ResourceLocationPtrList::iterator itLocation = pResourceGroup->listResourceLocation.begin(); 
			 itLocation != pResourceGroup->listResourceLocation.end(); ++itLocation)
		{
			FArchive* pArchive = (*itLocation)->pArchive;
			if (!pArchive->IsReadOnly() && 
				(strLocationPattern.empty() || FUtilString::Match(pArchive->GetName(), strLocationPattern, false)))
			{
				StringVector* pMatchingFiles = pArchive->Find(strFilePattern);
				for (StringVector::iterator it = pMatchingFiles->begin(); 
					 it != pMatchingFiles->end(); ++it)
				{
					pArchive->Remove(*it);
					pResourceGroup->RemoveFromIndex(*it, pArchive);
				}
			}
		}
        return true;
	}

	void ResourceGroupManager::AddResourceGroupListener(ResourceGroupListener* pResourceGroupListener)
	{
		m_aResourceGroupListener.push_back(pResourceGroupListener);
	}

	void ResourceGroupManager::RemoveResourceGroupListener(ResourceGroupListener* pResourceGroupListener)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			if (*it == pResourceGroupListener)
			{
				m_aResourceGroupListener.erase(it);
				break;
			}
		}
	}

	bool ResourceGroupManager::LinkWorldGeometryToResourceGroup(const String& strGroupName,
                                                                const String& strWorldGeometry,
										                        SceneManager* pSceneManager)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::LinkWorldGeometryToResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}

		pResourceGroup->strWorldGeometry = strWorldGeometry;
		pResourceGroup->pWorldGeometrySceneManager = pSceneManager;
        return true;
	}
	bool ResourceGroupManager::UnlinkWorldGeometryFromResourceGroup(const String& strGroupName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::UnlinkWorldGeometryFromResourceGroup: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}

		pResourceGroup->strWorldGeometry = FUtilString::BLANK;
		pResourceGroup->pWorldGeometrySceneManager = nullptr;
        return true;
	}
	
	void ResourceGroupManager::ShutdownAll()
	{
		ResourceManagerPtrMap::iterator it, itEnd;
		itEnd = m_mapResourceManager.end();
		for (it = m_mapResourceManager.begin();
             it != itEnd; ++it)
		{
			it->second->RemoveAll();
		}
	}

	void ResourceGroupManager::_RegisterResourceManager(const String& strResourceType, ResourceManager* pResourceManager)
	{
		m_mapResourceManager[strResourceType] = pResourceManager;
        F_LogInfo("ResourceGroupManager::_RegisterResourceManager: Registering ResourceManager type: [%s] !", strResourceType.c_str());
	}
	void ResourceGroupManager::_UnregisterResourceManager(const String& strResourceType)
	{
		ResourceManagerPtrMap::iterator itFind = m_mapResourceManager.find(strResourceType);
		if (itFind != m_mapResourceManager.end())
		{
			m_mapResourceManager.erase(itFind);
            F_LogInfo("ResourceGroupManager::_RegisterResourceManager: Unregistering ResourceManager type: [%s] !", strResourceType.c_str());
		}
	}

	void ResourceGroupManager::_RegisterScriptLoader(FScriptLoader* pScriptLoader)
	{
		m_mapScriptLoaderOrder.insert(FScriptLoaderOrderMap::value_type(pScriptLoader->GetLoadingOrder(), pScriptLoader));
	}
	void ResourceGroupManager::_UnregisterScriptLoader(FScriptLoader* pScriptLoader)
	{
		float fOrder = pScriptLoader->GetLoadingOrder();
		FScriptLoaderOrderMap::iterator itFind = m_mapScriptLoaderOrder.find(fOrder);
		while (itFind != m_mapScriptLoaderOrder.end() && itFind->first == fOrder)
		{
			if (itFind->second == pScriptLoader)
			{
				FScriptLoaderOrderMap::iterator itDel = itFind++;
				m_mapScriptLoaderOrder.erase(itDel);
			}
			else
			{
				++itFind;
			}
		}
	}

	ResourceManager* ResourceGroupManager::_GetResourceManager(const String& strResourceType)
	{
		ResourceManagerPtrMap::iterator itFind = m_mapResourceManager.find(strResourceType);
		if (itFind == m_mapResourceManager.end())
		{
			return nullptr;
		}
		return itFind->second;
	}

	void ResourceGroupManager::_NotifyResourceCreated(Resource* pResource)
	{
		if (m_pResourceGroupCurrent && pResource->GetGroupName() == m_pResourceGroupCurrent->strGroupName)
		{
			AddCreatedResource(pResource, m_pResourceGroupCurrent);
		}
		else
		{
			ResourceGroup* pResourceGroup = GetResourceGroup(pResource->GetGroupName());
			if (pResourceGroup)
			{
				AddCreatedResource(pResource, pResourceGroup);
			}
		}
	}
	void ResourceGroupManager::_NotifyResourceRemoved(Resource* pResource)
	{
		if (m_pResourceGroupCurrent)
		{
			
		}
		else
		{
			ResourceGroup* pResourceGroup = GetResourceGroup(pResource->GetGroupName());
			if (pResourceGroup)
			{
				ResourceLoadOrderMap::iterator itFind = pResourceGroup->mapResourceLoadOrder.find(pResource->GetResourceManager()->GetLoadingOrder());
				if (itFind != pResourceGroup->mapResourceLoadOrder.end())
				{
					ResourcePtrList* pResourcePtrList = itFind->second;
					for (ResourcePtrList::iterator itList = pResourcePtrList->begin();
						 itList != pResourcePtrList->end(); ++ itList)
					{
						if ((*itList) == pResource)
						{
							pResourcePtrList->erase(itList);
							break;
						}
					}
				}
			}
		}
	}
	void ResourceGroupManager::_NotifyResourceGroupChanged(const String& strGroupNameOld, Resource* pResource)
	{
		ResourceGroup* pResourceGroupNew = GetResourceGroup(pResource->GetGroupName());
		ResourceGroupPtrMap::iterator itGroup = m_mapResourceGroup.find(strGroupNameOld);
		F_Assert(itGroup != m_mapResourceGroup.end() && "ResourceGroupManager::_NotifyResourceGroupChanged")
		ResourceGroup* pResourceGroup = itGroup->second;
		float fOrder = pResource->GetResourceManager()->GetLoadingOrder();
		ResourceLoadOrderMap::iterator itFind = pResourceGroup->mapResourceLoadOrder.find(fOrder);
		F_Assert(itFind != pResourceGroup->mapResourceLoadOrder.end() && "ResourceGroupManager::_NotifyResourceGroupChanged")
		ResourcePtrList* pResourcePtrList = itFind->second;
		for (ResourcePtrList::iterator itList = pResourcePtrList->begin(); 
			 itList != pResourcePtrList->end(); ++itList)
		{
			if ((*itList) == pResource)
			{
				AddCreatedResource(*itList, pResourceGroupNew);
				pResourcePtrList->erase(itList);
				break;
			}
		}
	}
	void ResourceGroupManager::_NotifyAllResourcesRemoved(ResourceManager* pResourceManager)
	{
		for (ResourceGroupPtrMap::iterator itGroup = m_mapResourceGroup.begin();
			 itGroup != m_mapResourceGroup.end(); ++itGroup)
		{
			for (ResourceLoadOrderMap::iterator itLoadOrder = itGroup->second->mapResourceLoadOrder.begin();
				 itLoadOrder != itGroup->second->mapResourceLoadOrder.end(); ++itLoadOrder)
			{
				for (ResourcePtrList::iterator itList = itLoadOrder->second->begin();
					 itList != itLoadOrder->second->end();)
				{
					if ((*itList)->GetResourceManager() == pResourceManager)
					{
						ResourcePtrList::iterator itDel = itList++;
						itLoadOrder->second->erase(itDel);
					}
					else
					{
						++itList;
					}
				}
			}
		}
	}
	void ResourceGroupManager::_NotifyWorldGeometryStageStarted(const String& strDescription)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->WorldGeometryStageStarted(strDescription);
		}
	}
	void ResourceGroupManager::_NotifyWorldGeometryStageEnded()
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->WorldGeometryStageEnded();
		}
	}

	bool ResourceGroupManager::IsResourceGroupInGlobalPool(const String& strGroupName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::IsResourceGroupInGlobalPool: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return false;
		}
		return pResourceGroup->bIsInGlobalPool;
	}

	StringVector ResourceGroupManager::GetResourceGroups()
	{
		StringVector aResourceGroupNames;
		for (ResourceGroupPtrMap::iterator it = m_mapResourceGroup.begin();
		     it != m_mapResourceGroup.end(); ++it)
		{
			aResourceGroupNames.push_back(it->second->strGroupName);
		}
		return aResourceGroupNames;
	}

	ResourceDeclarationList ResourceGroupManager::GetResourceDeclarationList(const String& strGroupName)
	{
		ResourceGroup* pResourceGroup = GetResourceGroup(strGroupName);
		if (!pResourceGroup)
		{
            F_LogError("*********************** ResourceGroupManager::GetResourceDeclarationList: ResourceGroup with the nameGroup: [%s] not exists !", strGroupName.c_str());
            return ResourceDeclarationList();
		}
		return pResourceGroup->listDeclaration;
	}

	ResourceLoadingListener* ResourceGroupManager::GetResourceLoadingListener()
	{
		return m_pResourceLoadingListener;
	}
	void ResourceGroupManager::SetResourceLoadingListener(ResourceLoadingListener* pResourceLoadingListener)
	{
		m_pResourceLoadingListener = pResourceLoadingListener;
	}

	void ResourceGroupManager::ParseResourceGroupScripts(ResourceGroup* pResourceGroup)
	{
        F_LogInfo("ResourceGroupManager::ParseResourceGroupScripts: Start to parsing scripts for resource group: [%s] !", 
                  pResourceGroup->strGroupName.c_str());
        {
            typedef std::list<FFileInfoVector*>	FileListList;
            typedef std::pair<FScriptLoader*, FileListList*> LoaderFileListPair;
            typedef std::list<LoaderFileListPair> ScriptLoaderFileList;
            ScriptLoaderFileList scriptLoaderFileList;
            size_t nScriptCount = 0;

            FScriptLoaderOrderMap::iterator oi;
            for (oi = m_mapScriptLoaderOrder.begin();
                 oi != m_mapScriptLoaderOrder.end(); ++oi)
            {
                FScriptLoader* pScriptLoader = oi->second;
                FileListList* pFileListList = new FileListList;

                const StringVector& aPatterns = pScriptLoader->GetScriptPatterns();
                for (StringVector::const_iterator p = aPatterns.begin(); 
                     p != aPatterns.end(); ++p)
                {
                    FFileInfoVector* pFileList = FindResourceFileInfo(pResourceGroup->strGroupName, *p);
                    nScriptCount += pFileList->size();
                    pFileListList->push_back(pFileList);
                }
                scriptLoaderFileList.push_back(LoaderFileListPair(pScriptLoader, pFileListList));
            }
            

            FireResourceGroupScriptingStarted(pResourceGroup->strGroupName, nScriptCount);
            {
                for (ScriptLoaderFileList::iterator it = scriptLoaderFileList.begin();
                     it != scriptLoaderFileList.end(); ++it)
                {
                    FScriptLoader* pScriptLoader = it->first;
                    for (FileListList::iterator itFileList = it->second->begin(); 
                         itFileList != it->second->end(); ++itFileList)
                    {
                        for (FFileInfoVector::iterator itFile = (*itFileList)->begin(); 
                             itFile != (*itFileList)->end(); ++itFile)
                        {
                            bool bIsSkipScript = false;
                            FireScriptStarted(itFile->strFileName, bIsSkipScript);
                            {
                                if(bIsSkipScript)
                                {
                                    F_LogInfo("ResourceGroupManager::ParseResourceGroupScripts: Skipping script: [%s] !", itFile->strFileName.c_str());
                                }
                                else
                                {
                                    F_LogInfo("ResourceGroupManager::ParseResourceGroupScripts: Parsing script: [%s] !", itFile->strFileName.c_str());

                                    FStreamData* pStream = itFile->pArchive->Open(itFile->strFileName);
                                    if (pStream != nullptr)
                                    {
                                        if (m_pResourceLoadingListener)
                                            m_pResourceLoadingListener->ResourceStreamOpened(itFile->strFileName,
                                                                                             pResourceGroup->strGroupName,
                                                                                             nullptr,
                                                                                             pStream);
                                        pScriptLoader->ParseScript(pStream, pResourceGroup->strGroupName);
                                    }
                                }
                            }
                            FireScriptEnded(itFile->strFileName, bIsSkipScript);
                        }
                    }
                }
            }
            FireResourceGroupScriptingEnded(pResourceGroup->strGroupName);
        }   
        F_LogInfo("ResourceGroupManager::ParseResourceGroupScripts: End to parsing scripts for resource group: [%s] !", 
                  pResourceGroup->strGroupName.c_str());
	}

	void ResourceGroupManager::CreateDeclaredResources(ResourceGroup* pResourceGroup)
	{
		for (ResourceDeclarationList::iterator it = pResourceGroup->listDeclaration.begin();
			 it != pResourceGroup->listDeclaration.end(); ++it)
		{
			ResourceDeclaration& resourceDecl = *it;
			ResourceManager* pResourceManager = _GetResourceManager(resourceDecl.strResourceType);
			
			Resource* pResource = pResourceManager->Create(resourceDecl.strResourceName,
                                                           pResourceGroup->strGroupName,
				                                           resourceDecl.pManualLoader != nullptr, 
                                                           resourceDecl.pManualLoader, 
                                                           &resourceDecl.mapParameters);
		
			ResourceLoadOrderMap::iterator itLocation = pResourceGroup->mapResourceLoadOrder.find(pResourceManager->GetLoadingOrder());
			ResourcePtrList* pResourcePtrList = nullptr;
			if (itLocation == pResourceGroup->mapResourceLoadOrder.end())
			{
				pResourcePtrList = new ResourcePtrList;
				pResourceGroup->mapResourceLoadOrder[pResourceManager->GetLoadingOrder()] = pResourcePtrList;
			}
			else
			{
				pResourcePtrList = itLocation->second;
			}
			pResourcePtrList->push_back(pResource);
		}
	}
	void ResourceGroupManager::AddCreatedResource(Resource* pResource, ResourceGroup* pResourceGroup)
	{
		float fOrder = pResource->GetResourceManager()->GetLoadingOrder();
		ResourceLoadOrderMap::iterator itFind = pResourceGroup->mapResourceLoadOrder.find(fOrder);
		ResourcePtrList* pResourceList = nullptr;
		if (itFind == pResourceGroup->mapResourceLoadOrder.end())
		{
			pResourceList = new ResourcePtrList;
			pResourceGroup->mapResourceLoadOrder[fOrder] = pResourceList;
		}
		else
		{
			pResourceList = itFind->second;
		}
		pResourceList->push_back(pResource);
	}

	void ResourceGroupManager::DropGroupContents(ResourceGroup* pResourceGroup)
	{
		bool bIsGroupSet = false;
		if (!m_pResourceGroupCurrent)
		{
			m_pResourceGroupCurrent = pResourceGroup;
			bIsGroupSet = true;
		}

		ResourceLoadOrderMap::iterator it, itEnd;
		itEnd = pResourceGroup->mapResourceLoadOrder.end();
		for (it = pResourceGroup->mapResourceLoadOrder.begin(); 
             it != itEnd; ++it)
		{
			for (ResourcePtrList::iterator itList = it->second->begin();
				 itList != it->second->end(); ++itList)
			{
				(*itList)->GetResourceManager()->Remove((*itList)->GetHandle());
			}
			delete (it->second);
		}
		pResourceGroup->mapResourceLoadOrder.clear();

		if (bIsGroupSet)
		{
			m_pResourceGroupCurrent = nullptr;
		}
	}
	void ResourceGroupManager::DeleteGroup(ResourceGroup* pResourceGroup)
	{
		ResourceLoadOrderMap::iterator it, itEnd;
		itEnd = pResourceGroup->mapResourceLoadOrder.end();
		for (it = pResourceGroup->mapResourceLoadOrder.begin(); 
             it != itEnd; ++it)
		{
			delete (it->second);
		}

		for (ResourceLocationPtrList::iterator it = pResourceGroup->listResourceLocation.begin();
			 it != pResourceGroup->listResourceLocation.end(); ++it)
		{
			delete (*it);
		}

		delete (pResourceGroup);
	}

	ResourceGroup* ResourceGroupManager::FindGroupContainingResourceImpl(const String& strResourceName)
	{
		for (ResourceGroupPtrMap::iterator it = m_mapResourceGroup.begin();
			it != m_mapResourceGroup.end(); ++it)
		{
			ResourceGroup* pResourceGroup = it->second;
			if (ResourceExists(pResourceGroup, strResourceName))
				return pResourceGroup;
		}
		return nullptr;
	}

	void ResourceGroupManager::FireResourceGroupScriptingStarted(const String& strGroupName, size_t nScriptCount)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceGroupScriptingStarted(strGroupName, nScriptCount);
		}
	}
	void ResourceGroupManager::FireScriptStarted(const String& strScriptName,bool& bIsSkipScript)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			bool temp = false;
			(*it)->ScriptParseStarted(strScriptName, temp);
			if(temp)
				bIsSkipScript = true;
		}
	}
	void ResourceGroupManager::FireScriptEnded(const String& strScriptName, bool bIsSkipped)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ScriptParseEnded(strScriptName, bIsSkipped);
		}
	}
	void ResourceGroupManager::FireResourceGroupScriptingEnded(const String& strGroupName)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceGroupScriptingEnded(strGroupName);
		}
	}
    

	void ResourceGroupManager::FireResourceGroupLoadStarted(const String& strGroupName, size_t nResourceCount)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceGroupLoadStarted(strGroupName, nResourceCount);
		}
	}
	void ResourceGroupManager::FireResourceLoadStarted(const Resource* pResource)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceLoadStarted(pResource);
		}
	}
	void ResourceGroupManager::FireResourceLoadEnded()
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceLoadEnded();
		}
	}
	void ResourceGroupManager::FireResourceGroupLoadEnded(const String& strGroupName)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceGroupLoadEnded(strGroupName);
		}
	}
	

	void ResourceGroupManager::FireResourceGroupPrepareStarted(const String& strGroupName, size_t nResourceCount)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceGroupPrepareStarted(strGroupName, nResourceCount);
		}
	}
	void ResourceGroupManager::FireResourcePrepareStarted(const Resource* pResource)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourcePrepareStarted(pResource);
		}
	}
	void ResourceGroupManager::FireResourcePrepareEnded()
	{	
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourcePrepareEnded();
		}
	}
	void ResourceGroupManager::FireResourceGroupPrepareEnded(const String& strGroupName)
	{
		for (ResourceGroupListenerPtrVector::iterator it = m_aResourceGroupListener.begin();
			 it != m_aResourceGroupListener.end(); ++it)
		{
			(*it)->ResourceGroupPrepareEnded(strGroupName);
		}
	}

}; //LostPeterEngine