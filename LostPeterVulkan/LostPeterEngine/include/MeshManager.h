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
            static FMeshType ms_eMesh_Default;
            static FMeshVertexType ms_eMeshVertex_Default;
            static FMeshGeometryType ms_eMeshGeometry_Default;
            static bool ms_bIsFlipY_Default;
            static EStreamUsageType ms_eStreamUsageVertex_Default;
            static EStreamUsageType ms_eStreamUsageIndex_Default;
            static bool ms_bIsStreamUseShadowVertex_Default;
            static bool ms_bIsStreamUseShadowIndex_Default;

        static const String& GetMeshParamValue(EMeshParamType type);

        //Set MeshParam
        static void SetMeshParam_MeshType(NameValuePairMap& mapParam, FMeshType eMesh);
        static void SetMeshParam_MeshVertexType(NameValuePairMap& mapParam, FMeshVertexType eMeshVertex);
        static void SetMeshParam_MeshGeometryType(NameValuePairMap& mapParam, FMeshGeometryType eMeshGeometry);
        static void SetMeshParam_IsFlipY(NameValuePairMap& mapParam, bool bIsFlipY);
        static void SetMeshParam_StreamUsageVertex(NameValuePairMap& mapParam, EStreamUsageType eStreamUsageVertex);
        static void SetMeshParam_StreamUsageIndex(NameValuePairMap& mapParam, EStreamUsageType eStreamUsageIndex);
        static void SetMeshParam_IsStreamUseShadowVertex(NameValuePairMap& mapParam, bool bIsStreamUseShadowVertex);
        static void SetMeshParam_IsStreamUseShadowIndex(NameValuePairMap& mapParam, bool bIsStreamUseShadowIndex);

        //Get MeshParam
        static FMeshType GetMeshParam_MeshType(NameValuePairMap& mapParam);
        static FMeshVertexType GetMeshParam_MeshVertexType(NameValuePairMap& mapParam);
        static FMeshGeometryType GetMeshParam_MeshGeometryType(NameValuePairMap& mapParam);
        static bool GetMeshParam_IsFlipY(NameValuePairMap& mapParam);
        static EStreamUsageType GetMeshParam_StreamUsageVertex(NameValuePairMap& mapParam);
        static EStreamUsageType GetMeshParam_StreamUsageIndex(NameValuePairMap& mapParam);
        static bool GetMeshParam_IsStreamUseShadowVertex(NameValuePairMap& mapParam);
        static bool GetMeshParam_IsStreamUseShadowIndex(NameValuePairMap& mapParam);

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
                                                                    FMeshType eMesh = MeshManager::ms_eMesh_Default,
                                                                    FMeshVertexType eMeshVertex = MeshManager::ms_eMeshVertex_Default,
                                                                    FMeshGeometryType eMeshGeometry = MeshManager::ms_eMeshGeometry_Default,
                                                                    bool bIsFlipY = MeshManager::ms_bIsFlipY_Default,
                                                                    FMeshCreateParam* pMeshCreateParam = nullptr,
                                                                    EStreamUsageType eStreamUsageVertex = MeshManager::ms_eStreamUsageVertex_Default,
                                                                    EStreamUsageType eStreamUsageIndex = MeshManager::ms_eStreamUsageIndex_Default,
                                                                    bool bIsStreamUseShadowVertex = MeshManager::ms_bIsStreamUseShadowVertex_Default,
                                                                    bool bIsStreamUseShadowIndex = MeshManager::ms_bIsStreamUseShadowIndex_Default);

    public:
        virtual Mesh* Prepare(const String& strPath,
                              uint32 nGroup, 
                              const String& strName, 
                              const String& strGroupName, 
                              FMeshType eMesh = MeshManager::ms_eMesh_Default,
                              FMeshVertexType eMeshVertex = MeshManager::ms_eMeshVertex_Default,
                              FMeshGeometryType eMeshGeometry = MeshManager::ms_eMeshGeometry_Default,
                              bool bIsFlipY = MeshManager::ms_bIsFlipY_Default,
                              FMeshCreateParam* pMeshCreateParam = nullptr,
                              EStreamUsageType eStreamUsageVertex = MeshManager::ms_eStreamUsageVertex_Default,
                              EStreamUsageType eStreamUsageIndex = MeshManager::ms_eStreamUsageIndex_Default,
                              bool bIsStreamUseShadowVertex = MeshManager::ms_bIsStreamUseShadowVertex_Default,
                              bool bIsStreamUseShadowIndex = MeshManager::ms_bIsStreamUseShadowIndex_Default);

    public:
        virtual Mesh* CreateMesh(const String& strPath,
                                 uint32 nGroup, 
                                 const String& strName, 
                                 const String& strGroupName, 
                                 FMeshType eMesh = MeshManager::ms_eMesh_Default,
                                 FMeshVertexType eMeshVertex = MeshManager::ms_eMeshVertex_Default,
                                 FMeshGeometryType eMeshGeometry = MeshManager::ms_eMeshGeometry_Default,
                                 bool bIsFlipY = MeshManager::ms_bIsFlipY_Default,
                                 FMeshCreateParam* pMeshCreateParam = nullptr,
                                 EStreamUsageType eStreamUsageVertex = MeshManager::ms_eStreamUsageVertex_Default,
                                 EStreamUsageType eStreamUsageIndex = MeshManager::ms_eStreamUsageIndex_Default,
                                 bool bIsStreamUseShadowVertex = MeshManager::ms_bIsStreamUseShadowVertex_Default,
                                 bool bIsStreamUseShadowIndex = MeshManager::ms_bIsStreamUseShadowIndex_Default);

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