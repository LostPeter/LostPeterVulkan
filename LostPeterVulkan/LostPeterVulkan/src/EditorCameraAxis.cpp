/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/EditorCameraAxis.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/VKPipelineGraphics.h"
#include "../include/VKMultiRenderPass.h"

namespace LostPeterVulkan
{
    size_t EditorCameraAxis::s_nMeshConeIndex = 0;
    size_t EditorCameraAxis::s_nMeshAABBIndex = 1;
    size_t EditorCameraAxis::s_nMeshQuadIndex = 2;
    size_t EditorCameraAxis::s_nMeshCameraAxisCount = 2;
    const String EditorCameraAxis::s_strNameShader_CameraAxis_Vert = "vert_editor_camera_axis";
    const String EditorCameraAxis::s_strNameShader_CameraAxis_Frag = "frag_editor_camera_axis";
    const String EditorCameraAxis::s_strNameShader_QuadBlit_Vert = "vert_standard_copy_blit";
    const String EditorCameraAxis::s_strNameShader_QuadBlit_Frag = "frag_standard_copy_blit";
    const float EditorCameraAxis::s_fBlitAreaWidth = 128;
    const float EditorCameraAxis::s_fBlitAreaHeight = 128;
    FMatrix4 EditorCameraAxis::s_aMatrix4Transforms[7] = 
    {
        FMath::FromTRS(FVector3(-2.5f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f, -90.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone X+
        FMath::FromTRS(FVector3( 2.5f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,  90.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone X-
        FMath::FromTRS(FVector3( 0.0f,  2.5f,  0.0f), FVector3(180.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Y+
        FMath::FromTRS(FVector3( 0.0f, -2.5f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Y-
        FMath::FromTRS(FVector3( 0.0f,  0.0f, -2.5f), FVector3( 90.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Z+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  2.5f), FVector3(-90.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //Cone Z-
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(1.0f, 1.0f, 1.0f)), //AABB
    };
    float EditorCameraAxis::s_fCameraDistance = 10;
    FVector3 EditorCameraAxis::s_vCameraPos = FVector3(0, 0, - EditorCameraAxis::s_fCameraDistance);
    FVector3 EditorCameraAxis::s_vCameraLookTarget = FVector3(0, 0, 0);
    FVector3 EditorCameraAxis::s_vCameraUp = FVector3(0, 1, 0);
    float EditorCameraAxis::s_fCameraFOV = 45.0f;
    float EditorCameraAxis::s_fCameraAspectRatio = 1.0f;
    float EditorCameraAxis::s_fCameraZNear = 0.01f;
    float EditorCameraAxis::s_fCameraZFar = 10000.0f;
    EditorCameraAxis::EditorCameraAxis()
        : EditorBase("EditorCameraAxis")

        //CameraAxis
        , pCamera(nullptr)
        , poColorBackground(0.0f, 0.0f, 0.0f, 0.0f)

        , poBuffers_PassCB(VK_NULL_HANDLE)
        , poBuffersMemory_PassCB(VK_NULL_HANDLE)
        , poBuffers_ObjectCB(VK_NULL_HANDLE)
        , poBuffersMemory_ObjectCB(VK_NULL_HANDLE)
        , isNeedUpdate(true)

        //Quad Blit
        , nameDescriptorSetLayout_CopyBlit("")
        , poDescriptorSetLayout_CopyBlit(VK_NULL_HANDLE)
        , poPipelineLayout_CopyBlit(VK_NULL_HANDLE)
        , pPipelineGraphics_CopyBlit(nullptr)
        , poBuffers_CopyBlitObjectCB(VK_NULL_HANDLE)
        , poBuffersMemory_CopyBlitObjectCB(VK_NULL_HANDLE)
    {

    }
    EditorCameraAxis::~EditorCameraAxis()
    {
        Destroy();
    }
    void EditorCameraAxis::Destroy()
    {
        F_DELETE(this->pCamera)
        destroyBufferUniforms();
        destroyShaders();
        destroyMeshes();
    }
    void EditorCameraAxis::Init()
    {
        EditorBase::Init();
    }
    void EditorCameraAxis::UpdateCBs()
    {
        //Pass
        {
            FVector3 vDir = Base::GetWindowPtr()->pCamera->GetDir();
            FVector3 vPos = -vDir * s_fCameraDistance;
            this->pCamera->LookAtLH(vPos, s_vCameraLookTarget, s_vCameraUp);
            this->pCamera->UpdateViewMatrix();
            Base::GetWindowPtr()->updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(PassConstants), &this->passCB, this->poBuffersMemory_PassCB);
        }
        
        if (!IsNeedUpdate())
            return;
        SetIsNeedUpdate(false);

        //CameraAxis
        {
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size(), &this->cameraAxisObjectCBs[0], this->poBuffersMemory_ObjectCB);
        }
        //Quad Blit
        {
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(CopyBlitObjectConstants), &this->copyBlitObjectCB, this->poBuffersMemory_CopyBlitObjectCB);
        }
    }
    void EditorCameraAxis::Draw(VkCommandBuffer& commandBuffer)
    {
        uint32_t instanceStart = 0;
        size_t count_mesh = s_nMeshCameraAxisCount;
        for (size_t i = 0; i < count_mesh; i++)
        {
            Mesh* pMesh = this->aMeshes[i];
            size_t count_mesh_sub = pMesh->aMeshSubs.size();
            for (size_t j = 0; j < count_mesh_sub; j++)
            {
                MeshSub* pMeshSub = pMesh->aMeshSubs[j];

                VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
                VkDeviceSize offsets[] = { 0 };
                Base::GetWindowPtr()->bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
                Base::GetWindowPtr()->bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
                if (Base::GetWindowPtr()->cfg_isWireFrame)
                    Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline_WireFrame);
                else
                    Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline);
                Base::GetWindowPtr()->bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipelineLayout, 0, 1, &this->pPipelineGraphics->poDescriptorSets[Base::GetWindowPtr()->poSwapChainImageIndex], 0, nullptr);
                Base::GetWindowPtr()->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, instanceStart);
                instanceStart += pMeshSub->instanceCount;
            }
        }
    }   
    void EditorCameraAxis::DrawQuad(VkCommandBuffer& commandBuffer)
    {
        Mesh* pMesh = this->aMeshes[s_nMeshQuadIndex];
        MeshSub* pMeshSub = pMesh->aMeshSubs[0];
        VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        Base::GetWindowPtr()->bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        Base::GetWindowPtr()->bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        if (Base::GetWindowPtr()->cfg_isWireFrame)
            Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics_CopyBlit->poPipeline_WireFrame);
        else
            Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics_CopyBlit->poPipeline);
        Base::GetWindowPtr()->bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics_CopyBlit->poPipelineLayout, 0, 1, &this->pPipelineGraphics_CopyBlit->poDescriptorSets[Base::GetWindowPtr()->poSwapChainImageIndex], 0, nullptr);
        Base::GetWindowPtr()->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, 0);
    }
    void EditorCameraAxis::initConfigs()
    {
        //1> Mesh
        {
            //0: Cone
            MeshInfo* pMICone = new MeshInfo("EditorCameraAxis_Cone",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityCone,
                                             new FMeshCreateParam_EntityCone(0.5f, 2.0f, 0.0f, 16, 1, false, false),
                                             false,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMICone);
            //1: AABB
            MeshInfo* pMIAABB = new MeshInfo("EditorCameraAxis_AABB",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityAABB,
                                             nullptr,
                                             false,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMIAABB);
            //2: Quad
            MeshInfo* pMIQuad = new MeshInfo("EditorCameraAxis_Quad",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityQuad,
                                             nullptr,
                                             true,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMIQuad);
        }
        //2> Shader
        {
            //CameraAxis
            {
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_CameraAxis_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = "Assets/Shader/editor_camera_axis.vert.spv";
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_CameraAxis_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = "Assets/Shader/editor_camera_axis.frag.spv";
                this->aShaderModuleInfos.push_back(siFrag);
            }
            //Quad Blit
            {
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_QuadBlit_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = "Assets/Shader/standard_copy_blit.vert.spv";
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_QuadBlit_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = "Assets/Shader/standard_copy_blit.frag.spv";
                this->aShaderModuleInfos.push_back(siFrag);
            }
        }
        //3> BufferUniform
        {
            
        }
        //4> DescriptorSetLayout
        {
            //CameraAxis
            {
                this->nameDescriptorSetLayout = "Pass-ObjectCameraAxis";
                this->aNameDescriptorSetLayouts = FUtilString::Split(this->nameDescriptorSetLayout, "-");
            }
            //Quad Blit
            {
                this->nameDescriptorSetLayout_CopyBlit = "ObjectCopyBlit-TextureFrameColor";
                this->aNameDescriptorSetLayouts_CopyBlit = FUtilString::Split(this->nameDescriptorSetLayout_CopyBlit, "-");
            }
        }
        //5> Camera/Viewport
        {
            initCamera();
            initViewport();
        }
    }
        void EditorCameraAxis::initCamera()
        {
            this->pCamera = new FCamera();
            this->pCamera->LookAtLH(s_vCameraPos, s_vCameraLookTarget, s_vCameraUp);
            this->pCamera->PerspectiveLH(s_fCameraFOV, s_fCameraAspectRatio, s_fCameraZNear, s_fCameraZFar);
            this->pCamera->UpdateViewMatrix();
        }
        void EditorCameraAxis::initViewport()
        {
            Base::GetWindowPtr()->createViewport(s_fBlitAreaWidth,
                                                 s_fBlitAreaHeight,
                                                 this->poViewport,
                                                 this->poScissor);
            this->poOffset.x = 0.0f;
            this->poOffset.y = 0.0f;
            this->poExtent.width = s_fBlitAreaWidth;
            this->poExtent.height = s_fBlitAreaHeight;
        }
    void EditorCameraAxis::initBufferUniforms()
    {
        //CameraAxis
        {
            //Pass
            Base::GetWindowPtr()->updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
            Base::GetWindowPtr()->createVkBuffer(sizeof(PassConstants), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_PassCB, this->poBuffersMemory_PassCB);

            this->cameraAxisObjectCBs.clear();
            int indexConst = 0;
            //Axis Cone
            {
                //X
                {
                    //+
                    CameraAxisObjectConstants xConsts0;
                    xConsts0.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    xConsts0.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(xConsts0);
                    indexConst++;
                    //-
                    CameraAxisObjectConstants xConsts1;
                    xConsts1.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    xConsts1.color = FColor(1.0f, 0.0f, 0.0f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(xConsts1);
                    indexConst++;
                }
                //Y
                {
                    //+
                    CameraAxisObjectConstants yConsts0;
                    yConsts0.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    yConsts0.color = FColor(0.0f, 1.0f, 0.0f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(yConsts0);
                    indexConst++;
                    //-
                    CameraAxisObjectConstants yConsts1;
                    yConsts1.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    yConsts1.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(yConsts1);
                    indexConst++;
                }
                //Z
                {
                    //+
                    CameraAxisObjectConstants zConsts0;
                    zConsts0.g_MatWorld = s_aMatrix4Transforms[indexConst];
                    zConsts0.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(zConsts0);
                    indexConst++;
                    //-
                    CameraAxisObjectConstants zConsts1;
                    zConsts1.g_MatWorld = s_aMatrix4Transforms[indexConst]; 
                    zConsts1.color = FColor(0.0f, 0.0f, 1.0f, 1.0f);
                    this->cameraAxisObjectCBs.push_back(zConsts1);
                    indexConst++;
                }
            }
            //Axis AABB
            {
                CameraAxisObjectConstants constsAABB;
                constsAABB.g_MatWorld = s_aMatrix4Transforms[indexConst];
                constsAABB.color = FColor(0.5f, 0.5f, 0.5f, 1.0f);
                this->cameraAxisObjectCBs.push_back(constsAABB);
                indexConst++;
            }
            VkDeviceSize bufferSize = sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size();
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB, this->poBuffersMemory_ObjectCB);

            Mesh* pMesh = this->aMeshes[s_nMeshConeIndex]; //Cone
            MeshSub* pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 6;
            pMesh = this->aMeshes[s_nMeshAABBIndex]; //AABB
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
        }
        //Quad Blit
        {
            float width = (float)Base::GetWindowPtr()->poSwapChainExtent.width;
            float height = (float)Base::GetWindowPtr()->poSwapChainExtent.height;
            this->copyBlitObjectCB.offsetX = 1.0f - s_fBlitAreaWidth / width;
            this->copyBlitObjectCB.offsetY = 1.0f - s_fBlitAreaHeight / height;
            this->copyBlitObjectCB.scaleX = 2.0f * s_fBlitAreaWidth / width;
            this->copyBlitObjectCB.scaleY = 2.0f * s_fBlitAreaHeight / height;
            VkDeviceSize bufferSize = sizeof(CopyBlitObjectConstants);
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_CopyBlitObjectCB, this->poBuffersMemory_CopyBlitObjectCB);
        }
        SetIsNeedUpdate(true);
    }
    void EditorCameraAxis::initDescriptorSetLayout()
    {
        EditorBase::initDescriptorSetLayout();
        //Quad Blit
        {   
            this->poDescriptorSetLayout_CopyBlit = Base::GetWindowPtr()->CreateDescriptorSetLayout(this->nameDescriptorSetLayout_CopyBlit, &this->aNameDescriptorSetLayouts_CopyBlit);
            if (this->poDescriptorSetLayout_CopyBlit == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorCameraAxis::initDescriptorSetLayout: Can not create VkDescriptorSetLayout by name: " + this->nameDescriptorSetLayout_CopyBlit;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }   
    }
    void EditorCameraAxis::initPipelineLayout()
    {
        EditorBase::initPipelineLayout();
        //Quad Blit
        {   
            VkDescriptorSetLayoutVector aDescriptorSetLayout;
            aDescriptorSetLayout.push_back(this->poDescriptorSetLayout_CopyBlit);
            this->poPipelineLayout_CopyBlit = Base::GetWindowPtr()->createVkPipelineLayout(aDescriptorSetLayout);
            if (this->poPipelineLayout_CopyBlit == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorCameraAxis::initPipelineLayout: Can not create VkPipelineLayout by descriptorSetLayout name: " + this->nameDescriptorSetLayout_CopyBlit;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }   
    }
    void EditorCameraAxis::initPipelineGraphics()
    {
        //VKPipelineGraphics
        {
            //CameraAxis
            {
                this->pPipelineGraphics = new VKPipelineGraphics("PipelineG-EditorCameraAxis");
                this->pPipelineGraphics->nameDescriptorSetLayout = this->nameDescriptorSetLayout;
                this->pPipelineGraphics->poDescriptorSetLayoutNames = &this->aNameDescriptorSetLayouts;
                //1> DescriptorSetLayout / PipelineLayout
                this->pPipelineGraphics->poDescriptorSetLayout = this->poDescriptorSetLayout;
                this->pPipelineGraphics->poPipelineLayout = this->poPipelineLayout;
                //2> DescriptorSets
                Base::GetWindowPtr()->createVkDescriptorSets(this->pPipelineGraphics->poDescriptorSetLayout, this->pPipelineGraphics->poDescriptorSets);
                //3> VKMultiRenderPass
                this->pPipelineGraphics->pRenderPass = new VKMultiRenderPass("rp_editor_camera_axis", false, false);
                this->pPipelineGraphics->pRenderPass->Init(s_fBlitAreaWidth, s_fBlitAreaHeight);
            }
            //Quad Blit
            {
                this->pPipelineGraphics_CopyBlit = new VKPipelineGraphics("PipelineG-QuadBlit");
                this->pPipelineGraphics_CopyBlit->nameDescriptorSetLayout = this->nameDescriptorSetLayout_CopyBlit;
                this->pPipelineGraphics_CopyBlit->poDescriptorSetLayoutNames = &this->aNameDescriptorSetLayouts_CopyBlit;
                //1> DescriptorSetLayout / PipelineLayout
                this->pPipelineGraphics_CopyBlit->poDescriptorSetLayout = this->poDescriptorSetLayout_CopyBlit;
                this->pPipelineGraphics_CopyBlit->poPipelineLayout = this->poPipelineLayout_CopyBlit;
                //2> DescriptorSets
                Base::GetWindowPtr()->createVkDescriptorSets(this->pPipelineGraphics_CopyBlit->poDescriptorSetLayout, this->pPipelineGraphics_CopyBlit->poDescriptorSets);
            }
            updateDescriptorSets_Graphics();
        }
        
        //Pipeline
        {
            VkDynamicStateVector aDynamicStates =
            {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
            };

            VkStencilOpState stencilOpFront; 
            VkStencilOpState stencilOpBack;

            //CameraAxis
            {
                VkViewportVector aViewports;
                aViewports.push_back(this->poViewport);
                VkRect2DVector aScissors;
                aScissors.push_back(this->poScissor);
                

                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
                if (!Base::GetWindowPtr()->CreatePipelineShaderStageCreateInfos(s_strNameShader_CameraAxis_Vert,
                                                                                "",
                                                                                "",
                                                                                "",
                                                                                s_strNameShader_CameraAxis_Frag,
                                                                                this->mapShaderModules,
                                                                                aShaderStageCreateInfos_Graphics))
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                //pPipelineGraphics->poPipeline
                this->pPipelineGraphics->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                     false, 0, 3,
                                                                                                     Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                     Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                     this->pPipelineGraphics->pRenderPass->poRenderPass, this->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                                                     VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                                     VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                                     VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                     VK_FALSE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                     VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                     VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
                if (this->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics for [EditorCameraAxis] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis] Create pipeline graphics success !");

                //pPipelineGraphics->poPipeline_WireFrame
                this->pPipelineGraphics->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                               false, 0, 3,
                                                                                                               Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                               Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                               this->pPipelineGraphics->pRenderPass->poRenderPass, this->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                                                               VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                                               VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                                               VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                               VK_FALSE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                               VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                               VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
                if (this->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics wire frame for [EditorCameraAxis] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis] Create pipeline graphics wire frame success !");
            }
            //Quad Blit
            {
                VkViewportVector aViewports;
                aViewports.push_back(Base::GetWindowPtr()->poViewport);
                VkRect2DVector aScissors;
                aScissors.push_back(Base::GetWindowPtr()->poScissor);

                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
                if (!Base::GetWindowPtr()->CreatePipelineShaderStageCreateInfos(s_strNameShader_QuadBlit_Vert,
                                                                                "",
                                                                                "",
                                                                                "",
                                                                                s_strNameShader_QuadBlit_Frag,
                                                                                this->mapShaderModules,
                                                                                aShaderStageCreateInfos_Graphics))
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                //pPipelineGraphics_CopyBlit->poPipeline
                this->pPipelineGraphics_CopyBlit->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                              false, 0, 3,
                                                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                              Base::GetWindowPtr()->poRenderPass, this->pPipelineGraphics_CopyBlit->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                                                              VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                                              VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                                              VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                              VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                              VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                              VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
                if (this->pPipelineGraphics_CopyBlit->poPipeline == VK_NULL_HANDLE)
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics for [EditorCameraAxis_CopyBlit] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis_CopyBlit] Create pipeline graphics success !");

                //pPipelineGraphics_CopyBlit->poPipeline_WireFrame
                this->pPipelineGraphics_CopyBlit->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                                        false, 0, 3,
                                                                                                                        Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                                        Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                                        Base::GetWindowPtr()->poRenderPass, this->pPipelineGraphics_CopyBlit->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                                                                        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                                                        VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                                                        VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                                        VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                                        VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                                        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
                if (this->pPipelineGraphics_CopyBlit->poPipeline_WireFrame == VK_NULL_HANDLE)
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics wire frame for [EditorCameraAxis_CopyBlit] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis_CopyBlit] Create pipeline graphics wire frame success !");
            }
        }
    }
    void EditorCameraAxis::updateDescriptorSets_Graphics()
    {
        //CameraAxis
        {
            StringVector* pDescriptorSetLayoutNames = this->pPipelineGraphics->poDescriptorSetLayoutNames;
            F_Assert(pDescriptorSetLayoutNames != nullptr && "EditorCameraAxis::updateDescriptorSets_Graphics")
            size_t count_ds = this->pPipelineGraphics->poDescriptorSets.size();
            for (size_t i = 0; i < count_ds; i++)
            {
                VkWriteDescriptorSetVector descriptorWrites;

                size_t count_names = pDescriptorSetLayoutNames->size();
                for (size_t j = 0; j < count_names; j++)
                {
                    String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[j];
                    if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                    {
                        VkDescriptorBufferInfo bufferInfo_Pass = {};
                        bufferInfo_Pass.buffer = this->poBuffers_PassCB;
                        bufferInfo_Pass.offset = 0;
                        bufferInfo_Pass.range = sizeof(PassConstants);
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          this->pPipelineGraphics->poDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_Pass);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCameraAxis)) //ObjectCameraAxis
                    {
                        VkDescriptorBufferInfo bufferInfo_ObjectCameraAxis = {};
                        bufferInfo_ObjectCameraAxis.buffer = this->poBuffers_ObjectCB;
                        bufferInfo_ObjectCameraAxis.offset = 0;
                        bufferInfo_ObjectCameraAxis.range = sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          this->pPipelineGraphics->poDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_ObjectCameraAxis);
                    }
                    else
                    {
                        String msg = "*********************** EditorCameraAxis::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                }
                Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
            }
        }
        //Quad Blit
        {
            StringVector* pDescriptorSetLayoutNames = this->pPipelineGraphics_CopyBlit->poDescriptorSetLayoutNames;
            F_Assert(pDescriptorSetLayoutNames != nullptr && "EditorCameraAxis::updateDescriptorSets_Graphics")
            size_t count_ds = this->pPipelineGraphics_CopyBlit->poDescriptorSets.size();
            for (size_t i = 0; i < count_ds; i++)
            {
                VkWriteDescriptorSetVector descriptorWrites;

                size_t count_names = pDescriptorSetLayoutNames->size();
                for (size_t j = 0; j < count_names; j++)
                {
                    String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[j];
                    if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCopyBlit)) //ObjectCopyBlit
                    {
                        VkDescriptorBufferInfo bufferInfo_ObjectCopyBlit = {};
                        bufferInfo_ObjectCopyBlit.buffer = this->poBuffers_CopyBlitObjectCB;
                        bufferInfo_ObjectCopyBlit.offset = 0;
                        bufferInfo_ObjectCopyBlit.range = sizeof(CopyBlitObjectConstants);
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          this->pPipelineGraphics_CopyBlit->poDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_ObjectCopyBlit);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFrameColor)) //TextureFrameColor
                    {
                        Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                        this->pPipelineGraphics_CopyBlit->poDescriptorSets[i],
                                                                        j,
                                                                        0,
                                                                        1,
                                                                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                        this->pPipelineGraphics->pRenderPass->imageInfo);
                    }
                    else
                    {
                        String msg = "*********************** EditorCameraAxis::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                }
                Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
            }
        }
    }
    void EditorCameraAxis::destroyBufferUniforms()
    {
        //PassConstants
        {
            if (this->poBuffers_PassCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffers_PassCB, this->poBuffersMemory_PassCB);
            }
            this->poBuffers_PassCB = VK_NULL_HANDLE;
            this->poBuffersMemory_PassCB = VK_NULL_HANDLE;
        }
        //CameraAxis
        {
            if (this->poBuffers_ObjectCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffers_ObjectCB, this->poBuffersMemory_ObjectCB);
            }
            this->poBuffers_ObjectCB = VK_NULL_HANDLE;
            this->poBuffersMemory_ObjectCB = VK_NULL_HANDLE;
            this->cameraAxisObjectCBs.clear();
        }
        //Quad Blit
        {
            if (this->poBuffers_CopyBlitObjectCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffers_CopyBlitObjectCB, this->poBuffersMemory_CopyBlitObjectCB);
            }
            this->poBuffers_CopyBlitObjectCB = VK_NULL_HANDLE;
            this->poBuffersMemory_CopyBlitObjectCB = VK_NULL_HANDLE;
        }
    }
    void EditorCameraAxis::destroyPipelineGraphics()
    {
        if (this->pPipelineGraphics)
        {
            F_DELETE(this->pPipelineGraphics->pRenderPass)
        }
        EditorBase::destroyPipelineGraphics();

        //Quad Blit
        {   
            F_DELETE(this->pPipelineGraphics_CopyBlit)
        }   
    }
    void EditorCameraAxis::destroyPipelineLayout()
    {
        EditorBase::destroyPipelineLayout();
        //Quad Blit
        {   
            if (this->poPipelineLayout_CopyBlit != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipelineLayout(this->poPipelineLayout_CopyBlit);
            }
            this->poPipelineLayout_CopyBlit = VK_NULL_HANDLE;
        }   
    }
    void EditorCameraAxis::destroyDescriptorSetLayout()
    {
        EditorBase::destroyDescriptorSetLayout();
        //Quad Blit
        {   
            if (this->poDescriptorSetLayout_CopyBlit != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkDescriptorSetLayout(this->poDescriptorSetLayout_CopyBlit);
            }
            this->poDescriptorSetLayout_CopyBlit = VK_NULL_HANDLE;
        } 
    }
    void EditorCameraAxis::CleanupSwapChain()
    {
        EditorBase::CleanupSwapChain();

    }
    void EditorCameraAxis::RecreateSwapChain()
    {
        EditorBase::RecreateSwapChain();

    }

}; //LostPeterVulkan