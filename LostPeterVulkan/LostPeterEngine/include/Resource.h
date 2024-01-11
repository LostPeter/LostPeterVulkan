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

#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Resource : public FParameterInterface
                                , public Base
    {
    public:
        Resource(ResourceManager* pResourceManager,
                 uint32 nGroup, 
                 const String& strName,
                 const String& strGroupName,
                 ResourceHandle nHandle,
                 bool bIsManualLoad = false,
                 ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~Resource();

    public:
    protected:
        ResourceManager* m_pResourceManager;
        String m_strGroupName;
        ResourceHandle m_nHandle;
        bool m_bIsManualLoad;
        ResourceManualLoader* m_pResourceManualLoader;

        uint32 m_nSize;
        String m_strOrigin;

        std::atomic<EResourceLoadingType> m_eResourceLoading;
        volatile bool m_bIsBackgroundLoaded;
        uint32 m_nStateCount;

        ResourceListenerPtrList m_listResourceListener;

    public:
        F_FORCEINLINE ResourceManager* GetResourceManager() const { return m_pResourceManager; }
        F_FORCEINLINE const String& GetGroupName() const { return m_strGroupName; }
        F_FORCEINLINE ResourceHandle GetHandle() const { return m_nHandle; }
        F_FORCEINLINE bool IsReloadable() const	{ return !m_bIsManualLoad || m_pResourceManualLoader; }
        F_FORCEINLINE bool IsManualLoad() const	{ return m_bIsManualLoad; }
        F_FORCEINLINE ResourceManualLoader* GetResourceManualLoader() const { return m_pResourceManualLoader; }

        F_FORCEINLINE uint32 GetSize() const { return m_nSize; }
		F_FORCEINLINE const String&	GetOrigin() const { return m_strOrigin; }
		F_FORCEINLINE void SetOrigin(const String& strOrigin) { m_strOrigin = strOrigin; }

        F_FORCEINLINE EResourceLoadingType GetResourceLoadingState() const { return m_eResourceLoading.load(); }
        F_FORCEINLINE bool IsUnloaded() const { return (m_eResourceLoading.load() == E_ResourceLoading_Unloaded); }
        F_FORCEINLINE bool IsLoading() const { return (m_eResourceLoading.load() == E_ResourceLoading_Loading); }
		F_FORCEINLINE bool IsLoaded() const	{ return (m_eResourceLoading.load() == E_ResourceLoading_Loaded); }
        F_FORCEINLINE bool IsUnloading() const { return (m_eResourceLoading.load() == E_ResourceLoading_Unloading); }
        F_FORCEINLINE bool IsPrepared() const { return (m_eResourceLoading.load() == E_ResourceLoading_Prepared); }
        F_FORCEINLINE bool IsPreparing() const { return (m_eResourceLoading.load() == E_ResourceLoading_Preparing); }
        F_FORCEINLINE bool IsBackgroundLoaded() const { return m_bIsBackgroundLoaded; }
		F_FORCEINLINE void SetIsBackgroundLoaded(bool bIsBackgroundLoaded) { m_bIsBackgroundLoaded = bIsBackgroundLoaded; }
        F_FORCEINLINE uint32 GetStateCount() const { return m_nStateCount; }
		F_FORCEINLINE void AddStateCount() { ++m_nStateCount; }

    public:
        virtual void Destroy();

        virtual void AddResourceListener(ResourceListener* pResourceListener);
		virtual void RemoveResourceListener(ResourceListener* pResourceListener);
        virtual void RemoveResourceListenerAll();

    public:
		virtual void ChangeGroupOwnership(const String& strNewGroupName);

		virtual void Prepare();
		virtual void Load(bool bIsBackgroundThread = false);
		virtual void Reload();
		virtual void Unload();
		virtual void Touch();
		virtual void EscalateLoading();
		
		virtual void _FireLoadingComplete(bool bIsBackgroundLoaded);
		virtual void _FirePreparingComplete(bool bIsBackgroundLoaded);
        virtual void _FireUnloadingComplete();

    protected:
        virtual void preLoadImpl();
		virtual void postLoadImpl();
		virtual void preUnloadImpl();
		virtual void postUnloadImpl();
		virtual void prepareImpl();
		virtual void unprepareImpl();

		virtual void loadImpl() = 0;
		virtual void unloadImpl() = 0;
		virtual uint32 calculateSize() const = 0;

		virtual void queueFireBackgroundLoadingComplete();
		virtual void queueFireBackgroundPreparingComplete();

    protected:
        virtual void addParameterBase();
        virtual void addParameterInherit() = 0;
    };

}; //LostPeterEngine

#endif