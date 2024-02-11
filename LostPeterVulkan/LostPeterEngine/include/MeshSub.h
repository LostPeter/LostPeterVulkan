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

#ifndef _MESH_SUB_H_
#define _MESH_SUB_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport MeshSub : public Base
    {
        friend class Mesh;

    public:
        MeshSub(const String& nameMeshSub);
        virtual ~MeshSub();

    public:
        Mesh* m_pMesh;
        int32 m_nMeshSubIndex;

    public:
        F_FORCEINLINE Mesh* GetMesh() const { return m_pMesh; }
		F_FORCEINLINE void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
        F_FORCEINLINE int GetMeshSubIndex() const { return m_nMeshSubIndex; }
		F_FORCEINLINE void SetMeshSubIndex(int nMeshSubIndex) { m_nMeshSubIndex = nMeshSubIndex; }

    public:
        void Destroy();
        

    public:



    };

}; //LostPeterEngine

#endif