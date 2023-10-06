/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_005_camera.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


static const int g_CountLen = 4;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "plane",            "Assets/Mesh/Common/plane.fbx",                     "Assets/Texture/Common/texture2d.jpg", //plane
    "cube",             "Assets/Mesh/Common/cube.fbx",                      "Assets/Texture/Common/texture2d.jpg", //cube

    "viking_room",      "Assets/Mesh/Model/viking_room/viking_room.obj",    "Assets/Texture/Model/viking_room/viking_room.png", //viking_room
    "bunny",            "Assets/Mesh/Model/bunny/bunny.obj",                "Assets/Texture/Common/default_white.bmp", //bunny
};

static FVector3 g_tranformModels[3 * g_CountLen] = 
{
    FVector3(   0,   0,    0),     FVector3(     0, 0, 0),      FVector3(0.01f,  0.01f,  0.01f), //plane
    FVector3(   0,   0,    0),     FVector3(     0, 0, 0),      FVector3(0.01f,  0.01f,  0.01f), //cube

    FVector3(   0,   0,    0),     FVector3(     0, 0, 0),      FVector3( 1.0f,   1.0f,   1.0f), //viking_room
    FVector3(   0,   0,    0),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //bunny
};

static FMatrix4 g_tranformLocalModels[g_CountLen] = 
{
    FMath::ms_mat4Unit, //plane
    FMath::ms_mat4Unit, //cube

    FMath::RotateX(-90.0f), //viking_room
    FMath::ms_mat4Unit, //bunny
};
static FMatrix4 g_tranformLocal = FMath::ms_mat4Unit;
static bool g_isTranformLocalModels[] = 
{
    false, //plane
    false, //cube

    true, //viking_room
    false, //bunny
};
static bool g_isTranformLocal = false;

static bool g_isShowModels[g_CountLen] = 
{
    false, //plane
    false, //cube

    false, //viking_room
    false, //bunny
};
static void s_EnableShowModels(int index)
{
    g_isShowModels[index] = true;
}
static void s_DisableAllShowModels()
{
    for (int i = 0; i < g_CountLen; i++)
    {
        g_isShowModels[i] = false;
    }
}

static bool g_isFlipYModels[g_CountLen] = 
{
    true, //plane
    true, //cube

    false, //viking_room
    false, //bunny
};
static bool g_isFlipY = true;

static FVector3 g_vCameraPos[g_CountLen] = 
{
    FVector3(0.0f, 2.0f, -1.0f), //plane
    FVector3(0.0f, 2.0f, -1.0f), //cube

    FVector3(0.0f, 2.5f, -2.0f), //viking_room
    FVector3(0.0f, 3.0f, -1.5f), //bunny
};
static float g_fCameraFov[g_CountLen] =
{
    45.0f, //plane
    45.0f, //cube

    45.0f, //viking_room
    60.0f, //bunny
};


Vulkan_005_Camera::Vulkan_005_Camera(int width, int height, String name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isRotate = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->poTypeVertex = F_MeshVertex_Pos3Color4Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/pos3_color4_tex2_ubo.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/pos3_color4_tex2_ubo.frag.spv";
    
    resetSetting(0);
}

void Vulkan_005_Camera::resetSetting(int index)
{
    s_DisableAllShowModels();
    s_EnableShowModels(index);
    g_tranformLocal = g_tranformLocalModels[index];
    g_isTranformLocal = g_isTranformLocalModels[index];
    g_isFlipY = g_isFlipYModels[index];
    this->cfg_cameraPos = g_vCameraPos[index];
    this->cfg_cameraFov = g_fCameraFov[index];

    this->poMatWorld = FMath::FromTRS(g_tranformModels[index * 3 + 0],
                                      g_tranformModels[index * 3 + 1],
                                      g_tranformModels[index * 3 + 2]); 
    this->cfg_model_Path = g_pathModels[index * 3 + 1]; 
    this->cfg_texture_Path = g_pathModels[index * 3 + 2];
}
void Vulkan_005_Camera::changeModel(int index)
{
    //0> setting
    resetSetting(index);

    //1> Model
    cleanupVertexIndexBuffer();
    loadVertexIndexBuffer();

    //2> Texture
    cleanupTexture();
    loadTexture();
    updateDescriptorSets(this->poDescriptorSets, this->poTextureImageView, this->poTextureSampler);
}

void Vulkan_005_Camera::createCamera()
{
    this->pCamera = new FCamera();
    cameraReset();
}

void Vulkan_005_Camera::loadModel_Assimp()
{
    FMeshData meshData;
    meshData.bIsFlipY = g_isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(this->cfg_model_Path, meshData, eMeshParserFlags))
    {
        F_LogError("Vulkan_005_Camera::loadModel_Assimp load model failed: [%s] !", this->cfg_model_Path.c_str());
        return;
    }

    int count_vertex = (int)meshData.vertices.size();
    this->vertices.clear();
    this->vertices.reserve(count_vertex);
    for (int i = 0; i < count_vertex; i++)
    {
        FMeshVertex& vertex = meshData.vertices[i];
        FVertex_Pos3Color4Tex2 v;
        v.pos = vertex.pos;
        v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
        v.texCoord = vertex.texCoord;

        if (g_isTranformLocal)
        {
            v.pos = FMath::Transform(g_tranformLocal, v.pos);
        }

        this->vertices.push_back(v);
    }

    int count_index = (int)meshData.indices32.size();
    this->indices.clear();
    this->indices.reserve(count_index);
    for (int i = 0; i < count_index; i++)
    {
        this->indices.push_back(meshData.indices32[i]);
    }

    this->poVertexCount = (uint32_t)this->vertices.size();
    this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Tex2);
    this->poVertexBuffer_Data = &this->vertices[0];
    this->poIndexCount = (uint32_t)this->indices.size();
    this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
    this->poIndexBuffer_Data = &this->indices[0];

    F_LogInfo("Vertex count: [%d], Index count: [%d] !", (int)this->vertices.size(), (int)this->indices.size());
}

bool Vulkan_005_Camera::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_005_Camera", &windowOpened, 0);
    {
        //0> Common
        commonConfig();

        //1> Camera
        cameraConfig();
        
        //2> Model
        modelConfig();
        
    }
    ImGui::End();

    return true;
}
void Vulkan_005_Camera::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
        static String s_Name = "Model - ";
        for (int i = 0; i < g_CountLen; i++)
        {
            bool isShowModel = g_isShowModels[i];
            String nameModel = s_Name + g_pathModels[3 * i + 0];
            if (isShowModel)
            {
                nameModel += "(" + FUtilString::SaveSizeT(this->vertices.size()) + 
                             "/" + FUtilString::SaveSizeT(this->indices.size()) +
                             ")";
            }
            ImGui::Checkbox(nameModel.c_str(), &isShowModel);
            if (!g_isShowModels[i] && isShowModel != g_isShowModels[i])
            {
                changeModel(i);
            }
        }
        if (ImGui::CollapsingHeader("Model World"))
        {
            const FMatrix4& mat4World = this->objectCBs[0].g_MatWorld;
            if (ImGui::BeginTable("split_model_world", 4))
            {
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][0]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][1]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][2]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][3]);

                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][0]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][1]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][2]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][3]);

                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][0]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][1]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][2]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][3]);

                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][0]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][1]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][2]);
                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][3]);

                ImGui::EndTable();
            }
        }
    }
}

void Vulkan_005_Camera::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}