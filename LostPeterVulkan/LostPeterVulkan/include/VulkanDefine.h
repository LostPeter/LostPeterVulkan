/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_DEFINE_H_
#define _VULKAN_DEFINE_H_

#include "PreDefine.h"
#include "VulkanMath.h"

namespace LostPeter
{
/////Struct
    //////////////////////////////// Vertex_Pos2Color4 //////////////////////////////
    //0: Vulkan_Vertex_Pos2Color4
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


    //////////////////////////////// Vertex_Pos3Normal3 /////////////////////////////
    //1: Vulkan_Vertex_Pos3Normal3
    struct utilExport Vertex_Pos3Normal3
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec3 pos;
        glm::vec3 normal;


        Vertex_Pos3Normal3()
        {
            Reset();
        }

        Vertex_Pos3Normal3(const glm::vec3& _pos, 
                           const glm::vec3& _normal)
            : pos(_pos)
            , normal(_normal)
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
            bindingDescription0.stride = sizeof(Vertex_Pos3Normal3);
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
            attributeDescription0.offset = offsetof(Vertex_Pos3Normal3, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Normal3, normal);
            s_aAttributeDescriptions.push_back(attributeDescription1);
        }


        bool operator==(const Vertex_Pos3Normal3& other) const 
        {
            return this->pos == other.pos && 
                this->normal == other.normal;
        }

        void Reset()
        {
            this->pos = glm::vec3(0, 0, 0);
            this->normal = glm::vec3(0, 1, 0);
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3Tex2 /////////////////////////
    //2: Vulkan_Vertex_Pos3Normal3Tex2
    struct utilExport Vertex_Pos3Normal3Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;


        Vertex_Pos3Normal3Tex2()
        {
            Reset();
        }

        Vertex_Pos3Normal3Tex2(const glm::vec3& _pos, 
                               const glm::vec3& _normal,
                               const glm::vec2& _texCoord)
            : pos(_pos)
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
            bindingDescription0.stride = sizeof(Vertex_Pos3Normal3Tex2);
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
            attributeDescription0.offset = offsetof(Vertex_Pos3Normal3Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Normal3Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Normal3Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription2);
        }


        bool operator==(const Vertex_Pos3Normal3Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->normal == other.normal &&
                this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = glm::vec3(0, 0, 0);
            this->normal = glm::vec3(0, 1, 0);
            this->texCoord = glm::vec2(0, 0);
        }
    };


    //////////////////////////////// Vertex_Pos2Color4Tex2 //////////////////////////
    //3: Vulkan_Vertex_Pos2Color4Tex2
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
    //4: Vulkan_Vertex_Pos3Color4Tex2
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
    //5: Vulkan_Vertex_Pos3Color4Normal3Tex2
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


    //////////////////////////////// Vertex_Pos3Color4Normal3Tex4 ///////////////////
    //6: Vulkan_Vertex_Pos3Color4Normal3Tex4
    struct utilExport Vertex_Pos3Color4Normal3Tex4
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec3 pos;
        glm::vec4 color;
        glm::vec3 normal;
        glm::vec4 texCoord;

        Vertex_Pos3Color4Normal3Tex4()
        {
            Reset();
        }

        Vertex_Pos3Color4Normal3Tex4(const glm::vec3& _pos, 
                                     const glm::vec4& _color, 
                                     const glm::vec3& _normal, 
                                     const glm::vec4& _texCoord)
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
            bindingDescription0.stride = sizeof(Vertex_Pos3Color4Normal3Tex4);
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
            attributeDescription0.offset = offsetof(Vertex_Pos3Color4Normal3Tex4, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Color4Normal3Tex4, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Color4Normal3Tex4, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription3.offset = offsetof(Vertex_Pos3Color4Normal3Tex4, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription3);
        }

        bool operator==(const Vertex_Pos3Color4Normal3Tex4& other) const 
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
            this->texCoord = glm::vec4(0, 0, 0, 0);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex2 ///////////////
    //7: Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2
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


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex4 ///////////////
    //8: Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex4
    struct utilExport Vertex_Pos3Color4Normal3Tangent3Tex4
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        glm::vec3 pos;
        glm::vec4 color;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec4 texCoord;


        Vertex_Pos3Color4Normal3Tangent3Tex4()
        {
            Reset();
        }

        Vertex_Pos3Color4Normal3Tangent3Tex4(const glm::vec3& _pos, 
                                             const glm::vec4& _color, 
                                             const glm::vec3& _normal, 
                                             const glm::vec3& _tangent, 
                                             const glm::vec4& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , tangent(_tangent)
            , texCoord(_texCoord)
        {
            
        }

        Vertex_Pos3Color4Normal3Tangent3Tex4(float px, float py, float pz,
                                             float nx, float ny, float nz,
                                             float tx, float ty, float tz,
                                             float u, float v, float w, float x)
            : pos(px, py, pz)
            , color(1.0f, 1.0f, 1.0f, 1.0f)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v, w, x)
        {

        }

        Vertex_Pos3Color4Normal3Tangent3Tex4(float px, float py, float pz,
                                             float cx, float cy, float cz, float cw,
                                             float nx, float ny, float nz,
                                             float tx, float ty, float tz,
                                             float u, float v, float w, float x)
            : pos(px, py, pz)
            , color(cx, cy, cz, cw)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v, w, x)
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
            bindingDescription0.stride = sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4);
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
            attributeDescription0.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription3.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription4.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3Tex4, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription4);
        }

        bool operator==(const Vertex_Pos3Color4Normal3Tangent3Tex4& other) const 
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
            this->texCoord = glm::vec4(0, 0, 0, 0);
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3Tangent3BlendWI8Tex2 //////////
    //9:    Vulkan_Vertex_Pos3Normal3Tangent3BlendWI8Tex2
    struct Vertex_Pos3Normal3Tangent3BlendWI8Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        Vector3 pos;
        Vector3 normal;
        Vector3 tangent;
        Vector4 blendweight;
        Vector4 blendindex;
        Vector2 texCoord;


        Vertex_Pos3Normal3Tangent3BlendWI8Tex2()
        {
            Reset();
        }

        Vertex_Pos3Normal3Tangent3BlendWI8Tex2(const Vector3& _pos, 
                                               const Vector3& _normal, 
                                               const Vector3& _tangent, 
                                               const Vector4& _blendweight,
                                               const Vector4& _blendindex,
                                               const Vector2& _texCoord)
            : pos(_pos)
            , normal(_normal)
            , tangent(_tangent)
            , blendweight(_blendweight)
            , blendindex(_blendindex)
            , texCoord(_texCoord)
        {
            
        }

        Vertex_Pos3Normal3Tangent3BlendWI8Tex2(float px, float py, float pz,
                                               float nx, float ny, float nz,
                                               float tx, float ty, float tz,
                                               float bwx, float bwy, float bwz, float bww,
                                               float bix, float biy, float biz, float biw,  
                                               float u, float v)
            : pos(px, py, pz)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , blendweight(bwx, bwy, bwz, bww)
            , blendindex(bix, biy, biz, biw)
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
            bindingDescription0.stride = sizeof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2);
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
            attributeDescription0.offset = offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription3.offset = offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, blendweight);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription4.offset = offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, blendindex);
            s_aAttributeDescriptions.push_back(attributeDescription4);

            VkVertexInputAttributeDescription attributeDescription5 = {};
            attributeDescription5.binding = 0;
            attributeDescription5.location = 5;
            attributeDescription5.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription5.offset = offsetof(Vertex_Pos3Normal3Tangent3BlendWI8Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription5);
        }

        bool operator==(const Vertex_Pos3Normal3Tangent3BlendWI8Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->normal == other.normal && 
                this->tangent == other.tangent && 
                this->blendweight == other.blendweight &&
                this->blendindex == other.blendindex &&
                this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = Vector3(0, 0, 0);
            this->normal = Vector3(0, 1, 0);
            this->tangent = Vector3(0, 0, 1);
            this->blendweight = Vector4(0, 0, 0, 0);
            this->blendindex = Vector4(0, 0, 0, 0);
            this->texCoord = Vector2(0, 0);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 ////
    //10:    Vulkan_Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
    struct Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2     //SkinMeshVertex
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

        Vector3 pos;
        Vector4 color;
        Vector3 normal;
        Vector3 tangent;
        Vector4 blendweight;
        Vector4 blendindex;
        Vector2 texCoord;


        Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2()
        {
            Reset();
        }

        Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2(const Vector3& _pos, 
                                                     const Vector4& _color, 
                                                     const Vector3& _normal, 
                                                     const Vector3& _tangent, 
                                                     const Vector4& _blendweight,
                                                     const Vector4& _blendindex,
                                                     const Vector2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , tangent(_tangent)
            , blendweight(_blendweight)
            , blendindex(_blendindex)
            , texCoord(_texCoord)
        {
            
        }

        Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2(float px, float py, float pz,
                                                     float cx, float cy, float cz, float cw,
                                                     float nx, float ny, float nz,
                                                     float tx, float ty, float tz,
                                                     float bwx, float bwy, float bwz, float bww,
                                                     float bix, float biy, float biz, float biw,  
                                                     float u, float v)
            : pos(px, py, pz)
            , color(cx, cy, cz, cw)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , blendweight(bwx, bwy, bwz, bww)
            , blendindex(bix, biy, biz, biw)
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
            bindingDescription0.stride = sizeof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2);
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
            attributeDescription0.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription3.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription4.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, blendweight);
            s_aAttributeDescriptions.push_back(attributeDescription4);

            VkVertexInputAttributeDescription attributeDescription5 = {};
            attributeDescription5.binding = 0;
            attributeDescription5.location = 5;
            attributeDescription5.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription5.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, blendindex);
            s_aAttributeDescriptions.push_back(attributeDescription5);

            VkVertexInputAttributeDescription attributeDescription6 = {};
            attributeDescription6.binding = 0;
            attributeDescription6.location = 6;
            attributeDescription6.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription6.offset = offsetof(Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription6);
        }

        bool operator==(const Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2& other) const 
        {
            return this->pos == other.pos && 
                this->color == other.color && 
                this->normal == other.normal && 
                this->tangent == other.tangent && 
                this->blendweight == other.blendweight &&
                this->blendindex == other.blendindex &&
                this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = Vector3(0, 0, 0);
            this->color = Vector4(0, 0, 0, 1);
            this->normal = Vector3(0, 1, 0);
            this->tangent = Vector3(0, 0, 1);
            this->blendweight = Vector4(0, 0, 0, 0);
            this->blendindex = Vector4(0, 0, 0, 0);
            this->texCoord = Vector2(0, 0);
        }
    };

    //////////////////////////////// MeshPCTData ////////////////////////////////////
    struct MeshPCTData
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        MeshVertexPCTVector vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        //VulkanAABB aabb;

        MeshPCTData()
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
        void AddVertex(const MeshVertexPCT& meshVertex)
        {
            //aabb.Merge(meshVertex.pos);
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
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }


        Indices16Vector& GetIndices16()
        {
            return indices16;
        }
        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(MeshVertexPCT));
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
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
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
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<MeshPCTData> MeshPCTDataVector;


	//////////////////////////////// MeshData ///////////////////////////////////////
    struct utilExport MeshData
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        String nameMesh;
        std::vector<MeshVertex> vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        //VulkanAABB aabb;

        MeshData()
            : bIsFlipY(true)
        {

        }

        void Clear()
        {
            nameMesh = "";
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
            //aabb.Merge(meshVertex.pos);
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
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }

        Indices16Vector& GetIndices16()
        {
            return indices16;
        }

        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(MeshVertex));
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
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
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
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<MeshData> MeshDataVector;


    //////////////////////////////// MeshUV2Data ////////////////////////////////////
    struct MeshUV2Data
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        MeshVertexUV2Vector vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        //VulkanAABB aabb;

        MeshUV2Data()
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
        void AddVertex(const MeshVertexUV2& meshVertex)
        {
            //aabb.Merge(meshVertex.pos);
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
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }


        Indices16Vector& GetIndices16()
        {
            return indices16;
        }
        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(MeshVertexUV2));
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
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
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
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<MeshUV2Data> MeshUV2DataVector;


    //////////////////////////////// MeshSkinData ///////////////////////////////////
    struct SkinMeshData
    {
        const size_t c_nMaxUint16 = std::numeric_limits<uint16>::max();

        SkinMeshVertexVector vertices;
        Indices16Vector indices16;
        Indices32Vector indices32;
        bool bIsFlipY;
        //VulkanAABB aabb;

        SkinMeshData()
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
        void AddVertex(const SkinMeshVertex& meshVertex)
        {
            //aabb.Merge(meshVertex.pos);
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
        void AddIndex(uint32 nIndex)
        {
            indices32.push_back(nIndex);
        }
        void AddIndexTriangle(uint32 nIndex1, uint32 nIndex2, uint32 nIndex3)
        {
            indices32.push_back(nIndex1);
            indices32.push_back(nIndex2);
            indices32.push_back(nIndex3);
        }


        Indices16Vector& GetIndices16()
        {
            return indices16;
        }
        Indices32Vector& GetIndices32()
        {
            return indices32;
        }

        void* GetVertexData()
        {
            return (void*)vertices.data();
        }
        uint32 GetVertexCount()
        {
            return (uint32)vertices.size();
        }
        uint32 GetVertexDataSize()
        {
            return (uint32)(vertices.size() * sizeof(SkinMeshVertex));
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
        uint32 GetIndexCount()
        {
            return (uint32)indices32.size();
        }
        uint32 GetIndexDataSize()
        {
            EnsureIndices16();
            if (!indices16.empty())
            {
                return (uint32)(indices16.size() * sizeof(uint16));
            }
            return (uint32)(indices32.size() * sizeof(uint32));
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
                    indices16[i] = (uint16)indices32[i];
                }
            }
        }
    };
    typedef std::vector<SkinMeshData> SkinMeshDataVector;


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
        String name;
    };

    //////////////////////////////// TransformConstants /////////////////////////////
    struct utilExport TransformConstants
    {
        glm::mat4 mat4View;
        glm::mat4 mat4View_Inv;
        glm::mat4 mat4Proj;
        glm::mat4 mat4Proj_Inv;
        glm::mat4 mat4ViewProj;
        glm::mat4 mat4ViewProj_Inv;

        TransformConstants()
            : mat4View(VulkanMath::Identity4x4())
            , mat4View_Inv(VulkanMath::Identity4x4())
            , mat4Proj(VulkanMath::Identity4x4())
            , mat4Proj_Inv(VulkanMath::Identity4x4())
            , mat4ViewProj(VulkanMath::Identity4x4())
            , mat4ViewProj_Inv(VulkanMath::Identity4x4())
        {

        }
    };

    //////////////////////////////// CameraConstants ////////////////////////////////
    struct utilExport CameraConstants
    {
        glm::vec3 posEyeWorld;    
        float fNearZ;
        float fFarZ;
        float fReserved1;
        float fReserved2;
        float fReserved3;

        CameraConstants()
            : posEyeWorld(0.0f, 0.0f, 0.0f)
            , fNearZ(0.0f)
            , fFarZ(0.0f)
            , fReserved1(0.0f)
            , fReserved2(0.0f)
            , fReserved3(0.0f)
        {

        }
    }; 

    //////////////////////////////// LightConstants /////////////////////////////////
    struct utilExport LightConstants
    {
        glm::vec4 common;    // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
        glm::vec3 position;  // directional/point/spot
        float falloffStart;  // point/spot light only
        glm::vec3 direction; // directional/spot light only
        float falloffEnd;    // point/spot light only
        glm::vec4 ambient;   // ambient
        glm::vec4 diffuse;   // diffuse
        glm::vec4 specular;  // specular

        LightConstants()
            : common(0, 0, 0, 64)
            , position(0.0f, 100.0f, 0.0f)
            , falloffStart(1.0f)
            , direction(0.0f, -1.0f, 0.0f)
            , falloffEnd(10.0f)
            , ambient(1.0f, 1.0f, 1.0f, 1.0f)
            , diffuse(1.0f, 1.0f, 1.0f, 1.0f)
            , specular(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };

    //////////////////////////////// PassConstants //////////////////////////////////
    struct utilExport PassConstants
    {
        //TransformConstants
        TransformConstants g_Transforms[2]; //0: Eye Left(Main); 1: Eye Right
        //CameraConstants
        CameraConstants g_Cameras[2]; //0: Eye Left(Main); 1: Eye Right
        
        //TimeConstants
        float g_TotalTime;
        float g_DeltaTime;
        float g_Pad1;
        float g_Pad2;

        //RenderTarget
        glm::vec2 g_RenderTargetSize;   
        glm::vec2 g_RenderTargetSize_Inv;

        //Material
        glm::vec4 g_AmbientLight;

        //Light
        LightConstants g_MainLight;
        LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];

        PassConstants()
            : g_TotalTime(0.0f)
            , g_DeltaTime(0.0f)
            , g_Pad1(1.0f)
            , g_Pad2(1.0f)
            , g_RenderTargetSize(0.0f, 0.0f)
            , g_RenderTargetSize_Inv(0.0f, 0.0f)
            , g_AmbientLight(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };

    //////////////////////////////// ObjectConstants ////////////////////////////////
    struct utilExport ObjectConstants
    {
        glm::mat4 g_MatWorld;
        

        ObjectConstants()
            : g_MatWorld(VulkanMath::Identity4x4())
        {

        }
    };

    //////////////////////////////// TextureConstants ///////////////////////////////
    struct utilExport TextureConstants
    {
        float texWidth;
        float texHeight;
        float texDepth;
        float indexTextureArray;

        float texSpeedU;
        float texSpeedV;
        float texSpeedW;
        float reserve0;

        float texChunkMaxX;
        float texChunkMaxY;
        float texChunkIndexX;
        float texChunkIndexY;

        TextureConstants()
            : texWidth(512.0f)
            , texHeight(512.0f)
            , texDepth(512.0f)
            , indexTextureArray(0.0f)

            , texSpeedU(0.0f)
            , texSpeedV(0.0f)
            , texSpeedW(0.0f)
            , reserve0(0.0f)
            
            , texChunkMaxX(0.0f)
            , texChunkMaxY(0.0f)
            , texChunkIndexX(0.0f)
            , texChunkIndexY(0.0f)
        {

        }
    };
    
    //////////////////////////////// MaterialConstants //////////////////////////////
    struct utilExport MaterialConstants
    {
        glm::vec4 factorAmbient;
        glm::vec4 factorDiffuse;
        glm::vec4 factorSpecular;

        float shininess;
        float alpha;
        float lighting;
        float reserve0;

        TextureConstants aTexLayers[MAX_TEXTURE_COUNT];

        MaterialConstants()
            : factorAmbient(1.0f, 1.0f, 1.0f, 1.0f)
            , factorDiffuse(1.0f, 1.0f, 1.0f, 1.0f)
            , factorSpecular(1.0f, 1.0f, 1.0f, 1.0f)
            , shininess(20.0f)
            , alpha(1.0f)
            , lighting(1.0f)
            , reserve0(0.0f)
        {
            
        }
    };

    //////////////////////////////// InstanceConstants //////////////////////////////
    struct utilExport InstanceConstants
    {
        int indexObject = 0;
        int indexMaterial = 0;

        InstanceConstants()
            : indexObject(0)
            , indexMaterial(0)
        {

        }  
    };

    //////////////////////////////// TextureCopyConstants ///////////////////////////
    struct utilExport TextureCopyConstants
    {
        glm::vec4 texInfo;
        glm::vec4 texOffset;
        glm::vec4 texIndexArray;
        glm::vec4 texClearColor;

        TextureCopyConstants()
            : texInfo(0, 0, 0, 0)
            , texOffset(0, 0, 0, 0)
            , texIndexArray(0, 0, 0, 0)
            , texClearColor(0, 0, 0, 1)
        {

        }
    };

    //////////////////////////////// TessellationConstants //////////////////////////
    struct utilExport TessellationConstants
    {
    ////tesc
        float tessLevelOuter;
        float tessLevelInner;

    ////tese
        float tessAlpha;
        float tessStrength;

        TessellationConstants()
            : tessLevelOuter(3.0f)
            , tessLevelInner(3.0f)
            , tessAlpha(1.0f)
            , tessStrength(0.1f)
        {

        }
    };

    //////////////////////////////// GeometryConstants //////////////////////////////
    struct utilExport GeometryConstants
    {
        float width;
        float height;
        float length;
        float reserve;

        GeometryConstants()
            : width(0.05f)
            , height(3.0f)
            , length(0.02f)
            , reserve(0.0f)
        {

        }
    };

}; //LostPeter

#endif