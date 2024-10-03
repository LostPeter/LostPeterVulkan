/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-26
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_018_subpass.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 1;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Common/plane.fbx", //plane


};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //plane

    
};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //plane  


};
static FMatrix4 g_MeshTranformLocals[g_MeshCount] = 
{
    FMath::ms_mat4Unit, //plane
    
};


/////////////////////////// Texture /////////////////////////////
static const int g_TextureCount = 1;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
    "texture2d",                        "2D",            "false",                  "false",                           "Assets/Texture/Common/texture2d.jpg", //texture2d
    
};
static FTexturePixelFormatType g_TextureFormats[g_TextureCount] = 
{
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d


};
static FTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    F_TextureFilter_Bilinear, //texture2d


};
static FTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    F_TextureAddressing_Clamp, //texture2d


};
static FTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    F_TextureBorderColor_OpaqueBlack, //texture2d

   

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //texture2d

 

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //texture2d


};


/////////////////////////// DescriptorSetLayout /////////////////
static const int g_DescriptorSetLayoutCount = 2;
static const char* g_DescriptorSetLayoutNames[g_DescriptorSetLayoutCount] =
{
    "Pass-Object-Material-Instance-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-InputAttachRed-InputAttachGreen-InputAttachBlue",
};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 6;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type               //path
    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    
    "vert_standard_subpass_0",                                 "vert",              "Assets/Shader/standard_subpass_0.vert.spv", //standard_subpass_0 vert
    "vert_standard_subpass_1",                                 "vert",              "Assets/Shader/standard_subpass_1.vert.spv", //standard_subpass_1 vert

    ///////////////////////////////////////// tesc /////////////////////////////////////////
   

    ///////////////////////////////////////// tese /////////////////////////////////////////
   

    ///////////////////////////////////////// geom /////////////////////////////////////////

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag

    "frag_standard_subpass_0",                                 "frag",              "Assets/Shader/standard_subpass_0.frag.spv", //standard_subpass_0 frag
    "frag_standard_subpass_1",                                 "frag",              "Assets/Shader/standard_subpass_1.frag.spv", //standard_subpass_1 frag
    
    ///////////////////////////////////////// comp /////////////////////////////////////////
    

};


/////////////////////////// Object //////////////////////////////
static const int g_Object_Count = 1;
static const char* g_Object_Configs[2 * g_Object_Count] = 
{
    //Object Name                          //Mesh Name                                                                    
    "object_texture",                      "plane", //object_texture

};
static const char* g_Object_MeshSubsUsed[g_Object_Count] =
{
    "0", //object_texture

};  

static float g_Object_InstanceGap = 3.0f;
static int g_Object_InstanceExtCount[g_Object_Count] =
{
    0, //object_texture

};
static bool g_Object_IsShows[] = 
{
    true, //object_texture

};
static bool g_Object_IsRotates[g_Object_Count] =
{
    false, //object_texture

};
static bool g_Object_IsLightings[g_Object_Count] =
{
    true, //object_texture

};


/////////////////////////// ObjectRend //////////////////////////
static const int g_ObjectRend_Count = 1;
static const char* g_ObjectRend_Configs[7 * g_ObjectRend_Count] = 
{
    //Object Rend Name                     //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS            //Texture FS                                                                    //Texture CS
    "object_texture-1",                    "",                     "",                              "",                         "",                    "texture2d",                                                                    "", //object_texture-1
    
};
static const char* g_ObjectRend_NameShaderModules[6 * g_ObjectRend_Count] = 
{
    //vert                                //tesc                   //tese                           //geom                      //frag                                      //comp
    "vert_standard_subpass_0",            "",                      "",                              "",                         "frag_standard_subpass_0",                  "", //object_texture-1

};
static const char* g_ObjectRend_NameShaderModulesNextSubpass[6 * g_ObjectRend_Count] = 
{
    //vert                                //tesc                   //tese                           //geom                      //frag                                      //comp
    "vert_standard_subpass_1",            "",                      "",                              "",                         "frag_standard_subpass_1",                  "", //object_texture-1

};
static const char* g_ObjectRend_NameDescriptorSetLayouts[2 * g_ObjectRend_Count] = 
{
    //Pipeline Graphics                                                 //Pipeline2 Graphics
    "Pass-Object-Material-Instance-TextureFS",                          "Pass-Object-Material-Instance-TextureFS-InputAttachRed-InputAttachGreen-InputAttachBlue", //object_texture-1

};
static VkPipelineColorBlendAttachmentStateVector g_ObjectRend_ColorBlendAttachmentStates[g_ObjectRend_Count] =
{
    { Util_PipelineColorBlendAttachmentState(0xf, VK_FALSE), Util_PipelineColorBlendAttachmentState(0xf, VK_FALSE), Util_PipelineColorBlendAttachmentState(0xf, VK_FALSE), Util_PipelineColorBlendAttachmentState(0xf, VK_FALSE), },
};
static VkPipelineColorBlendAttachmentStateVector g_ObjectRend_ColorBlendAttachmentStatesNextSubpass[g_ObjectRend_Count] =
{
    { Util_PipelineColorBlendAttachmentState(0xf, VK_FALSE), },
};

static FVector3 g_ObjectRend_Tranforms[3 * g_ObjectRend_Count] = 
{   
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,    1.0f), //object_texture-1

};
static bool g_ObjectRend_IsTransparents[g_ObjectRend_Count] = 
{
    false, //object_texture-1

};
static bool g_ObjectRend_IsTopologyPatchLists[g_ObjectRend_Count] =
{
    false, //object_texture-1

};



/////////////////////////// ModelObjectRend /////////////////////


/////////////////////////// ModelObject /////////////////////////


/////////////////////////// SubPassRenderPass ///////////////////
Vulkan_018_SubPass::SubPassRenderPass::SubPassRenderPass(const String& _nameRenderPass)
    : Base(_nameRenderPass)
    , sampler(VK_NULL_HANDLE)
    , poRenderPass_SubPass(VK_NULL_HANDLE)
    , poFrameBuffer_SubPass(VK_NULL_HANDLE)
{

}

Vulkan_018_SubPass::SubPassRenderPass::~SubPassRenderPass()
{
    Destroy();
}

void Vulkan_018_SubPass::SubPassRenderPass::Destroy()
{
    //RenderPass
    if (this->poRenderPass_SubPass != VK_NULL_HANDLE)
    {
        Base::GetWindowPtr()->destroyVkRenderPass(this->poRenderPass_SubPass);
    }
    this->poRenderPass_SubPass = VK_NULL_HANDLE;

    //FrameBuffer
    size_t count = this->aColorImageLists.size();
    for (size_t i = 0; i < count; i++)
    {
        Base::GetWindowPtr()->destroyVkImage(this->aColorImageLists[i], this->aColorImageMemoryLists[i], this->aColorImageViewLists[i]);
    }
    this->aColorImageLists.clear();
    this->aColorImageMemoryLists.clear();
    this->aColorImageViewLists.clear();
    this->aColors.clear();
    this->aImageInfos.clear();

    if (this->sampler != VK_NULL_HANDLE)
    {
        Base::GetWindowPtr()->destroyVkImageSampler(this->sampler);
    }
    this->sampler = VK_NULL_HANDLE;
    
    if (this->poFrameBuffer_SubPass != VK_NULL_HANDLE)
    {
        Base::GetWindowPtr()->destroyVkFramebuffer(this->poFrameBuffer_SubPass);
    }
    this->poFrameBuffer_SubPass = VK_NULL_HANDLE;
}

