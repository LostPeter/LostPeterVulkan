#include "preinclude.h"
#include "vulkan_006_depth.h"
#include "vulkanmeshloader.h"
#include "vulkancamera.h"
#include "vulkantimer.h"

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

static bool g_isTranformLocalModels[] = 
{
    false, //plane
    false, //cube

    true, //viking_room
    false, //bunny
};

static bool g_isFlipYModels[g_CountLen] = 
{
    true, //plane
    true, //cube

    false, //viking_room
    false, //bunny
};


Vulkan_006_Depth::Vulkan_006_Depth(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->poTypeVertex = VertexType_Pos3Color3Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/pos3_color3_tex2_ubo.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/pos3_color3_tex2_ubo.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/texture.jpg";

    this->cfg_cameraPos = glm::vec3(0.0f, 2.0f, -1.0f);
}

void Vulkan_006_Depth::createCamera()
{
    this->pCamera = new VulkanCamera();
    cameraReset();
}

void Vulkan_006_Depth::loadModel_Custom()
{
    for (int i = 0; i < g_CountLen; i++)
    {
        ModelObject* pModelObject = new ModelObject(this->poDevice);
        pModelObject->nameModel = g_pathModels[3 * i + 0];
        pModelObject->pathModel = g_pathModels[3 * i + 1];
        pModelObject->pathTexture = g_pathModels[3 * i + 2];

        bool isFlipY = g_isFlipYModels[i];
        bool isTranformLocal = g_isTranformLocalModels[i];
        
        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTranformLocal, g_tranformLocalModels[i]))
        {
            std::string msg = "Vulkan_006_Depth::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        pModelObject->poMatWorld = MathUtil::FromTRS(g_tranformModels[i * 3 + 0],
                                                     g_tranformModels[i * 3 + 1],
                                                     g_tranformModels[i * 3 + 2]); 

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            std::string msg = "Vulkan_006_Depth::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        m_aModelObjects.push_back(pModelObject);
        m_mapModelObjects[pModelObject->nameModel] = pModelObject;
    }
}
bool Vulkan_006_Depth::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
    //1> Load 
    MeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!VulkanMeshLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        Util_LogError("Vulkan_006_Depth::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    pModelObject->vertices.clear();
    pModelObject->vertices.reserve(count_vertex);
    for (int i = 0; i < count_vertex; i++)
    {
        MeshVertex& vertex = meshData.vertices[i];
        Vertex_Pos3Color3Tex2 v;
        v.pos = vertex.pos;
        v.color = glm::vec3(1.0f, 1.0f, 1.0f);
        v.texCoord = vertex.texCoord;

        if (isTranformLocal)
        {
            v.pos = MathUtil::Transform(matTransformLocal, v.pos);
        }

        pModelObject->vertices.push_back(v);
    }

    int count_index = (int)meshData.indices32.size();
    pModelObject->indices.clear();
    pModelObject->indices.reserve(count_index);
    for (int i = 0; i < count_index; i++)
    {
        pModelObject->indices.push_back(meshData.indices32[i]);
    }
    pModelObject->poVertexCount = (uint32_t)pModelObject->vertices.size();
    pModelObject->poVertexBuffer_Size = pModelObject->poVertexCount * sizeof(Vertex_Pos3Color3Tex2);
    pModelObject->poVertexBuffer_Data = &pModelObject->vertices[0];
    pModelObject->poIndexCount = (uint32_t)pModelObject->indices.size();
    pModelObject->poIndexBuffer_Size = pModelObject->poIndexCount * sizeof(uint32_t);
    pModelObject->poIndexBuffer_Data = &pModelObject->indices[0];

    Util_LogInfo("Vulkan_006_Depth::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
                 pModelObject->nameModel.c_str(),
                 (int)pModelObject->vertices.size(), 
                 (int)pModelObject->indices.size());

    //2> createVertexBuffer
    createVertexBuffer(pModelObject->poVertexBuffer_Size, pModelObject->poVertexBuffer_Data, pModelObject->poVertexBuffer, pModelObject->poVertexBufferMemory);

    //3> createIndexBuffer
    if (pModelObject->poIndexBuffer_Size > 0 &&
        pModelObject->poIndexBuffer_Data != nullptr)
    {
        createIndexBuffer(pModelObject->poIndexBuffer_Size, pModelObject->poIndexBuffer_Data, pModelObject->poIndexBuffer, pModelObject->poIndexBufferMemory);
    }

    return true;
}
bool Vulkan_006_Depth::loadModel_Texture(ModelObject* pModelObject)
{
    if (!pModelObject->pathTexture.empty())
    {
        createTextureImage(pModelObject->pathTexture, pModelObject->poTextureImage, pModelObject->poTextureImageMemory, pModelObject->poMipLevels);
        createTextureImageView(pModelObject->poTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, pModelObject->poMipLevels, pModelObject->poTextureImageView);
        createTextureSampler(pModelObject->poMipLevels, pModelObject->poTextureSampler);

        Util_LogInfo("Vulkan_006_Depth::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void Vulkan_006_Depth::buildObjectCB()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        ObjectConstants objectConstants;
        objectConstants.g_MatWorld = pModelObject->poMatWorld;
        this->objectCBs.push_back(objectConstants);
    }
}
void Vulkan_006_Depth::buildMaterialCB()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        MaterialConstants materialConstants;
        this->materialCBs.push_back(materialConstants);
    }
}
void Vulkan_006_Depth::buildInstanceCB()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        InstanceConstants instanceConstants;
        instanceConstants.indexObject = i;
        instanceConstants.indexMaterial = i;
        this->instanceCBs.push_back(instanceConstants);
    }
}

