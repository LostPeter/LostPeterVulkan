/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_MESH_DATA_H_
#define _F_MESH_DATA_H_

#include "FPreDefine.h"
#include "FMeshVertex.h"
#include "FAABB.h"

namespace LostPeterFoundation
{
    //////////////////////////////// FMeshDataPCT ////////////////////////////////////
    struct LPF_Export FMeshDataPCT
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        FMeshVertexPCTVector vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        FAABB aabb;

        FMeshDataPCT()
            : bIsFlipY(true)
        {

        }

        void Clear()
        {
            vertices.clear();
            indices16.clear();
            indices32.clear();
        }

        void ReserveVertexCount(size_t nCount)
        {
            vertices.reserve(nCount);
        }
        void ResizeVertexCount(size_t nCount)
        {
            vertices.resize(nCount);
        }
        FMeshVertexPCT& GetVertex(uint32 nIndex)
        {
            return vertices[nIndex];
        }
        void AddVertex(const FMeshVertexPCT& meshVertex)
        {
            aabb.Merge(meshVertex.pos);
            vertices.push_back(meshVertex);
        }
        void SetVertex(int nIndex, const FMeshVertexPCT& meshVertex)
        {
            vertices[nIndex] = meshVertex;
        }
        void RefreshAABB()
        {
            aabb.SetNull();
            size_t count = vertices.size();
            for (size_t i = 0; i < count; i++)
            {
                FMeshVertexPCT& meshVertex = vertices[i];
                aabb.Merge(meshVertex.pos);
            }
        }