void Vulkan_018_SubPass::SubPassRenderPass::Init(uint32_t width, 
                                                 uint32_t height, 
                                                 int countColorAttachment)
{
    //1> Attachment
    Base::GetWindowPtr()->createVkSampler("Texture-Color-" + this->name,
                                          F_TextureFilter_Bilinear, 
                                          F_TextureAddressing_Clamp,
                                          F_TextureBorderColor_OpaqueWhite,
                                          false,
                                          1.0f,
                                          0.0f,
                                          1.0f,
                                          0.0f,
                                          this->sampler);
    for (int i = 0; i < countColorAttachment; i++)
    {
        VkImage vkColorImage;
        VkDeviceMemory vkColorImageMemory;
        VkImageView vkColorImageView;

        String nameTexture = "Texture-Color-" + FUtilString::SaveInt(i) + "-" + this->name;
        Base::GetWindowPtr()->createVkImage(nameTexture,
                                            width, 
                                            height, 
                                            1,
                                            1,
                                            1,
                                            VK_IMAGE_TYPE_2D, 
                                            false,
                                            Base::GetWindowPtr()->poMSAASamples, 
                                            Base::GetWindowPtr()->poSwapChainImageFormat, 
                                            VK_IMAGE_TILING_OPTIMAL, 
                                            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                            VK_SHARING_MODE_EXCLUSIVE,
                                            false,
                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                                            vkColorImage, 
                                            vkColorImageMemory);
        this->aColorImageLists.push_back(vkColorImage);
        this->aColorImageMemoryLists.push_back(vkColorImageMemory);
        if (i == 0)
            this->aColors.push_back(FVector4(0.0f, 0.0f, 0.0f, 0.0f));
        else
            this->aColors.push_back(FVector4(0.0f, 0.0f, 0.0f, 1.0f));

        Base::GetWindowPtr()->createVkImageView(nameTexture,
                                                vkColorImage, 
                                                VK_IMAGE_VIEW_TYPE_2D,
                                                Base::GetWindowPtr()->poSwapChainImageFormat, 
                                                VK_IMAGE_ASPECT_COLOR_BIT, 
                                                1, 
                                                1,
                                                vkColorImageView);
        this->aColorImageViewLists.push_back(vkColorImageView);

        VkDescriptorImageInfo imageInfo = {};
        if (i == 0)
            imageInfo.sampler = this->sampler;
        imageInfo.imageView = vkColorImageView;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        this->aImageInfos.push_back(imageInfo);
    }

    //2> RenderPass
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkAttachmentDescription> aAttachmentDescription_Colors;
        std::vector<VkAttachmentDescription> aAttachmentDescription_Inputs;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        VkFormat formatColor = Base::GetWindowPtr()->poSwapChainImageFormat;
        VkFormat formatDepth = Base::GetWindowPtr()->poDepthImageFormat;
        
        //1> Attachment SceneRender Color 
        int count = (int)aColorImageViewLists.size();
        for (int i = 0; i < count; i++)
        {
            VkAttachmentDescription attachmentColor = {};
            if (i == 0)
            {
                Base::GetWindowPtr()->createAttachmentDescription(attachmentColor,
                                                                  0,
                                                                  formatColor,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                  VK_ATTACHMENT_STORE_OP_STORE,
                                                                  VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                  VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }
            else
            {
                Base::GetWindowPtr()->createAttachmentDescription(attachmentColor,
                                                                  0,
                                                                  formatColor,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                  VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                  VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                  VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                                                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            }
            
            aAttachmentDescription.push_back(attachmentColor);
            aAttachmentDescription_Colors.push_back(attachmentColor);
            if (i > 0)
            {
                aAttachmentDescription_Inputs.push_back(attachmentColor);
            }
        }

        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        Base::GetWindowPtr()->createAttachmentDescription(attachmentSR_Depth,
                                                          0,
                                                          formatDepth,
                                                          VK_SAMPLE_COUNT_1_BIT,
                                                          VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                          VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                          VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                          VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        aAttachmentDescription.push_back(attachmentSR_Depth);
        uint32_t indexDepth = (uint32_t)aAttachmentDescription.size() - 1;
            
        //3> Subpass 0
        std::vector<VkAttachmentReference> aAttachmentReference_Colors;
        for (size_t i = 0; i < aAttachmentDescription_Colors.size(); i++)
        {
            VkAttachmentReference attachRefColor = {};
            attachRefColor.attachment = (uint32_t)i;
            attachRefColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            aAttachmentReference_Colors.push_back(attachRefColor);
        }

        VkAttachmentReference attachRef_Depth = {};
        attachRef_Depth.attachment = indexDepth;
        attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass0_SceneRender = {};
        subpass0_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass0_SceneRender.inputAttachmentCount = 0;
        subpass0_SceneRender.pInputAttachments = nullptr;
        subpass0_SceneRender.colorAttachmentCount = (uint32_t)aAttachmentReference_Colors.size();
        subpass0_SceneRender.pColorAttachments = aAttachmentReference_Colors.data();
        subpass0_SceneRender.pResolveAttachments = nullptr;
        subpass0_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
        subpass0_SceneRender.preserveAttachmentCount = 0;
        subpass0_SceneRender.pPreserveAttachments = nullptr;
        aSubpassDescription.push_back(subpass0_SceneRender);

        //4> Subpass 1 
        VkAttachmentReference attachRef_Color = {};
        attachRef_Color.attachment = 0;
        attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        std::vector<VkAttachmentReference> aAttachmentReference_Inputs;
        for (size_t i = 0; i < aAttachmentDescription_Inputs.size(); i++)
        {
            VkAttachmentReference attachRefInput = {};
            attachRefInput.attachment = (uint32_t)(i + 1);
            attachRefInput.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            aAttachmentReference_Inputs.push_back(attachRefInput);
        }

        VkSubpassDescription subpass1_SceneRender = {};
        subpass1_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass1_SceneRender.colorAttachmentCount = 1;
        subpass1_SceneRender.pColorAttachments = &attachRef_Color;
        subpass1_SceneRender.inputAttachmentCount = (uint32_t)aAttachmentReference_Inputs.size();
        subpass1_SceneRender.pInputAttachments = aAttachmentReference_Inputs.data();
        subpass1_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
        aSubpassDescription.push_back(subpass1_SceneRender);

        //5> Subpass Dependency
        VkSubpassDependency subpassDependency0 = {};
        subpassDependency0.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency0.dstSubpass = 0;
        subpassDependency0.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependency0.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency0.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependency0.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency0.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        aSubpassDependency.push_back(subpassDependency0);

        VkSubpassDependency subpassDependency1 = {};
        subpassDependency1.srcSubpass = 0; 
        subpassDependency1.dstSubpass = 1; 
        subpassDependency1.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency1.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependency1.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency1.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependency1.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        aSubpassDependency.push_back(subpassDependency1);

        //6> createVkRenderPass
        if (!Base::GetWindowPtr()->createVkRenderPass("RenderPass_Default_Custom",
                                                      aAttachmentDescription,
                                                      aSubpassDescription,
                                                      aSubpassDependency,
                                                      nullptr,
                                                      this->poRenderPass_SubPass))
        {
            String msg = "*********************** Vulkan_018_SubPass::SubPassRenderPass::Init: Failed to create RenderPass_Default_Custom !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }

    //3> Framebuffer
    {
        VkImageViewVector aImageViews;
        size_t count = this->aColorImageViewLists.size();
        for (size_t i = 0; i < count; i++)
        {
            aImageViews.push_back(this->aColorImageViewLists[i]);
        }
        aImageViews.push_back(Base::GetWindowPtr()->poDepthImageView);
        if (!Base::GetWindowPtr()->createVkFramebuffer(GetName(),
                                                       aImageViews,
                                                       this->poRenderPass_SubPass,
                                                       0,
                                                       width,
                                                       height,
                                                       1,
                                                       this->poFrameBuffer_SubPass))
        {
            String msg = "*********************** Vulkan_018_SubPass::SubPassRenderPass::Init: Failed to create framebuffer: " + GetName();
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }

}

void Vulkan_018_SubPass::SubPassRenderPass::CleanupSwapChain()
{
    Destroy();
}
void Vulkan_018_SubPass::SubPassRenderPass::RecreateSwapChain()
{

}



Vulkan_018_SubPass::Vulkan_018_SubPass(int width, int height, String name)
    : VulkanWindow(width, height, name)
    , m_pSubPassRenderPass(nullptr)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = FVector3(0, -1, 0); //y-
}

void Vulkan_018_SubPass::setUpEnabledFeatures()
{
    VulkanWindow::setUpEnabledFeatures();

}

void Vulkan_018_SubPass::createRenderPass_Custom()
{
    m_pSubPassRenderPass = new SubPassRenderPass("RenderPass-SubPass");
    m_pSubPassRenderPass->Init(this->poSwapChainExtent.width, 
                               this->poSwapChainExtent.height, 
                               4);
}

void Vulkan_018_SubPass::createCamera()
{
    VulkanWindow::createCamera();
    
    cameraReset();
}
void Vulkan_018_SubPass::cameraReset()
{
    VulkanWindow::cameraReset();

    this->pCamera->SetPos(FVector3(0.0f, 120.0f, -65.0f));
    this->pCamera->SetEulerAngles(FVector3(65.0f, 0.0f, 0.0f));
}

void Vulkan_018_SubPass::loadModel_Custom()
{
    createMeshes();
    createTextures();

    int nIndexObjectRend = 0;
    for (int i = 0; i < g_Object_Count; i++)
    {
        ModelObject* pModelObject = new ModelObject(this, i);

        //1> Object
        {
            pModelObject->indexModel = i;
            pModelObject->nameObject = g_Object_Configs[2 * i + 0];
            pModelObject->nameMesh = g_Object_Configs[2 * i + 1];
            //Mesh
            {
                Mesh* pMesh = this->findMesh(pModelObject->nameMesh);
                F_Assert(pMesh != nullptr && "Vulkan_018_SubPass::loadModel_Custom")
                pModelObject->SetMesh(pMesh);
            }
            //MeshSub Used
            {
                String nameMeshSubUsed = g_Object_MeshSubsUsed[i];
                StringVector aMeshSubUsed = FUtilString::Split(nameMeshSubUsed, ";");
                pModelObject->aMeshSubUsed.clear();
                size_t count_mesh_sub_used = aMeshSubUsed.size();
                for (size_t j = 0; j < count_mesh_sub_used; j++)
                {
                    String& name = aMeshSubUsed[j];
                    int indexMeshSub = FUtilString::ParserInt(name);
                    pModelObject->aMeshSubUsed.push_back(indexMeshSub);
                }
            }
            pModelObject->isShow = g_Object_IsShows[i];
            pModelObject->isRotate = g_Object_IsRotates[i];
            pModelObject->countInstanceExt = g_Object_InstanceExtCount[i];
            pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;
        }

        //2> ObjectRend
        {
            size_t count_mesh_sub = pModelObject->pMesh->aMeshSubs.size();
            size_t count_mesh_sub_used = pModelObject->aMeshSubUsed.size();
            for (size_t j = 0; j < count_mesh_sub_used; j++)
            {
                int indexMeshSub = pModelObject->aMeshSubUsed[j];
                F_Assert(indexMeshSub >= 0 && indexMeshSub < count_mesh_sub && "Vulkan_018_SubPass::loadModel_Custom")

                MeshSub* pMeshSub = pModelObject->pMesh->aMeshSubs[indexMeshSub];
                String nameObjectRend = g_ObjectRend_Configs[7 * nIndexObjectRend + 0];
                ModelObjectRend* pRend = new ModelObjectRend(nameObjectRend, pModelObject, pMeshSub);

                //Texture VS
                {
                    String nameTextureVS = g_ObjectRend_Configs[7 * nIndexObjectRend + 1]; //Texture VS
                    if (!nameTextureVS.empty())
                    {
                        StringVector aTextureVS = FUtilString::Split(nameTextureVS, ";");
                        size_t count_tex = aTextureVS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureVS[p];
                            Texture* pTextureVS = this->findTexture(nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Vertex), pTextureVS);
                        }
                    }
                }
                //Texture TESC
                {
                    String nameTextureTESC = g_ObjectRend_Configs[7 * nIndexObjectRend + 2]; //Texture TESC
                    if (!nameTextureTESC.empty())
                    {
                        StringVector aTextureTESC = FUtilString::Split(nameTextureTESC, ";");
                        size_t count_tex = aTextureTESC.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureTESC[p];
                            Texture* pTextureTESC = this->findTexture(nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_TessellationControl), pTextureTESC);
                        }
                    }
                }
                //Texture TESE
                {
                    String nameTextureTESE = g_ObjectRend_Configs[7 * nIndexObjectRend + 3]; //Texture TESE
                    if (!nameTextureTESE.empty())
                    {
                        StringVector aTextureTESE = FUtilString::Split(nameTextureTESE, ";");
                        size_t count_tex = aTextureTESE.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureTESE[p];
                            Texture* pTextureTESE = this->findTexture(nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_TessellationEvaluation), pTextureTESE);
                        }
                    }
                }
                //Texture GS
                {
                    String nameTextureGS = g_ObjectRend_Configs[7 * nIndexObjectRend + 4]; //Texture GS
                    if (!nameTextureGS.empty())
                    {
                        StringVector aTextureGS = FUtilString::Split(nameTextureGS, ";");
                        size_t count_tex = aTextureGS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureGS[p];
                            Texture* pTextureGS = this->findTexture(nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureGS);
                        }
                    }
                }
                //Texture FS
                {
                    String nameTextureFS = g_ObjectRend_Configs[7 * nIndexObjectRend + 5]; //Texture FS
                    if (!nameTextureFS.empty())
                    {
                        StringVector aTextureFS = FUtilString::Split(nameTextureFS, ";");
                        size_t count_tex = aTextureFS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureFS[p];
                            Texture* pTextureFS = this->findTexture(nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureFS);
                        }
                    }
                }
                //Texture CS
                {
                    String nameTextureCS = g_ObjectRend_Configs[7 * nIndexObjectRend + 6]; //Texture CS
                    if (!nameTextureCS.empty())
                    {
                        StringVector aTextureCS = FUtilString::Split(nameTextureCS, ";");
                        size_t count_tex = aTextureCS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureCS[p];
                            Texture* pTextureCS = this->findTexture(nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Compute), pTextureCS);
                        }
                    }
                }

                String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 1];
                String nameShaderTese = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 2];
                if (!nameShaderTesc.empty() || !nameShaderTese.empty())
                {
                    pRend->isUsedTessellation = true;
                    if (g_ObjectRend_IsTopologyPatchLists[nIndexObjectRend])
                    {
                        pRend->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
                    }
                }

                //Pipeline Graphics - DescriptorSetLayout
                pRend->pPipelineGraphics->nameDescriptorSetLayout = g_ObjectRend_NameDescriptorSetLayouts[2 * nIndexObjectRend + 0];

                //Pipeline2 Graphics - DescriptorSetLayout2
                pRend->pPipelineGraphics->nameDescriptorSetLayout2 = g_ObjectRend_NameDescriptorSetLayouts[2 * nIndexObjectRend + 1];

                //Common
                pRend->isTransparent = g_ObjectRend_IsTransparents[nIndexObjectRend];

                pModelObject->AddObjectRend(pRend);
                m_aModelObjectRends_All.push_back(pRend);
                if (pRend->isTransparent)
                    m_aModelObjectRends_Transparent.push_back(pRend);
                else 
                    m_aModelObjectRends_Opaque.push_back(pRend);

                nIndexObjectRend ++;
            }

            m_aModelObjects.push_back(pModelObject);
            m_mapModelObjects[pModelObject->nameObject] = pModelObject;
        }
        
    }
}

