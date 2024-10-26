/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineGraphicsCopyBlitFromFrame.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"

namespace LostPeterVulkan
{
     VKPipelineGraphicsCopyBlitFromFrame::VKPipelineGraphicsCopyBlitFromFrame(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)
    {

    }

    VKPipelineGraphicsCopyBlitFromFrame::~VKPipelineGraphicsCopyBlitFromFrame()
    {
        Destroy();
    }

    void VKPipelineGraphicsCopyBlitFromFrame::Destroy()
    {
        
    }

}; //LostPeterVulkan