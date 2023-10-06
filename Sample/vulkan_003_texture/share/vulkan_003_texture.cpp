/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_003_texture.h"


Vulkan_003_Texture::Vulkan_003_Texture(int width, int height, String name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->cfg_isNegativeViewport = false;
    this->cfg_vkFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    this->poTypeVertex = F_MeshVertex_Pos2Color4Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/notrans_pos2_color4_tex2.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/notrans_pos2_color4_tex2.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/Common/texture2d.jpg";
}

void Vulkan_003_Texture::loadModel_Custom()
{   
    //1> vertices
    this->vertices.push_back(FVertex_Pos2Color4Tex2(FVector2(-0.5f, -0.5f), FVector4(1.0f, 1.0f, 1.0f, 1.0f), FVector2(0.0f, 0.0f)));
    this->vertices.push_back(FVertex_Pos2Color4Tex2(FVector2(-0.5f,  0.5f), FVector4(1.0f, 1.0f, 1.0f, 1.0f), FVector2(0.0f, 1.0f)));
    this->vertices.push_back(FVertex_Pos2Color4Tex2(FVector2( 0.5f,  0.5f), FVector4(1.0f, 1.0f, 1.0f, 1.0f), FVector2(1.0f, 1.0f)));
    this->vertices.push_back(FVertex_Pos2Color4Tex2(FVector2( 0.5f,  0.5f), FVector4(1.0f, 1.0f, 1.0f, 1.0f), FVector2(1.0f, 1.0f)));
    this->vertices.push_back(FVertex_Pos2Color4Tex2(FVector2( 0.5f, -0.5f), FVector4(1.0f, 1.0f, 1.0f, 1.0f), FVector2(1.0f, 0.0f)));
    this->vertices.push_back(FVertex_Pos2Color4Tex2(FVector2(-0.5f, -0.5f), FVector4(1.0f, 1.0f, 1.0f, 1.0f), FVector2(0.0f, 0.0f)));
    this->poVertexCount = (uint32_t)this->vertices.size();
    this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos2Color4Tex2);
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
    ImGui::Begin("Vulkan_003_Texture", &windowOpened, 0);
    {
        //0> Common
        commonConfig();
        
    }
    ImGui::End();

    return true;
}

void Vulkan_003_Texture::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}