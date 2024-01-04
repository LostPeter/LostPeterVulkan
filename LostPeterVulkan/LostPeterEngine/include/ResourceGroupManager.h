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

#ifndef _RESOURCE_GROUP_MANAGER_H_
#define _RESOURCE_GROUP_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    ////////////////////////// ResourceDeclaration /////////////////
    struct ResourceDeclaration
    {
        String strResourceName;	
        String strResourceType;	
        ResourceManualLoader* pManualLoader;			
        NameValuePairMap mapParameters;
    };
    typedef std::list<ResourceDeclaration> ResourceDeclarationList;


    ////////////////////////// ResourceLocation ////////////////////
    struct ResourceLocation
    {
        FArchive* pArchive;			
        bool bRecursive;
    };
	typedef std::list<ResourceLocation*> ResourceLocationPtrList;	
    typedef std::map<String, FArchive*>	ResourceLocationPtrMap;

    ////////////////////////// ResourceGroup ///////////////////////
    struct ResourceGroup
    {
        String strGroupName;			
        EResourceGroupStatusType eResourceGroupStatus;			
        ResourceLocationPtrList	listResourceLocation;			
        ResourceLocationPtrMap mapIndexCaseSensitive;	
        ResourceLocationPtrMap mapIndexCaseInsensitive;	
        ResourceDeclarationList	listDeclaration;			
        ResourceLoadOrderMap mapResourceLoadOrder;
        
		String strWorldGeometry;
		SceneManager* pWorldGeometrySceneManager;
        
        bool bIsInGlobalPool;

        void AddToIndex(const String& strResourceName, FArchive* pArchive);
        void RemoveFromIndex(const String& strResourceName, FArchive* pArchive);
        void RemoveFromIndex(FArchive* pArchive);
    };
    typedef std::map<String, ResourceGroup*> ResourceGroupPtrMap;
    
    ////////////////////////// ResourceGroupManager ////////////////
    class engineExport ResourceGroupManager : public FSingleton<ResourceGroupManager>
                                            , public Base
    {
    public:
        ResourceGroupManager();
        virtual ~ResourceGroupManager();

    public:
        static String ms_strNameResourceGroup_Default;			
		static String ms_strNameResourceGroup_Internal;			
		static String ms_strNameResourceGroup_Bootstrap;				
		static String ms_strNameResourceGroup_AutoDetect;			
		static size_t ms_nResourceSysRefCounts;				
		
    protected:
        ResourceManagerPtrMap m_mapResourceManager;	
		FScriptLoaderOrderMap m_mapScriptLoaderOrder;
		ResourceGroupListenerPtrVector m_aResourceGroupListener;	
		ResourceLoadingListener* m_pResourceLoadingListener;
		ResourceGroupPtrMap	m_mapResourceGroup;
		String m_strWorldGroupName;	
		ResourceGroup* m_pResourceGroupCurrent;		

    public:
        static ResourceGroupManager& GetSingleton();
		static ResourceGroupManager* GetSingletonPtr();
        
    public:
        void Destroy();

    public:
        ResourceGroup* GetResourceGroup(const String& strGroupName);
		bool CreateResourceGroup(const String& strGroupName, const bool bIsInGlobalPool = true);
		bool InitializeResourceGroup(const String& strGroupName);

		void InitializeAllResourceGroups();

		bool PrepareResourceGroup(const String& strGroupName,
                                  bool bIsPrepareMainResources = true, 
								  bool bIsPrepareWorldGeometry = true);
			
		bool LoadResourceGroup(const String& strGroupName,
                               bool bIsLoadMainResources = true, 
							   bool bIsLoadWorldGeometry = true);

		bool UnloadResourceGroup(const String& strGroupName, bool bIsReloadableOnly = true);
		bool UnloadUnreferencedResourcesInGroup(const String& strGroupName, bool bIsReloadableOnly = true);

		bool ClearResourceGroup(const String& strGroupName);
		bool DestroyResourceGroup(const String& strGroupName);

		bool IsResourceGroupInitialized(const String& strGroupName);
		bool IsResourceGroupLoaded(const String& strGroupName);

		void AddResourceLocation(const String& strResourceName,
                                 const String& strLocationType, 
								 const String& strGroupName = ms_strNameResourceGroup_Default,
                                 bool bIsRecursive = false);
		bool RemoveResourceLocation(const String& strResourceName,
                                    const String& strGroupName = ms_strNameResourceGroup_Default);

		void DeclareResource(const String& strResourceName,
                             const String& strResourceType,
							 const String& strGroupName = ms_strNameResourceGroup_Default,
							 const NameValuePairMap& mapParameters = NameValuePairMap());
		void DeclareResource(const String& strResourceName,
                             const String& strResourceType,
							 const String& strGroupName,ResourceManualLoader* pManualLoader,
							 const NameValuePairMap& mapParameters = NameValuePairMap());
		void UndeclareResource(const String& strResourceName,
                               const String& strGroupName);

		FStreamData* OpenResource(const String& strResourceName, 
							      const String& strGroupName = ms_strNameResourceGroup_Default,
								  bool bIsSearchGroupsIfNotFound = true,
                                  Resource* pResourceBeingLoaded = nullptr);
		FStreamDataPtrList* OpenResources(const String& strPattern,
                                          const String& strGroupName = ms_strNameResourceGroup_Default);

		StringVector* ListResourceNames(const String& strGroupName,
                                        bool bDirs = false);
		FFileInfoVector* ListResourceFileInfo(const String& strGroupName,
                                              bool bDirs = false);
		StringVector* FindResourceNames(const String& strGroupName,
                                        const String& strPattern,
										bool bDirs = false);
        FFileInfoVector* FindResourceFileInfo(const String& strGroupName,
                                              const String& strPattern,
											  bool bDirs = false);

		bool ResourceExists(const String& strGroupName, const String& strResourceName);
		bool ResourceExists(ResourceGroup* pResourceGroup, const String& strResourceName);

		const String& FindGroupContainingResource(const String& strResourceName);

		time_t ResourceModifiedTime(const String& strGroupName, const String& strResourceName); 
		time_t ResourceModifiedTime(ResourceGroup* pResourceGroup, const String& strResourceName); 
		
		FStreamData* CreateResource(const String& strResourceName, 
                                    const String& strGroupName = ms_strNameResourceGroup_Default, 
                                    bool bIsOverwrite = false, 
                                    const String& strLocationPattern = FUtilString::BLANK);
		bool DeleteResource(const String& strResourceName, 
                            const String& strGroupName = ms_strNameResourceGroup_Default, 
                            const String& strLocationPattern = FUtilString::BLANK);
		bool DeleteMatchingResources(const String& strFilePattern, 
                                     const String& strGroupName = ms_strNameResourceGroup_Default, 
                                     const String& strLocationPattern = FUtilString::BLANK);

		void AddResourceGroupListener(ResourceGroupListener* pResourceGroupListener);
		void RemoveResourceGroupListener(ResourceGroupListener* pResourceGroupListener);

		const String& GetWorldResourceGroupName() const { return m_strWorldGroupName; }
		void SetWorldResourceGroupName(const String& strGroupName) { m_strWorldGroupName = strGroupName; }
		bool LinkWorldGeometryToResourceGroup(const String& strGroupName,
                                              const String& strWorldGeometry,
											  SceneManager* pSceneManager);
		bool UnlinkWorldGeometryFromResourceGroup(const String& strGroupName);

		void ShutdownAll();
    
		void _RegisterResourceManager(const String& strResourceType, ResourceManager* rm);
		void _UnregisterResourceManager(const String& strResourceType);
		
		ResourceManagerPtrMapIterator GetResourceManagerPtrMapIterator()
		{ 
			return ResourceManagerPtrMapIterator(m_mapResourceManager.begin(), m_mapResourceManager.end());
		}

		void _RegisterScriptLoader(FScriptLoader* pScriptLoader);
		void _UnregisterScriptLoader(FScriptLoader* pScriptLoader);

		ResourceManager* _GetResourceManager(const String& strResourceType);

		void _NotifyResourceCreated(Resource* pResource);
		void _NotifyResourceRemoved(Resource* pResource);
		void _NotifyResourceGroupChanged(const String& strGroupNameOld, Resource* pResource);
		void _NotifyAllResourcesRemoved(ResourceManager* pResourceManager);
		void _NotifyWorldGeometryStageStarted(const String& strDescription);
		void _NotifyWorldGeometryStageEnded();

        bool IsResourceGroupInGlobalPool(const String& strGroupName);
		StringVector GetResourceGroups();       

		ResourceDeclarationList GetResourceDeclarationList(const String& strGroupName);

		ResourceLoadingListener* GetResourceLoadingListener();
		void SetResourceLoadingListener(ResourceLoadingListener* pResourceLoadingListener);
		
	protected:
		void ParseResourceGroupScripts(ResourceGroup* pResourceGroup);
		void CreateDeclaredResources(ResourceGroup* pResourceGroup);
		void AddCreatedResource(Resource* pResource, ResourceGroup* pResourceGroup);
		void DropGroupContents(ResourceGroup* pResourceGroup);
		void DeleteGroup(ResourceGroup* pGroup);

		ResourceGroup* FindGroupContainingResourceImpl(const String& strResourceName);
		
		void FireResourceGroupScriptingStarted(const String& strGroupName,size_t nScriptCount);
		void FireScriptStarted(const String& strScriptName,bool& bIsSkipScript);
		void FireScriptEnded(const String& strScriptName,bool bIsSkipped);	
		void FireResourceGroupScriptingEnded(const String& strGroupName);
		
		void FireResourceGroupLoadStarted(const String& strGroupName, size_t nResourceCount);
		void FireResourceLoadStarted(const Resource* pResource);
		void FireResourceLoadEnded();
		void FireResourceGroupLoadEnded(const String& strGroupName);
		
		void FireResourceGroupPrepareStarted(const String& strGroupName, size_t nResourceCount);
		void FireResourcePrepareStarted(const Resource* pResource);
		void FireResourcePrepareEnded();
		void FireResourceGroupPrepareEnded(const String& strGroupName);
    };

}; //LostPeterEngine

#endif