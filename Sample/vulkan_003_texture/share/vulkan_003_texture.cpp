/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-11-12
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_003_texture.h"


Vulkan_003_Texture::Vulkan_003_Texture(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->cfg_isNegativeViewport = false;
    this->cfg_vkFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    this->poTypeVertex = Vulkan_Vertex_Pos2Color4Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/notrans_pos2_color4_tex2.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/notrans_pos2_color4_tex2.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/texture2d.jpg";
}

void Vulkan_003_Texture::loadModel_Custom()
{   
    //1> vertices
    this->vertices.push_back(Vertex_Pos2Color4Tex2(glm::vec2(-0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
    this->vertices.push_back(Vertex_Pos2Color4Tex2(glm::vec2(-0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
    this->vertices.push_back(Vertex_Pos2Color4Tex2(glm::vec2( 0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    this->vertices.push_back(Vertex_Pos2Color4Tex2(glm::vec2( 0.5f,  0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
    this->vertices.push_back(Vertex_Pos2Color4Tex2(glm::vec2( 0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
    this->vertices.push_back(Vertex_Pos2Color4Tex2(glm::vec2(-0.5f, -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
    this->poVertexCount = (uint32_t)this->vertices.size();
    this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos2Color4Tex2);
    this->poVertexBuffer_Data = &this->vertices[0];
    this->poIndexCount = 0;
    this->poIndexBuffer_Size = 0;
    this->poIndexBuffer_Data = nullptr;
}

bool Vulkan_003_Texture::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    static bool showDemoWindow = false;
    ImGui::Begin("Vulkan_003_Texture", &windowOpened, 0);
    {
        ImGui::Text("Frametime: %f", this->fFPS);
        ImGui::Checkbox("Is WireFrame", &cfg_isWireFrame);
    }
    ImGui::End();
    if (showDemoWindow) 
    {
        ImGui::ShowDemoWindow();
    }

    return true;
}

void Vulkan_003_Texture::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}