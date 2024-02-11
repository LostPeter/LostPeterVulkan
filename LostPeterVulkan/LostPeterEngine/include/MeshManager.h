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

#ifndef _MESH_MANAGER_H_
#define _MESH_MANAGER_H_

#include "ResourceManager.h"

namespace LostPeterEngine
{
   class engineExport MeshManager : public FSingleton<MeshManager>
                                  , public ResourceManager
    {
    public:
        MeshManager();
        virtual ~MeshManager();

    public:
        static const String ms_strMeshConfigName;

    public:
        static NameValuePairMap ms_mapParam_Default;
            static uint32 ms_nUsage_Default;
            static FMeshType ms_eMesh_Default;
            static FMeshVertexType ms_eMeshVertex_Default;
            static FMeshGeometryType ms_eMeshGeometry_Default;
            static bool ms_bIsFlipY_Default;

        static const String& GetMeshParamValue(EMeshParamType type);

        //Set MeshParam
        static void SetMeshParam_Usage(NameValuePairMap& mapParam, uint32 nUsage);
        static void SetMeshParam_MeshType(NameValuePairMap& mapParam, FMeshType eMesh);
        static void SetMeshParam_MeshVertexType(NameValuePairMap& mapParam, FMeshVertexType eMeshVertex);
        static void SetMeshParam_MeshGeometryType(NameValuePairMap& mapParam, FMeshGeometryType eMeshGeometry);
        static void SetMesheParam_IsFlipY(NameValuePairMap& mapParam, bool bIsFlipY);

        //Get MeshParam
        static uint32 GetMeshParam_Usage(NameValuePairMap& mapParam);
        static FMeshType GetMeshParam_MeshType(NameValuePairMap& mapParam);
        static FMeshVertexType GetMeshParam_MeshVertexType(NameValuePairMap& mapParam);
        static FMeshGeometryType GetMeshParam_MeshGeometryType(NameValuePairMap& mapParam);
        static bool GetMeshParam_IsFlipY(NameValuePairMap& mapParam);

    protected:
        MeshSerializer* m_pMeshSerializer;

    public:
        F_FORCEINLINE MeshSerializer* GetMeshSerializer() const { return m_pMeshSerializer; }

    public:
        static MeshManager&	GetSingleton();
		static MeshManager*	GetSingletonPtr();

    public:
        virtual void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        Mesh* LoadMesh(uint nGroup, const String& strName, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);
        void UnloadMesh(Mesh* pMesh);

        bool HasMesh(const String& strName);
        bool HasMesh(const String& strName, const String& strGroupName);
        Mesh* GetMesh(const String& strName);
        Mesh* GetMesh(const String& strName, const String& strGroupName);

    private:
        Mesh* loadMesh(MeshInfo* pMI);

    public:
		virtual ResourceCreateOrRetrieveResult CreateOrRetrieveMesh(const String& strPath,
                                                                    uint32 nGroup, 
                                                                    const String& strName, 
                                                                    const String& strGroupName, 
                                                                    bool bIsManualLoad = false,
                                                                    ResourceManualLoader* pManualLoader = nullptr, 
                                                                    const NameValuePairMap* pLoadParams = nullptr,
                                                                    uint32 nUsage = MeshManager::ms_nUsage_Default,
                                                                    FMeshType eMesh = MeshManager::ms_eMesh_Default,
                                                                    FMeshVertexType eMeshVertex = MeshManager::ms_eMeshVertex_Default,
                                                                    FMeshGeometryType eMeshGeometry = MeshManager::ms_eMeshGeometry_Default,
                                                                    bool bIsFlipY = MeshManager::ms_bIsFlipY_Default,
                                                                    FMeshCreateParam* pMeshCreateParam = nullptr);

    public:
        virtual Mesh* Prepare(const String& strPath,
                              uint32 nGroup, 
                              const String& strName, 
                              const String& strGroupName, 
                              uint32 nUsage = MeshManager::ms_nUsage_Default,
                              FMeshType eMesh = MeshManager::ms_eMesh_Default,
                              FMeshVertexType eMeshVertex = MeshManager::ms_eMeshVertex_Default,
                              FMeshGeometryType eMeshGeometry = MeshManager::ms_eMeshGeometry_Default,
                              bool bIsFlipY = MeshManager::ms_bIsFlipY_Default,
                              FMeshCreateParam* pMeshCreateParam = nullptr);

    public:
        virtual Mesh* CreateMesh(const String& strPath,
                                 uint32 nGroup, 
                                 const String& strName, 
                                 const String& strGroupName, 
                                 uint32 nUsage = MeshManager::ms_nUsage_Default,
                                 FMeshType eMesh = MeshManager::ms_eMesh_Default,
                                 FMeshVertexType eMeshVertex = MeshManager::ms_eMeshVertex_Default,
                                 FMeshGeometryType eMeshGeometry = MeshManager::ms_eMeshGeometry_Default,
                                 bool bIsFlipY = MeshManager::ms_bIsFlipY_Default,
                                 FMeshCreateParam* pMeshCreateParam = nullptr);

    protected:
        virtual Resource* createImpl(uint32 nGroup,
									 const String& strName,
			                         const String& strGroupName,
									 ResourceHandle nHandle, 
                                     bool bIsManualLoad,
                                     ResourceManualLoader* pManualLoader, 
			                         const NameValuePairMap* pLoadParams);
    };

}; //LostPeterEngine

#endif