void Vulkan_006_Depth::createGraphicsPipeline()
{
    VkShaderModule vertShaderModule;
    if (!this->cfg_shaderVertex_Path.empty())
    {
        vertShaderModule = createShaderModule("VertexShader: ", this->cfg_shaderVertex_Path);
    }
    VkShaderModule fragShaderModule;
    if (!this->cfg_shaderFragment_Path.empty())
    {
        fragShaderModule = createShaderModule("FragmentShader: ", this->cfg_shaderFragment_Path);
    }

    //1> Pipeline Shader Stage
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    //2> Pipeline VertexInput State
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    createPipelineVertexInputStateCreateInfo(vertexInputInfo);

    //3> Pipeline InputAssembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = this->cfg_vkPrimitiveTopology;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    //4> Pipeline Viewport State
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &this->poViewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &this->poScissor;

    //5> Pipeline Rasterization State
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = this->cfg_vkPolygonMode;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = this->cfg_vkCullModeFlagBits;
    rasterizer.frontFace = this->cfg_vkFrontFace;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    //6> Pipeline Multisample State
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = this->poMSAASamples;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    //7> Pipeline DepthStencil State
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = this->cfg_isDepthTest;
    depthStencil.depthWriteEnable = this->cfg_isDepthWrite;
    depthStencil.depthCompareOp = this->cfg_DepthCompareOp;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = this->cfg_isStencilTest;
    if (this->cfg_isStencilTest)
    {
        depthStencil.front = this->cfg_StencilOpFront;
        depthStencil.back = this->cfg_StencilOpBack;
    }

    //8> Pipeline ColorBlend State 
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.blendEnable = this->cfg_isBlend;
    colorBlendAttachment.colorWriteMask = this->cfg_ColorWriteMask;
    if (this->cfg_isBlend)
    {
        colorBlendAttachment.srcColorBlendFactor = this->cfg_BlendColorFactorSrc;
        colorBlendAttachment.dstColorBlendFactor = this->cfg_BlendColorFactorDst;
        colorBlendAttachment.colorBlendOp = this->cfg_BlendColorOp;
        colorBlendAttachment.srcAlphaBlendFactor = this->cfg_BlendAlphaFactorSrc;
        colorBlendAttachment.dstAlphaBlendFactor = this->cfg_BlendAlphaFactorDst;
        colorBlendAttachment.alphaBlendOp = this->cfg_BlendAlphaOp;
    }

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    //9> Pipeline Dynamic State
    VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
    const std::vector<VkDynamicState> dynamicStateEnables = 
    { 
        VK_DYNAMIC_STATE_VIEWPORT, 
        VK_DYNAMIC_STATE_SCISSOR 
    };
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.pDynamicStates = dynamicStateEnables.data();
    dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    dynamicStateInfo.flags = 0;

    //10> Pipeline Layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    if (this->poDescriptorSetLayout != nullptr)
    {
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &this->poDescriptorSetLayout;
    }
    if (vkCreatePipelineLayout(this->poDevice, &pipelineLayoutInfo, nullptr, &this->poPipelineLayout) != VK_SUCCESS) 
    {
        throw std::runtime_error("VulkanWindow::createGraphicsPipeline: Failed to create pipeline layout !");
    }

    //11> Graphics Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicStateInfo;
    pipelineInfo.layout = this->poPipelineLayout;
    pipelineInfo.renderPass = this->poRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = 0;

    VkResult result = vkCreateGraphicsPipelines(this->poDevice, nullptr, 1, &pipelineInfo, nullptr, &this->poPipelineGraphics);
    if (result != VK_SUCCESS)
    {
        std::ostringstream os;
        os << (int)result;
        Util_LogError("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline, result: [%s] !", os.str().c_str());
        throw std::runtime_error("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline, result: " + os.str());
    }

    //12> Graphics Pipeline WireFrame
    rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
    pipelineInfo.pRasterizationState = &rasterizer;
    result = vkCreateGraphicsPipelines(this->poDevice, nullptr, 1, &pipelineInfo, nullptr, &this->poPipelineGraphics_WireFrame);
    if (result != VK_SUCCESS)
    {
        std::ostringstream os;
        os << (int)result;
        Util_LogError("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline wireframe, result: [%s] !", os.str().c_str());
        throw std::runtime_error("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline wireframe, result: " + os.str());
    }

    vkDestroyShaderModule(this->poDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(this->poDevice, vertShaderModule, nullptr);

    Util_LogInfo("<2-2-4> VulkanWindow::createGraphicsPipeline finish !");
}

void Vulkan_006_Depth::updateCBs_ObjectsContent()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        ObjectConstants& objectCB = this->objectCBs[i];
        if (pModelObject->isRotate)
        {
            float time = this->pTimer->GetTimeSinceStart();
            objectCB.g_MatWorld = glm::rotate(pModelObject->poMatWorld, 
                                              time * glm::radians(90.0f), 
                                              glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else
        {
            objectCB.g_MatWorld = pModelObject->poMatWorld;
        }
    }
}

void Vulkan_006_Depth::updateCBs_MaterialsContent()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

    }
}

