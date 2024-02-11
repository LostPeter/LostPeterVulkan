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

#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/MeshManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace LostPeterEngine
{



    const String Mesh::ms_nameMesh = "Mesh";
    Mesh::Mesh(ResourceManager* pResourceManager,
               uint32 nGroup, 
               const String& strName,
               const String& strGroupName,
               ResourceHandle nHandle,
               bool bIsManualLoad /*= false*/,
               ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : Resource(pResourceManager,
				   nGroup, 
				   strName,
				   strGroupName,
				   nHandle,
				   bIsManualLoad,
				   pResourceManualLoader)
        , m_nUsage(MeshManager::ms_nUsage_Default)
        , m_eMesh(MeshManager::ms_eMesh_Default)
        , m_eMeshVertex(MeshManager::ms_eMeshVertex_Default)
        , m_eMeshGeometry(MeshManager::ms_eMeshGeometry_Default)
        , m_bIsFlipY(MeshManager::ms_bIsFlipY_Default)
        , m_pMeshCreateParam(nullptr)

        , m_bInternalResourcesCreated(false)
    {
         if (createParameterDictionary(ms_nameMesh))
		{
			addParameterBase();
            addParameterInherit();
		}
    }
        void Mesh::addParameterBase()
        {

        }
        void Mesh::addParameterInherit()
        {

        }


    Mesh::~Mesh()
    {
        Destroy();
    }

    void Mesh::Destroy()
    {
        

        Resource::Destroy();
    }

    uint32 Mesh::GetMeshSubCount() const
    {
        return static_cast<uint32>(m_aMeshSub.size());
    }
    MeshSub* Mesh::GetMeshSubByIndex(uint32 nIndex)
    {
        F_Assert(nIndex >= 0 && nIndex < (uint32)m_aMeshSub.size() && "Mesh::GetMeshSubByIndex")
		return m_aMeshSub[nIndex];
    }
    MeshSub* Mesh::GetMeshSubByName(const String& strName)
    {
        MeshSubPtrMap::iterator itFind = m_mapMeshSub.find(strName);
		if (itFind != m_mapMeshSub.end())
			return itFind->second;
		return nullptr;
    }
    MeshSub* Mesh::CreateMeshSub(const String& strName)
    {
        MeshSubPtrMap::iterator itFind = m_mapMeshSub.find(strName);
		if (itFind != m_mapMeshSub.end())
		{
			F_Assert(false && "Mesh::CreateMeshSub: The same name mesh sub already exist !")
			return itFind->second;
		}
		
		MeshSub* pMeshSub = new MeshSub(strName);
		pMeshSub->SetMesh(this);
		pMeshSub->SetMeshSubIndex((int)m_aMeshSub.size());

		m_aMeshSub.push_back(pMeshSub);
		m_mapMeshSub[strName] = pMeshSub;
		return pMeshSub;
    }
    bool Mesh::DeleteMeshSub(MeshSub* pMeshSub)
    {
        return DeleteMeshSub(pMeshSub->GetName());
    }
    bool Mesh::DeleteMeshSub(uint32 nIndex)
    {
        F_Assert(nIndex >= 0 && nIndex < (uint32)m_aMeshSub.size() && "Mesh::DeleteMeshSub")
		return DeleteMeshSub(m_aMeshSub[nIndex]->GetName());
    }
    bool Mesh::DeleteMeshSub(const String& strName)
    {
        MeshSubPtrMap::iterator itFind = m_mapMeshSub.find(strName);
		if (itFind != m_mapMeshSub.end())
		{
			F_Assert(false && "Mesh::DeleteMeshSub: The name mesh sub is not exist !")
			return false;
		}
		MeshSub* pMeshSub = itFind->second;
		m_mapMeshSub.erase(itFind);
		m_aMeshSub.erase(m_aMeshSub.begin() + pMeshSub->GetMeshSubIndex());

        F_DELETE(pMeshSub)
		return true;
    }
    void Mesh::DeleteMeshSubAll()
    {
        int count = (int)m_aMeshSub.size();
        for (int i = 0; i < count; i++)
        {
            MeshSub* pMeshSub = m_aMeshSub[i];
            F_DELETE(pMeshSub)
        }
        m_aMeshSub.clear();
        m_mapMeshSub.clear();
    }

    void Mesh::loadImpl()
    {
        createInternalResources();
    }

    void Mesh::unloadImpl()
    {
        destroyInternalResources();
    }

    uint32 Mesh::calculateSize() const
    {
        

        return 0;
    }

    void Mesh::destroyInternalResources()
    {
        if (m_bInternalResourcesCreated)
		{
			destroyInternalResourcesImpl();
			m_bInternalResourcesCreated = false;

			if (m_eResourceLoading.load() != E_ResourceLoading_Unloading)
            {
                m_eResourceLoading.store(E_ResourceLoading_Unloaded);
                _FireUnloadingComplete();
            }
		}
    }
        void Mesh::destroyInternalResourcesImpl()
        {

        }


    bool Mesh::createInternalResources()
    {
        if (!m_bInternalResourcesCreated)
		{
			createInternalResourcesImpl();
			m_bInternalResourcesCreated = true;

			if (!IsLoading())
            {
                m_eResourceLoading.store(E_ResourceLoading_Loaded);
                _FireLoadingComplete(false);
            }
		}
		return true;
    }
        void Mesh::createInternalResourcesImpl()
        {

        }
    
}; //LostPeterEngine