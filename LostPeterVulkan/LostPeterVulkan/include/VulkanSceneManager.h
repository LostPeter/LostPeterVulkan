/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_SCENE_MANAGER_H_
#define _VULKAN_SCENE_MANAGER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanSceneManager
    {
    public:
        VulkanSceneManager();
        virtual~VulkanSceneManager();

    public: 
        static int s_numFrameResources;

    };

};  //LostPeter

#endif