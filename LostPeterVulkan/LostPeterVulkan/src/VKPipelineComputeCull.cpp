/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-08-25
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineComputeCull.h"
#include "../include/VKRenderPassCull.h"
#include "../include/VulkanWindow.h"
#include "../include/CullManager.h"
#include "../include/BufferCompute.h"
#include "../include/BufferIndirectCommand.h"

namespace LostPeterVulkan
{
    VKPipelineComputeCull::VKPipelineComputeCull(const String& namePipelineCompute, VKRenderPassCull* pVKRenderPassCull)
        : Base(namePipelineCompute)
        , m_pVKRenderPassCull(pVKRenderPassCull)
        , m_pCullManager(new CullManager())

        //PipelineCompute-CullClearArgs
        , nameDescriptorSetLayout_CullClearArgs("")
        , poDescriptorSetLayoutNames_CullClearArgs(nullptr)
        , poDescriptorSetLayout_CullClearArgs(VK_NULL_HANDLE)
        , poPipelineLayout_CullClearArgs(VK_NULL_HANDLE)
        , poPipeline_CullClearArgs(VK_NULL_HANDLE)
        , poDescriptorSet_CullClearArgs(VK_NULL_HANDLE)

        //PipelineCompute-CullFrustum
        , nameDescriptorSetLayout_CullFrustum("")
        , poDescriptorSetLayoutNames_CullFrustum(nullptr)
        , poDescriptorSetLayout_CullFrustum(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustum(VK_NULL_HANDLE)
        , poPipeline_CullFrustum(VK_NULL_HANDLE)
        , poDescriptorSet_CullFrustum(VK_NULL_HANDLE)

        //PipelineCompute-CullFrustumDepthHiz
        , nameDescriptorSetLayout_CullFrustumDepthHiz("")
        , poDescriptorSetLayoutNames_CullFrustumDepthHiz(nullptr)
        , poDescriptorSetLayout_CullFrustumDepthHiz(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustumDepthHiz(VK_NULL_HANDLE)
        , poPipeline_CullFrustumDepthHiz(VK_NULL_HANDLE)
        , poDescriptorSet_CullFrustumDepthHiz(VK_NULL_HANDLE)

        //PipelineCompute-CullFrustumDepthHizClip
        , nameDescriptorSetLayout_CullFrustumDepthHizClip("")
        , poDescriptorSetLayoutNames_CullFrustumDepthHizClip(nullptr)
        , poDescriptorSetLayout_CullFrustumDepthHizClip(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustumDepthHizClip(VK_NULL_HANDLE)
        , poPipeline_CullFrustumDepthHizClip(VK_NULL_HANDLE)
        , poDescriptorSet_CullFrustumDepthHizClip(VK_NULL_HANDLE)

        //PipelineCompute-HizDepthGenerate
        , nameDescriptorSetLayout_HizDepthGenerate("")
        , poDescriptorSetLayoutNames_HizDepthGenerate(nullptr)
        , poDescriptorSetLayout_HizDepthGenerate(VK_NULL_HANDLE)
        , poPipelineLayout_HizDepthGenerate(VK_NULL_HANDLE)
        , poPipeline_HizDepthGenerate(VK_NULL_HANDLE)
        , poDescriptorSet_HizDepthGenerate(VK_NULL_HANDLE)

        //CullConstants
        , poBuffer_CullCB(VK_NULL_HANDLE)
        , poBufferMemory_CullCB(VK_NULL_HANDLE)

        //Camera Param
        , mat4VPLast(FMath::Identity4x4())  

    {

    }

    VKPipelineComputeCull::~VKPipelineComputeCull()
    {
        Destroy();
    }

    void VKPipelineComputeCull::Destroy()
    {
        F_DELETE(m_pCullManager)
        CleanupSwapChain();
        destroyBufferCull();
    }
        void VKPipelineComputeCull::destroyBufferCull()
        {
            if (this->poBuffer_CullCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_CullCB, this->poBufferMemory_CullCB);
            }
            this->poBuffer_CullCB = VK_NULL_HANDLE;
            this->poBufferMemory_CullCB = VK_NULL_HANDLE;
        }
        void VKPipelineComputeCull::destroyVkComputePipeline(VkPipeline& poPipeline)
        {
            if (poPipeline != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(poPipeline);
            }       
        }

    bool VKPipelineComputeCull::Init()
    {
        //CullConstants
        if (this->poBuffer_CullCB == VK_NULL_HANDLE)
        {
            if (!createBufferCull())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferCull failed !");
                return false;
            }
        }
        //CullManager
        m_pCullManager->Init(this);
        UpdateMatrixVP();

        return true;
    }
    bool VKPipelineComputeCull::InitCullClearArgs(const String& descriptorSetLayout,
                                                  StringVector* pDescriptorSetLayoutNames,
                                                  const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                  const VkPipelineLayout& vkPipelineLayout,
                                                  const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullClearArgs = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullClearArgs = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullClearArgs = vkDescriptorSetLayout;
        this->poPipelineLayout_CullClearArgs = vkPipelineLayout;

        if (!createVkComputePipeline("PipelineCompute-CullClearArgs",
                                     descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullClearArgs,
                                     this->poDescriptorSet_CullClearArgs))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullClearArgs: createVkComputePipeline failed !");
            return false;
        }
        
        return true;
    }
    bool VKPipelineComputeCull::InitCullFrustum(const String& descriptorSetLayout,
                                                StringVector* pDescriptorSetLayoutNames,
                                                const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                const VkPipelineLayout& vkPipelineLayout,
                                                const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullFrustum = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullFrustum = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullFrustum = vkDescriptorSetLayout;
        this->poPipelineLayout_CullFrustum = vkPipelineLayout;

        if (!createVkComputePipeline("PipelineCompute-CullFrustum",
                                     descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullFrustum,
                                     this->poDescriptorSet_CullFrustum))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullFrustum: createVkComputePipeline failed !");
            return false;
        }

