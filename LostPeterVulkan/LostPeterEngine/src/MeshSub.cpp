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

#include "../include/MeshSub.h"
#include "../include/Mesh.h"
#include "../include/DataVertex.h"
#include "../include/DataIndex.h"
#include "../include/DataVertexIndex.h"
#include "../include/VertexDeclaration.h"
#include "../include/VertexDeclarationManager.h"
#include "../include/StreamVertexBinding.h"
#include "../include/StreamVertexBindingManager.h"
#include "../include/StreamVertex.h"
#include "../include/StreamIndex.h"
#include "../include/StreamManager.h"

namespace LostPeterEngine
{
    MeshSub::MeshSub(const String& nameMeshSub)
        : Base(nameMeshSub)
        , m_pMesh(nullptr)
        , m_nMeshSubIndex(0)
        , m_eRenderPrimitive(F_RenderPrimitive_TriangleList)
        , m_pDataVertex(nullptr)
        , m_pDataIndex(nullptr)
        , m_bIsUseSharedVertex(false)

        , m_nVertexCount(0)
        , m_nVertexBuffer_Size(0)
        , m_pVertexBuffer_Data(nullptr)
        , m_nIndexCount(0)
        , m_nIndexBuffer_Size(0)
        , m_pIndexBuffer_Data(nullptr)
    {

    }

    MeshSub::~MeshSub()
    {
        Destroy();
    }

    void MeshSub::Destroy()
    {
        F_DELETE(m_pDataVertex)
		F_DELETE(m_pDataIndex)
    }

