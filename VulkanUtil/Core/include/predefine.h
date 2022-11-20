// author: LostPeter
// time:   2022-10-30

#ifndef _PRE_DEFINE_H_
#define _PRE_DEFINE_H_

#include "preincludeStd.h"

namespace LibUtil
{
////Typedef
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;

    #define UTIL_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))    
    #define UTIL_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))     


////Class
    class App;
    class VulkanBase;
    class VulkanWindow;

    struct Vertex_Pos2Color3;
    struct Vertex_Pos2Color3Tex2;
    struct Vertex_Pos3Color3Tex2;
    struct Vertex_Pos3Color3Normal3Tex2;
    struct MeshVertex;

    struct MeshData;
    struct SubmeshGeometry;
    struct MeshGeometry;

    struct Light;
    struct MaterialConstants;
    struct Material;
    struct Texture;
    
    struct ObjectConstants;
    struct PassConstants;

    class SceneObject;
    class SceneManager;
    

////Enum
    enum VertexType
    {
        VertexType_Pos2Color3 = 0,
        VertexType_Pos2Color3Tex2,
        VertexType_Pos3Color3Tex2,
        VertexType_Pos3Color3Normal3Tex2,
        VertexType_Default,

        Count
    };

}; //LibUtil


#endif