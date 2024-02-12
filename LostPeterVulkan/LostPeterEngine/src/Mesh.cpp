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
#include "../include/DataVertex.h"

namespace LostPeterEngine
{
    ////////////////////////// MeshCmd /////////////////////////////
    //MeshCmd_MeshType
	class MeshCmd_MeshType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return F_GetMeshTypeName(pMesh->GetMeshType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetMeshType(F_ParseMeshType(strValue));
		}
	};
    //MeshCmd_MeshVertexType
	class MeshCmd_MeshVertexType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return F_GetMeshVertexTypeName(pMesh->GetMeshVertexType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetMeshVertexType(F_ParseMeshVertexType(strValue));
		}
	};
    //MeshCmd_MeshGeometryType
	class MeshCmd_MeshGeometryType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return F_GetMeshGeometryTypeName(pMesh->GetMeshGeometryType());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetMeshGeometryType(F_ParseMeshGeometryType(strValue));
		}
	};
    //MeshCmd_IsFlipY
	class MeshCmd_IsFlipY : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return FUtilString::SaveBool(pMesh->GetIsFlipY());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetIsFlipY(FUtilString::ParserBool(strValue));
		}
	};
    //MeshCmd_StreamUsageVertexType
	class MeshCmd_StreamUsageVertexType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return E_GetStreamUsageTypeName(pMesh->GetStreamUsageVertex());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetStreamUsageVertex(E_ParseStreamUsageType(strValue));
		}
	};
    //MeshCmd_StreamUsageIndexType
	class MeshCmd_StreamUsageIndexType : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return E_GetStreamUsageTypeName(pMesh->GetStreamUsageIndex());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetStreamUsageIndex(E_ParseStreamUsageType(strValue));
		}
	};
    //MeshCmd_IsStreamUseShadowVertex
	class MeshCmd_IsStreamUseShadowVertex : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return FUtilString::SaveBool(pMesh->GetIsStreamUseShadowVertex());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetIsStreamUseShadowVertex(FUtilString::ParserBool(strValue));
		}
	};
    //MeshCmd_IsStreamUseShadowIndex
	class MeshCmd_IsStreamUseShadowIndex : public FParameterCommand
	{
	public:	
		virtual String DoGet(const void* pTarget) const
		{
			const Mesh* pMesh = static_cast<const Mesh*>(pTarget);
			return FUtilString::SaveBool(pMesh->GetIsStreamUseShadowIndex());
		}
		virtual void DoSet(void* pTarget, const String& strValue)
		{
			Mesh* pMesh = static_cast<Mesh*>(pTarget);
			pMesh->SetIsStreamUseShadowIndex(FUtilString::ParserBool(strValue));
		}
	};

    ////////////////////////// Mesh ////////////////////////////////
    static MeshCmd_MeshType s_MeshCmd_MeshType;
	static MeshCmd_MeshVertexType s_MeshCmd_MeshVertexType;
	static MeshCmd_MeshGeometryType s_MeshCmd_MeshGeometryType;
	static MeshCmd_IsFlipY s_MeshCmd_IsFlipY;
	static MeshCmd_StreamUsageVertexType s_MeshCmd_StreamUsageVertexType;
	static MeshCmd_StreamUsageIndexType s_MeshCmd_StreamUsageIndexType;
	static MeshCmd_IsStreamUseShadowVertex s_MeshCmd_IsStreamUseShadowVertex;
	static MeshCmd_IsStreamUseShadowIndex s_MeshCmd_IsStreamUseShadowIndex;
	

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
        , m_eMesh(MeshManager::ms_eMesh_Default)
        , m_eMeshVertex(MeshManager::ms_eMeshVertex_Default)
        , m_eMeshGeometry(MeshManager::ms_eMeshGeometry_Default)
        , m_bIsFlipY(MeshManager::ms_bIsFlipY_Default)
        , m_pMeshCreateParam(nullptr)
        , m_eStreamUsageVertex(MeshManager::ms_eStreamUsageVertex_Default)
        , m_eStreamUsageIndex(MeshManager::ms_eStreamUsageIndex_Default)
        , m_bIsStreamUseShadowVertex(MeshManager::ms_bIsStreamUseShadowVertex_Default)
        , m_bIsStreamUseShadowIndex(MeshManager::ms_bIsStreamUseShadowIndex_Default)

        , m_pDataVertexShared(nullptr)

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
            FParameterDictionary* pDictionary = GetParameterDictionary();
            pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_MeshType), MeshManager::GetMeshParamValue(E_MeshParam_MeshType), F_Parameter_Int, &s_MeshCmd_MeshType);
			pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_MeshVertexType), MeshManager::GetMeshParamValue(E_MeshParam_MeshVertexType), F_Parameter_Int, &s_MeshCmd_MeshVertexType);
			pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_MeshGeometryType), MeshManager::GetMeshParamValue(E_MeshParam_MeshGeometryType), F_Parameter_Int, &s_MeshCmd_MeshGeometryType);
			pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_IsFlipY), MeshManager::GetMeshParamValue(E_MeshParam_IsFlipY), F_Parameter_Bool, &s_MeshCmd_IsFlipY);
			pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_StreamUsageVertex), MeshManager::GetMeshParamValue(E_MeshParam_StreamUsageVertex), F_Parameter_Int, &s_MeshCmd_StreamUsageVertexType);
			pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_StreamUsageIndex), MeshManager::GetMeshParamValue(E_MeshParam_StreamUsageIndex), F_Parameter_Int, &s_MeshCmd_StreamUsageIndexType);
			pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_IsStreamUseShadowVertex), MeshManager::GetMeshParamValue(E_MeshParam_IsStreamUseShadowVertex), F_Parameter_Bool, &s_MeshCmd_IsStreamUseShadowVertex);
			pDictionary->AddParameter(E_GetMeshParamTypeName(E_MeshParam_IsStreamUseShadowIndex), MeshManager::GetMeshParamValue(E_MeshParam_IsStreamUseShadowIndex), F_Parameter_Bool, &s_MeshCmd_IsStreamUseShadowIndex);
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
        DeleteMeshSubAll();
        F_DELETE(m_pDataVertexShared)

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
			return nullptr;
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
    
}; //LostPeterEngine