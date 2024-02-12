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

#ifndef _MESH_H_
#define _MESH_H_

#include "Resource.h"

namespace LostPeterEngine
{
    class engineExport Mesh : public Resource
    {
        friend class MeshManager;
        friend class MeshSub;

    public:
        Mesh(ResourceManager* pResourceManager,
             uint32 nGroup, 
             const String& strName,
             const String& strGroupName,
             ResourceHandle nHandle,
             bool bIsManualLoad = false,
             ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~Mesh();

    public:
		static const String ms_nameMesh;

    public:
    protected:
        String m_strPath;

        FMeshType m_eMesh;
        FMeshVertexType m_eMeshVertex;
        FMeshGeometryType m_eMeshGeometry;
        bool m_bIsFlipY;
        FMeshCreateParam* m_pMeshCreateParam;

        EStreamUsageType m_eStreamUsageVertex;
		EStreamUsageType m_eStreamUsageIndex;
		bool m_bIsStreamUseShadowVertex;
		bool m_bIsStreamUseShadowIndex;

        MeshSubPtrVector m_aMeshSub;
        MeshSubPtrMap m_mapMeshSub;

        FAABB m_boundAABB;
        FSphere m_boundSphere;
        
        DataVertex*	m_pDataVertexShared;


        bool m_bInternalResourcesCreated;

    public:
        F_FORCEINLINE const String& GetPath() const { return m_strPath; }
		F_FORCEINLINE void SetPath(const String& strPath) { m_strPath = strPath; }
        
        F_FORCEINLINE FMeshType GetMeshType() const { return m_eMesh; }
		F_FORCEINLINE void SetMeshType(FMeshType eMesh) { m_eMesh = eMesh; }
        F_FORCEINLINE FMeshVertexType GetMeshVertexType() const { return m_eMeshVertex; }
		F_FORCEINLINE void SetMeshVertexType(FMeshVertexType eMeshVertex) { m_eMeshVertex = eMeshVertex; }
        F_FORCEINLINE FMeshGeometryType GetMeshGeometryType() const { return m_eMeshGeometry; }
		F_FORCEINLINE void SetMeshGeometryType(FMeshGeometryType eMeshGeometry) { m_eMeshGeometry = eMeshGeometry; }
        F_FORCEINLINE bool GetIsFlipY() const { return m_bIsFlipY; }
		F_FORCEINLINE void SetIsFlipY(bool bIsFlipY) { m_bIsFlipY = bIsFlipY; }
        F_FORCEINLINE FMeshCreateParam* GetMeshCreateParam() const { return m_pMeshCreateParam; }
		F_FORCEINLINE void SetMeshCreateParam(FMeshCreateParam* pMeshCreateParam) { m_pMeshCreateParam = pMeshCreateParam; }

        F_FORCEINLINE EStreamUsageType GetStreamUsageVertex() const	{ return m_eStreamUsageVertex; }
		F_FORCEINLINE void SetStreamUsageVertex(EStreamUsageType eStreamUsageVertex) { m_eStreamUsageVertex = eStreamUsageVertex; }
		F_FORCEINLINE EStreamUsageType GetStreamUsageIndex() const { return m_eStreamUsageIndex; }
		F_FORCEINLINE void SetStreamUsageIndex(EStreamUsageType eStreamUsageIndex) { m_eStreamUsageIndex = eStreamUsageIndex; }
		F_FORCEINLINE bool GetIsStreamUseShadowVertex() const { return m_bIsStreamUseShadowVertex; }
		F_FORCEINLINE void SetIsStreamUseShadowVertex(bool bIsStreamUseShadowVertex) { m_bIsStreamUseShadowVertex = bIsStreamUseShadowVertex; }
		F_FORCEINLINE bool GetIsStreamUseShadowIndex() const { return m_bIsStreamUseShadowIndex; }
		F_FORCEINLINE void SetIsStreamUseShadowIndex(bool bIsStreamUseShadowIndex) { m_bIsStreamUseShadowIndex = bIsStreamUseShadowIndex; }

        F_FORCEINLINE const FAABB& GetBoundAABB() const { return m_boundAABB; }
		F_FORCEINLINE void SetBoundAABB(const FAABB& boundAABB)	{ m_boundAABB = boundAABB; }
        F_FORCEINLINE const FSphere& GetBoundSphere() const { return m_boundSphere; }
		F_FORCEINLINE void SetBoundSphere(const FSphere& boundSphere)	{ m_boundSphere = boundSphere; }

        F_FORCEINLINE DataVertex* GetDataVertexShared() const { return m_pDataVertexShared; }
		F_FORCEINLINE void SetDataVertexShared(DataVertex* pDataVertexShared) { m_pDataVertexShared = pDataVertexShared; }

    public:
        virtual void Destroy();

    public:
    ////MeshSub
        uint32 GetMeshSubCount() const;
		MeshSub* GetMeshSubByIndex(uint32 nIndex);
		MeshSub* GetMeshSubByName(const String& strName);
		MeshSub* CreateMeshSub(const String& strName);
		bool DeleteMeshSub(MeshSub* pMeshSub);
		bool DeleteMeshSub(uint32 nIndex);
		bool DeleteMeshSub(const String& strName);
		void DeleteMeshSubAll();

    ////Skeleton


    ////Animation


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