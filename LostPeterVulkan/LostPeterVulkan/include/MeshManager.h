/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
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

#include "Base.h"

namespace LostPeter
{
   class utilExport MeshManager : public FSingleton<MeshManager>
                                , public Base
    {
    public:
        MeshManager();
        virtual ~MeshManager();

    public:
    protected:
        MeshSerializer* m_pMeshSerializer;
        MeshPtrVector m_aMesh;
        MeshGroupPtrMap m_mapMeshGroup;

    public:
        LP_FORCEINLINE MeshSerializer* GetMeshSerializer() const { return m_pMeshSerializer; }
        LP_FORCEINLINE const MeshPtrVector& GetMeshPtrVector() const { return m_aMesh; }
        LP_FORCEINLINE MeshPtrVector& GetMeshPtrVector() { return m_aMesh; }
        LP_FORCEINLINE const MeshGroupPtrMap& GetMeshGroupPtrMap() const { return m_mapMeshGroup; }
        LP_FORCEINLINE MeshGroupPtrMap& GetMeshGroupPtrMap() { return m_mapMeshGroup; }

    public:
        static MeshManager&	GetSingleton();
		static MeshManager*	GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroupCfgMesh, const String& strNameCfgMesh);

    public:
        bool LoadMeshAll();
        Mesh* LoadMesh(uint nGroup, const String& strName);

        bool HasMesh(uint nGroup, const String& strName);
        Mesh* GetMesh(uint nGroup, const String& strName);
        bool AddMesh(uint nGroup, Mesh* pMesh);
        void DeleteMesh(uint nGroup, const String& strName);
        void DeleteMeshAll();

    private:
        Mesh* loadMesh(MeshInfo* pMI);
    };

}; //LostPeter

#endif