void Vulkan_018_SubPass::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_018_SubPass::rebuildInstanceCBs(bool isCreateVkBuffer)
{   
    VkDeviceSize bufferSize;
    size_t count_sci = this->poSwapChainImages.size();

    //1> ObjectRends
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        int indexObject = pRend->pModelObject->index;
        int count_instance = pRend->pModelObject->countInstance;
        bool isObjectLighting = g_Object_IsLightings[indexObject];

        pRend->instanceMatWorld.clear();
        pRend->objectCBs.clear();
        pRend->materialCBs.clear();
        for (int j = 0; j < count_instance; j++)
        {
            //ObjectConstants
            {
                ObjectConstants objectConstants;
                objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectRend_Tranforms[3 * i + 0] + FVector3((j - pRend->pModelObject->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                            g_ObjectRend_Tranforms[3 * i + 1],
                                                            g_ObjectRend_Tranforms[3 * i + 2]);
                pRend->objectCBs.push_back(objectConstants);
                pRend->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = FMath::RandomColor(false);
                materialConstants.factorDiffuse = FMath::RandomColor(false);
                materialConstants.factorSpecular = FMath::RandomColor(false);
                materialConstants.shininess = FMath::RandF(10.0f, 100.0f);
                materialConstants.alpha = FMath::RandF(0.2f, 0.9f);
                materialConstants.lighting = isObjectLighting;
                //Texture VS
                {
                    TexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                    if (pTextureVSs != nullptr)
                    {

                    }
                }
                //Texture FS
                {
                    TexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                    if (pTextureFSs != nullptr)
                    {

                    }
                }
                //Texture CS
                {
                    TexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                    if (pTextureCSs != nullptr)
                    {

                    }
                }
                pRend->materialCBs.push_back(materialConstants);
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants tessellationConstants;
                tessellationConstants.tessLevelOuter = 3.0f;
                tessellationConstants.tessLevelInner = 3.0f;
                tessellationConstants.tessAlpha = 1.0f;
                pRend->tessellationCBs.push_back(tessellationConstants);
            }
        }
        
        if (isCreateVkBuffer)
        {
            //ObjectConstants
            bufferSize = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
            pRend->poBuffers_ObjectCB.resize(count_sci);
            pRend->poBuffersMemory_ObjectCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                String nameBuffer = "ObjectConstants-" + FUtilString::SavePointI(FPointI(i,j));
                createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB[j], pRend->poBuffersMemory_ObjectCB[j]);
            }

            //MaterialConstants
            bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
            pRend->poBuffers_materialCB.resize(count_sci);
            pRend->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                String nameBuffer = "MaterialConstants-" + FUtilString::SavePointI(FPointI(i,j));
                createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_materialCB[j], pRend->poBuffersMemory_materialCB[j]);
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                bufferSize = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pRend->poBuffers_tessellationCB.resize(count_sci);
                pRend->poBuffersMemory_tessellationCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    String nameBuffer = "TessellationConstants-" + FUtilString::SavePointI(FPointI(i,j));
                    createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_tessellationCB[j], pRend->poBuffersMemory_tessellationCB[j]);
                }
            }
        }
    }
}

