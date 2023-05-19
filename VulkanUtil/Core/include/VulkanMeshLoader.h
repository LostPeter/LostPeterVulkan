/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_MESH_LOADER_H_
#define _VULKAN_MESH_LOADER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanMeshLoader
    {
    public:
    ////MeshData 
        static bool LoadMeshData(const String& pathMesh, MeshData& meshData, unsigned int eMeshParserFlags);
        static bool LoadMeshData(const char* szPathMesh, MeshData& meshData, unsigned int eMeshParserFlags);

        static bool LoadMeshDatas(const String& pathMesh, std::vector<MeshData>& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags);
        static bool LoadMeshDatas(const char* szPathMesh, std::vector<MeshData>& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags);


    public:
    protected:
        

    };

}; //LostPeter

#endif