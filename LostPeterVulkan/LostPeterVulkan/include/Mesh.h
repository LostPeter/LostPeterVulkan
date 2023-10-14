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

#ifndef _MESH_H_
#define _MESH_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Mesh : public Base
    {
    public:
        Mesh(uint32 _group,
             const String& _nameMesh,
             const String& _pathMesh,
             FMeshType _typeMesh,
             FMeshVertexType _typeVertex,
             FMeshGeometryType _typeGeometryType,
             FMeshCreateParam* _pMeshCreateParam);
        virtual ~Mesh();

    public:
        String pathMesh;
        FMeshType typeMesh;
        FMeshVertexType typeVertex;
        FMeshGeometryType typeGeometryType;
        FMeshCreateParam* pMeshCreateParam;
        MeshSubPtrVector aMeshSubs;
        MeshSubPtrMap mapMeshSubs;

    public:
        void Destroy();

        virtual bool AddMeshSub(MeshSub* pMeshSub);
        virtual bool LoadMesh(bool isFlipY, 
                              bool isTransformLocal, 
                              const FMatrix4& matTransformLocal,
                              bool isUpdateVertexBuffer = false,
                              bool isUpdateIndexBuffer = false);
    };

}; //LostPeter

#endif