        void ReserveIndexCount(size_t nCount)
        {
            indices32.reserve(nCount);
        }
        void ResizeIndexCount(size_t nCount)
        {
            indices32.resize(nCount);
        }
        uint32 GetIndex(uint32 nIndex)
        {
            return indices32[nIndex];
        }
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndices(uint32 nCount, uint32* pIndex)
        {
            for (uint32 i = 0; i < nCount; i++)
            {
                AddIndex(pIndex[i]);
            }
        }
        void SetIndex(uint32 index, uint32 nIndex)
        {
            indices32[index] = nIndex;
        }
        void AddIndexLine(uint32 nIndex1, uint32 nIndex2)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }
        void SetIndexTriangle(uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
        {
            indices32[indexStart + 0] = index1;
            indices32[indexStart + 1] = index2;
            indices32[indexStart + 2] = index3;
        }


        Indices16Vector& GetIndices16()
        {
            return indices16;
        }
        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(FMeshVertexPCT));
        }

        void* GetIndexData()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (void*)indices16.data();
            }
            return (void*)indices32.data();
        }
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
        }
        

        bool IsIndices16()
        {
            return indices32.size() < c_nMaxUint16;
        }
        void EnsureIndices16()
        {
            if (IsIndices16() && indices16.empty())
            {
                indices16.clear();
                indices16.resize(indices32.size());
                for (size_t i = 0; i < indices32.size(); i++)
                {
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<FMeshDataPCT> FMeshDataPCTVector;


	//////////////////////////////// FMeshData ///////////////////////////////////////
    struct LPF_Export FMeshData
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        String nameMesh;
        FMeshVertexVector vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        FAABB aabb;

        FMeshData()
            : bIsFlipY(true)
        {

        }

        void Clear()
        {
            nameMesh = "";
            vertices.clear();
            indices16.clear();
            indices32.clear();
        }

        void ReserveVertexCount(size_t nCount)
        {
            vertices.reserve(nCount);
        }
        void ResizeVertexCount(size_t nCount)
        {
            vertices.resize(nCount);
        }
        FMeshVertex& GetVertex(uint32 nIndex)
        {
            return vertices[nIndex];
        }
        void AddVertex(const FMeshVertex& meshVertex)
        {
            aabb.Merge(meshVertex.pos);
            vertices.push_back(meshVertex);
        }
        void SetVertex(int nIndex, const FMeshVertex& meshVertex)
        {
            vertices[nIndex] = meshVertex;
        }
        void RefreshAABB()
        {
            aabb.SetNull();
            size_t count = vertices.size();
            for (size_t i = 0; i < count; i++)
            {
                FMeshVertex& meshVertex = vertices[i];
                aabb.Merge(meshVertex.pos);
            }
        }

        void ReserveIndexCount(size_t nCount)
        {
            indices32.reserve(nCount);
        }
        void ResizeIndexCount(size_t nCount)
        {
            indices32.resize(nCount);
        }
        uint32 GetIndex(uint32 nIndex)
        {
            return indices32[nIndex];
        }
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndices(uint32 nCount, uint32* pIndex)
        {
            for (uint32 i = 0; i < nCount; i++)
            {
                AddIndex(pIndex[i]);
            }
        }
        void SetIndex(uint32 index, uint32 nIndex)
        {
            indices32[index] = nIndex;
        }
        void AddIndexLine(uint32 nIndex1, uint32 nIndex2)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }
        void SetIndexTriangle(uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
        {
            indices32[indexStart + 0] = index1;
            indices32[indexStart + 1] = index2;
            indices32[indexStart + 2] = index3;
        }

        Indices16Vector& GetIndices16()
        {
            return indices16;
        }

        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(FMeshVertex));
        }

        void* GetIndexData()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (void*)indices16.data();
            }
            return (void*)indices32.data();
        }
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
        }
        

        bool IsIndices16()
        {
            return indices32.size() < c_nMaxUint16;
        }
        void EnsureIndices16()
        {
            if (IsIndices16() && indices16.empty())
            {
                indices16.clear();
                indices16.resize(indices32.size());
                for (size_t i = 0; i < indices32.size(); i++)
                {
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<FMeshData> FMeshDataVector;


    //////////////////////////////// FMeshDataUV2 ////////////////////////////////////
    struct FMeshDataUV2
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        FMeshVertexUV2Vector vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        FAABB aabb;

        FMeshDataUV2()
            : bIsFlipY(true)
        {

        }

        void Clear()
        {
            vertices.clear();
            indices16.clear();
            indices32.clear();
        }

        void ReserveVertexCount(size_t nCount)
        {
            vertices.reserve(nCount);
        }
        void ResizeVertexCount(size_t nCount)
        {
            vertices.resize(nCount);
        }
        FMeshVertexUV2& GetVertex(uint32 nIndex)
        {
            return vertices[nIndex];
        }
        void AddVertex(const FMeshVertexUV2& meshVertex)
        {
            aabb.Merge(meshVertex.pos);
            vertices.push_back(meshVertex);
        }
        void SetVertex(int nIndex, const FMeshVertexUV2& meshVertex)
        {
            vertices[nIndex] = meshVertex;
        }
        void RefreshAABB()
        {
            aabb.SetNull();
            size_t count = vertices.size();
            for (size_t i = 0; i < count; i++)
            {
                FMeshVertexUV2& meshVertex = vertices[i];
                aabb.Merge(meshVertex.pos);
            }
        }

        void ReserveIndexCount(size_t nCount)
        {
            indices32.reserve(nCount);
        }
        void ResizeIndexCount(size_t nCount)
        {
            indices32.resize(nCount);
        }
        uint32 GetIndex(uint32 nIndex)
        {
            return indices32[nIndex];
        }
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndices(uint32 nCount, uint32* pIndex)
        {
            for (uint32 i = 0; i < nCount; i++)
            {
                AddIndex(pIndex[i]);
            }
        }
        void SetIndex(uint32 index, uint32 nIndex)
        {
            indices32[index] = nIndex;
        }
          void AddIndexLine(uint32 nIndex1, uint32 nIndex2)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }
        void SetIndexTriangle(uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
        {
            indices32[indexStart + 0] = index1;
            indices32[indexStart + 1] = index2;
            indices32[indexStart + 2] = index3;
        }


        Indices16Vector& GetIndices16()
        {
            return indices16;
        }
        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(FMeshVertexUV2));
        }

        void* GetIndexData()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (void*)indices16.data();
            }
            return (void*)indices32.data();
        }
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
        }
        

        bool IsIndices16()
        {
            return indices32.size() < c_nMaxUint16;
        }
        void EnsureIndices16()
        {
            if (IsIndices16() && indices16.empty())
            {
                indices16.clear();
                indices16.resize(indices32.size());
                for (size_t i = 0; i < indices32.size(); i++)
                {
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<FMeshDataUV2> FMeshDataUV2Vector;


    //////////////////////////////// FMeshDataSkin ///////////////////////////////////
    struct FMeshDataSkin
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        FMeshVertexSkinVector vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        FAABB aabb;

        FMeshDataSkin()
            : bIsFlipY(true)
        {

        }

        void Clear()
        {
            vertices.clear();
            indices16.clear();
            indices32.clear();
        }

        void ReserveVertexCount(size_t nCount)
        {
            vertices.reserve(nCount);
        }
        void ResizeVertexCount(size_t nCount)
        {
            vertices.resize(nCount);
        }
        FMeshVertexSkin& GetVertex(uint32 nIndex)
        {
            return vertices[nIndex];
        }
        void AddVertex(const FMeshVertexSkin& meshVertex)
        {
            aabb.Merge(meshVertex.pos);
            vertices.push_back(meshVertex);
        }
        void SetVertex(int nIndex, const FMeshVertexSkin& meshVertex)
        {
            vertices[nIndex] = meshVertex;
        }
        void RefreshAABB()
        {
            aabb.SetNull();
            size_t count = vertices.size();
            for (size_t i = 0; i < count; i++)
            {
                FMeshVertexSkin& meshVertex = vertices[i];
                aabb.Merge(meshVertex.pos);
            }
        }

        void ReserveIndexCount(size_t nCount)
        {
            indices32.reserve(nCount);
        }
        void ResizeIndexCount(size_t nCount)
        {
            indices32.resize(nCount);
        }
        uint32 GetIndex(uint32 nIndex)
        {
            return indices32[nIndex];
        }
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndices(uint32 nCount, uint32* pIndex)
        {
            for (uint32 i = 0; i < nCount; i++)
            {
                AddIndex(pIndex[i]);
            }
        }
        void SetIndex(uint32 index, uint32 nIndex)
        {
            indices32[index] = nIndex;
        }
        void AddIndexLine(uint32 nIndex1, uint32 nIndex2)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }
        void SetIndexTriangle(uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
        {
            indices32[indexStart + 0] = index1;
            indices32[indexStart + 1] = index2;
            indices32[indexStart + 2] = index3;
        }


        Indices16Vector& GetIndices16()
        {
            return indices16;
        }
        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(FMeshVertexSkin));
        }

        void* GetIndexData()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (void*)indices16.data();
            }
            return (void*)indices32.data();
        }
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
        }


        bool IsIndices16()
        {
            return indices32.size() < c_nMaxUint16;
        }
        void EnsureIndices16()
        {
            if (IsIndices16() && indices16.empty())
            {
                indices16.clear();
                indices16.resize(indices32.size());
                for (size_t i = 0; i < indices32.size(); i++)
                {
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<FMeshDataSkin> FMeshDataSkinVector;
    

}; //LostPeterFoundation

#endif