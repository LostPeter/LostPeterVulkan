// author: LostPeter
// time:   2022-10-30

#ifndef _PRE_INCLUDE_H_
#define _PRE_INCLUDE_H_

#include "premacro.h"

//C
#include <float.h>
#include <assert.h>
#include <string.h>


//C++
#include <stdexcept>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <memory>

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>


//Platform
#if UTIL_PLATFORM == UTIL_PLATFORM_WIN32
	#undef min
	#undef max
	#if defined(__MINGW32__)
		#include <unistd.h>
	#endif
	#include <io.h>
	#include <process.h>
    #include <Windows.h>
    #include <shlwapi.h>

#elif UTIL_PLATFORM == UTIL_PLATFORM_MAC

#elif UTIL_PLATFORM == UTIL_PLATFORM_LINUX

#elif UTIL_PLATFORM == UTIL_PLATFORM_ANDROID

#elif UTIL_PLATFORM == UTIL_PLATFORM_IOS
    

#endif

//GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

//Vulkan
#include <vulkan/vulkan.h>

//tiny_obj_loader
#include <tiny_obj_loader.h>

//stb_image
#include <stb_image.h>

//imgui
#include <imgui/imgui.h>
#include <impl_glfw/imgui_impl_glfw.h>
#include <impl_vulkan/imgui_impl_vulkan.h>

//User
#include "predefine.h"
#include "vulkanutil.h"
#include "app.h"
#include "mathutil.h"

namespace LibUtil
{	
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;

    #define UTIL_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))    
    #define UTIL_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))     

    enum VertexType
    {
        VertexType_Pos2Color3 = 0,
        VertexType_Pos2Color3Tex2,
        VertexType_Pos3Color3Tex2,
        VertexType_Pos3Color3Normal3Tex2,
        VertexType_Default,

        Count
    };
    
    //Struct
    //////////////////////////////// Vertex_Pos2Color3 //////////////////////////////
    struct utilExport Vertex_Pos2Color3
    {
        static VkVertexInputBindingDescription* s_pBindingDescription;
        static std::array<VkVertexInputAttributeDescription, 2>* s_pVertexInputAttributeDescriptions;

        glm::vec2 pos;
        glm::vec3 color;


        Vertex_Pos2Color3()
        {

        }

        Vertex_Pos2Color3(const glm::vec2& _pos, 
                          const glm::vec3& _color)
            : pos(_pos)
            , color(_color)
        {

        }

