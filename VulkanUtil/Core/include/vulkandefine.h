// author: LostPeter
// time:   2022-12-05

#ifndef _VULKAN_DEFINE_H_
#define _VULKAN_DEFINE_H_

#include "predefine.h"
#include "mathutil.h"

namespace LostPeter
{
/////Struct
    //////////////////////////////// Vertex_Pos2Color4 //////////////////////////////
    //0: Vulkan_VertexType_Pos2Color4
    struct utilExport Vertex_Pos2Color4
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec2 pos;
        glm::vec4 color;


        Vertex_Pos2Color4()
        {
            Reset();
        }

        Vertex_Pos2Color4(const glm::vec2& _pos, 
                          const glm::vec4& _color)
            : pos(_pos)
            , color(_color)
        {

        }

        static const VkVertexInputBindingDescriptionVector& GetBindingDescriptions()
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return s_aBindingDescriptions;
        } 
        static VkVertexInputBindingDescriptionVector* GetBindingDescriptionsPtr()
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return &s_aBindingDescriptions;
        }
        static void createBindingDescriptions()
        {
            s_aBindingDescriptions.clear();

            VkVertexInputBindingDescription bindingDescription0 = {};
            bindingDescription0.binding = 0;
            bindingDescription0.stride = sizeof(Vertex_Pos2Color4);
            bindingDescription0.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            s_aBindingDescriptions.push_back(bindingDescription0);
        }

        static const VkVertexInputAttributeDescriptionVector& GetAttributeDescriptions()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return s_aAttributeDescriptions;
        }
        static VkVertexInputAttributeDescriptionVector* GetAttributeDescriptionsPtr()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return &s_aAttributeDescriptions;
        }
        static void createAttributeDescriptions()
        {
            s_aAttributeDescriptions.clear();

            VkVertexInputAttributeDescription attributeDescription0 = {};
            attributeDescription0.binding = 0;
            attributeDescription0.location = 0;
            attributeDescription0.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription0.offset = offsetof(Vertex_Pos2Color4, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos2Color4, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);
        }


        bool operator==(const Vertex_Pos2Color4& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color;
        }

        void Reset()
        {
            this->pos = glm::vec2(0, 0);
            this->color = glm::vec4(0, 0, 0, 1);
        }
    };


    //////////////////////////////// Vertex_Pos2Color4Tex2 //////////////////////////
    //1: Vulkan_VertexType_Pos2Color4Tex2
    struct utilExport Vertex_Pos2Color4Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec2 pos;
        glm::vec4 color;
        glm::vec2 texCoord;


        Vertex_Pos2Color4Tex2()
        {
            Reset();
        }

        Vertex_Pos2Color4Tex2(const glm::vec2& _pos, 
                              const glm::vec4& _color, 
                              const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , texCoord(_texCoord)
        {

        }
        
        static const VkVertexInputBindingDescriptionVector& GetBindingDescriptions()
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return s_aBindingDescriptions;
        } 
        static VkVertexInputBindingDescriptionVector* GetBindingDescriptionsPtr()
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return &s_aBindingDescriptions;
        }
        static void createBindingDescriptions()
        {
            s_aBindingDescriptions.clear();

            VkVertexInputBindingDescription bindingDescription0 = {};
            bindingDescription0.binding = 0;
            bindingDescription0.stride = sizeof(Vertex_Pos2Color4Tex2);
            bindingDescription0.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            s_aBindingDescriptions.push_back(bindingDescription0);
        }

        static const VkVertexInputAttributeDescriptionVector& GetAttributeDescriptions()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return s_aAttributeDescriptions;
        }
        static VkVertexInputAttributeDescriptionVector* GetAttributeDescriptionsPtr()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return &s_aAttributeDescriptions;
        }
        static void createAttributeDescriptions()
        {
            s_aAttributeDescriptions.clear();

            VkVertexInputAttributeDescription attributeDescription0 = {};
            attributeDescription0.binding = 0;
            attributeDescription0.location = 0;
            attributeDescription0.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription0.offset = offsetof(Vertex_Pos2Color4Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos2Color4Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos2Color4Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription2);
        }

        bool operator==(const Vertex_Pos2Color4Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = glm::vec2(0, 0);
            this->color = glm::vec4(0, 0, 0, 1);
            this->texCoord = glm::vec2(0, 0);
        }
    };

    //////////////////////////////// Vertex_Pos3Color4Tex2 //////////////////////////
    //2: Vulkan_VertexType_Pos3Color4Tex2
    struct utilExport Vertex_Pos3Color4Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 texCoord;

        Vertex_Pos3Color4Tex2()
        {
            Reset();
        }

        Vertex_Pos3Color4Tex2(const glm::vec3& _pos, 
                              const glm::vec4& _color, 
                              const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , texCoord(_texCoord)
        {

        }

        static const VkVertexInputBindingDescriptionVector& GetBindingDescriptions() 
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return s_aBindingDescriptions;
        } 
        static VkVertexInputBindingDescriptionVector* GetBindingDescriptionsPtr()
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return &s_aBindingDescriptions;
        }
        static void createBindingDescriptions()
        {
            s_aBindingDescriptions.clear();

            VkVertexInputBindingDescription bindingDescription0 = {};
            bindingDescription0.binding = 0;
            bindingDescription0.stride = sizeof(Vertex_Pos3Color4Tex2);
            bindingDescription0.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            s_aBindingDescriptions.push_back(bindingDescription0);
        }
        
        static const VkVertexInputAttributeDescriptionVector& GetAttributeDescriptions()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return s_aAttributeDescriptions;
        }
        static VkVertexInputAttributeDescriptionVector* GetAttributeDescriptionsPtr()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return &s_aAttributeDescriptions;
        }
        static void createAttributeDescriptions()
        {
            s_aAttributeDescriptions.clear();

            VkVertexInputAttributeDescription attributeDescription0 = {};
            attributeDescription0.binding = 0;
            attributeDescription0.location = 0;
            attributeDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription0.offset = offsetof(Vertex_Pos3Color4Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Color4Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Color4Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription2);
        }

        bool operator==(const Vertex_Pos3Color4Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = glm::vec3(0, 0, 0);
            this->color = glm::vec4(0, 0, 0, 1);
            this->texCoord = glm::vec2(0, 0);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tex2 ///////////////////
    //3: Vulkan_VertexType_Pos3Color4Normal3Tex2
    struct utilExport Vertex_Pos3Color4Normal3Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec3 pos;
        glm::vec4 color;
        glm::vec3 normal;
        glm::vec2 texCoord;

        Vertex_Pos3Color4Normal3Tex2()
        {
            Reset();
        }

        Vertex_Pos3Color4Normal3Tex2(const glm::vec3& _pos, 
                                     const glm::vec4& _color, 
                                     const glm::vec3& _normal, 
                                     const glm::vec2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , texCoord(_texCoord)
        {
            
        }

        static const VkVertexInputBindingDescriptionVector& GetBindingDescriptions() 
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return s_aBindingDescriptions;
        } 
        static VkVertexInputBindingDescriptionVector* GetBindingDescriptionsPtr()
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return &s_aBindingDescriptions;
        }
        static void createBindingDescriptions()
        {
            s_aBindingDescriptions.clear();

            VkVertexInputBindingDescription bindingDescription0 = {};
            bindingDescription0.binding = 0;
            bindingDescription0.stride = sizeof(Vertex_Pos3Color4Normal3Tex2);
            bindingDescription0.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            s_aBindingDescriptions.push_back(bindingDescription0);
        }

        static const VkVertexInputAttributeDescriptionVector& GetAttributeDescriptions()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return s_aAttributeDescriptions;
        }
        static VkVertexInputAttributeDescriptionVector* GetAttributeDescriptionsPtr()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return &s_aAttributeDescriptions;
        }
        static void createAttributeDescriptions()
        {
            s_aAttributeDescriptions.clear();

            VkVertexInputAttributeDescription attributeDescription0 = {};
            attributeDescription0.binding = 0;
            attributeDescription0.location = 0;
            attributeDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription0.offset = offsetof(Vertex_Pos3Color4Normal3Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Color4Normal3Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Color4Normal3Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription3.offset = offsetof(Vertex_Pos3Color4Normal3Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription3);
        }

        bool operator==(const Vertex_Pos3Color4Normal3Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->normal == other.normal && 
                this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = glm::vec3(0, 0, 0);
            this->color = glm::vec4(0, 0, 0, 1);
            this->normal = glm::vec3(0, 1, 0);
            this->texCoord = glm::vec2(0, 0);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex2 ///////////////
    //4: Vulkan_VertexType_Pos3Color4Normal3Tangent3Tex2
    struct utilExport Vertex_Pos3Color4Normal3Tangent3Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec3 pos;
        glm::vec4 color;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texCoord;


        Vertex_Pos3Color4Normal3Tangent3Tex2()
        {
            Reset();
        }

        Vertex_Pos3Color4Normal3Tangent3Tex2(const glm::vec3& _pos, 
                                             const glm::vec4& _color, 
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

        Vertex_Pos3Color4Normal3Tangent3Tex2(float px, float py, float pz,
                                             float nx, float ny, float nz,
                                             float tx, float ty, float tz,
                                             float u, float v)
            : pos(px, py, pz)
            , color(1.0f, 1.0f, 1.0f, 1.0f)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v)
        {

        }

        Vertex_Pos3Color4Normal3Tangent3Tex2(float px, float py, float pz,
                                             float cx, float cy, float cz, float cw,
                                             float nx, float ny, float nz,
                                             float tx, float ty, float tz,
                                             float u, float v)
            : pos(px, py, pz)
            , color(cx, cy, cz, cw)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v)
        {

        }

        static const VkVertexInputBindingDescriptionVector& GetBindingDescriptions() 
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return s_aBindingDescriptions;
        } 
        static VkVertexInputBindingDescriptionVector* GetBindingDescriptionsPtr()
        {
            if (s_aBindingDescriptions.empty())
            {
                createBindingDescriptions();
            }
            return &s_aBindingDescriptions;
        }
        static void createBindingDescriptions()
        {
            s_aBindingDescriptions.clear();

            VkVertexInputBindingDescription bindingDescription0 = {};
            bindingDescription0.binding = 0;
            bindingDescription0.stride = sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2);
            bindingDescription0.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            s_aBindingDescriptions.push_back(bindingDescription0);
        }

        static const VkVertexInputAttributeDescriptionVector& GetAttributeDescriptions()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return s_aAttributeDescriptions;
        }
        static VkVertexInputAttributeDescriptionVector* GetAttributeDescriptionsPtr()
        {
            if (s_aAttributeDescriptions.empty())
            {
                createAttributeDescriptions();
            }
            return &s_aAttributeDescriptions;
        }
        static void createAttributeDescriptions()
        {
            s_aAttributeDescriptions.clear();

            VkVertexInputAttributeDescription attributeDescription0 = {};
            attributeDescription0.binding = 0;
            attributeDescription0.location = 0;
            attributeDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription0.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription3.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription4.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription4);
        }

        bool operator==(const Vertex_Pos3Color4Normal3Tangent3Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->normal == other.normal && 
                this->tangent == other.tangent && 
                this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = glm::vec3(0, 0, 0);
            this->color = glm::vec4(0, 0, 0, 1);
            this->normal = glm::vec3(0, 1, 0);
            this->tangent = glm::vec3(0, 0, 1);
            this->texCoord = glm::vec2(0, 0);
        }
    };


	//////////////////////////////// MeshData ///////////////////////////////////////
    struct utilExport MeshData
    {
        const size_t c_nMaxUint16 = std::numeric_limits<unsigned short>::max();

        std::vector<MeshVertex> vertices;
        std::vector<unsigned short> indices16;
        std::vector<unsigned int> indices32;
        bool bIsFlipY;

        MeshData()
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
        void AddVertex(const MeshVertex& meshVertex)
        {
            vertices.push_back(meshVertex);
        }

        void ReserveIndexCount(size_t nCount)
        {
            indices32.reserve(nCount);
        }
        void ResizeIndexCount(size_t nCount)
        {
            indices32.resize(nCount);
        }
        void AddIndex(unsigned int nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndexTriangle(unsigned int nIndex1, unsigned int nIndex2, unsigned int nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }

        std::vector<unsigned short>& GetIndices16()
        {
            return indices16;
        }

        std::vector<unsigned int>& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        unsigned int GetVertexCount()
        {
            return (unsigned int)vertices.size();
        }
        unsigned int GetVertexDataSize()
        {
            return (unsigned int)(vertices.size() * sizeof(MeshVertex));
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
        unsigned int GetIndexCount()
        {
            return (unsigned int)indices32.size();
        }
        unsigned int GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (unsigned int)(indices16.size() * sizeof(unsigned short));
            }
            return (unsigned int)(indices32.size() * sizeof(unsigned int));
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
                    indices16[i] = (unsigned short)indices32[i];
                }
            }
        }
    };

	//////////////////////////////// SubmeshGeometry ////////////////////////////////
    struct utilExport SubmeshGeometry
    {
        unsigned int indexCount = 0;
        unsigned int startIndexLocation = 0;
        int32 baseVertexLocation = 0;
    };

    //////////////////////////////// MeshGeometry ///////////////////////////////////
    struct utilExport MeshGeometry
    {
        std::string name;
    };


    //////////////////////////////// Light //////////////////////////////////////////
    struct utilExport Light
    {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };   // point light only
        float falloffStart = 1.0f;                   // point/spot light only
        glm::vec3 direction = { 0.0f, -1.0f, 0.0f }; // directional/spot light only
        float falloffEnd = 10.0f;                    // point/spot light only
        glm::vec3 strength = { 0.5f, 0.5f, 0.5f };   // directional/point/spot
        float spotPower = 64.0f;                     // spot light only
    };

    //////////////////////////////// Texture ////////////////////////////////////////
    struct utilExport Texture
    {
        std::string name;
        std::string nameFile;
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

        Light g_Lights[MAX_LIGHT_COUNT];
    };

    //////////////////////////////// ObjectConstants ////////////////////////////////
    struct utilExport ObjectConstants
    {
        glm::mat4 g_MatWorld = MathUtil::Identity4x4();
        
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

    //////////////////////////////// InstanceConstants //////////////////////////////
    struct utilExport InstanceConstants
    {
        int indexObject = 0;
        int indexMaterial = 0;
        
    };

}; //LostPeter

