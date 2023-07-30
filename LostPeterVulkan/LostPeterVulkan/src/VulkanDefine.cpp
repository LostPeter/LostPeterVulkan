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

#include "../include/VulkanDefine.h"

namespace LostPeter
{
////Struct
    //////////////////////////////// Vertex_Pos2Color4 //////////////////////////////
    //0: F_MeshVertex_Pos2Color4
    VkVertexInputBindingDescriptionVector Vertex_Pos2Color4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos2Color4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4 //////////////////////////////
    //1: F_MeshVertex_Pos3Color4
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Normal3 /////////////////////////////
    //2: F_MeshVertex_Pos3Normal3
    VkVertexInputBindingDescriptionVector Vertex_Pos3Normal3::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Normal3::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Normal3Tex2 /////////////////////////
    //3: F_MeshVertex_Pos3Normal3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Normal3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Normal3Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos2Color4Tex2 //////////////////////////
    //4: F_MeshVertex_Pos2Color4Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos2Color4Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos2Color4Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Tex2 //////////////////////////
    //5: F_MeshVertex_Pos3Color4Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tex2 ///////////////////
    //6: F_MeshVertex_Pos3Color4Normal3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tex4 ///////////////////
    //7: F_MeshVertex_Pos3Color4Normal3Tex4
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tex4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tex4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex2 ///////////
    //8: F_MeshVertex_Pos3Color4Normal3Tangent3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex2::s_aAttributeDescriptions;
    
    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex4 ///////////
    //9: F_MeshVertex_Pos3Color4Normal3Tangent3Tex4
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Normal3Tangent3BlendWI8Tex2 //////////
    //10: F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Normal3Tangent3BlendWI8Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Normal3Tangent3BlendWI8Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 ////
    //11: F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::s_aAttributeDescriptions;

}; //LostPeter