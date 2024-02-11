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

namespace LostPeterEngine
{
    MeshSub::MeshSub(const String& nameMeshSub)
        : Base(nameMeshSub)
        , m_pMesh(nullptr)
        , m_nMeshSubIndex(0)
    {

    }

    MeshSub::~MeshSub()
    {
        Destroy();
    }

    void MeshSub::Destroy()
    {
        
    }
    

}; //LostPeterEngine