        static VkVertexInputBindingDescription* getBindingDescriptionPtr() 
        {
            if (s_pBindingDescription == nullptr)
            {
                s_pBindingDescription = new VkVertexInputBindingDescription();
                s_pBindingDescription->binding = 0;
                s_pBindingDescription->stride = sizeof(Vertex_Pos2Color3);
                s_pBindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            }
            return s_pBindingDescription;
        } 
        static VkVertexInputBindingDescription getBindingDescription() 
        {
            VkVertexInputBindingDescription* pBindingDescription = getBindingDescriptionPtr();
            return *pBindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2>* getAttributeDescriptionsPtr()
        {
            if (s_pVertexInputAttributeDescriptions == nullptr)
            {
                s_pVertexInputAttributeDescriptions = new std::array<VkVertexInputAttributeDescription, 2>();

                (*s_pVertexInputAttributeDescriptions)[0].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[0].location = 0;
                (*s_pVertexInputAttributeDescriptions)[0].format = VK_FORMAT_R32G32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[0].offset = offsetof(Vertex_Pos2Color3, pos);

                (*s_pVertexInputAttributeDescriptions)[1].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[1].location = 1;
                (*s_pVertexInputAttributeDescriptions)[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[1].offset = offsetof(Vertex_Pos2Color3, color);
            }
            return s_pVertexInputAttributeDescriptions;
        }
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() 
        {
            std::array<VkVertexInputAttributeDescription, 2>* pVertexInputAttributeDescriptions = getAttributeDescriptionsPtr();
            return *pVertexInputAttributeDescriptions;
        }

        bool operator==(const Vertex_Pos2Color3& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color;
        }
    };


    //////////////////////////////// Vertex_Pos2Color3Tex2 //////////////////////////
    struct utilExport Vertex_Pos2Color3Tex2
    {
        static VkVertexInputBindingDescription* s_pBindingDescription;
        static std::array<VkVertexInputAttributeDescription, 3>* s_pVertexInputAttributeDescriptions;

        glm::vec2 pos;
        glm::vec3 color;
        glm::vec2 texCoord;


        Vertex_Pos2Color3Tex2()
        {

        }

        Vertex_Pos2Color3Tex2(const glm::vec2& _pos, 
                              const glm::vec3& _color, 
                              const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , texCoord(_texCoord)
        {

        }
        
        static VkVertexInputBindingDescription* getBindingDescriptionPtr() 
        {
            if (s_pBindingDescription == nullptr)
            {
                s_pBindingDescription = new VkVertexInputBindingDescription();
                s_pBindingDescription->binding = 0;
                s_pBindingDescription->stride = sizeof(Vertex_Pos2Color3Tex2);
                s_pBindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            }
            return s_pBindingDescription;
        } 
        static VkVertexInputBindingDescription getBindingDescription() 
        {
            VkVertexInputBindingDescription* pBindingDescription = getBindingDescriptionPtr();
            return *pBindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3>* getAttributeDescriptionsPtr()
        {
            if (s_pVertexInputAttributeDescriptions == nullptr)
            {
                s_pVertexInputAttributeDescriptions = new std::array<VkVertexInputAttributeDescription, 3>();

                (*s_pVertexInputAttributeDescriptions)[0].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[0].location = 0;
                (*s_pVertexInputAttributeDescriptions)[0].format = VK_FORMAT_R32G32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[0].offset = offsetof(Vertex_Pos2Color3Tex2, pos);

                (*s_pVertexInputAttributeDescriptions)[1].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[1].location = 1;
                (*s_pVertexInputAttributeDescriptions)[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[1].offset = offsetof(Vertex_Pos2Color3Tex2, color);

                (*s_pVertexInputAttributeDescriptions)[2].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[2].location = 2;
                (*s_pVertexInputAttributeDescriptions)[2].format = VK_FORMAT_R32G32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[2].offset = offsetof(Vertex_Pos2Color3Tex2, texCoord);
            }
            return s_pVertexInputAttributeDescriptions;
        }
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() 
        {
            std::array<VkVertexInputAttributeDescription, 3>* pVertexInputAttributeDescriptions = getAttributeDescriptionsPtr();
            return *pVertexInputAttributeDescriptions;
        }

        bool operator==(const Vertex_Pos2Color3Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->texCoord == other.texCoord;
        }
    };

    //////////////////////////////// Vertex_Pos3Color3Tex2 //////////////////////////
    struct utilExport Vertex_Pos3Color3Tex2
    {
        static VkVertexInputBindingDescription* s_pBindingDescription;
        static std::array<VkVertexInputAttributeDescription, 3>* s_pVertexInputAttributeDescriptions;

        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        Vertex_Pos3Color3Tex2()
        {

        }

        Vertex_Pos3Color3Tex2(const glm::vec3& _pos, 
                              const glm::vec3& _color, 
                              const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , texCoord(_texCoord)
        {

        }

        static VkVertexInputBindingDescription* getBindingDescriptionPtr() 
        {
            if (s_pBindingDescription == nullptr)
            {
                s_pBindingDescription = new VkVertexInputBindingDescription();
                s_pBindingDescription->binding = 0;
                s_pBindingDescription->stride = sizeof(Vertex_Pos3Color3Tex2);
                s_pBindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            }
            return s_pBindingDescription;
        } 
        static VkVertexInputBindingDescription getBindingDescription() 
        {
            VkVertexInputBindingDescription* pBindingDescription = getBindingDescriptionPtr();
            return *pBindingDescription;
        }
        
        static std::array<VkVertexInputAttributeDescription, 3>* getAttributeDescriptionsPtr()
        {
            if (s_pVertexInputAttributeDescriptions == nullptr)
            {
                s_pVertexInputAttributeDescriptions = new std::array<VkVertexInputAttributeDescription, 3>();

                (*s_pVertexInputAttributeDescriptions)[0].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[0].location = 0;
                (*s_pVertexInputAttributeDescriptions)[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[0].offset = offsetof(Vertex_Pos3Color3Tex2, pos);

                (*s_pVertexInputAttributeDescriptions)[1].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[1].location = 1;
                (*s_pVertexInputAttributeDescriptions)[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[1].offset = offsetof(Vertex_Pos3Color3Tex2, color);

                (*s_pVertexInputAttributeDescriptions)[2].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[2].location = 2;
                (*s_pVertexInputAttributeDescriptions)[2].format = VK_FORMAT_R32G32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[2].offset = offsetof(Vertex_Pos3Color3Tex2, texCoord);
            }
            return s_pVertexInputAttributeDescriptions;
        }
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() 
        {
            std::array<VkVertexInputAttributeDescription, 3>* pVertexInputAttributeDescriptions = getAttributeDescriptionsPtr();
            return *pVertexInputAttributeDescriptions;
        }

        bool operator==(const Vertex_Pos3Color3Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->texCoord == other.texCoord;
        }
    };


    //////////////////////////////// Vertex_Pos3Color3Normal3Tex2 ///////////////////
    struct utilExport Vertex_Pos3Color3Normal3Tex2
    {
        static VkVertexInputBindingDescription* s_pBindingDescription;
        static std::array<VkVertexInputAttributeDescription, 4>* s_pVertexInputAttributeDescriptions;

        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec2 texCoord;

        Vertex_Pos3Color3Normal3Tex2()
        {

        }

        Vertex_Pos3Color3Normal3Tex2(const glm::vec3& _pos, 
                                     const glm::vec3& _color, 
                                     const glm::vec3& _normal, 
                                     const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , texCoord(_texCoord)
        {
            
        }

        static VkVertexInputBindingDescription* getBindingDescriptionPtr() 
        {
            if (s_pBindingDescription == nullptr)
            {
                s_pBindingDescription = new VkVertexInputBindingDescription();
                s_pBindingDescription->binding = 0;
                s_pBindingDescription->stride = sizeof(Vertex_Pos3Color3Normal3Tex2);
                s_pBindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            }
            return s_pBindingDescription;
        } 
        static VkVertexInputBindingDescription getBindingDescription() 
        {
            VkVertexInputBindingDescription* pBindingDescription = getBindingDescriptionPtr();
            return *pBindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 4>* getAttributeDescriptionsPtr()
        {
            if (s_pVertexInputAttributeDescriptions == nullptr)
            {
                s_pVertexInputAttributeDescriptions = new std::array<VkVertexInputAttributeDescription, 4>();

                (*s_pVertexInputAttributeDescriptions)[0].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[0].location = 0;
                (*s_pVertexInputAttributeDescriptions)[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[0].offset = offsetof(Vertex_Pos3Color3Normal3Tex2, pos);

                (*s_pVertexInputAttributeDescriptions)[1].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[1].location = 1;
                (*s_pVertexInputAttributeDescriptions)[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[1].offset = offsetof(Vertex_Pos3Color3Normal3Tex2, color);

                (*s_pVertexInputAttributeDescriptions)[2].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[2].location = 2;
                (*s_pVertexInputAttributeDescriptions)[2].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[2].offset = offsetof(Vertex_Pos3Color3Normal3Tex2, normal);

                (*s_pVertexInputAttributeDescriptions)[3].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[3].location = 3;
                (*s_pVertexInputAttributeDescriptions)[3].format = VK_FORMAT_R32G32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[3].offset = offsetof(Vertex_Pos3Color3Normal3Tex2, texCoord);
            }
            return s_pVertexInputAttributeDescriptions;
        }
        static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() 
        {
            std::array<VkVertexInputAttributeDescription, 4>* pVertexInputAttributeDescriptions = getAttributeDescriptionsPtr();
            return *pVertexInputAttributeDescriptions;
        }

        bool operator==(const Vertex_Pos3Color3Normal3Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->normal == other.normal && 
                this->texCoord == other.texCoord;
        }
    };


    //////////////////////////////// Vertex /////////////////////////////////////////
    struct utilExport Vertex
    {
        static VkVertexInputBindingDescription* s_pBindingDescription;
        static std::array<VkVertexInputAttributeDescription, 5>* s_pVertexInputAttributeDescriptions;

        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texCoord;


        Vertex()
        {

        }

        Vertex(const glm::vec3& _pos, 
               const glm::vec3& _color, 
               const glm::vec3& _normal, 
               const glm::vec3& _tangent, 
               const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , tangent(_tangent)
            , texCoord(_texCoord)
        {
            
        }

        Vertex(float px, float py, float pz,
               float nx, float ny, float nz,
               float tx, float ty, float tz,
               float u, float v)
            : pos(px, py, pz)
            , color(1.0f, 1.0f, 1.0f)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v)
        {

        }

        Vertex(float px, float py, float pz,
               float cx, float cy, float cz,  
               float nx, float ny, float nz,
               float tx, float ty, float tz,
               float u, float v)
            : pos(px, py, pz)
            , color(cx, cy, cz)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v)
        {

        }

        static VkVertexInputBindingDescription* getBindingDescriptionPtr() 
        {
            if (s_pBindingDescription == nullptr)
            {
                s_pBindingDescription = new VkVertexInputBindingDescription();
                s_pBindingDescription->binding = 0;
                s_pBindingDescription->stride = sizeof(Vertex);
                s_pBindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            }
            return s_pBindingDescription;
        } 
        static VkVertexInputBindingDescription getBindingDescription() 
        {
            VkVertexInputBindingDescription* pBindingDescription = getBindingDescriptionPtr();
            return *pBindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 5>* getAttributeDescriptionsPtr()
        {
            if (s_pVertexInputAttributeDescriptions == nullptr)
            {
                s_pVertexInputAttributeDescriptions = new std::array<VkVertexInputAttributeDescription, 5>();

                (*s_pVertexInputAttributeDescriptions)[0].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[0].location = 0;
                (*s_pVertexInputAttributeDescriptions)[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[0].offset = offsetof(Vertex, pos);

                (*s_pVertexInputAttributeDescriptions)[1].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[1].location = 1;
                (*s_pVertexInputAttributeDescriptions)[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[1].offset = offsetof(Vertex, color);

                (*s_pVertexInputAttributeDescriptions)[2].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[2].location = 2;
                (*s_pVertexInputAttributeDescriptions)[2].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[2].offset = offsetof(Vertex, normal);

                (*s_pVertexInputAttributeDescriptions)[3].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[3].location = 3;
                (*s_pVertexInputAttributeDescriptions)[3].format = VK_FORMAT_R32G32B32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[3].offset = offsetof(Vertex, tangent);

                (*s_pVertexInputAttributeDescriptions)[4].binding = 0;
                (*s_pVertexInputAttributeDescriptions)[4].location = 4;
                (*s_pVertexInputAttributeDescriptions)[4].format = VK_FORMAT_R32G32_SFLOAT;
                (*s_pVertexInputAttributeDescriptions)[4].offset = offsetof(Vertex, texCoord);
            }
            return s_pVertexInputAttributeDescriptions;
        }
        static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions() 
        {
            std::array<VkVertexInputAttributeDescription, 5>* pVertexInputAttributeDescriptions = getAttributeDescriptionsPtr();
            return *pVertexInputAttributeDescriptions;
        }

        bool operator==(const Vertex& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->normal == other.normal && 
                this->tangent == other.tangent && 
                this->texCoord == other.texCoord;
        }
    };


	//////////////////////////////// MeshData ///////////////////////////////////////
    struct utilExport MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint16> indices16;
        std::vector<uint32> indices32;

        std::vector<uint16>& GetIndices16()
        {
            return indices16;
        }

        std::vector<uint32>& GetIndices32()
        {
            return indices32;
        }

    };

	//////////////////////////////// SubmeshGeometry ////////////////////////////////
    struct utilExport SubmeshGeometry
    {
        uint32 indexCount = 0;
        uint32 startIndexLocation = 0;
        int32 baseVertexLocation = 0;
    };

    //////////////////////////////// MeshGeometry ///////////////////////////////////
    struct utilExport MeshGeometry
    {
        std::string name;
    };


    //////////////////////////////// Light //////////////////////////////////////////
    #define MaxLights 16
    struct utilExport Light
    {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };   // point light only
        float falloffStart = 1.0f;                   // point/spot light only
        glm::vec3 direction = { 0.0f, -1.0f, 0.0f }; // directional/spot light only
        float falloffEnd = 10.0f;                    // point/spot light only
        glm::vec3 strength = { 0.5f, 0.5f, 0.5f };   // directional/point/spot
        float spotPower = 64.0f;                     // spot light only
    };


    //////////////////////////////// MaterialConstants //////////////////////////////
    struct utilExport MaterialConstants
    {
        glm::vec4 diffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec3 fresnelR0 = { 0.01f, 0.01f, 0.01f };
        float roughness = 0.25f;

        // Used in texture mapping.
        glm::mat4 MatTransform = MathUtil::Identity4x4();
    };


    //////////////////////////////// Material ///////////////////////////////////////
    struct utilExport Material
    {
        std::string name;
        int matCBIndex = -1;
        int diffuseSrvHeapIndex = -1;
        int normalSrvHeapIndex = -1;
        int numFramesDirty = 0;

        glm::vec4 diffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec3 fresnelR0 = { 0.01f, 0.01f, 0.01f };
        float roughness = .25f;
        glm::mat4 matTransform = MathUtil::Identity4x4();
    };


    //////////////////////////////// Texture ////////////////////////////////////////
    struct utilExport Texture
    {
        std::string name;
        std::string nameFile;
    };


    //////////////////////////////// ObjectConstants ////////////////////////////////
    struct utilExport ObjectConstants
    {
        glm::mat4 g_MatWorld = MathUtil::Identity4x4();
        glm::mat4 g_TexTransform = MathUtil::Identity4x4();
    };


    //////////////////////////////// PassConstants //////////////////////////////////
    struct utilExport PassConstants
    {
        glm::mat4 g_MatView = MathUtil::Identity4x4();
        glm::mat4 g_MatView_Inv = MathUtil::Identity4x4();
        glm::mat4 g_MatProj = MathUtil::Identity4x4();
        glm::mat4 g_MatProj_Inv = MathUtil::Identity4x4();
        glm::mat4 g_MatViewProj = MathUtil::Identity4x4();
        glm::mat4 g_MatViewProj_Inv = MathUtil::Identity4x4();

        glm::vec3 g_EyePosW = { 0.0f, 0.0f, 0.0f };
        float g_cbPerObjectPad1 = 0.0f;
        glm::vec2 g_RenderTargetSize = { 0.0f, 0.0f };
        glm::vec2 g_RenderTargetSize_Inv = { 0.0f, 0.0f };
        float g_NearZ = 0.0f;
        float g_FarZ = 0.0f;
        float g_TotalTime = 0.0f;
        float g_DeltaTime = 0.0f;
        glm::vec4 g_AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f };

        Light g_Lights[MaxLights];
    };

}; //LibUtil


namespace std {

    template<> struct hash<LibUtil::Vertex_Pos2Color3> {
        size_t operator()(LibUtil::Vertex_Pos2Color3 const& vertex) const 
        {
            size_t hash = std::hash<glm::vec2>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            return hash;
        }
    };

    template<> struct hash<LibUtil::Vertex_Pos2Color3Tex2> {
        size_t operator()(LibUtil::Vertex_Pos2Color3Tex2 const& vertex) const 
        {
            size_t hash = std::hash<glm::vec2>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
            return hash;
        }
    };

    template<> struct hash<LibUtil::Vertex_Pos3Color3Tex2> {
        size_t operator()(LibUtil::Vertex_Pos3Color3Tex2 const& vertex) const 
        {
            size_t hash = std::hash<glm::vec3>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
            return hash;
        }
    };

    template<> struct hash<LibUtil::Vertex_Pos3Color3Normal3Tex2> {
        size_t operator()(LibUtil::Vertex_Pos3Color3Normal3Tex2 const& vertex) const
        {
            size_t hash = std::hash<glm::vec3>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec3>()(vertex.normal) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
            return hash;
        }
    };

    template<> struct hash<LibUtil::Vertex> {
        size_t operator()(LibUtil::Vertex const& vertex) const
        {
            size_t hash = std::hash<glm::vec3>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec3>()(vertex.normal) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec3>()(vertex.tangent) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
            return hash;
        }
    };

}


#endif