void Vulkan_018_SubPass::createCustomBeforePipeline()
{
    //1> DescriptorSetLayout
    createDescriptorSetLayouts();

    //2> PipelineLayout
    createPipelineLayouts();

    //3> Shader
    createShaderModules();

    //4> Graphics_CopyBlit
    UpdateDescriptorSets_Graphics_CopyBlit(this->m_pSubPassRenderPass->aImageInfos[0]);
}   
void Vulkan_018_SubPass::createGraphicsPipeline_Custom()
{
    //1> Viewport
    VkViewportVector aViewports;
    aViewports.push_back(this->poViewport);
    VkRect2DVector aScissors;
    aScissors.push_back(this->poScissor);

    //2> Pipeline
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //[1] Shaders
        String nameShaderVert = g_ObjectRend_NameShaderModules[6 * i + 0];
        String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * i + 1];
        String nameShaderTese = g_ObjectRend_NameShaderModules[6 * i + 2];
        String nameShaderGeom = g_ObjectRend_NameShaderModules[6 * i + 3];
        String nameShaderFrag = g_ObjectRend_NameShaderModules[6 * i + 4];
        if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                  nameShaderTesc,
                                                  nameShaderTese,
                                                  nameShaderGeom,
                                                  nameShaderFrag,
                                                  m_mapVkShaderModules,
                                                  pRend->aShaderStageCreateInfos_Graphics))
        {
            String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //GraphicsNextSubpass
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_GraphicsNextSubpass;
        nameShaderVert = g_ObjectRend_NameShaderModulesNextSubpass[6 * i + 0];
        nameShaderTesc = g_ObjectRend_NameShaderModulesNextSubpass[6 * i + 1];
        nameShaderTese = g_ObjectRend_NameShaderModulesNextSubpass[6 * i + 2];
        nameShaderGeom = g_ObjectRend_NameShaderModulesNextSubpass[6 * i + 3];
        nameShaderFrag = g_ObjectRend_NameShaderModulesNextSubpass[6 * i + 4];
        bool hasGraphicsNextSubpass = false;
        if (!nameShaderVert.empty() && !nameShaderFrag.empty())
        {
            if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                      nameShaderTesc,
                                                      nameShaderTese,
                                                      nameShaderGeom,
                                                      nameShaderFrag,
                                                      m_mapVkShaderModules,
                                                      aShaderStageCreateInfos_GraphicsNextSubpass))
            {
                String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: NextSubpass Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            hasGraphicsNextSubpass = true;
        }

        //[2] Pipeline Graphics
        {
            //(1) poPipeline
            pRend->pPipelineGraphics->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poDescriptorSetLayout = findDescriptorSetLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poPipelineLayout = findPipelineLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            //pPipelineGraphics->poPipeline_WireFrame
            VkPipelineColorBlendAttachmentStateVector& aColorBlendAttachmentState = g_ObjectRend_ColorBlendAttachmentStates[i];
            pRend->pPipelineGraphics->poPipeline_WireFrame = createVkGraphicsPipeline("PipelineGraphics-Wire-" + pRend->nameObjectRend,
                                                                                      pRend->aShaderStageCreateInfos_Graphics,
                                                                                      pRend->isUsedTessellation, 0, 3,
                                                                                      Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      this->m_pSubPassRenderPass->poRenderPass_SubPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                      pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pRend->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                      pRend->cfg_isDepthTest, pRend->cfg_isDepthWrite, pRend->cfg_DepthCompareOp,
                                                                                      pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                      aColorBlendAttachmentState, 0);
            if (pRend->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Failed to create pipeline graphics wire frame: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_018_SubPass::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics wire frame success !", pRend->nameObjectRend.c_str());

            //pPipelineGraphics->poPipeline
            VkBool32 isDepthTestEnable = pRend->cfg_isDepthTest;
            VkBool32 isDepthWriteEnable = pRend->cfg_isDepthWrite;
            VkBool32 isBlend = pRend->cfg_isBlend;
            VkBlendFactor blendColorFactorSrc = pRend->cfg_BlendColorFactorSrc; 
            VkBlendFactor blendColorFactorDst = pRend->cfg_BlendColorFactorDst; 
            if (pRend->isTransparent)
            {
                isDepthTestEnable = VK_FALSE;
                isDepthWriteEnable = VK_FALSE;

                isBlend = VK_TRUE;
                blendColorFactorSrc = VK_BLEND_FACTOR_SRC_ALPHA;
                blendColorFactorDst = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            }
            pRend->pPipelineGraphics->poPipeline = createVkGraphicsPipeline("PipelineGraphics-" + pRend->nameObjectRend,
                                                                            pRend->aShaderStageCreateInfos_Graphics,
                                                                            pRend->isUsedTessellation, 0, 3,
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                            this->m_pSubPassRenderPass->poRenderPass_SubPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                            pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                            isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                            pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                            aColorBlendAttachmentState, 0);
            if (pRend->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Failed to create pipeline graphics: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_018_SubPass::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics success !", pRend->nameObjectRend.c_str());
        

            //NextSubpass
            if (hasGraphicsNextSubpass)
            {
                pRend->pPipelineGraphics->hasNextSubpass = true;

                //(2) poPipeline2
                pRend->pPipelineGraphics->poDescriptorSetLayoutNames2 = findDescriptorSetLayoutNames(pRend->pPipelineGraphics->nameDescriptorSetLayout2);
                if (pRend->pPipelineGraphics->poDescriptorSetLayoutNames2 == nullptr)
                {
                    String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames2 by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout2;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                pRend->pPipelineGraphics->poDescriptorSetLayout2 = findDescriptorSetLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout2);
                if (pRend->pPipelineGraphics->poDescriptorSetLayout2 == VK_NULL_HANDLE)
                {
                    String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout2;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                pRend->pPipelineGraphics->poPipelineLayout2 = findPipelineLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout2);
                if (pRend->pPipelineGraphics->poPipelineLayout2 == VK_NULL_HANDLE)
                {
                    String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout2;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                //pPipelineGraphics->poPipeline_WireFrame2
                VkPipelineColorBlendAttachmentStateVector& aColorBlendAttachmentStateNextSubpass = g_ObjectRend_ColorBlendAttachmentStatesNextSubpass[i];
                pRend->pPipelineGraphics->poPipeline_WireFrame2 = createVkGraphicsPipeline("PipelineGraphics-Wire2-" + pRend->nameObjectRend,
                                                                                           aShaderStageCreateInfos_GraphicsNextSubpass,
                                                                                           pRend->isUsedTessellation, 0, 3,
                                                                                           Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                           Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                           this->m_pSubPassRenderPass->poRenderPass_SubPass, pRend->pPipelineGraphics->poPipelineLayout2, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                           pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pRend->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                           pRend->cfg_isDepthTest, pRend->cfg_isDepthWrite, pRend->cfg_DepthCompareOp,
                                                                                           pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                           aColorBlendAttachmentStateNextSubpass, 1);
                if (pRend->pPipelineGraphics->poPipeline_WireFrame2 == VK_NULL_HANDLE)
                {
                    String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Failed to create pipeline2 graphics wire frame: " + pRend->nameObjectRend;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("Vulkan_018_SubPass::createGraphicsPipeline_Custom: Object: [%s] Create pipeline2 graphics wire frame success !", pRend->nameObjectRend.c_str());


                //pPipelineGraphics->poPipeline2
                VkBool32 isDepthTestEnable = pRend->cfg_isDepthTest;
                VkBool32 isDepthWriteEnable = pRend->cfg_isDepthWrite;
                VkBool32 isBlend = pRend->cfg_isBlend;
                VkBlendFactor blendColorFactorSrc = pRend->cfg_BlendColorFactorSrc; 
                VkBlendFactor blendColorFactorDst = pRend->cfg_BlendColorFactorDst; 
                if (pRend->isTransparent)
                {
                    isDepthTestEnable = VK_FALSE;
                    isDepthWriteEnable = VK_FALSE;

                    isBlend = VK_TRUE;
                    blendColorFactorSrc = VK_BLEND_FACTOR_SRC_ALPHA;
                    blendColorFactorDst = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                }
                pRend->pPipelineGraphics->poPipeline2 = createVkGraphicsPipeline("PipelineGraphics-2-" + pRend->nameObjectRend,
                                                                                 aShaderStageCreateInfos_GraphicsNextSubpass,
                                                                                 pRend->isUsedTessellation, 0, 3,
                                                                                 Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex), 
                                                                                 Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                 this->m_pSubPassRenderPass->poRenderPass_SubPass, pRend->pPipelineGraphics->poPipelineLayout2, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                 pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                 isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                                 pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                 aColorBlendAttachmentStateNextSubpass, 1);
                if (pRend->pPipelineGraphics->poPipeline2 == VK_NULL_HANDLE)
                {
                    String msg = "*********************** Vulkan_018_SubPass::createGraphicsPipeline_Custom: Failed to create pipeline2 graphics: " + pRend->nameObjectRend;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("Vulkan_018_SubPass::createGraphicsPipeline_Custom: Object: [%s] Create pipeline2 graphics success !", pRend->nameObjectRend.c_str());
            }
        }
    }
}
void Vulkan_018_SubPass::createComputePipeline_Custom()
{

}

void Vulkan_018_SubPass::destroyMeshes()
{
    size_t count = this->m_aModelMesh.size();
    for (size_t i = 0; i < count; i++)
    {
        Mesh* pMesh = this->m_aModelMesh[i];
        delete pMesh;
    }
    this->m_aModelMesh.clear();
    this->m_mapModelMesh.clear();
}
void Vulkan_018_SubPass::createMeshes()
{
    for (int i = 0; i < g_MeshCount; i++)
    {
        String nameMesh = g_MeshPaths[5 * i + 0];
        String nameVertexType = g_MeshPaths[5 * i + 1];
        String nameMeshType = g_MeshPaths[5 * i + 2];
        String nameGeometryType = g_MeshPaths[5 * i + 3];
        String pathMesh = g_MeshPaths[5 * i + 4];
        
        FMeshVertexType typeVertex = F_ParseMeshVertexType(nameVertexType); 
        FMeshType typeMesh = F_ParseMeshType(nameMeshType);
        FMeshGeometryType typeGeometryType = F_MeshGeometry_EntityTriangle;
        if (!nameGeometryType.empty())
        {
            typeGeometryType = F_ParseMeshGeometryType(nameGeometryType);
        }

        Mesh* pMesh = new Mesh(0,
                               nameMesh,
                               pathMesh,
                               typeMesh,
                               typeVertex,
                               typeGeometryType,
                               nullptr);
        bool isFlipY = g_MeshIsFlipYs[i];
        bool isTransformLocal = g_MeshIsTranformLocals[i];
        if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals[i]))
        {
            String msg = "*********************** Vulkan_018_SubPass::createMeshes: create mesh: [" + nameMesh + "] failed !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        F_LogInfo("Vulkan_018_SubPass::createMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], mesh sub count: [%d], path: [%s] success !", 
                  nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), (int)pMesh->aMeshSubs.size(), pathMesh.c_str());
    }
}
Mesh* Vulkan_018_SubPass::findMesh(const String& nameMesh)
{
    MeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_018_SubPass::destroyTextures()
{
    size_t count = this->m_aModelTexture.size();
    for (size_t i = 0; i < count; i++)
    {
        Texture* pTexture = this->m_aModelTexture[i];
        delete pTexture;
    }
    this->m_aModelTexture.clear();
    this->m_mapModelTexture.clear();
}
void Vulkan_018_SubPass::createTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        String nameTexture = g_TexturePaths[5 * i + 0];
        String nameType = g_TexturePaths[5 * i + 1];
        FTextureType typeTexture = F_ParseTextureType(nameType);
        String nameIsRenderTarget = g_TexturePaths[5 * i + 2];
        bool isRenderTarget = FUtilString::ParserBool(nameIsRenderTarget);
        String nameIsGraphicsComputeShared = g_TexturePaths[5 * i + 3];
        bool isGraphicsComputeShared = FUtilString::ParserBool(nameIsGraphicsComputeShared);
        String pathTextures = g_TexturePaths[5 * i + 4];

        StringVector aPathTexture = FUtilString::Split(pathTextures, ";");
        Texture* pTexture = new Texture(0,
                                        nameTexture,
                                        aPathTexture,
                                        typeTexture,
                                        g_TextureFormats[i],
                                        g_TextureFilters[i],
                                        g_TextureAddressings[i],
                                        g_TextureBorderColors[i],
                                        isRenderTarget,
                                        isGraphicsComputeShared);
        pTexture->texChunkMaxX = g_TextureAnimChunks[i * 2 + 0];
        pTexture->texChunkMaxY = g_TextureAnimChunks[i * 2 + 1];
        if (pTexture->texChunkMaxX > 0 && 
            pTexture->texChunkMaxY > 0)
        {
            pTexture->texChunkIndex = FMath::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
        }
        pTexture->AddRef();

        int width = g_TextureSizes[3 * i + 0];
        int height = g_TextureSizes[3 * i + 1];
        int depth = g_TextureSizes[3 * i + 1];
        pTexture->LoadTexture(width, 
                              height,
                              depth);

        this->m_aModelTexture.push_back(pTexture);
        this->m_mapModelTexture[nameTexture] = pTexture;

        F_LogInfo("Vulkan_018_SubPass::createTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                  nameTexture.c_str(), 
                  nameType.c_str(), 
                  isRenderTarget ? "true" : "false",
                  pathTextures.c_str());
    }
}
Texture* Vulkan_018_SubPass::findTexture(const String& nameTexture)
{
    TexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_018_SubPass::destroyDescriptorSetLayouts()
{
    size_t count = this->m_aVkDescriptorSetLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        destroyVkDescriptorSetLayout(this->m_aVkDescriptorSetLayouts[i]);
    }
    this->m_aVkDescriptorSetLayouts.clear();
    this->m_mapVkDescriptorSetLayout.clear();
    this->m_mapName2Layouts.clear();
}
void Vulkan_018_SubPass::createDescriptorSetLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameLayout(g_DescriptorSetLayoutNames[i]);
        StringVector aLayouts = FUtilString::Split(nameLayout, "-");
        VkDescriptorSetLayout vkDescriptorSetLayout = CreateDescriptorSetLayout(nameLayout, &aLayouts);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_018_SubPass::createDescriptorSetLayouts: Failed to create descriptor set layout: " + nameLayout;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aVkDescriptorSetLayouts.push_back(vkDescriptorSetLayout);
        this->m_mapVkDescriptorSetLayout[nameLayout] = vkDescriptorSetLayout;
        this->m_mapName2Layouts[nameLayout] = aLayouts;

        F_LogInfo("Vulkan_018_SubPass::createDescriptorSetLayouts: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
    }
}
VkDescriptorSetLayout Vulkan_018_SubPass::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayout.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapVkDescriptorSetLayout.end())
    {
        return nullptr;
    }
    return itFind->second;
}
StringVector* Vulkan_018_SubPass::findDescriptorSetLayoutNames(const String& nameDescriptorSetLayout)
{
    std::map<String, StringVector>::iterator itFind = this->m_mapName2Layouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapName2Layouts.end())
    {
        return nullptr;
    }
    return &(itFind->second);
}


