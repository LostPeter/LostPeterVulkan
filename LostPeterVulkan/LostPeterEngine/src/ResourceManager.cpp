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

#include "../include/ResourceManager.h"
#include "../include/ResourceGroupManager.h"
#include "../include/Resource.h"
#include "../include/ResourceLoadingListener.h"

namespace LostPeterEngine
{
    ResourceManager::ResourceManager(const String& strName, EResourceType eResource)
        : Base(strName)
        , m_eResource(eResource)
        , m_nNextHandle(1)
		, m_nMemoryUsage(0)
		, m_fLoadingOrder(0)
		, m_bIsVerbose(true)
    {
        m_strResourceType = E_GetResourceTypeName(eResource);
        m_nMemoryBudget = std::numeric_limits<unsigned long>::max();
    }

    ResourceManager::~ResourceManager()
    {

    }   

    void ResourceManager::Destroy()
    {
		DeleteAll();
    }

    Resource* ResourceManager::GetResourceByName(const String& strName, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
	{
		Resource* pResource = nullptr;
		if (!ResourceGroupManager::GetSingleton().IsResourceGroupInGlobalPool(strGroupName))
		{
			ResourcePtrGroupMap::iterator itGroup = m_mapResourceGroup.find(strGroupName);
			if (itGroup != m_mapResourceGroup.end())
			{
				ResourcePtrMap::iterator itFind = itGroup->second.find(strName);
				if (itFind != itGroup->second.end())
				{
					pResource = itFind->second;
				}
			}
		}

		if (pResource == nullptr)
		{
			ResourcePtrMap::iterator itFind = m_mapResource.find(strName);
			if (itFind != m_mapResource.end())
			{
				pResource = itFind->second;
			}
			else
			{
				if (strGroupName == ResourceGroupManager::ms_strNameResourceGroup_AutoDetect)
				{
					for (ResourcePtrGroupMap::iterator it = m_mapResourceGroup.begin(); 
                         it != m_mapResourceGroup.end(); ++it)
					{
						ResourcePtrMap::iterator itMap = it->second.find(strName);
						if (itMap != it->second.end())
						{
							pResource = itMap->second;
							break;
						}
					}
				}
			}
		}
		return pResource;
	}

	Resource* ResourceManager::GetResourceByHandle(ResourceHandle nHandle)
	{
		ResourcePtrHandleMap::iterator itFind = m_mapResourcesByHandle.find(nHandle);
		if (itFind != m_mapResourcesByHandle.end())
		{
			return itFind->second;
		}
		return nullptr;
	}

	Resource* ResourceManager::Create(uint32 nGroup,
									  const String& strName,
                                      const String& strGroupName,
									  bool bIsManualLoad /*= false*/,
                                      ResourceManualLoader* pManualLoader /*= nullptr*/, 
									  const NameValuePairMap* pLoadParams /*= nullptr*/)
	{
        Resource* pResource = createImpl(nGroup,
										 strName,
                                         strGroupName,
										 getNextHandle(),
                                         bIsManualLoad,
                                         pManualLoader,
                                         pLoadParams);
        if (pResource == nullptr)
        {
            F_LogError("*********************** ResourceManager::Create: Failed to create resource, name: [%s], group: [%s] !", strName.c_str(), strGroupName.c_str());
            return nullptr;
        }

		if (pLoadParams)
		{
			pResource->SetParameterMap(*pLoadParams);
		}

		addImpl(pResource);
		ResourceGroupManager::GetSingleton()._NotifyResourceCreated(pResource);
		return pResource;
	}

	ResourceCreateOrRetrieveResult ResourceManager::CreateOrRetrieve(uint32 nGroup,
																	 const String& strName, 
		                                                             const String& strGroupName,
                                                                     bool bIsManualLoad /*= false*/,
                                                                     ResourceManualLoader* pManualLoader /*= nullptr*/, 
		                                                             const NameValuePairMap* pLoadParams /*= nullptr*/)
	{
		Resource* pResource = GetResourceByName(strName);
		bool bIsCreated = false;
		if (pResource == nullptr)
		{
			bIsCreated = true;
			pResource = Create(nGroup,
							   strName,
                               strGroupName,
                               bIsManualLoad,
                               pManualLoader,
                               pLoadParams);
		}
		return ResourceCreateOrRetrieveResult(pResource, bIsCreated);
	}

	Resource* ResourceManager::Prepare(uint32 nGroup,
									   const String& strName, 
                                       const String& strGroupName,
									   bool bIsManualLoad /*= false*/,
                                       ResourceManualLoader* pManualLoader /*= nullptr*/, 
									   const NameValuePairMap* pLoadParams /*= nullptr*/)
	{
		Resource* pResource = CreateOrRetrieve(nGroup,
											   strName, 
                                               strGroupName, 
                                               bIsManualLoad, 
                                               pManualLoader, 
                                               pLoadParams).first;
		
		pResource->Prepare();
		return pResource;
	}

	Resource* ResourceManager::Load(uint32 nGroup,
									const String& strName, 
                                    const String& strGroupName,
									bool bIsManualLoad /*= false*/,
                                    ResourceManualLoader* pManualLoader /*= nullptr*/, 
									const NameValuePairMap* pLoadParams /*= nullptr*/)
	{
		Resource* pResource = CreateOrRetrieve(nGroup,
											   strName, 
                                               strGroupName, 
                                               bIsManualLoad, 
                                               pManualLoader, 
                                               pLoadParams).first;
		
		pResource->Load();
		return pResource;
	}

	void ResourceManager::Unload(Resource* pResource)
	{
		if (pResource != nullptr)
		{
			pResource->Unload();
		}
	}
	Resource* ResourceManager::Unload(const String& strName)
	{
		Resource* pResource = GetResourceByName(strName);
		if (pResource != nullptr)
		{
			pResource->Unload();
		}
		return pResource;
	}
	Resource* ResourceManager::Unload(ResourceHandle nHandle)
	{
		Resource* pResource = GetResourceByHandle(nHandle);
		if (pResource != nullptr)
		{
			pResource->Unload();
		}
		return pResource;
	}	
	void ResourceManager::UnloadAll(bool bReloadableOnly /*= true*/)
	{
		for (ResourcePtrMap::iterator it = m_mapResource.begin(); 
             it != m_mapResource.end(); ++it)
		{
            Resource* pResource = it->second;
			if (!bReloadableOnly || pResource->IsReloadable())
			{
				pResource->Unload();
			}
		}
	}
	void ResourceManager::UnloadUnreferencedResources(bool bReloadableOnly /*= true*/)
	{
		for (ResourcePtrMap::iterator it = m_mapResource.begin(); 
             it != m_mapResource.end(); ++it)
		{
            Resource* pResource = it->second;
			if (pResource->GetRef() == ResourceGroupManager::ms_nResourceSysRefCounts)
			{
				if (!bReloadableOnly || pResource->IsReloadable())
				{
					pResource->Unload();
				}
			}
		}
	}

	void ResourceManager::ReloadAll(bool bReloadableOnly /*= true*/)
	{
		for (ResourcePtrMap::iterator it = m_mapResource.begin(); 
             it != m_mapResource.end(); ++it)
		{
            Resource* pResource = it->second;
			if (!bReloadableOnly || pResource->IsReloadable())
			{
				pResource->Reload();
			}
		}
	}
	void ResourceManager::ReloadUnreferencedResources(bool bReloadableOnly /*= true*/)
	{
		for (ResourcePtrMap::iterator it = m_mapResource.begin(); 
             it != m_mapResource.end(); ++it)
		{
            Resource* pResource = it->second;
			if (pResource->GetRef() == ResourceGroupManager::ms_nResourceSysRefCounts)
			{
				if (!bReloadableOnly || pResource->IsReloadable())
				{
					pResource->Reload();
				}
			}
		}
	}

	Resource* ResourceManager::Remove(Resource* pResource)
	{
		removeImpl(pResource);
		return pResource;
	}
	Resource* ResourceManager::Remove(const String& strName)
	{
		Resource* pResource = GetResourceByName(strName);
		if (pResource != nullptr)
		{
			removeImpl(pResource);
		}
		return pResource;
	}
	Resource* ResourceManager::Remove(ResourceHandle nHandle)
	{
		Resource* pResource = GetResourceByHandle(nHandle);
		if (pResource != nullptr)
		{
			removeImpl(pResource);
		}
		return pResource;
	}
	void ResourceManager::RemoveAll()
	{
		m_mapResource.clear();
		m_mapResourceGroup.clear();
		m_mapResourcesByHandle.clear();

		ResourceGroupManager::GetSingleton()._NotifyAllResourcesRemoved(this);
	}

	void ResourceManager::Delete(Resource* pResource)
	{
		if (pResource != nullptr)
		{
			pResource->DelRef();
			if (pResource->CanDel())
			{
				pResource->Unload();
				Remove(pResource);
				F_DELETE(pResource)
			}
		}
	}
	void ResourceManager::Delete(const String& strName)
	{
		Resource* pResource = GetResourceByName(strName);
		Delete(pResource);
	}
	void ResourceManager::Delete(ResourceHandle nHandle)
	{
		Resource* pResource = GetResourceByHandle(nHandle);
		Delete(pResource);
	}
	void ResourceManager::DeleteAll()
	{
		for (ResourcePtrMap::iterator it = m_mapResource.begin(); 
             it != m_mapResource.end(); ++it)
		{
            Resource* pResource = it->second;
			pResource->Unload();
			F_DELETE(pResource)
		}
		RemoveAll();
	}

	void ResourceManager::_NotifyResourceTouched(Resource* pResource)
	{

	}
	void ResourceManager::_NotifyResourceLoaded(Resource* pResource)
	{
		m_nMemoryUsage += (ulong)pResource->GetSize();
	}
	void ResourceManager::_NotifyResourceUnloaded(Resource* pResource)
	{
		m_nMemoryUsage -= (ulong)pResource->GetSize();
	}

	ulong ResourceManager::GetMemoryBudget() const
	{
		return m_nMemoryBudget;
	}
	void ResourceManager::SetMemoryBudget(ulong bytes)
	{
		m_nMemoryBudget = bytes;
		checkUsage();
	}

	ResourceHandle ResourceManager::getNextHandle()
	{
		return ++m_nNextHandle;
	}	

	void ResourceManager::addImpl(Resource* pResource)
	{
		std::pair<ResourcePtrMap::iterator, bool> result;
		if (ResourceGroupManager::GetSingleton().IsResourceGroupInGlobalPool(pResource->GetGroupName()))
		{
			result = m_mapResource.insert(ResourcePtrMap::value_type(pResource->GetName(), pResource));
		}
		else
		{
			ResourcePtrGroupMap::iterator itGroup = m_mapResourceGroup.find(pResource->GetGroupName());
			if (itGroup == m_mapResourceGroup.end())
			{
				ResourcePtrMap dummy;
				m_mapResourceGroup.insert(ResourcePtrGroupMap::value_type(pResource->GetGroupName(), dummy));
				itGroup = m_mapResourceGroup.find(pResource->GetGroupName());
			}
			result = itGroup->second.insert(ResourcePtrMap::value_type(pResource->GetName(), pResource));
		}

		if (!result.second)
		{
			if (ResourceGroupManager::GetSingleton().GetResourceLoadingListener())
			{
				if (ResourceGroupManager::GetSingleton().GetResourceLoadingListener()->ResourceCollision(pResource, this))
				{
					std::pair<ResourcePtrMap::iterator, bool> insertResult;
					if(ResourceGroupManager::GetSingleton().IsResourceGroupInGlobalPool(pResource->GetGroupName()))
					{
						insertResult = m_mapResource.insert(ResourcePtrMap::value_type(pResource->GetName(), pResource));
					}
					else
					{
						ResourcePtrGroupMap::iterator itGroup = m_mapResourceGroup.find(pResource->GetGroupName());
						insertResult = itGroup->second.insert(ResourcePtrMap::value_type(pResource->GetName(), pResource));
					}

					if (!insertResult.second)
					{
                        F_LogError("*********************** ResourceManager::addImpl: Resource with the name: [%s], group: [%s] already exists !!", pResource->GetName().c_str(), pResource->GetGroupName().c_str());
                        return;
					}

					std::pair<ResourcePtrHandleMap::iterator, bool> resultHandle = m_mapResourcesByHandle.insert(ResourcePtrHandleMap::value_type(pResource->GetHandle(), pResource));
					if (!resultHandle.second)
					{
                        F_LogError("*********************** ResourceManager::addImpl: Resource with the name: [%s], group: [%s], handle: [%u] already exists !!", pResource->GetName().c_str(), pResource->GetGroupName().c_str(), pResource->GetHandle());
                        return;
					}
				}
			}
		}
		else
		{
			std::pair<ResourcePtrHandleMap::iterator, bool> resultHandle = m_mapResourcesByHandle.insert(ResourcePtrHandleMap::value_type(pResource->GetHandle(), pResource));
			if (!resultHandle.second)
			{
                F_LogError("*********************** ResourceManager::addImpl: Resource with the name: [%s], group: [%s], handle: [%u] already exists !!", pResource->GetName().c_str(), pResource->GetGroupName().c_str(), pResource->GetHandle());
                return;
			}
		}
	}

	void ResourceManager::removeImpl(Resource* pResource)
	{	
		if (ResourceGroupManager::GetSingleton().IsResourceGroupInGlobalPool(pResource->GetGroupName()))
		{
			ResourcePtrMap::iterator itFind = m_mapResource.find(pResource->GetName());
			if (itFind != m_mapResource.end())
			{
				m_mapResource.erase(itFind);
			}
		}
		else
		{
			ResourcePtrGroupMap::iterator itGroup = m_mapResourceGroup.find(pResource->GetGroupName());
			if (itGroup != m_mapResourceGroup.end())
			{
				ResourcePtrMap::iterator itFind = itGroup->second.find(pResource->GetName());
				if (itFind != itGroup->second.end())
				{
					itGroup->second.erase(itFind);
				}

				if (itGroup->second.empty())
				{
					m_mapResourceGroup.erase(itGroup);
				}
			}
		}

		ResourcePtrHandleMap::iterator itHandle = m_mapResourcesByHandle.find(pResource->GetHandle());
		if (itHandle != m_mapResourcesByHandle.end())
		{
			m_mapResourcesByHandle.erase(itHandle);
		}
		
		ResourceGroupManager::GetSingleton()._NotifyResourceRemoved(pResource);
	}

	void ResourceManager::checkUsage()
	{
		
	}

}; //LostPeterEngine