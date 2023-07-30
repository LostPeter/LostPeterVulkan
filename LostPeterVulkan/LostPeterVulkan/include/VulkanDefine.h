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
#include "FMath.h"
#include "FMeshVertex.h"

namespace LostPeter
{
/////Struct
    //////////////////////////////// Vertex_Pos2Color4 //////////////////////////////
    //0: F_MeshVertex_Pos2Color4
    struct utilExport Vertex_Pos2Color4
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos2Color4);
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
            attributeDescription0.offset = offsetof(FVertex_Pos2Color4, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos2Color4, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4 //////////////////////////////
    //1: F_MeshVertex_Pos3Color4
    struct utilExport Vertex_Pos3Color4
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Color4);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Color4, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Color4, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3 /////////////////////////////
    //2: F_MeshVertex_Pos3Normal3
    struct utilExport Vertex_Pos3Normal3
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Normal3);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Normal3, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Normal3, normal);
            s_aAttributeDescriptions.push_back(attributeDescription1);
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3Tex2 /////////////////////////
    //3: F_MeshVertex_Pos3Normal3Tex2
    struct utilExport Vertex_Pos3Normal3Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Normal3Tex2);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Normal3Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Normal3Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Normal3Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription2);
        }
    };


    //////////////////////////////// Vertex_Pos2Color4Tex2 //////////////////////////
    //4: F_MeshVertex_Pos2Color4Tex2
    struct utilExport Vertex_Pos2Color4Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;
        
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
            bindingDescription0.stride = sizeof(FVertex_Pos2Color4Tex2);
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
            attributeDescription0.offset = offsetof(FVertex_Pos2Color4Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos2Color4Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos2Color4Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription2);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Tex2 //////////////////////////
    //5: F_MeshVertex_Pos3Color4Tex2
    struct utilExport Vertex_Pos3Color4Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Color4Tex2);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Color4Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Color4Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Color4Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription2);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tex2 ///////////////////
    //6: F_MeshVertex_Pos3Color4Normal3Tex2 
    struct utilExport Vertex_Pos3Color4Normal3Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Color4Normal3Tex2);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Color4Normal3Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Color4Normal3Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Color4Normal3Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription3.offset = offsetof(FVertex_Pos3Color4Normal3Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription3);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tex4 ///////////////////
    //7: F_MeshVertex_Pos3Color4Normal3Tex4
    struct utilExport Vertex_Pos3Color4Normal3Tex4
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Color4Normal3Tex4);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Color4Normal3Tex4, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Color4Normal3Tex4, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Color4Normal3Tex4, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription3.offset = offsetof(FVertex_Pos3Color4Normal3Tex4, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription3);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex2 ///////////////
    //8: F_MeshVertex_Pos3Color4Normal3Tangent3Tex2
    struct utilExport Vertex_Pos3Color4Normal3Tangent3Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription3.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex2, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription4.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription4);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex4 ///////////////
    //9: F_MeshVertex_Pos3Color4Normal3Tangent3Tex4
    struct utilExport Vertex_Pos3Color4Normal3Tangent3Tex4
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Color4Normal3Tangent3Tex4);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex4, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex4, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex4, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription3.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex4, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription4.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3Tex4, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription4);
        }
    };


    //////////////////////////////// Vertex_Pos3Normal3Tangent3BlendWI8Tex2 //////////
    //10: F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
    struct Vertex_Pos3Normal3Tangent3BlendWI8Tex2
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Normal3Tangent3BlendWI8Tex2);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Normal3Tangent3BlendWI8Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Normal3Tangent3BlendWI8Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Normal3Tangent3BlendWI8Tex2, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription3.offset = offsetof(FVertex_Pos3Normal3Tangent3BlendWI8Tex2, blendweight);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription4.offset = offsetof(FVertex_Pos3Normal3Tangent3BlendWI8Tex2, blendindex);
            s_aAttributeDescriptions.push_back(attributeDescription4);

            VkVertexInputAttributeDescription attributeDescription5 = {};
            attributeDescription5.binding = 0;
            attributeDescription5.location = 5;
            attributeDescription5.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription5.offset = offsetof(FVertex_Pos3Normal3Tangent3BlendWI8Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription5);
        }
    };


    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 ////
    //11: F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
    struct Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 
    {
        static VkVertexInputBindingDescriptionVector s_aBindingDescriptions;
        static VkVertexInputAttributeDescriptionVector s_aAttributeDescriptions;

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
            bindingDescription0.stride = sizeof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2);
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
            attributeDescription0.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, pos);
            s_aAttributeDescriptions.push_back(attributeDescription0);

            VkVertexInputAttributeDescription attributeDescription1 = {};
            attributeDescription1.binding = 0;
            attributeDescription1.location = 1;
            attributeDescription1.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription1.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, color);
            s_aAttributeDescriptions.push_back(attributeDescription1);

            VkVertexInputAttributeDescription attributeDescription2 = {};
            attributeDescription2.binding = 0;
            attributeDescription2.location = 2;
            attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription2.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, normal);
            s_aAttributeDescriptions.push_back(attributeDescription2);

            VkVertexInputAttributeDescription attributeDescription3 = {};
            attributeDescription3.binding = 0;
            attributeDescription3.location = 3;
            attributeDescription3.format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription3.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, tangent);
            s_aAttributeDescriptions.push_back(attributeDescription3);

            VkVertexInputAttributeDescription attributeDescription4 = {};
            attributeDescription4.binding = 0;
            attributeDescription4.location = 4;
            attributeDescription4.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription4.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, blendweight);
            s_aAttributeDescriptions.push_back(attributeDescription4);

            VkVertexInputAttributeDescription attributeDescription5 = {};
            attributeDescription5.binding = 0;
            attributeDescription5.location = 5;
            attributeDescription5.format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescription5.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, blendindex);
            s_aAttributeDescriptions.push_back(attributeDescription5);

            VkVertexInputAttributeDescription attributeDescription6 = {};
            attributeDescription6.binding = 0;
            attributeDescription6.location = 6;
            attributeDescription6.format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription6.offset = offsetof(FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2, texCoord);
            s_aAttributeDescriptions.push_back(attributeDescription6);
        }
    };


    //////////////////////////////// TransformConstants /////////////////////////////
    struct utilExport TransformConstants
    {
        FMatrix4 mat4View;
        FMatrix4 mat4View_Inv;
        FMatrix4 mat4Proj;
        FMatrix4 mat4Proj_Inv;
        FMatrix4 mat4ViewProj;
        FMatrix4 mat4ViewProj_Inv;

        TransformConstants()
            : mat4View(FMath::Identity4x4())
            , mat4View_Inv(FMath::Identity4x4())
            , mat4Proj(FMath::Identity4x4())
            , mat4Proj_Inv(FMath::Identity4x4())
            , mat4ViewProj(FMath::Identity4x4())
            , mat4ViewProj_Inv(FMath::Identity4x4())
        {

        }
    };

    //////////////////////////////// CameraConstants ////////////////////////////////
    struct utilExport CameraConstants
    {
        FVector3 posEyeWorld;    
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
        FVector4 common;    // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
        FVector3 position;  // directional/point/spot
        float falloffStart;  // point/spot light only
        FVector3 direction; // directional/spot light only
        float falloffEnd;    // point/spot light only
        FVector4 ambient;   // ambient
        FVector4 diffuse;   // diffuse
        FVector4 specular;  // specular

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
        FVector2 g_RenderTargetSize;   
        FVector2 g_RenderTargetSize_Inv;

        //Material
        FVector4 g_AmbientLight;

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
        FMatrix4 g_MatWorld;
        
        ObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
        {

        }
    };
    struct utilExport TerrainObjectConstants
    {
        FMatrix4 g_MatWorld;
    

        TerrainObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
        {

        }
    };
    struct utilExport GridObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        GridObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct utilExport CameraAxisObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        CameraAxisObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct utilExport CoordinateAxisObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        CoordinateAxisObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct utilExport CopyBlitObjectConstants
    {
        float offsetX;
        float offsetY;
        float scaleX;
        float scaleY;

        CopyBlitObjectConstants()
            : offsetX(0.0f)
            , offsetY(0.0f)
            , scaleX(1.0f)
            , scaleY(1.0f)
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
        FVector4 factorAmbient;
        FVector4 factorDiffuse;
        FVector4 factorSpecular;

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
        int offsetObject;
        int indexObject;
        int offsetMaterial;
        int indexMaterial;

        InstanceConstants()
            : offsetObject(0)
            , indexObject(0)
            , offsetMaterial(0)
            , indexMaterial(0)
        {

        }  
    };

    //////////////////////////////// TextureCopyConstants ///////////////////////////
    struct utilExport TextureCopyConstants
    {
        FVector4 texInfo;
        FVector4 texOffset;
        FVector4 texIndexArray;
        FVector4 texClearColor;

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