void Vulkan_018_SubPass::destroyShaderModules()
{   
    size_t count = this->m_aVkShaderModules.size();
    for (size_t i = 0; i < count; i++)
    {
        VkShaderModule& vkShaderModule= this->m_aVkShaderModules[i];
        destroyVkShaderModule(vkShaderModule);
    }
    this->m_aVkShaderModules.clear();
    this->m_mapVkShaderModules.clear();
}
void Vulkan_018_SubPass::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = g_ShaderModulePaths[3 * i + 2];

        VkShaderModule shaderModule = createVkShaderModule(shaderName, shaderType, shaderPath);
        this->m_aVkShaderModules.push_back(shaderModule);
        this->m_mapVkShaderModules[shaderName] = shaderModule;
        F_LogInfo("Vulkan_018_SubPass::createShaderModules: create shader, name: [%s], type: [%s], path: [%s] success !", 
                  shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
    }
}
VkShaderModule Vulkan_018_SubPass::findShaderModule(const String& nameShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(nameShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   


void Vulkan_018_SubPass::destroyPipelineLayouts()
{
    size_t count = this->m_aVkPipelineLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        destroyVkPipelineLayout(this->m_aVkPipelineLayouts[i]);
    }
    this->m_aVkPipelineLayouts.clear();
    this->m_mapVkPipelineLayouts.clear();
}
void Vulkan_018_SubPass::createPipelineLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameDSL(g_DescriptorSetLayoutNames[i]);
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(nameDSL);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_018_SubPass::createPipelineLayouts: Can not find DescriptorSetLayout by name: [%s]", nameDSL.c_str());
            return;
        }

        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(vkDescriptorSetLayout);
        VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(nameDSL, aDescriptorSetLayout);
        if (vkPipelineLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_018_SubPass::createPipelineLayouts: createVkPipelineLayout failed !");
            return;
        }

        this->m_aVkPipelineLayouts.push_back(vkPipelineLayout);
        this->m_mapVkPipelineLayouts[nameDSL] = vkPipelineLayout;
    }
}
VkPipelineLayout Vulkan_018_SubPass::findPipelineLayout(const String& namePipelineLayout)
{
    VkPipelineLayoutMap::iterator itFind = this->m_mapVkPipelineLayouts.find(namePipelineLayout);
    if (itFind == this->m_mapVkPipelineLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}



void Vulkan_018_SubPass::createDescriptorSets_Custom()
{
    //1> Object Rend
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //Pipeline Graphics
        {
            createVkDescriptorSets("DescriptorSets-" + pRend->nameObjectRend, pRend->pPipelineGraphics->poDescriptorSetLayout, pRend->pPipelineGraphics->poDescriptorSets);
            createDescriptorSets_Graphics(pRend->pPipelineGraphics->poDescriptorSetLayoutNames, pRend->pPipelineGraphics->poDescriptorSets, pRend);
        }   

        //poPipeline2 Graphics
        if (pRend->pPipelineGraphics->hasNextSubpass)
        {
            createVkDescriptorSets("DescriptorSets-2-" + pRend->nameObjectRend, pRend->pPipelineGraphics->poDescriptorSetLayout2, pRend->pPipelineGraphics->poDescriptorSets2);
            createDescriptorSets_Graphics(pRend->pPipelineGraphics->poDescriptorSetLayoutNames2, pRend->pPipelineGraphics->poDescriptorSets2, pRend);
        }
    }
}
void Vulkan_018_SubPass::createDescriptorSets_Graphics(StringVector* pDescriptorSetLayoutNames,
                                                       VkDescriptorSetVector& listDescriptorSets, 
                                                       ModelObjectRend* pRend)
{
    F_Assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_018_SubPass::createDescriptorSets_Graphics")

    size_t count_ds = listDescriptorSets.size();
    for (size_t j = 0; j < count_ds; j++)
    {   
        VkWriteDescriptorSetVector descriptorWrites;
        int nIndexTextureVS = 0;
        int nIndexTextureTESC = 0;
        int nIndexTextureTESE = 0;
        int nIndexTextureFS = 0;
        int nIndexInputAttach = 1;

        size_t count_names = pDescriptorSetLayoutNames->size();
        for (size_t p = 0; p < count_names; p++)
        {
            String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];

            if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            listDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Pass);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
            {
                VkDescriptorBufferInfo bufferInfo_Object = {};
                bufferInfo_Object.buffer = pRend->poBuffers_ObjectCB[j];
                bufferInfo_Object.offset = 0;
                bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            listDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Object);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Material)) //Material
            {
                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = pRend->poBuffers_materialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            listDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Material);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Instance)) //Instance
            {
                VkDescriptorBufferInfo bufferInfo_Instance = {};
                bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j]; 
                bufferInfo_Instance.offset = 0;
                bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            listDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Instance);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Tessellation)) //Tessellation
            {
                VkDescriptorBufferInfo bufferInfo_Tessellation = {};
                bufferInfo_Tessellation.buffer = pRend->poBuffers_tessellationCB[j];
                bufferInfo_Tessellation.offset = 0;
                bufferInfo_Tessellation.range = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            listDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Tessellation);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureVS)) //TextureVS
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Vertex), nIndexTextureVS);
                nIndexTextureVS ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          listDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESC))//TextureTESC
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_TessellationControl), nIndexTextureTESC);
                nIndexTextureTESC ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          listDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESE))//TextureTESE
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_TessellationEvaluation), nIndexTextureTESE);
                nIndexTextureTESE ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          listDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS)) //TextureFS
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Fragment), nIndexTextureFS);
                nIndexTextureFS ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          listDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_InputAttachRed) || //InputAttachRed
                     nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_InputAttachGreen) || //InputAttachGreen
                     nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_InputAttachBlue))  //InputAttachBlue
            {
                pushVkDescriptorSet_Image(descriptorWrites,
                                          listDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
                                          this->m_pSubPassRenderPass->aImageInfos[nIndexInputAttach]);
                nIndexInputAttach ++;
            }
            else
            {
                String msg = "*********************** Vulkan_018_SubPass::createDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }

        updateVkDescriptorSets(descriptorWrites);
    }
}

