#include "PreInclude.h"
#include "vulkan_001_triangle.h"


Vulkan_001_Triangle::Vulkan_001_Triangle(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isNegativeViewport = false;
    this->cfg_vkFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    this->poTypeVertex = Vulkan_VertexType_Pos2Color4;
    this->cfg_shaderVertex_Path = "Assets/Shader/notrans_pos2_color4.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/notrans_pos2_color4.frag.spv";
}

void Vulkan_001_Triangle::loadModel_Custom()
{
    //1> vertices
    this->vertices.push_back(Vertex_Pos2Color4(glm::vec2( 0.0f, -0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
    this->vertices.push_back(Vertex_Pos2Color4(glm::vec2(-0.5f,  0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
    this->vertices.push_back(Vertex_Pos2Color4(glm::vec2( 0.5f,  0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
    this->poVertexCount = (uint32_t)this->vertices.size();
    this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos2Color4);
    this->poVertexBuffer_Data = &this->vertices[0];
    this->poIndexCount = 0;
    this->poIndexBuffer_Size = 0;
    this->poIndexBuffer_Data = nullptr;
}