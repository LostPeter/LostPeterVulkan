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

namespace LostPeter
{
    VKPipelineGraphics::VKPipelineGraphics(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)
        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline_WireFrame(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)

        , isMultiView(false)
        , poPipeline_WireFrame2(VK_NULL_HANDLE)
        , poPipeline2(VK_NULL_HANDLE)

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

        this->pRenderPass = nullptr;
    }  

}; //LostPeter