void Vulkan_018_SubPass::updateCBs_Custom()
{
    //1> Object Rend
    float time = this->pTimer->GetTimeSinceStart();
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_object = pRend->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pRend->objectCBs[j];
            if (pRend->pModelObject->isRotate || pRend->isRotate || this->cfg_isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pRend->instanceMatWorld[j], 
                                                  time * glm::radians(90.0f), 
                                                  FVector3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pRend->instanceMatWorld[j];
            }

            //MaterialConstants
            MaterialConstants& materialCB = pRend->materialCBs[j];
            
            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants& tessellationCB = pRend->tessellationCBs[j];
            }
        }

        //ObjectConstants
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pRend->objectCBs.data(), memory);
        }

        //MaterialConstants
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pRend->materialCBs.data(), memory);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(TessellationConstants) * count_object, pRend->tessellationCBs.data(), memory);
        }
    }
}

void Vulkan_018_SubPass::updateRenderPass_CustomBeforeDefault(VkCommandBuffer& commandBuffer)
{
    beginRenderPass(commandBuffer,
                    "[RenderPass-CustomBeforeDefault]",
                    this->m_pSubPassRenderPass->poRenderPass_SubPass,
                    this->m_pSubPassRenderPass->poFrameBuffer_SubPass,
                    this->poOffset,
                    this->poExtent,
                    this->m_pSubPassRenderPass->aColors,
                    1.0f,
                    0);
    {
        //1> Viewport
        bindViewport(commandBuffer, this->poViewport, this->poScissor);

        //2> Custom Render Pass
        drawCustomBeforeDefault(commandBuffer);
    }
    endRenderPass(commandBuffer);
}
    void Vulkan_018_SubPass::drawCustomBeforeDefault(VkCommandBuffer& commandBuffer)
    {   
        //1> Opaque
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Opaque);
        }
        //2> Transparent
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Transparent);
        }
    }
        void Vulkan_018_SubPass::drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
        {
            size_t count_rend = aRends.size();
            for (size_t i = 0; i < count_rend; i++)
            {
                ModelObjectRend* pRend = aRends[i];
                if (!pRend->isShow ||
                    !pRend->pModelObject->isShow)
                    continue;
                drawModelObjectRend(commandBuffer, pRend);
            }
        }
        void Vulkan_018_SubPass::drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend)
        {
            ModelObject* pModelObject = pRend->pModelObject;
            MeshSub* pMeshSub = pRend->pMeshSub;

            VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
            if (pMeshSub->poIndexBuffer != nullptr)
            {
                bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            }

            drawModelObjectRendPipeline(commandBuffer, 
                                        pRend, 
                                        pMeshSub, 
                                        pRend->pPipelineGraphics->poPipelineLayout,
                                        pRend->pPipelineGraphics->poDescriptorSets,
                                        pRend->pPipelineGraphics->poPipeline, 
                                        pRend->pPipelineGraphics->poPipeline_WireFrame);
            if (pRend->pPipelineGraphics->hasNextSubpass)
            {
                vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
                drawModelObjectRendPipeline(commandBuffer, 
                                            pRend, 
                                            pMeshSub, 
                                            pRend->pPipelineGraphics->poPipelineLayout2,
                                            pRend->pPipelineGraphics->poDescriptorSets2,
                                            pRend->pPipelineGraphics->poPipeline2, 
                                            pRend->pPipelineGraphics->poPipeline_WireFrame2);
            }
        }
        void Vulkan_018_SubPass::drawModelObjectRendPipeline(VkCommandBuffer& commandBuffer, 
                                                            ModelObjectRend* pRend, 
                                                            MeshSub* pMeshSub, 
                                                            VkPipelineLayout pipelineLayout,
                                                            VkDescriptorSetVector& descriptorSets,
                                                            VkPipeline pipeline, 
                                                            VkPipeline pipeline_WireFrame)
        {
            ModelObject* pModelObject = pRend->pModelObject;
            if (pModelObject->isWireFrame || pRend->isWireFrame || this->cfg_isWireFrame)
            {
                if (pipeline_WireFrame != VK_NULL_HANDLE)
                    bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_WireFrame);
                else
                    bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
                if (descriptorSets.size() > 0)
                {
                    bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[this->poSwapChainImageIndex], 0, nullptr);
                }
                if (pMeshSub->poIndexBuffer != nullptr)
                {
                    drawIndexed(commandBuffer, pMeshSub->poIndexCount, pModelObject->countInstance, 0, 0, 0);
                }
                else
                {
                    draw(commandBuffer, pMeshSub->poVertexCount, pModelObject->countInstance, 0, 0);
                }
            }
            else
            {
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
                if (descriptorSets.size() > 0)
                {
                    bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[this->poSwapChainImageIndex], 0, nullptr);
                }
                if (pMeshSub->poIndexBuffer != nullptr)
                {
                    drawIndexed(commandBuffer, pMeshSub->poIndexCount, pModelObject->countInstance, 0, 0, 0);
                }
                else
                {
                    draw(commandBuffer, pMeshSub->poVertexCount, pModelObject->countInstance, 0, 0);
                }
            }
        }

    void Vulkan_018_SubPass::drawMeshDefault_CustomBeforeImgui(VkCommandBuffer& commandBuffer)
    {
        Draw_Graphics_CopyBlit(commandBuffer);
    }

