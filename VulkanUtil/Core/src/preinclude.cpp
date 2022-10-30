#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../include/preinclude.h"

namespace LibUtil
{   
    //Struct
    //////////////////////////////// Vertex_Pos2Color3 //////////////////////////////
    VkVertexInputBindingDescription* Vertex_Pos2Color3::s_pBindingDescription = nullptr;
    std::array<VkVertexInputAttributeDescription, 2>* Vertex_Pos2Color3::s_pVertexInputAttributeDescriptions = nullptr;

    //////////////////////////////// Vertex_Pos2Color3Tex2 //////////////////////////
    VkVertexInputBindingDescription* Vertex_Pos2Color3Tex2::s_pBindingDescription = nullptr;
    std::array<VkVertexInputAttributeDescription, 3>* Vertex_Pos2Color3Tex2::s_pVertexInputAttributeDescriptions = nullptr;

    //////////////////////////////// Vertex_Pos3Color3Tex2 //////////////////////////
    VkVertexInputBindingDescription* Vertex_Pos3Color3Tex2::s_pBindingDescription = nullptr;
    std::array<VkVertexInputAttributeDescription, 3>* Vertex_Pos3Color3Tex2::s_pVertexInputAttributeDescriptions = nullptr;

    //////////////////////////////// Vertex_Pos3Color3Normal3Tex2 ///////////////////
    VkVertexInputBindingDescription* Vertex_Pos3Color3Normal3Tex2::s_pBindingDescription = nullptr;
    std::array<VkVertexInputAttributeDescription, 4>* Vertex_Pos3Color3Normal3Tex2::s_pVertexInputAttributeDescriptions = nullptr;

    //////////////////////////////// Vertex /////////////////////////////////////////
    VkVertexInputBindingDescription* Vertex::s_pBindingDescription = nullptr;
    std::array<VkVertexInputAttributeDescription, 5>* Vertex::s_pVertexInputAttributeDescriptions = nullptr;


}; //LibUtil