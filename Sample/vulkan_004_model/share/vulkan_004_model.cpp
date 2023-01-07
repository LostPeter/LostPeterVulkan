/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-11-15
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_004_model.h"
#include "VulkanMeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


static const int g_CountLen = 4;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "plane",            "Assets/Model/Fbx/plane.fbx",                       "Assets/Texture/texture.jpg", //plane
    "cube",             "Assets/Model/Fbx/cube.fbx",                        "Assets/Texture/texture.jpg", //cube

    "viking_room",      "Assets/Model/Obj/viking_room/viking_room.obj",     "Assets/Model/Obj/viking_room/viking_room.png", //viking_room
    "bunny",            "Assets/Model/Obj/bunny/bunny.obj",                 "Assets/Texture/white.bmp", //bunny
};

static glm::vec3 g_tranformModels[3 * g_CountLen] = 
{
    glm::vec3(   0,   0,    0),     glm::vec3(     0, 0, 0),      glm::vec3(0.01f,  0.01f,  0.01f), //plane
    glm::vec3(   0,   0,    0),     glm::vec3(     0, 0, 0),      glm::vec3(0.01f,  0.01f,  0.01f), //cube

    glm::vec3(   0,   0,    0),     glm::vec3(     0, 0, 0),      glm::vec3( 1.0f,   1.0f,   1.0f), //viking_room
    glm::vec3(   0,   0,    0),     glm::vec3(     0, 180, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //bunny
};

static glm::mat4 g_tranformLocalModels[g_CountLen] = 
{
    MathUtil::ms_mat4Unit, //plane
    MathUtil::ms_mat4Unit, //cube

    MathUtil::RotateX(-90.0f), //viking_room
    MathUtil::ms_mat4Unit, //bunny
};
static glm::mat4 g_tranformLocal = MathUtil::ms_mat4Unit;
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

static glm::vec3 g_vCameraPos[g_CountLen] = 
{
    glm::vec3(0.0f, 2.0f, -1.0f), //plane
    glm::vec3(0.0f, 2.0f, -1.0f), //cube

    glm::vec3(0.0f, 2.5f, -2.0f), //viking_room
    glm::vec3(0.0f, 3.0f, -1.5f), //bunny
};
static float g_fCameraFov[g_CountLen] =
{
    45.0f, //plane
    45.0f, //cube

    45.0f, //viking_room
    60.0f, //bunny
};


Vulkan_004_Model::Vulkan_004_Model(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->poTypeVertex = Vulkan_VertexType_Pos3Color4Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/pos3_color4_tex2_ubo.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/pos3_color4_tex2_ubo.frag.spv";
    
    resetSetting(0);
}

void Vulkan_004_Model::resetSetting(int index)
{
    s_DisableAllShowModels();
    s_EnableShowModels(index);
    g_tranformLocal = g_tranformLocalModels[index];
    g_isTranformLocal = g_isTranformLocalModels[index];
    g_isFlipY = g_isFlipYModels[index];
    this->cfg_cameraPos = g_vCameraPos[index];
    this->cfg_cameraFov = g_fCameraFov[index];

    this->poMatWorld = MathUtil::FromTRS(g_tranformModels[index * 3 + 0],
                                         g_tranformModels[index * 3 + 1],
                                         g_tranformModels[index * 3 + 2]); 
    this->cfg_model_Path = g_pathModels[index * 3 + 1]; 
    this->cfg_texture_Path = g_pathModels[index * 3 + 2];
}
void Vulkan_004_Model::changeModel(int index)
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

void Vulkan_004_Model::loadModel_Assimp()
{
    MeshData meshData;
    meshData.bIsFlipY = g_isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!VulkanMeshLoader::LoadMeshData(this->cfg_model_Path, meshData, eMeshParserFlags))
    {
        Util_LogError("Vulkan_004_Model::loadModel_Assimp load model failed: [%s] !", this->cfg_model_Path.c_str());
        return;
    }

    int count_vertex = (int)meshData.vertices.size();
    this->vertices.clear();
    this->vertices.reserve(count_vertex);
    for (int i = 0; i < count_vertex; i++)
    {
        MeshVertex& vertex = meshData.vertices[i];
        Vertex_Pos3Color4Tex2 v;
        v.pos = vertex.pos;
        v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        v.texCoord = vertex.texCoord;

        if (g_isTranformLocal)
        {
            v.pos = MathUtil::Transform(g_tranformLocal, v.pos);
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
    this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos3Color4Tex2);
    this->poVertexBuffer_Data = &this->vertices[0];
    this->poIndexCount = (uint32_t)this->indices.size();
    this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
    this->poIndexBuffer_Data = &this->indices[0];

    Util_LogInfo("Vertex count: [%d], Index count: [%d] !", (int)this->vertices.size(), (int)this->indices.size());
}

bool Vulkan_004_Model::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    static bool showDemoWindow = false;
    ImGui::Begin("Vulkan_004_Model", &windowOpened, 0);
    {
        ImGui::Text("Frametime: %f", this->fFPS);
        ImGui::Separator();

        ImGui::Checkbox("Is WireFrame", &cfg_isWireFrame);
        ImGui::Checkbox("Is Rotate", &cfg_isRotate);
        ImGui::Separator();

        if (ImGui::BeginTable("split_model", 1))
        {
            static std::string s_Name = "Model - ";
            for (int i = 0; i < g_CountLen; i++)
            {
                ImGui::TableNextColumn(); 
                bool isShowModel = g_isShowModels[i];
                std::string nameModel = s_Name + g_pathModels[3 * i + 0];
                if (isShowModel)
                {
                    nameModel += "(" + StringUtil::SaveSizeT(this->vertices.size()) + 
                                 "/" + StringUtil::SaveSizeT(this->indices.size()) +
                                 ")";
                }
                ImGui::Checkbox(nameModel.c_str(), &isShowModel);
                if (!g_isShowModels[i] && isShowModel != g_isShowModels[i])
                {
                    changeModel(i);
                }
            }

            ImGui::EndTable();
        }
    }
    ImGui::End();
    if (showDemoWindow) 
    {
        ImGui::ShowDemoWindow();
    }

    return true;
}

void Vulkan_004_Model::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}