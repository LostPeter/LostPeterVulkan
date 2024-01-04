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

#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include "Base.h"
#include "ResourceGroupManager.h"

namespace LostPeterEngine
{
    class engineExport ResourceManager : public Base
    {
    public:
        ResourceManager(const String& strName, EResourceType eResource);
        virtual ~ResourceManager();

    public:
    protected:
        EResourceType m_eResource;
        String m_strResourceType;		

        ResourcePtrHandleMap m_mapResourcesByHandle;	
		ResourcePtrMap m_mapResource;			
		ResourcePtrGroupMap	m_mapResourceGroup;	
		ResourceHandle m_nNextHandle;			
		ulong m_nMemoryBudget;		
		ulong m_nMemoryUsage;			

		bool m_bVerbose;				
		StringVector m_aScriptPatterns;
		
		float m_fLoadingOrder;			

    public:
        F_FORCEINLINE EResourceType GetResourceType() const { return m_eResource; }
        F_FORCEINLINE const String& GetResourceTypeName() const { return m_strResourceType; }

    public:
        virtual void Destroy();

    public:
        ResourcePtrHandleMapIterator GetResourceIterator() 
		{
			return ResourcePtrHandleMapIterator(m_mapResourcesByHandle.begin(), m_mapResourcesByHandle.end());
		}
		
		virtual Resource* GetResourceByName(const String& strName, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);
		virtual Resource* GetResourceByHandle(ResourceHandle nHandle);

		virtual bool ResourceExists(const String& strName)
		{
			return GetResourceByName(strName) != nullptr;
		}
		virtual bool ResourceExists(ResourceHandle nHandle)
		{
            return GetResourceByHandle(nHandle) != nullptr;
		}

		virtual float GetLoadingOrder() const
		{
			return m_fLoadingOrder; 
		}

		virtual bool GetVerbose() { return m_bVerbose; }
		virtual void SetVerbose(bool v)	{ m_bVerbose = v; }

		virtual const StringVector&	GetScriptPatterns() const
		{
			return m_aScriptPatterns; 
		}

		virtual void ParseScript(FStreamData* pStream, const String& strGroupName)
		{

		}

		virtual Resource* Create(const String& strName,
                                 const String& strGroupName,
								 bool bIsManual = false,
                                 ResourceManualLoader* pManualLoader = nullptr, 
								 const NameValuePairMap* pParamsCreate = nullptr);

		virtual ResourceCreateOrRetrieveResult CreateOrRetrieve(const String& strName, 
			                                                    const String& strGroupName,
                                                                bool bIsManual = false,
                                                                ResourceManualLoader* loader = nullptr, 
			                                                    const NameValuePairMap* pParamsCreate = nullptr);

    public:
		virtual Resource* Prepare(const String& strName, 
                                  const String& strGroupName,
								  bool bIsManual = false,
                                  ResourceManualLoader* pManualLoader = nullptr, 
								  const NameValuePairMap* pLoadParams = nullptr);
		
		virtual Resource* Load(const String& strName, 
                               const String& strGroupName,
							   bool bIsManual = false,
                               ResourceManualLoader* pManualLoader = nullptr, 
							   const NameValuePairMap* pParamsCreate = nullptr);

		virtual void Unload(const String& strName);
		virtual void Unload(ResourceHandle nHandle);
		virtual void UnloadAll(bool bReloadableOnly = true);
		virtual void UnloadUnreferencedResources(bool bReloadableOnly = true);
		
		virtual void ReloadAll(bool bReloadableOnly = true);
		virtual void ReloadUnreferencedResources(bool bReloadableOnly = true);

		virtual void Remove(Resource* pResource);
		virtual void Remove(const String& strName);
		virtual void Remove(ResourceHandle nHandle);
		virtual void RemoveAll();
    
		virtual void _NotifyResourceTouched(Resource* pResource);
		virtual void _NotifyResourceLoaded(Resource* pResource);
		virtual void _NotifyResourceUnloaded(Resource* pResource);

		virtual ulong GetMemoryBudget() const;
		virtual void SetMemoryBudget(ulong bytes);
		virtual ulong GetMemoryUsage() const { return m_nMemoryUsage; }

	protected:
		ResourceHandle getNextHandle();

		virtual Resource* createImpl(const String& strName,
                                     ResourceHandle nHandle, 
			                         const String& strGroupName,
                                     bool isManual,
                                     ResourceManualLoader* pManualLoader, 
			                         const NameValuePairMap* pParamsCreate)	= 0;

		virtual void addImpl(Resource* pResource);
		virtual void removeImpl(Resource* pResource);
		virtual void checkUsage();
    };

}; //LostPeterEngine

#endif