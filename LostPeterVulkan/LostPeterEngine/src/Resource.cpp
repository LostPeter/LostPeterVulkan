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

#include "../include/Resource.h"
#include "../include/ResourceManager.h"
#include "../include/ResourceManualLoader.h"
#include "../include/ResourceListener.h"
#include "../include/ResourceGroupManager.h"

namespace LostPeterEngine
{
    Resource::Resource(ResourceManager* pResourceManager,
                       uint32 nGroup, 
                       const String& strName,
                       const String& strGroupName,
                       ResourceHandle nHandle,
                       bool bIsManualLoad /*= false*/,
                       ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : Base(nGroup, strName)
        , m_pResourceManager(pResourceManager)
        , m_strGroupName(strGroupName)
        , m_nHandle(nHandle)
        , m_bIsManualLoad(bIsManualLoad)
        , m_pResourceManualLoader(pResourceManualLoader)
        , m_nSize(0)
        , m_bIsBackgroundLoaded(false)
        , m_nStateCount(0)
    {
        F_Assert(m_pResourceManager && "Resource::Resource")
    }

    Resource::~Resource()
    {

    }   

    void Resource::Destroy()
    {
        RemoveResourceListenerAll();
    }   

    void Resource::AddResourceListener(ResourceListener* pResourceListener)
    {
        m_listResourceListener.push_back(pResourceListener);
    }
	void Resource::RemoveResourceListener(ResourceListener* pResourceListener)
    {
        m_listResourceListener.remove(pResourceListener);
    }
    void Resource::RemoveResourceListenerAll()
    {
        m_listResourceListener.clear();
    }

    void Resource::ChangeGroupOwnership(const String& strNewGroupName)
    {
        if (m_strGroupName != strNewGroupName)
		{
			String strOldGroupName = m_strGroupName;
			m_strGroupName = strNewGroupName;
			ResourceGroupManager::GetSingleton()._NotifyResourceGroupChanged(strOldGroupName, this);
		}
    }

	void Resource::Prepare()
    {
        EResourceLoadingType typeLoading_Old = m_eResourceLoading.load();
        if (typeLoading_Old != E_ResourceLoading_Unloaded ||
            typeLoading_Old != E_ResourceLoading_Preparing) 
			return;

        typeLoading_Old = E_ResourceLoading_Unloaded;
		if (!m_eResourceLoading.compare_exchange_strong(typeLoading_Old, E_ResourceLoading_Preparing))
			return;

		try
		{
			if (m_bIsManualLoad)
			{
				if (m_pResourceManualLoader)
				{
					m_pResourceManualLoader->PrepareResource(this);
				}
				else
				{
                    F_LogError("*********************** Resource::Prepare: No ResourceManualLoader provided, name: [%s] !", GetName().c_str());
				}
			}
			else
			{
				if (m_strGroupName == ResourceGroupManager::ms_strNameResourceGroup_AutoDetect)
				{
					ChangeGroupOwnership(ResourceGroupManager::GetSingleton().FindGroupContainingResource(GetName()));
				}
				prepareImpl();
			}
		}
		catch (...)
		{
			m_eResourceLoading.store(E_ResourceLoading_Unloaded);
			throw;
		}

		m_eResourceLoading.store(E_ResourceLoading_Prepared);

		if (m_bIsBackgroundLoaded)
			queueFireBackgroundPreparingComplete();
        else
            _FirePreparingComplete(false);
    }
	void Resource::Load(bool bIsBackgroundThread /*= false*/)
    {
        if (m_bIsBackgroundLoaded && !bIsBackgroundThread)
			return;

		EResourceLoadingType typeLoading_Old = m_eResourceLoading.load();
		if (typeLoading_Old != E_ResourceLoading_Unloaded && typeLoading_Old != E_ResourceLoading_Prepared) 
			return;

		if (!m_eResourceLoading.compare_exchange_strong(typeLoading_Old, E_ResourceLoading_Loading)) 
			return;

		try
		{
			if (m_bIsManualLoad)
			{
				preLoadImpl();
                {
                    if (m_pResourceManualLoader)
                    {
                        m_pResourceManualLoader->LoadResource(this);
                    }
                    else
                    {
                        F_LogError("*********************** Resource::Load: No ResourceManualLoader provided, name: [%s] !", GetName().c_str());
                    }
                }
				postLoadImpl();
			}
			else
			{
				if (typeLoading_Old == E_ResourceLoading_Unloaded)
					prepareImpl();

				preLoadImpl();
                {
                    typeLoading_Old = E_ResourceLoading_Prepared;

                    if (m_strGroupName == ResourceGroupManager::ms_strNameResourceGroup_AutoDetect)
                    {
                    	ChangeGroupOwnership(ResourceGroupManager::GetSingleton().FindGroupContainingResource(GetName()));
                    }

                    loadImpl();
                }
				postLoadImpl();
			}

			m_nSize = calculateSize();
		}
		catch (...)
		{
			m_eResourceLoading.store(E_ResourceLoading_Unloaded);
			throw;
		}

		m_eResourceLoading.store(E_ResourceLoading_Loaded);
		AddStateCount();

		if(m_pResourceManager)
			m_pResourceManager->_NotifyResourceLoaded(this);

		if (m_bIsBackgroundLoaded)
			queueFireBackgroundLoadingComplete();
        else
            _FireLoadingComplete(false);
    }
    void Resource::Reload()
    {
        if (m_eResourceLoading.load() == E_ResourceLoading_Loaded)
		{
			Unload();
			Load();
		}
    }
    void Resource::Unload()
    {
        EResourceLoadingType typeLoading_Old = m_eResourceLoading.load();
		if (typeLoading_Old != E_ResourceLoading_Loaded && typeLoading_Old != E_ResourceLoading_Prepared) 
			return;

		if (!m_eResourceLoading.compare_exchange_strong(typeLoading_Old, E_ResourceLoading_Unloading)) 
			return;

		if (typeLoading_Old == E_ResourceLoading_Prepared) 
		{
			unprepareImpl();
		} 
		else 
		{
			preUnloadImpl();
            {
                unloadImpl();
            }
			postUnloadImpl();
		}

		m_eResourceLoading.store(E_ResourceLoading_Unloaded);

		if(typeLoading_Old == E_ResourceLoading_Loaded && m_pResourceManager)
			m_pResourceManager->_NotifyResourceUnloaded(this);

        _FireUnloadingComplete();
    }
    void Resource::Touch()
    {
		Load();

		if(m_pResourceManager)
			m_pResourceManager->_NotifyResourceTouched(this);
    }
    void Resource::EscalateLoading()
    {
        Load(true);
    }

    void Resource::_FireLoadingComplete(bool bIsBackgroundLoaded)
    {
        for (ResourceListenerPtrList::iterator it = m_listResourceListener.begin();
			 it != m_listResourceListener.end(); ++it)
		{
			(*it)->LoadingComplete(this);
		}
    }
    void Resource::_FirePreparingComplete(bool bIsBackgroundLoaded)
    {
        for (ResourceListenerPtrList::iterator it = m_listResourceListener.begin();
			 it != m_listResourceListener.end(); ++it)
		{
			(*it)->PreparingComplete(this);
		}
    }
    void Resource::_FireUnloadingComplete()
    {
        for (ResourceListenerPtrList::iterator it = m_listResourceListener.begin();
			 it != m_listResourceListener.end(); ++it)
		{
			(*it)->UnloadingComplete(this);
		}
    }

    void Resource::preLoadImpl() 
    {

    }
    void Resource::postLoadImpl()	
    {

    }
    void Resource::preUnloadImpl() 
    {

    }
    void Resource::postUnloadImpl() 
    {

    }
    void Resource::prepareImpl() 
    {

    }
    void Resource::unprepareImpl() 
    {

    }

    void Resource::queueFireBackgroundLoadingComplete()
    {
        //if(!m_listResourceListener.empty())
		//	ResourceBackgroundQueue::GetSingleton()._QueueFireBackgroundLoadingComplete(this);
    }
	void Resource::queueFireBackgroundPreparingComplete()
    {
        //if(!m_listResourceListener.empty())
		//	ResourceBackgroundQueue::GetSingleton()._QueueFireBackgroundPreparingComplete(this);
    }

    void Resource::addParameterBase()
    {
        
    }

}; //LostPeterEngine