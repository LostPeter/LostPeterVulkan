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
#include "../include/VKStatePipelineGraphics.h"
#include "../include/VKMultiRenderPass.h"
#include "../include/VKBufferUniform.h"
#include "../include/VKBufferVertexIndex.h"

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

		, poBufferUniform_PassCB(nullptr)
		, poBufferUniform_ObjectCB(nullptr)
        , isNeedUpdate(true)

        //Quad Blit
		, pRenderPass(nullptr)
		, pDescriptorSetLayout_CopyBlit(nullptr)
        , nameDescriptorSetLayout_CopyBlit("")
		, poStatePipelineGraphics_CopyBlit(nullptr)
        , poBufferUniform_CopyBlitObjectCB(nullptr)
    {

    }
    EditorCameraAxis::~EditorCameraAxis()
    {
        Destroy();
    }
    void EditorCameraAxis::Destroy()
    {
        F_DELETE(this->pCamera)
        CleanupSwapChain();
        destroyMeshes();
    }
    void EditorCameraAxis::Init()
    {
        EditorBase::Init();
    }
    void EditorCameraAxis::UpdateCBs()
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        //Pass
        {
            FVector3 vDir = pWindow->pCamera->GetDir();
            FVector3 vPos = -vDir * s_fCameraDistance;
            this->pCamera->LookAtLH(vPos, s_vCameraLookTarget, s_vCameraUp);
            this->pCamera->UpdateViewMatrix();
            pWindow->updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
			this->poBufferUniform_PassCB->UpdateBuffer(0,
													   sizeof(PassConstants), 
													   (uint8*)&this->passCB);
        }
        
        if (!IsNeedUpdate())
            return;
        SetIsNeedUpdate(false);

        //CameraAxis
        {
			this->poBufferUniform_ObjectCB->UpdateBuffer(0,
														 sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size(),
														 (uint8*)(this->cameraAxisObjectCBs.data()));
        }
        //Quad Blit
        {
			this->poBufferUniform_CopyBlitObjectCB->UpdateBuffer(0,
																 sizeof(CopyBlitObjectConstants),
																 (uint8*)(&this->copyBlitObjectCB));
        }
    }
    void EditorCameraAxis::Draw(VkCommandBuffer& commandBuffer)
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        uint32_t instanceStart = 0;
        size_t count_mesh = s_nMeshCameraAxisCount;
        for (size_t i = 0; i < count_mesh; i++)
        {
            Mesh* pMesh = this->aMeshes[i];
            size_t count_mesh_sub = pMesh->aMeshSubs.size();
            for (size_t j = 0; j < count_mesh_sub; j++)
            {
                MeshSub* pMeshSub = pMesh->aMeshSubs[j];
				pMeshSub->pBufferVertexIndex->BindVertexIndexBuffer(commandBuffer);

				//State/Shader/BufferUniform/Texture
				poStatePipelineGraphics->BindState(commandBuffer, pWindow->cfg_isWireFrame);
				poStatePipelineGraphics->BindShader(commandBuffer);
				poStatePipelineGraphics->BindBufferUniforms(commandBuffer);
				poStatePipelineGraphics->BindTextures(commandBuffer);

                pWindow->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, instanceStart);
                instanceStart += pMeshSub->instanceCount;

				poStatePipelineGraphics->UnBindState(commandBuffer);
            }
        }
    }   
    void EditorCameraAxis::DrawQuad(VkCommandBuffer& commandBuffer)
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        Mesh* pMesh = this->aMeshes[s_nMeshQuadIndex];
        MeshSub* pMeshSub = pMesh->aMeshSubs[0];
		pMeshSub->pBufferVertexIndex->BindVertexIndexBuffer(commandBuffer);

		//State/Shader/BufferUniform/Texture
		poStatePipelineGraphics_CopyBlit->BindState(commandBuffer, pWindow->cfg_isWireFrame);
		poStatePipelineGraphics_CopyBlit->BindShader(commandBuffer);
		poStatePipelineGraphics_CopyBlit->BindBufferUniforms(commandBuffer);
		poStatePipelineGraphics_CopyBlit->BindTextures(commandBuffer);

        pWindow->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, 0);

		poStatePipelineGraphics_CopyBlit->UnBindState(commandBuffer);
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
                siVert.pathShader = "Assets/Shader/standard_copy_blit_to_frame.vert.spv";
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_QuadBlit_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = "Assets/Shader/standard_copy_blit_to_frame.frag.spv";
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
            }
            //Quad Blit
            {
                this->nameDescriptorSetLayout_CopyBlit = "ObjectCopyBlit-TextureFrameColor";
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
            Base::GetWindowPtr()->createViewport((uint32_t)s_fBlitAreaWidth,
                                                 (uint32_t)s_fBlitAreaHeight,
                                                 this->poViewport,
                                                 this->poScissor);
            this->poOffset.x = 0;
            this->poOffset.y = 0;
            this->poExtent.width = (uint32_t)s_fBlitAreaWidth;
            this->poExtent.height = (uint32_t)s_fBlitAreaHeight;
        }
    void EditorCameraAxis::initBufferUniforms()
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        //CameraAxis
        {
            //Pass
            pWindow->updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
			this->poBufferUniform_PassCB = pWindow->createBufferUniform("EditorCameraAxis-PassConstants", 
																		sizeof(PassConstants), 
																		(uint8*)(&this->passCB),
																		false);

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
			this->poBufferUniform_ObjectCB = pWindow->createBufferUniform("EditorCameraAxis-CameraAxisObjectConstants",
																		  sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size(), 
																		  (uint8*)(this->cameraAxisObjectCBs.data()),
																		  false);

            Mesh* pMesh = this->aMeshes[s_nMeshConeIndex]; //Cone
            MeshSub* pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 6;
            pMesh = this->aMeshes[s_nMeshAABBIndex]; //AABB
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
        }
        //Quad Blit
        {
            float width = (float)pWindow->poSwapChainExtent.width;
            float height = (float)pWindow->poSwapChainExtent.height;
            this->copyBlitObjectCB.offsetX = 1.0f - s_fBlitAreaWidth / width;
            this->copyBlitObjectCB.offsetY = 1.0f - s_fBlitAreaHeight / height;
            this->copyBlitObjectCB.scaleX = 2.0f * s_fBlitAreaWidth / width;
            this->copyBlitObjectCB.scaleY = 2.0f * s_fBlitAreaHeight / height;
			this->poBufferUniform_CopyBlitObjectCB = pWindow->createBufferUniform("EditorCameraAxis-CopyBlitObjectConstants",
																				  sizeof(CopyBlitObjectConstants),
																				  (uint8*)(&this->copyBlitObjectCB),
																				  false);
		}
        SetIsNeedUpdate(true);
    }
    void EditorCameraAxis::initDescriptorSetLayout()
    {
        EditorBase::initDescriptorSetLayout();
        //Quad Blit
        {   
            this->pDescriptorSetLayout_CopyBlit = new DescriptorSetLayout();
			this->pDescriptorSetLayout_CopyBlit->Init(this->nameDescriptorSetLayout_CopyBlit, true, false, false);
        }   
    }
    void EditorCameraAxis::initPipelineGraphics()
    {
		//1> MultiRenderPass
		this->pRenderPass = new VKMultiRenderPass("rp_editor_camera_axis", false, false);
        this->pRenderPass->Init((uint32_t)s_fBlitAreaWidth, (uint32_t)s_fBlitAreaHeight);
		
        //2> Pipeline
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
                                                                                this->mapShaders,
                                                                                aShaderStageCreateInfos_Graphics))
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                //poStatePipelineGraphics
				String namePipelineGraphics = "PipelineGraphics-" + GetName();
                this->poStatePipelineGraphics = Base::GetWindowPtr()->createStatePipelineGraphics(namePipelineGraphics,
																								  this->pDescriptorSetLayout,
                                                                                                  aShaderStageCreateInfos_Graphics,
																								  F_MeshVertex_Pos3Color4Tex2,
                                                                                                  false, 0, 3,
																								  this->pRenderPass->poRenderPass, aViewports, aScissors, aDynamicStates,
																								  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
																								  VK_TRUE, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
																								  VK_FALSE, stencilOpFront, stencilOpBack, 
																								  VK_FALSE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
																								  VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
																								  VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
                if (this->poStatePipelineGraphics == nullptr)
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics for [EditorCameraAxis] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis] Create pipeline graphics success !");
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
                                                                                this->mapShaders,
                                                                                aShaderStageCreateInfos_Graphics))
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                //poStatePipelineGraphics_CopyBlit
				String namePipelineGraphics_CopyBlit = "PipelineGraphics-CopyBlit-" + GetName();
                this->poStatePipelineGraphics_CopyBlit = Base::GetWindowPtr()->createStatePipelineGraphics(namePipelineGraphics_CopyBlit,
																										   this->pDescriptorSetLayout_CopyBlit,
                                                                                                           aShaderStageCreateInfos_Graphics,
																										   F_MeshVertex_Pos3Color4Tex2,
																										   false, 0, 3,
																										   Base::GetWindowPtr()->poRenderPass, aViewports, aScissors, aDynamicStates,
																										   VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
																										   VK_TRUE, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
																										   VK_FALSE, stencilOpFront, stencilOpBack, 
																										   VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
																										   VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
																										   VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
                if (this->poStatePipelineGraphics_CopyBlit == nullptr)
                {
                    String msg = "*********************** EditorCameraAxis::initPipelineGraphics: Failed to create pipeline graphics for [EditorCameraAxis_CopyBlit] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("EditorCameraAxis::initPipelineGraphics: [EditorCameraAxis_CopyBlit] Create pipeline graphics success !");

            }
        }

		//3> DescriptorSets
		updateDescriptorSets_Graphics();
    }
    void EditorCameraAxis::updateDescriptorSets_Graphics()
    {
        //CameraAxis
        {
            uint32_t count_ds = (uint32_t)this->poStatePipelineGraphics->poDescriptorSets.size();
            for (uint32_t i = 0; i < count_ds; i++)
            {
                VkWriteDescriptorSetVector descriptorWrites;

                uint32_t count_names = (uint32_t)this->poStatePipelineGraphics->pDescriptorSetLayout->aLayouts.size();
                for (uint32_t j = 0; j < count_names; j++)
                {
                    String& nameDescriptorSet = this->poStatePipelineGraphics->pDescriptorSetLayout->aLayouts[j];
                    if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                    {
                        VkDescriptorBufferInfo bufferInfo_Pass = {};
                        bufferInfo_Pass.buffer = this->poBufferUniform_PassCB->GetVkBuffer();
                        bufferInfo_Pass.offset = 0;
                        bufferInfo_Pass.range = sizeof(PassConstants);
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          this->poStatePipelineGraphics->poDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_Pass);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCameraAxis)) //ObjectCameraAxis
                    {
                        VkDescriptorBufferInfo bufferInfo_ObjectCameraAxis = {};
                        bufferInfo_ObjectCameraAxis.buffer = this->poBufferUniform_ObjectCB->GetVkBuffer();
                        bufferInfo_ObjectCameraAxis.offset = 0;
                        bufferInfo_ObjectCameraAxis.range = sizeof(CameraAxisObjectConstants) * this->cameraAxisObjectCBs.size();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          this->poStatePipelineGraphics->poDescriptorSets[i],
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
            uint32_t count_ds = (uint32_t)this->poStatePipelineGraphics_CopyBlit->poDescriptorSets.size();
            for (uint32_t i = 0; i < count_ds; i++)
            {
                VkWriteDescriptorSetVector descriptorWrites;

                uint32_t count_names = (uint32_t)this->poStatePipelineGraphics_CopyBlit->pDescriptorSetLayout->aLayouts.size();
                for (uint32_t j = 0; j < count_names; j++)
                {
                    String& nameDescriptorSet = this->poStatePipelineGraphics_CopyBlit->pDescriptorSetLayout->aLayouts[j];
                    if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCopyBlit)) //ObjectCopyBlit
                    {
                        VkDescriptorBufferInfo bufferInfo_ObjectCopyBlit = {};
                        bufferInfo_ObjectCopyBlit.buffer = this->poBufferUniform_CopyBlitObjectCB->GetVkBuffer();
                        bufferInfo_ObjectCopyBlit.offset = 0;
                        bufferInfo_ObjectCopyBlit.range = sizeof(CopyBlitObjectConstants);
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          this->poStatePipelineGraphics_CopyBlit->poDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_ObjectCopyBlit);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFrameColor)) //TextureFrameColor
                    {
                        Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                        this->poStatePipelineGraphics_CopyBlit->poDescriptorSets[i],
                                                                        j,
                                                                        0,
                                                                        1,
                                                                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                        this->pRenderPass->imageInfo);
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
			F_DELETE(this->poBufferUniform_PassCB)
        }
        //CameraAxis
        {
			F_DELETE(this->poBufferUniform_ObjectCB)
            this->cameraAxisObjectCBs.clear();
        }
        //Quad Blit
        {
			F_DELETE(this->poBufferUniform_CopyBlitObjectCB)
        }
    }
    void EditorCameraAxis::destroyPipelineGraphics()
    {
		F_DELETE(this->pRenderPass)

        EditorBase::destroyPipelineGraphics();

        //Quad Blit
        {   
            F_DELETE(this->poStatePipelineGraphics_CopyBlit)
        }   
    }
    void EditorCameraAxis::destroyDescriptorSetLayout()
    {
        EditorBase::destroyDescriptorSetLayout();
        //Quad Blit
        {   
			F_DELETE(this->pDescriptorSetLayout_CopyBlit)
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