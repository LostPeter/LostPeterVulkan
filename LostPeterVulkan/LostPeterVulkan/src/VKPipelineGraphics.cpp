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

#include "../include/VKPipelineGraphics.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKPipelineGraphics::VKPipelineGraphics(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)
        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline_WireFrame(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        
        , hasNextSubpass(false)
        , isMultiView(false)
        , nameDescriptorSetLayout2("")
        , poDescriptorSetLayoutNames2(nullptr)
        , poDescriptorSetLayout2(VK_NULL_HANDLE)
        , poPipelineLayout2(VK_NULL_HANDLE)
        , poPipeline_WireFrame2(VK_NULL_HANDLE)
        , poPipeline2(VK_NULL_HANDLE)

        , nameDescriptorSetLayout_Cull("")
        , poDescriptorSetLayoutNames_Cull(nullptr)
        , poDescriptorSetLayout_Cull(VK_NULL_HANDLE)
        , poPipelineLayout_Cull(VK_NULL_HANDLE)
        , poPipeline_WireFrame_Cull(VK_NULL_HANDLE)
        , poPipeline_Cull(VK_NULL_HANDLE)

        , pRenderPass(nullptr)
    {

    }
    VKPipelineGraphics::~VKPipelineGraphics()
    {
        Destroy();
    }
    void VKPipelineGraphics::Destroy()
    {
        CleanupSwapChain();
    }
    void VKPipelineGraphics::CleanupSwapChain()
    {
        //1> poPipeline
        this->poDescriptorSetLayoutNames = nullptr;
        this->poDescriptorSetLayout = VK_NULL_HANDLE;
        this->poPipelineLayout = VK_NULL_HANDLE;
        if (this->poPipeline_WireFrame != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_WireFrame);
        }
        this->poPipeline_WireFrame = VK_NULL_HANDLE;

        if (this->poPipeline != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline);
        }
        this->poPipeline = VK_NULL_HANDLE;
        this->poDescriptorSets.clear();

        //2> poPipeline2
        this->poDescriptorSetLayoutNames2 = nullptr;
        this->poDescriptorSetLayout2 = VK_NULL_HANDLE;
        this->poPipelineLayout2 = VK_NULL_HANDLE;
        if (this->poPipeline_WireFrame2 != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_WireFrame2);
        }
        this->poPipeline_WireFrame2 = VK_NULL_HANDLE;
        if (this->poPipeline2 != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline2);
        }
        this->poPipeline2 = VK_NULL_HANDLE;
        this->poDescriptorSets2.clear();

        //3> poPipeline_Cull
        this->poDescriptorSetLayoutNames_Cull = nullptr;
        this->poDescriptorSetLayout_Cull = VK_NULL_HANDLE;
        this->poPipelineLayout_Cull = VK_NULL_HANDLE;
        if (this->poPipeline_WireFrame_Cull != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_WireFrame_Cull);
        }
        this->poPipeline_WireFrame_Cull = VK_NULL_HANDLE;

        if (this->poPipeline_Cull != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_Cull);
        }
        this->poPipeline_Cull = VK_NULL_HANDLE;
        this->poDescriptorSets_Cull.clear();

        //4> RenderPass
        this->pRenderPass = nullptr;
    }  

}; //LostPeterVulkan