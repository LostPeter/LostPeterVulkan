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

#ifndef _MATERIAL_DATA_H_
#define _MATERIAL_DATA_H_

#include "Resource.h"
#include "RenderState.h"

namespace LostPeterEngine
{
    class engineExport MaterialData : public Resource
                                    , public FNonCopyable
    {
    public:
        MaterialData(ResourceManager* pResourceManager,
                     uint32 nGroup, 
                     const String& strName,
                     const String& strGroupName,
                     ResourceHandle nHandle,
                     bool bIsManualLoad = false,
                     ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~MaterialData();

    public:
		static const String ms_nameMaterialData;

    public:
    protected:
        String m_strPath;

        RenderStatePtrVector m_aRenderState;
        RenderStatePtrMap m_mapRenderState;

        bool m_bInternalResourcesCreated;

    public:
        F_FORCEINLINE const String& GetPath() const { return m_strPath; }
        F_FORCEINLINE void SetPath(const String& path) { m_strPath = path; }

		F_FORCEINLINE const RenderStatePtrVector& GetRenderStatePtrVector() const { return m_aRenderState; }
        F_FORCEINLINE RenderStatePtrVector& GetRenderStatePtrVector() { return m_aRenderState; }
        F_FORCEINLINE const RenderStatePtrMap& GetRenderStatePtrMap() const { return m_mapRenderState; }
        F_FORCEINLINE RenderStatePtrMap& GetRenderStatePtrMap() { return m_mapRenderState; }
        
    public:
		virtual void Destroy();

	public:
		bool HasRenderState(const String& strName);
        RenderState* GetRenderState(const String& strName);
        bool AddRenderState(RenderState* pRenderState);
        void DeleteRenderState(const String& strName);
        void DeleteRenderStateAll();

    public:
        void SerializerFrom(const MaterialData* pMaterialData);

    protected:
        virtual void loadImpl();
		virtual void unloadImpl();
		virtual uint32 calculateSize() const;

    protected:
        virtual void destroyInternalResources();
			virtual void destroyInternalResourcesImpl();

		virtual bool createInternalResources();
			virtual void createInternalResourcesImpl();

    protected:
        virtual void addParameterBase();
        virtual void addParameterInherit();
    };

}; //LostPeterEngine

#endif