    bool MeshSub::Init(FMeshData& meshData, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        UpdateBound(meshData.aabb, meshData.sphere, bIsTransformLocal, mat4TransformLocal);
        
        //1> DataVertex
        FMeshVertexType eMeshVertex = m_pMesh->GetMeshVertexType();
        if (eMeshVertex == F_MeshVertex_Pos3Color4Tex2) //5:    F_MeshVertex_Pos3Color4Tex2 (FMeshVertexPCT)
        {
            FMeshDataLoader::ExtractVertexData_Pos3Color4Tex2(meshData, m_aVertices_Pos3Color4Tex2, bIsTransformLocal, mat4TransformLocal);
            m_nVertexCount = (uint32)m_aVertices_Pos3Color4Tex2.size();
            m_nVertexBuffer_Size = m_nVertexCount * sizeof(FVertex_Pos3Color4Tex2);
            m_pVertexBuffer_Data = &m_aVertices_Pos3Color4Tex2[0];
        }
        else if (eMeshVertex == F_MeshVertex_Pos3Color4Normal3Tex2) //6:    F_MeshVertex_Pos3Color4Normal3Tex2
        {
            FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tex2(meshData, m_aVertices_Pos3Color4Normal3Tex2, bIsTransformLocal, mat4TransformLocal);
            m_nVertexCount = (uint32)m_aVertices_Pos3Color4Normal3Tex2.size();
            m_nVertexBuffer_Size = m_nVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
            m_pVertexBuffer_Data = &m_aVertices_Pos3Color4Normal3Tex2[0];
        }
        else if (eMeshVertex == F_MeshVertex_Pos3Color4Normal3Tex4) //7:    F_MeshVertex_Pos3Color4Normal3Tex4
        {
            FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tex4(meshData, m_aVertices_Pos3Color4Normal3Tex4, bIsTransformLocal, mat4TransformLocal);
            m_nVertexCount = (uint32)m_aVertices_Pos3Color4Normal3Tex4.size();
            m_nVertexBuffer_Size = m_nVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex4);
            m_pVertexBuffer_Data = &m_aVertices_Pos3Color4Normal3Tex4[0];
        }
        else if (eMeshVertex == F_MeshVertex_Pos3Color4Normal3Tangent3Tex2) //8:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex2
        {
            FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tangent3Tex2(meshData, m_aVertices_Pos3Color4Normal3Tangent3Tex2, bIsTransformLocal, mat4TransformLocal);
            m_nVertexCount = (uint32)m_aVertices_Pos3Color4Normal3Tangent3Tex2.size();
            m_nVertexBuffer_Size = m_nVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
            m_pVertexBuffer_Data = &m_aVertices_Pos3Color4Normal3Tangent3Tex2[0];
        }
        else if (eMeshVertex == F_MeshVertex_Pos3Color4Normal3Tangent3Tex4) //9:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex4
        {
            FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tangent3Tex4(meshData, m_aVertices_Pos3Color4Normal3Tangent3Tex4, bIsTransformLocal, mat4TransformLocal);
            m_nVertexCount = (uint32)m_aVertices_Pos3Color4Normal3Tangent3Tex4.size();
            m_nVertexBuffer_Size = m_nVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex4);
            m_pVertexBuffer_Data = &m_aVertices_Pos3Color4Normal3Tangent3Tex4[0];
        }
        else
        {
            F_LogError("*********************** MeshSub::Init: create mesh sub failed: [%s], wrong FMeshVertexType: [%s] !", GetName().c_str(), F_GetMeshVertexTypeName(eMeshVertex).c_str());
            return false; 
        }
        if (!createDataVertex())
        {
            F_LogError("*********************** MeshSub::Init: create mesh sub data vertex failed: [%s], wrong FMeshVertexType: [%s] !", GetName().c_str(), F_GetMeshVertexTypeName(eMeshVertex).c_str());
            return false; 
        }

        //2> DataIndex
        FMeshDataLoader::ExtractIndexData(meshData, m_aIndices);
        m_nIndexCount = (uint32)m_aIndices.size();
        m_nIndexBuffer_Size = m_nIndexCount * sizeof(uint32);
        m_pIndexBuffer_Data = &m_aIndices[0];
        if (!createDataIndex())
        {
            F_LogError("*********************** MeshSub::Init: create mesh sub data index failed: [%s], wrong FMeshVertexType: [%s] !", GetName().c_str(), F_GetMeshVertexTypeName(eMeshVertex).c_str());
            return false; 
        }

        return true;
    }
    bool MeshSub::Init(FMeshDataPC& meshDataPC, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        UpdateBound(meshDataPC.aabb, meshDataPC.sphere, bIsTransformLocal, mat4TransformLocal);
        FMeshVertexType eMeshVertex = m_pMesh->GetMeshVertexType();

        //1> DataVertex
        FMeshDataLoader::ExtractVertexData_Pos3Color4Tex2(meshDataPC, m_aVertices_Pos3Color4, bIsTransformLocal, mat4TransformLocal);
        m_nVertexCount = (uint32)m_aVertices_Pos3Color4.size();
        m_nVertexBuffer_Size = m_nVertexCount * sizeof(FVertex_Pos3Color4);
        m_pVertexBuffer_Data = &m_aVertices_Pos3Color4[0];
        if (!createDataVertex())
        {
            F_LogError("*********************** MeshSub::Init: create mesh sub data vertex failed: [%s], wrong FMeshVertexType: [%s] !", GetName().c_str(), F_GetMeshVertexTypeName(eMeshVertex).c_str());
            return false; 
        }

        //2> DataIndex
        FMeshDataLoader::ExtractIndexData(meshDataPC, m_aIndices);
        m_nIndexCount = (uint32)m_aIndices.size();
        m_nIndexBuffer_Size = m_nIndexCount * sizeof(uint32);
        m_pIndexBuffer_Data = &m_aIndices[0];
        if (!createDataIndex())
        {
            F_LogError("*********************** MeshSub::Init: create mesh sub data index failed: [%s], wrong FMeshVertexType: [%s] !", GetName().c_str(), F_GetMeshVertexTypeName(eMeshVertex).c_str());
            return false; 
        }

        return true;
    }
    bool MeshSub::createDataVertex()
    {
        if (m_nVertexCount <= 0 ||
            m_nVertexBuffer_Size <= 0 ||
            m_pVertexBuffer_Data == nullptr)
        {
            return false;
        }   

        VertexDeclaration* pVertexDeclaration = VertexDeclarationManager::GetSingleton().GetVertexDeclaration(m_pMesh->GetMeshVertexType());
        StreamVertex* pStreamVertex = StreamManager::GetSingleton().CreateStreamVertex(m_nVertexBuffer_Size, m_pMesh->GetStreamUsageVertex(), m_pMesh->GetIsStreamUseShadowVertex());
        pStreamVertex->WriteData(0, m_nVertexBuffer_Size, m_pVertexBuffer_Data);
        StreamVertexBinding* pStreamVertexBinding = StreamVertexBindingManager::GetSingleton().CreateStreamVertexBinding();
        pStreamVertexBinding->SetBinding(0, pStreamVertex);
        m_pDataVertex = new DataVertex(pVertexDeclaration, false,
                                       pStreamVertexBinding, true);
        
        return true;
    }
    bool MeshSub::createDataIndex()
    {
        if (m_nIndexCount <= 0 ||
            m_nIndexBuffer_Size <= 0 ||
            m_pIndexBuffer_Data == nullptr)
        {
            return false;
        }   

        StreamIndex* pStreamIndex = StreamManager::GetSingleton().CreateStreamIndex(E_StreamIndex_32Bit, m_nIndexCount, m_pMesh->GetStreamUsageIndex(), m_pMesh->GetIsStreamUseShadowIndex());
        pStreamIndex->WriteData(0, m_nIndexBuffer_Size, m_pIndexBuffer_Data);
        m_pDataIndex = new DataIndex(pStreamIndex, 0, m_nIndexCount, true);

        return true;
    }

    bool MeshSub::GetDataVertexIndex(DataVertexIndex& dataVI, uint32 nIndexLOD /*= 0*/)
    {
        if (m_pMesh == nullptr)
            return false;

        dataVI.m_eRenderPrimitive = m_eRenderPrimitive;
        dataVI.m_pDataVertex = m_bIsUseSharedVertex ? m_pMesh->GetDataVertexShared() : m_pDataVertex;
		if (nIndexLOD > 0)
		{
			
		}
		else
		{
			dataVI.m_pDataIndex = m_pDataIndex;
		}
         dataVI.m_bIsUseIndex = m_pDataIndex != nullptr ? true : false;

		F_Assert(dataVI.m_pDataVertex && "MeshSub::GetDataVertexIndex")
		return true;
    }
    
    void MeshSub::UpdateBound(const FAABB& aabb, const FSphere& sphere, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        m_boundAABB = aabb;
        m_boundSphere = sphere;
        if (bIsTransformLocal)
        {
            m_boundAABB.TransformAffine(mat4TransformLocal);
            m_boundSphere.Transform(mat4TransformLocal);
        }
    }

    uint32 MeshSub::calculateSize() const
    {
        uint32 nMemSize = (uint32)(GetName().size() * sizeof(char));
        nMemSize += m_nVertexBuffer_Size;
        nMemSize += m_nIndexBuffer_Size;
        return nMemSize;
    }

}; //LostPeterEngine