namespace std 
{
    template<> struct hash<LostPeter::Vertex_Pos2Color4> {
        size_t operator()(LostPeter::Vertex_Pos2Color4 const& vertex) const 
        {
            size_t hash = std::hash<glm::vec2>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            return hash;
        }
    };

    template<> struct hash<LostPeter::Vertex_Pos2Color4Tex2> {
        size_t operator()(LostPeter::Vertex_Pos2Color4Tex2 const& vertex) const 
        {
            size_t hash = std::hash<glm::vec2>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
            return hash;
        }
    };

    template<> struct hash<LostPeter::Vertex_Pos3Color4Tex2> {
        size_t operator()(LostPeter::Vertex_Pos3Color4Tex2 const& vertex) const 
        {
            size_t hash = std::hash<glm::vec3>()(vertex.pos);
            hash = hash ^ (std::hash<glm::vec3>()(vertex.color) << 1);
            hash = hash >> 1;
            hash = hash ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
            return hash;
        }
    };

    template<> struct hash<LostPeter::Vertex_Pos3Color4Normal3Tex2> {
        size_t operator()(LostPeter::Vertex_Pos3Color4Normal3Tex2 const& vertex) const
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

    template<> struct hash<LostPeter::Vertex_Pos3Color4Normal3Tangent3Tex2> {
        size_t operator()(LostPeter::Vertex_Pos3Color4Normal3Tangent3Tex2 const& vertex) const
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

}; //LostPeter

#endif