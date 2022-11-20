// author: LostPeter
// time:   2022-10-30

#ifndef _MESH_LOADER_H_
#define _MESH_LOADER_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport MeshLoader
    {
    public:
    ////MeshData 
        static bool LoadMeshData(const std::string& pathMesh, MeshData& meshData, unsigned int eMeshParserFlags);
        static bool LoadMeshData(const char* szPathMesh, MeshData& meshData, unsigned int eMeshParserFlags);

        static bool LoadMeshDatas(const std::string& pathMesh, std::vector<std::string>& aMaterials,  std::vector<MeshData>& aMeshDatas, unsigned int eMeshParserFlags);
        static bool LoadMeshDatas(const char* szPathMesh, std::vector<std::string>& aMaterials,  std::vector<MeshData>& aMeshDatas, unsigned int eMeshParserFlags);


    public:
    protected:
        

    };

}; //LibUtil

#endif