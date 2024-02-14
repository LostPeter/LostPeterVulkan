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

#ifndef _F_MESH_DATA_LOADER_H_
#define _F_MESH_DATA_LOADER_H_

#include "FPreDefine.h"
#include "FMeshData.h"
#include "FMeshVertex.h"

namespace LostPeterFoundation
{
    class foundationExport FMeshDataLoader
    {
    public:
    ////FMeshData 
        static bool LoadMeshData(const String& pathMesh, FMeshData& meshData, unsigned int eMeshParserFlags);
        static bool LoadMeshData(const char* szPathMesh, FMeshData& meshData, unsigned int eMeshParserFlags);

        static bool LoadMeshDatas(const String& pathMesh, FMeshDataVector& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags);
        static bool LoadMeshDatas(const char* szPathMesh, FMeshDataVector& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags);


    public:
        //FMeshData
        static void ExtractVertexData_Pos3Color4Tex2(FMeshData& meshData, std::vector<FVertex_Pos3Color4Tex2>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);
        static void ExtractVertexData_Pos3Color4Normal3Tex2(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tex2>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);
        static void ExtractVertexData_Pos3Color4Normal3Tex4(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tex4>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);
        static void ExtractVertexData_Pos3Color4Normal3Tangent3Tex2(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);
        static void ExtractVertexData_Pos3Color4Normal3Tangent3Tex4(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tangent3Tex4>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);
        static void ExtractIndexData(FMeshData& meshData, std::vector<uint32_t>& aIndices);

        //FMeshDataPC
        static void ExtractVertexData_Pos3Color4Tex2(FMeshDataPC& meshDataPC, std::vector<FVertex_Pos3Color4>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);
        static void ExtractIndexData(FMeshDataPC& meshDataPC, std::vector<uint32_t>& aIndices);
    };

}; //LostPeterFoundation

#endif