        return true;
    }
    bool VKPipelineComputeCull::InitCullFrustumDepthHiz(const String& descriptorSetLayout,
                                                        StringVector* pDescriptorSetLayoutNames,
                                                        const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                        const VkPipelineLayout& vkPipelineLayout,
                                                        const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullFrustumDepthHiz = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullFrustumDepthHiz = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullFrustumDepthHiz = vkDescriptorSetLayout;
        this->poPipelineLayout_CullFrustumDepthHiz = vkPipelineLayout;

        if (!createVkComputePipeline("PipelineCompute-CullFrustumDepthHiz",
                                     descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullFrustumDepthHiz,
                                     this->poDescriptorSet_CullFrustumDepthHiz))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullFrustumDepthHiz: createVkComputePipeline failed !");
            return false;
        }

        return true;
    }
    bool VKPipelineComputeCull::InitCullFrustumDepthHizClip(const String& descriptorSetLayout,
                                                            StringVector* pDescriptorSetLayoutNames,
                                                            const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                            const VkPipelineLayout& vkPipelineLayout,
                                                            const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullFrustumDepthHizClip = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullFrustumDepthHizClip = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullFrustumDepthHizClip = vkDescriptorSetLayout;
        this->poPipelineLayout_CullFrustumDepthHizClip = vkPipelineLayout;

        if (!createVkComputePipeline("PipelineCompute-CullFrustumDepthHizClip",
                                     descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullFrustumDepthHizClip,
                                     this->poDescriptorSet_CullFrustumDepthHizClip))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullFrustumDepthHizClip: createVkComputePipeline failed !");
            return false;
        }

        return true;
    }
    bool VKPipelineComputeCull::InitHizDepthGenerate(const String& descriptorSetLayout,
                                                     StringVector* pDescriptorSetLayoutNames,
                                                     const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                     const VkPipelineLayout& vkPipelineLayout,
                                                     const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_HizDepthGenerate = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_HizDepthGenerate = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_HizDepthGenerate = vkDescriptorSetLayout;
        this->poPipelineLayout_HizDepthGenerate = vkPipelineLayout;

        if (!createVkComputePipeline("PipelineCompute-HizDepthGenerate",
                                     descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_HizDepthGenerate,
                                     this->poDescriptorSet_HizDepthGenerate))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitHizDepthGenerate: createVkComputePipeline failed !");
            return false;
        }

        return true;
    }
        bool VKPipelineComputeCull::createBufferCull()
        {
            destroyBufferCull();
            this->cullCB.nMaxLodCount = (int)Vulkan_Lod_Count;
            this->cullCB.nMaxRenderCount = CullManager::s_nRenderCount_Max;
            this->cullCB.nMaxObjectCount = CullManager::s_nInstanceCount_Max;
            this->cullCB.nIsNoCulling = 0;

            VkDeviceSize bufferSize = sizeof(CullConstants);
            Base::GetWindowPtr()->createVkBuffer("CullConstants-" + this->name, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_CullCB, this->poBufferMemory_CullCB);
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(CullConstants), &this->cullCB, this->poBufferMemory_CullCB);
            return true;
        }
        bool VKPipelineComputeCull::createVkComputePipeline(const String& nameComputePipeline,
                                                            const String& descriptorSetLayout,
                                                            StringVector* pDescriptorSetLayoutNames,
                                                            const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                            const VkPipelineLayout& vkPipelineLayout,
                                                            const VkShaderModule& vkShaderModule,
                                                            VkPipeline& vkPipeline,
                                                            VkDescriptorSet& vkDescriptorSet)
        {
            //1> VkPipeline
            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
            shaderStageInfo.module = vkShaderModule;
            shaderStageInfo.pName = "main";
            vkPipeline = Base::GetWindowPtr()->createVkComputePipeline(nameComputePipeline, shaderStageInfo, vkPipelineLayout);
            if (vkPipeline == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VKPipelineComputeCull::createVkComputePipeline: createVkComputePipeline failed !");
                return false;
            }

            //2> VkDescriptorSet
            Base::GetWindowPtr()->createVkDescriptorSet(descriptorSetLayout, vkDescriptorSetLayout, vkDescriptorSet);
            if (vkDescriptorSet == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VKPipelineComputeCull::createVkComputePipeline: createVkDescriptorSet failed !");
                return false;
            }
            updateDescriptorSet(vkDescriptorSet, 
                                pDescriptorSetLayoutNames,
                                nullptr,
                                nullptr,
                                nullptr,
                                nullptr,
                                nullptr);

            return true;
        }

    void VKPipelineComputeCull::CleanupSwapChain()
    {
        //PipelineCompute-CullClearArgs
        this->poDescriptorSetLayoutNames_CullClearArgs = nullptr;
        this->poDescriptorSetLayout_CullClearArgs = VK_NULL_HANDLE;
        this->poPipelineLayout_CullClearArgs = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullClearArgs);  
        this->poPipeline_CullClearArgs = VK_NULL_HANDLE;
        this->poDescriptorSet_CullClearArgs = VK_NULL_HANDLE;

        //PipelineCompute-CullFrustum
        this->poDescriptorSetLayoutNames_CullFrustum = nullptr;
        this->poDescriptorSetLayout_CullFrustum = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustum = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustum);  
        this->poPipeline_CullFrustum = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustum = VK_NULL_HANDLE;

        //PipelineCompute-CullFrustumDepthHiz
        this->poDescriptorSetLayoutNames_CullFrustumDepthHiz = nullptr;
        this->poDescriptorSetLayout_CullFrustumDepthHiz = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustumDepthHiz = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustumDepthHiz);  
        this->poPipeline_CullFrustumDepthHiz = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustumDepthHiz = VK_NULL_HANDLE;

        //PipelineCompute-CullFrustumDepthHizClip
        this->poDescriptorSetLayoutNames_CullFrustumDepthHizClip = nullptr;
        this->poDescriptorSetLayout_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustumDepthHizClip);  
        this->poPipeline_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustumDepthHizClip = VK_NULL_HANDLE;

        //PipelineCompute-HizDepthGenerate
        this->poDescriptorSetLayoutNames_HizDepthGenerate = nullptr;
        this->poDescriptorSetLayout_HizDepthGenerate = VK_NULL_HANDLE;
        this->poPipelineLayout_HizDepthGenerate = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_HizDepthGenerate);  
        this->poPipeline_HizDepthGenerate = VK_NULL_HANDLE;
        this->poDescriptorSet_HizDepthGenerate = VK_NULL_HANDLE;

    }  

    void VKPipelineComputeCull::Dispatch_Cull(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pCullManager)
        {
            this->m_pCullManager->ExecuteHizCullTest(commandBuffer);
        }
    }
    void VKPipelineComputeCull::Dispatch_HizDepthGenerate(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pCullManager)
        {
            this->m_pCullManager->ExecuteHizDepthGenerate(commandBuffer);
        }
    }

    void VKPipelineComputeCull::UpdateMatrixVP()
    {
        FCamera* pCamera = Base::GetWindowPtr()->GetCamera();
        FMatrix4 matView = pCamera->GetMatrix4View();
        FMatrix4 matProj = pCamera->GetMatrix4Projection();
        this->mat4VPLast = matProj * matView;
    }
    void VKPipelineComputeCull::UpdateBuffer_Cull()
    {
        VulkanWindow* pVulkanWindow = Base::GetWindowPtr();
        FCamera* pCamera = pVulkanWindow->GetCamera();
        FVector3 vCameraPos = pCamera->GetPos();

        this->cullCB.mat4VPLast = this->mat4VPLast;
        pCamera->GetWorldFrustumCorners(this->aWorldFrustumCorners);
        FVector3 vMinFP = this->aWorldFrustumCorners[0];
        FVector3 vMaxFP = this->aWorldFrustumCorners[0];
        for (int i = 1; i < MAX_FRUSTUM_CORNER_COUNT; i++)
        {
            FMath::Min(vMinFP, this->aWorldFrustumCorners[i]);
            FMath::Max(vMaxFP, this->aWorldFrustumCorners[i]);
        }
        pCamera->GetWorldFrustumPlanes(this->aWorldFrustumPlanes);
        for (int i = 0; i < MAX_FRUSTUM_PLANE_COUNT; i++)
        {
            FPlane& plane = this->aWorldFrustumPlanes[i];
            this->cullCB.v4FrustumPlanes[i] = FVector4(-plane.m_vNormal.x, -plane.m_vNormal.y, -plane.m_vNormal.z, -plane.m_fDistance);
        }
        this->cullCB.v4ParamComon[0] = FVector4(vCameraPos.x, vCameraPos.y, vCameraPos.z, 0);
        this->cullCB.v4ParamComon[1] = FVector4(vMinFP.x, vMinFP.y, vMinFP.z, 0);
        this->cullCB.v4ParamComon[2] = FVector4(vMaxFP.x, vMaxFP.y, vMaxFP.z, 0);
        this->cullCB.v4ParamComon[3] = FVector4(m_pVKRenderPassCull->nHizDepthWidth, m_pVKRenderPassCull->nHizDepthHeight, m_pVKRenderPassCull->nHizDepthMinmapCount - 0.5f, m_pVKRenderPassCull->nHizDepthMinmapCount - 1.0f);

        this->cullCB.v4ParamRender = FVector4(0, 0, 0, 0);
        this->cullCB.v4PosPlayer = FVector4(vCameraPos.x, vCameraPos.y, vCameraPos.z, 0);

        pVulkanWindow->updateVKBuffer(0, sizeof(CullConstants), &this->cullCB, this->poBufferMemory_CullCB);
    }

    void VKPipelineComputeCull::UpdateDescriptorSet_CullClearArgs(BufferIndirectCommand* pCB_RenderArgs)
    {
        updateDescriptorSet(this->poDescriptorSet_CullClearArgs, 
                            this->poDescriptorSetLayoutNames_CullClearArgs,
                            nullptr,
                            pCB_RenderArgs,
                            nullptr,
                            nullptr,
                            nullptr);
    }
    void VKPipelineComputeCull::UpdateDescriptorSet_CullFrustum(BufferCompute* pCB_CullObjects,
                                                                BufferIndirectCommand* pCB_RenderArgs,
                                                                BufferCompute* pCB_LodArgs,
                                                                BufferCompute* pCB_Result)
    {
        updateDescriptorSet(this->poDescriptorSet_CullFrustum, 
                            this->poDescriptorSetLayoutNames_CullFrustum,
                            pCB_CullObjects,
                            pCB_RenderArgs,
                            pCB_LodArgs,
                            pCB_Result,
                            nullptr);
    }
    void VKPipelineComputeCull::UpdateDescriptorSet_CullFrustumDepthHiz(BufferCompute* pCB_CullObjects,
                                                                        BufferIndirectCommand* pCB_RenderArgs,
                                                                        BufferCompute* pCB_LodArgs,
                                                                        BufferCompute* pCB_Result)
    {
        updateDescriptorSet(this->poDescriptorSet_CullFrustumDepthHiz, 
                            this->poDescriptorSetLayoutNames_CullFrustumDepthHiz,
                            pCB_CullObjects,
                            pCB_RenderArgs,
                            pCB_LodArgs,
                            pCB_Result,
                            nullptr);
    }
    void VKPipelineComputeCull::UpdateDescriptorSet_CullFrustumDepthHizClip(BufferCompute* pCB_CullObjects,
                                                                            BufferIndirectCommand* pCB_RenderArgs,
                                                                            BufferCompute* pCB_LodArgs,
                                                                            BufferCompute* pCB_Result,
                                                                            BufferCompute* pCB_Clip)
    {
        updateDescriptorSet(this->poDescriptorSet_CullFrustumDepthHizClip, 
                            this->poDescriptorSetLayoutNames_CullFrustumDepthHizClip,
                            pCB_CullObjects,
                            pCB_RenderArgs,
                            pCB_LodArgs,
                            pCB_Result,
                            pCB_Clip);
    }
    void VKPipelineComputeCull::UpdateDescriptorSet_HizDepthGenerate()
    {
        updateDescriptorSet(this->poDescriptorSet_HizDepthGenerate, 
                            this->poDescriptorSetLayoutNames_HizDepthGenerate,
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr);
    }

    void VKPipelineComputeCull::updateDescriptorSet(VkDescriptorSet& descriptorSet,
                                                    StringVector* poDescriptorSetLayoutNames,
                                                    BufferCompute* pCB_CullObjects,
                                                    BufferIndirectCommand* pCB_RenderArgs,
                                                    BufferCompute* pCB_LodArgs,
                                                    BufferCompute* pCB_Result,
                                                    BufferCompute* pCB_Clip)
    {
        VkWriteDescriptorSetVector descriptorWrites;
        uint32_t count = (uint32_t)poDescriptorSetLayoutNames->size();
        for (uint32_t i = 0; i < count; i++)
        {
            const String& nameDescriptor = poDescriptorSetLayoutNames->at(i);

            if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Cull)) //Cull
            {
                VkDescriptorBufferInfo bufferInfo_Cull = {};
                bufferInfo_Cull.buffer = this->poBuffer_CullCB;
                bufferInfo_Cull.offset = 0;
                bufferInfo_Cull.range = sizeof(CullConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  descriptorSet,
                                                                  i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_Cull);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_HizDepth)) //HizDepth
            {
                VkDescriptorBufferInfo bufferInfo_HizDepth = {};
                bufferInfo_HizDepth.buffer = this->m_pVKRenderPassCull->poBuffer_HizDepthCB;
                bufferInfo_HizDepth.offset = 0;
                bufferInfo_HizDepth.range = sizeof(HizDepthConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  descriptorSet,
                                                                  i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_HizDepth);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCull)) //ObjectCull
            {
                if (pCB_CullObjects != nullptr)
                {
                    VkDescriptorBufferInfo bufferInfo_ObjectCull = {};
                    bufferInfo_ObjectCull.buffer = pCB_CullObjects->poBuffer_Compute;
                    bufferInfo_ObjectCull.offset = 0;
                    bufferInfo_ObjectCull.range = (VkDeviceSize)pCB_CullObjects->GetBufferSize();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                      descriptorSet,
                                                                      i,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_ObjectCull);
                }
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWArgsCB)) //BufferRWArgsCB
            {
                if (pCB_RenderArgs != nullptr)
                {
                    VkDescriptorBufferInfo bufferInfo_RenderArgs = {};
                    bufferInfo_RenderArgs.buffer = pCB_RenderArgs->poBuffer_IndirectCommand;
                    bufferInfo_RenderArgs.offset = 0;
                    bufferInfo_RenderArgs.range = (VkDeviceSize)pCB_RenderArgs->GetBufferSize();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                      descriptorSet,
                                                                      i,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_RenderArgs);
                }
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWLodCB)) //BufferRWLodCB
            {
                if (pCB_LodArgs != nullptr)
                {
                    VkDescriptorBufferInfo bufferInfo_LodArgs = {};
                    bufferInfo_LodArgs.buffer = pCB_LodArgs->poBuffer_Compute;
                    bufferInfo_LodArgs.offset = 0;
                    bufferInfo_LodArgs.range = (VkDeviceSize)pCB_LodArgs->GetBufferSize();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                      descriptorSet,
                                                                      i,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_LodArgs);
                }
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWResultCB)) //BufferRWResultCB
            {
                if (pCB_Result != nullptr)
                {
                    VkDescriptorBufferInfo bufferInfo_Result = {};
                    bufferInfo_Result.buffer = pCB_Result->poBuffer_Compute;
                    bufferInfo_Result.offset = 0;
                    bufferInfo_Result.range = (VkDeviceSize)pCB_Result->GetBufferSize();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                      descriptorSet,
                                                                      i,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Result);
                }
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWClipCB)) //BufferRWClipCB
            {
                if (pCB_Clip != nullptr)
                {
                    VkDescriptorBufferInfo bufferInfo_Clip = {};
                    bufferInfo_Clip.buffer = pCB_Clip->poBuffer_Compute;
                    bufferInfo_Clip.offset = 0;
                    bufferInfo_Clip.range = (VkDeviceSize)pCB_Clip->GetBufferSize();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                      descriptorSet,
                                                                      i,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Clip);
                }
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                descriptorSet,
                                                                i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassCull->poHizDepthImageInfo_Sampler);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRWSrc)) //TextureCSRWSrc
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                descriptorSet,
                                                                i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                this->m_pVKRenderPassCull->poHizDepthImageInfo_NoSampler);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRWDst)) //TextureCSRWDst
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                descriptorSet,
                                                                i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                this->m_pVKRenderPassCull->poHizDepthImageInfo_NoSampler);
            }
            else
            {
                String msg = "*********************** VKPipelineComputeCull::updateDescriptorSet: Wrong DescriptorSetLayout type: " + nameDescriptor;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
    }

}; //LostPeterVulkan