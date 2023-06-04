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

namespace LostPeterFoundation
{
    class LPF_Export FMeshDataLoader
    {
    public:
    ////FMeshData 
        static bool LoadMeshData(const String& pathMesh, FMeshData& meshData, unsigned int eMeshParserFlags);
        static bool LoadMeshData(const char* szPathMesh, FMeshData& meshData, unsigned int eMeshParserFlags);

        static bool LoadMeshDatas(const String& pathMesh, FMeshDataVector& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags);
        static bool LoadMeshDatas(const char* szPathMesh, FMeshDataVector& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags);


    public:
    protected:
        

    };

}; //LostPeterFoundation

#endif