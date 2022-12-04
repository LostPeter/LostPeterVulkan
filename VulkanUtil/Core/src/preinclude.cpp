#include "../include/preinclude.h"

namespace LostPeter
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

    //////////////////////////////// MeshVertex /////////////////////////////////////////
    VkVertexInputBindingDescription* MeshVertex::s_pBindingDescription = nullptr;
    std::array<VkVertexInputAttributeDescription, 5>* MeshVertex::s_pVertexInputAttributeDescriptions = nullptr;


}; //LostPeter