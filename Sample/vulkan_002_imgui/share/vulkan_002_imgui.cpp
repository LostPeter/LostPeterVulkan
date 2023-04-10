/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-08
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_002_imgui.h"


Vulkan_002_Imgui::Vulkan_002_Imgui(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->poTypeVertex = Vulkan_Vertex_Pos2Color4;
    this->cfg_shaderVertex_Path = "Assets/Shader/notrans_pos2_color4.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/notrans_pos2_color4.frag.spv";
}

void Vulkan_002_Imgui::loadModel_Custom()
{
    //1> vertices
    this->vertices.push_back(Vertex_Pos2Color4(glm::vec2( 0.0f,  0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
    this->vertices.push_back(Vertex_Pos2Color4(glm::vec2( 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
    this->vertices.push_back(Vertex_Pos2Color4(glm::vec2(-0.5f, -0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
    this->poVertexCount = (uint32_t)this->vertices.size();
    this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos2Color4);
    this->poVertexBuffer_Data = &this->vertices[0];
    this->poIndexCount = 0;
    this->poIndexBuffer_Size = 0;
    this->poIndexBuffer_Data = nullptr;
}

bool Vulkan_002_Imgui::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    static bool showDemoWindow = false;
    ImGui::Begin("Vulkan_002_Imgui", &windowOpened, 0);
    ImGui::Text("Frametime: %f", this->fFPS);
    ImGui::Checkbox("Show ImGui demo window", &showDemoWindow);
    ImGui::End();
    if (showDemoWindow) 
    {
        ImGui::ShowDemoWindow();
    }

    return true;
}

void Vulkan_002_Imgui::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}