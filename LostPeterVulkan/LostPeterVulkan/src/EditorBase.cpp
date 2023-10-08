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
#include "../include/PipelineGraphics.h"

namespace LostPeter
{
    EditorBase::EditorBase(const String& nameEditor)
        : Base(nameEditor)

        //DescriptorSetLayouts
        , nameDescriptorSetLayout("")
        , poDescriptorSetLayout(VK_NULL_HANDLE)

        //PipelineLayout
        , poPipelineLayout(VK_NULL_HANDLE)

        //PipelineGraphics
        , pPipelineGraphics(nullptr)
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
        Base::GetWindowPtr()->CreateMeshes(this->aMeshInfos, this->aMeshes, this->mapMeshes);
    }
    void EditorBase::initShaders()
    {   
        Base::GetWindowPtr()->CreateShaderModules(this->aShaderModuleInfos, this->aShaderModules, this->mapShaderModules);
    }
    void EditorBase::initDescriptorSetLayout()
    {
        this->poDescriptorSetLayout = Base::GetWindowPtr()->CreateDescriptorSetLayout(this->nameDescriptorSetLayout, &this->aNameDescriptorSetLayouts);
        if (this->poDescriptorSetLayout == VK_NULL_HANDLE)
        {
            String msg = "*********************** EditorBase::initDescriptorSetLayout: Can not create VkDescriptorSetLayout by name: " + this->nameDescriptorSetLayout;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
    void EditorBase::initPipelineLayout()
    {
        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
        this->poPipelineLayout = Base::GetWindowPtr()->createVkPipelineLayout(aDescriptorSetLayout);
        if (this->poPipelineLayout == VK_NULL_HANDLE)
        {
            String msg = "*********************** EditorBase::initPipelineLayout: Can not create VkPipelineLayout by descriptorSetLayout name: " + this->nameDescriptorSetLayout;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
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
        size_t count = this->aShaderModules.size();
        for (size_t i = 0; i < count; i++)
        {
            VkShaderModule& vkShaderModule= this->aShaderModules[i];
            Base::GetWindowPtr()->destroyVkShaderModule(vkShaderModule);
        }
        this->aShaderModules.clear();
        this->mapShaderModules.clear();
    }
    void EditorBase::destroyPipelineGraphics()
    {
        F_DELETE(this->pPipelineGraphics)
    }
    void EditorBase::destroyPipelineLayout()
    {
        if (this->poPipelineLayout != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipelineLayout(this->poPipelineLayout);
        }
        this->poPipelineLayout = VK_NULL_HANDLE;
    }
    void EditorBase::destroyDescriptorSetLayout()
    {
        if (this->poDescriptorSetLayout != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkDescriptorSetLayout(this->poDescriptorSetLayout);
        }
        this->poDescriptorSetLayout = VK_NULL_HANDLE;
    }   

    void EditorBase::Draw(VkCommandBuffer& commandBuffer)
    {
        size_t count_mesh = this->aMeshes.size();
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
                Base::GetWindowPtr()->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, 0);
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

}; //LostPeter