bool Vulkan_018_SubPass::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_018_SubPass", &windowOpened, 0);
    {
        //0> Common
        commonConfig();

        //1> Camera
        cameraConfig();

        //2> Light
        lightConfig();

        //3> PassConstants
        passConstantsConfig();

        //4> Model
        modelConfig();

    }
    ImGui::End();

    return true;
}
void Vulkan_018_SubPass::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
        float fGap = g_Object_InstanceGap;
        if (ImGui::DragFloat("Instance Gap: ", &fGap, 0.1f, 1.0f, 100.0f))
        {
            g_Object_InstanceGap = fGap;
            rebuildInstanceCBs(false);
        }

        size_t count_object = this->m_aModelObjects.size();
        for (size_t i = 0; i < count_object; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];
            size_t count_object_rend = pModelObject->aRends.size();

            //1> ModelObject
            String nameObject = FUtilString::SaveInt(i) + " - " + pModelObject->nameObject;
            if (ImGui::CollapsingHeader(nameObject.c_str()))
            {
                //isShow
                String nameIsShow = "Is Show - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isShow = pModelObject->isShow;
                    }
                }
                //isWireFrame
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isWireFrame = pModelObject->isWireFrame;
                    }
                }
                //isRotate
                String nameIsRotate = "Is Rotate - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isRotate = pModelObject->isRotate;
                    }
                }
                //isLighting
                String nameIsLighting = "Is Lighting - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        for (int p = 0; p < pModelObject->countInstance; p++)
                        {
                            MaterialConstants& mat = pRend->materialCBs[p];
                            mat.lighting = pModelObject->isLighting;
                        }
                    }
                }

                String nameInstances = "Instance - " + pModelObject->nameObject;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 3);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                //2> ModelObjectRend
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        String& nameObjectRend = pRend->nameObjectRend;
                        if (ImGui::CollapsingHeader(nameObjectRend.c_str()))
                        {
                            ImGui::Text("Vertex: [%d], Index: [%d]", (int)pRend->pMeshSub->poVertexCount, (int)pRend->pMeshSub->poIndexCount);
                            //isShow
                            String nameIsShowRend = "Is Show - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsShowRend.c_str(), &pRend->isShow))
                            {
                                if (pRend->isShow)
                                {
                                    pModelObject->isShow = true;
                                }
                            }
                            //isWireFrame
                            String nameIsWireFrameRend = "Is WireFrame - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsWireFrameRend.c_str(), &pRend->isWireFrame))
                            {
                                if (!pRend->isWireFrame)
                                {
                                    pModelObject->isWireFrame = false;
                                }
                            }
                            //isRotate
                            String nameIsRotateRend = "Is Rotate - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsRotateRend.c_str(), &pRend->isRotate))
                            {
                                
                            }
                            //isLighting
                            String nameIsLightingRend = "Is Lighting - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsLightingRend.c_str(), &pRend->isLighting))
                            {
                                if (pRend->isLighting)
                                {
                                    pModelObject->isLighting = true;
                                }
                                for (int p = 0; p < pModelObject->countInstance; p++)
                                {
                                    MaterialConstants& mat = pRend->materialCBs[p];
                                    mat.lighting = pModelObject->isLighting;
                                }
                            }
                            //isTransparent
                            String nameIsTransparent = "Is Transparent(Read Only) - " + nameObjectRend;
                            bool isTransparent = pRend->isTransparent;
                            ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);

                            String nameWorld = "Model Object - " + nameObjectRend;
                            if (ImGui::CollapsingHeader(nameWorld.c_str()))
                            {
                                int count_instance = pModelObject->countInstance;
                                for (int p = 0; p < count_instance; p++)
                                {
                                    ObjectConstants& obj = pRend->objectCBs[p];
                                    MaterialConstants& mat = pRend->materialCBs[p];

                                    String nameModelInstance = FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                    if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                                    {
                                        //ObjectConstants
                                        String nameObject = FUtilString::SaveInt(p) + " - Object - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameObject.c_str()))
                                        {
                                            const FMatrix4& mat4World = obj.g_MatWorld;
                                            String nameTable = FUtilString::SaveInt(p) + " - matWorld - " + nameObjectRend;
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
                                        
                                        //MaterialConstants
                                        String nameMaterial = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                                        {
                                            //factorAmbient
                                            String nameFactorAmbient = "FactorAmbient - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorDiffuse
                                            String nameFactorDiffuse = "FactorDiffuse - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorSpecular
                                            String nameFactorSpecular = "FactorSpecular - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //shininess
                                            String nameShininess = "Shininess - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //alpha
                                            String nameAlpha = "Alpha - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //lighting
                                            String nameLighting = "Lighting - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            bool isLighting = mat.lighting == 1.0f ? true : false;
                                            if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                            {
                                                mat.lighting = isLighting ? 1.0f : 0.0f;
                                            }

                                            //Texture VS
                                            {
                                                TexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                                                if (pTextureVSs != nullptr)
                                                {

                                                }
                                            }
                                            //Texture FS
                                            {
                                                TexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                                                if (pTextureFSs != nullptr)
                                                {
                                                    size_t count_texture = pTextureFSs->size();
                                                    for (size_t q = 0; q < count_texture; q++)
                                                    {
                                                        Texture* pTexture = (*pTextureFSs)[q];

                                                        String nameMaterial_Texture = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend + " - TextureFS - " + FUtilString::SaveInt(q);
                                                        if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                                        {
                                                            //texWidth
                                                            String nameWidth = "Width - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int width = pTexture->width;
                                                            ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                            //texHeight
                                                            String nameHeight = "Height - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int height = pTexture->height;
                                                            ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                            //texDepth
                                                            String nameDepth = "Depth - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int depth = pTexture->depth;
                                                            ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                                            //indexTextureArray
                                                            String nameIndexTextureArray = "IndexTextureArray - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            if (pTexture->typeTexture == F_Texture_2DArray)
                                                            {
                                                                int count_tex = (int)pTexture->aPathTexture.size();
                                                                int indexTextureArray = (int)mat.aTexLayers[q].indexTextureArray;
                                                                if (ImGui::DragInt(nameIndexTextureArray.c_str(), &indexTextureArray, 1, 0, count_tex - 1))
                                                                {
                                                                    mat.aTexLayers[p].indexTextureArray = indexTextureArray;
                                                                }
                                                            }
                                                            else 
                                                            {
                                                                if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[q].indexTextureArray, 0.001f, 0.0f, 1.0f))
                                                                {

                                                                }
                                                            }

                                                            //texSpeedU
                                                            String nameTexSpeedU = "TexSpeedU - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedV
                                                            String nameTexSpeedV = "texSpeedV - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedW
                                                            String nameTexSpeedW = "texSpeedW - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeedW, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }

                                                            //texChunkMaxX
                                                            String nameTexChunkMaxX = "texChunkMaxX - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxX = mat.aTexLayers[q].texChunkMaxX;
                                                            ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                                            //texChunkMaxY
                                                            String nameTexChunkMaxY = "texChunkMaxY - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxY = mat.aTexLayers[q].texChunkMaxY;
                                                            ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                                            //texChunkIndexX
                                                            String nameTexChunkIndexX = "texChunkIndexX - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexX = mat.aTexLayers[q].texChunkIndexX;
                                                            ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                                            //texChunkIndexY
                                                            String nameTexChunkIndexY = "texChunkIndexY - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexY = mat.aTexLayers[q].texChunkIndexY;
                                                            ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                                        }
                                                    }
                                                }
                                            }
                                            //Texture CS
                                            {
                                                TexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                                                if (pTextureCSs != nullptr)
                                                {

                                                }
                                            }

                                            ImGui::Spacing();
                                        }

                                        //TessellationConstants
                                        String nameTessellation = FUtilString::SaveInt(p) + " - Tessellation - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameTessellation.c_str()))
                                        {
                                            if (pRend->isUsedTessellation)
                                            {
                                                TessellationConstants& tess = pRend->tessellationCBs[j];
                                                //tessLevelOuter
                                                String nameTessLevelOuter = "tessLevelOuter - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelOuter.c_str(), &tess.tessLevelOuter, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessLevelInner
                                                String nameTessLevelInner = "tessLevelInner - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelInner.c_str(), &tess.tessLevelInner, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessAlpha
                                                String nameTessAlpha = "tessAlpha - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                                {
                                                    
                                                }
                                                //tessStrength
                                                String nameTessStrength = "tessStrength - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessStrength.c_str(), &tess.tessStrength, 0.025f, 0.1f, 100.0f))
                                                {
                                                    
                                                }
                                            }

                                            ImGui::Spacing();
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
                
            }
        }
    }
}

void Vulkan_018_SubPass::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_018_SubPass::cleanupCustom()
{   
    destroyTextures();
    destroyMeshes();

    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        F_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_mapModelObjects.clear();
    this->m_aModelObjectRends_All.clear();
    this->m_aModelObjectRends_Opaque.clear();
    this->m_aModelObjectRends_Transparent.clear();
}

void Vulkan_018_SubPass::cleanupSwapChain_Custom()
{
    F_DELETE(m_pSubPassRenderPass)
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }

    destroyDescriptorSetLayouts();
    destroyPipelineLayouts();
    destroyShaderModules();
}

void Vulkan_018_SubPass::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->RecreateSwapChain();
    }
}