void Vulkan_006_Depth::updateCBs_InstancesContent()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];


    }
}

bool Vulkan_006_Depth::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_006_Depth", &windowOpened, 0);
    {
        ImGui::Text("Frametime: %f", this->fFPS);
        ImGui::Separator();

        //1> Model
        int count = this->m_aModelObjects.size();
        for (int i = 0; i < count; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];

            std::string nameModel = StringUtil::SaveInt(i) + " - " + pModelObject->nameModel;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                ImGui::Checkbox("Is Show", &pModelObject->isShow);
                ImGui::Checkbox("Is Rotate", &pModelObject->isRotate);
                ImGui::Text("VertexCount: [%d]", (int)pModelObject->poVertexCount);
                ImGui::Text("IndexCount: [%d]", (int)pModelObject->poIndexCount);

                if (ImGui::CollapsingHeader("Model World"))
                {
                    const glm::mat4& mat4World = this->objectCBs[i].g_MatWorld;
                    std::string nameTable = StringUtil::SaveInt(i) + " - split_model_world";
                    if (ImGui::BeginTable(nameTable.c_str(), 4))
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
        
        ImGui::Separator();
        ImGui::Spacing();

        //2> Camera
        cameraConfig();
    }
    ImGui::End();

    return true;
}

void Vulkan_006_Depth::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_006_Depth::drawMesh_Custom(VkCommandBuffer& commandBuffer)
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        VkBuffer vertexBuffers[] = { pModelObject->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (this->poDescriptorSets.size() > 0)
        {
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &this->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
        if (pModelObject->poIndexBuffer != nullptr)
        {
            vkCmdBindIndexBuffer(commandBuffer, pModelObject->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commandBuffer, pModelObject->poIndexCount, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, pModelObject->poVertexCount, 1, 0, 0);
        }
    }
}

void Vulkan_006_Depth::cleanupCustom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        UTIL_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_mapModelObjects.clear();
}