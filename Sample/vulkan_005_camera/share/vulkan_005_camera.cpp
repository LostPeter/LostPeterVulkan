#include "PreInclude.h"
#include "vulkan_005_camera.h"
#include "VulkanMeshLoader.h"
#include "VulkanCamera.h"

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


Vulkan_005_Camera::Vulkan_005_Camera(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->poTypeVertex = Vulkan_VertexType_Pos3Color4Tex2;
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

    this->poMatWorld = MathUtil::FromTRS(g_tranformModels[index * 3 + 0],
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
    this->pCamera = new VulkanCamera();
    cameraReset();
}

void Vulkan_005_Camera::loadModel_Assimp()
{
    MeshData meshData;
    meshData.bIsFlipY = g_isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!VulkanMeshLoader::LoadMeshData(this->cfg_model_Path, meshData, eMeshParserFlags))
    {
        Util_LogError("Vulkan_005_Camera::loadModel_Assimp load model failed: [%s] !", this->cfg_model_Path.c_str());
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

bool Vulkan_005_Camera::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    static bool showDemoWindow = false;
    ImGui::Begin("Vulkan_005_Camera", &windowOpened, 0);
    {
        ImGui::Text("Frametime: %f", this->fFPS);
        ImGui::Separator();

        //1> Common
        if (ImGui::CollapsingHeader("Common"))
        {
            ImGui::Checkbox("Is WireFrame", &cfg_isWireFrame);
            ImGui::Checkbox("Is Rotate", &cfg_isRotate);
        }
        ImGui::Separator();
        ImGui::Spacing();
        
        //3> Model
        if (ImGui::CollapsingHeader("Model"))
        {
            static std::string s_Name = "Model - ";
            for (int i = 0; i < g_CountLen; i++)
            {
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
            if (ImGui::CollapsingHeader("Model World"))
            {
                const glm::mat4& mat4World = this->objectCBs[0].g_MatWorld;
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
        ImGui::Separator();
        ImGui::Spacing();

        //2> Camera
        cameraConfig();
    }
    ImGui::End();
    if (showDemoWindow) 
    {
        ImGui::ShowDemoWindow();
    }

    return true;
}

void Vulkan_005_Camera::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}