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

#include "../include/EditorBase.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/VKStatePipelineGraphics.h"
#include "../include/VKBufferVertexIndex.h"
#include "../include/VKShader.h"

namespace LostPeterVulkan
{
    EditorBase::EditorBase(const String& nameEditor)
        : Base(nameEditor)

        //DescriptorSetLayouts
        , pDescriptorSetLayout(nullptr)
		, nameDescriptorSetLayout("")

		//Pipeline Graphics
		, poStatePipelineGraphics(nullptr)
    {

    }
    EditorBase::~EditorBase()
    {
       
    }
    void EditorBase::Init()
    {
        //0> initConfigs
        initConfigs();

        //1> initMeshes
        initMeshes();

        //2> initShader
        initShaders();

        //3> initBufferUniforms
        initBufferUniforms();

        //4> initDescriptorSetLayout
        initDescriptorSetLayout();

        //5> initPipelineLayout
        initPipelineLayout();

        //6> initPipelineGraphics
        initPipelineGraphics();
    }
    void EditorBase::initMeshes()
    {
        if (this->aMeshInfos.size() <= 0)
            return;
        Base::GetWindowPtr()->CreateMeshes(this->aMeshInfos, this->aMeshes, this->mapMeshes);
    }
    void EditorBase::initShaders()
    {   
        if (this->aShaderModuleInfos.size() <= 0)
            return;
        Base::GetWindowPtr()->CreateShaders(this->aShaderModuleInfos, this->aShaders, this->mapShaders);
    }
    void EditorBase::initDescriptorSetLayout()
    {
        this->pDescriptorSetLayout = new DescriptorSetLayout();
		this->pDescriptorSetLayout->Init(this->nameDescriptorSetLayout, true, false, false);
    }
    void EditorBase::initPipelineLayout()
    {
		
    }
    void EditorBase::destroyMeshes()
    {
        size_t count = this->aMeshes.size();
        for (size_t i = 0; i < count; i++)
        {
            Mesh* pMesh = this->aMeshes[i];
            F_DELETE(pMesh)
        }
        this->aMeshes.clear();
        this->mapMeshes.clear();

        count = this->aMeshInfos.size();
        for (size_t i = 0; i < count; i++)
        {
            MeshInfo* pMI = this->aMeshInfos[i];
            F_DELETE(pMI)
        }
        this->aMeshInfos.clear();
    }
    void EditorBase::destroyShaders()
    {
        size_t count = this->aShaders.size();
        for (size_t i = 0; i < count; i++)
        {
            VKShader* pShader = this->aShaders[i];
			delete pShader;
        }
        this->aShaders.clear();
        this->mapShaders.clear();
    }
    void EditorBase::destroyPipelineGraphics()
    {
        F_DELETE(this->poStatePipelineGraphics)
    }
    void EditorBase::destroyPipelineLayout()
    {
		
    }
    void EditorBase::destroyDescriptorSetLayout()
    {
		F_DELETE(this->pDescriptorSetLayout)
    }   

    void EditorBase::Draw(VkCommandBuffer& commandBuffer)
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        size_t count_mesh = this->aMeshes.size();
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

                pWindow->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, 0);

				poStatePipelineGraphics->UnBindState(commandBuffer);
            }
        }
    }   

    void EditorBase::CleanupSwapChain()
    {
        destroyPipelineGraphics();
        destroyPipelineLayout();
        destroyDescriptorSetLayout();
        destroyShaders();
        destroyBufferUniforms();
    }
    void EditorBase::RecreateSwapChain()
    {
        initShaders();
        initBufferUniforms();
        initDescriptorSetLayout();
        initPipelineLayout();
        initPipelineGraphics();
    }

}; //LostPeterVulkan