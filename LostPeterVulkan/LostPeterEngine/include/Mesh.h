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

        uint32 m_nUsage;
        FMeshType m_eMesh;
        FMeshVertexType m_eMeshVertex;
        FMeshGeometryType m_eMeshGeometry;
        bool m_bIsFlipY;
        FMeshCreateParam* m_pMeshCreateParam;

        MeshSubPtrVector m_aMeshSub;
        MeshSubPtrMap m_mapMeshSub;

        FAABB m_boundAABB;
        FSphere m_boundSphere;
        


        bool m_bInternalResourcesCreated;

    public:
        F_FORCEINLINE const String& GetPath() const { return m_strPath; }
		F_FORCEINLINE void SetPath(const String& strPath) { m_strPath = strPath; }

        F_FORCEINLINE uint32 GetUsage() const { return m_nUsage; }
		F_FORCEINLINE void SetUsage(uint32 nUsage) { m_nUsage = nUsage; }
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

        F_FORCEINLINE const FAABB& GetBoundAABB() const { return m_boundAABB; }
		F_FORCEINLINE void SetBoundAABB(const FAABB& boundAABB)	{ m_boundAABB = boundAABB; }
        F_FORCEINLINE const FSphere& GetBoundSphere() const { return m_boundSphere; }
		F_FORCEINLINE void SetBoundSphere(const FSphere& boundSphere)	{ m_boundSphere = boundSphere; }

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