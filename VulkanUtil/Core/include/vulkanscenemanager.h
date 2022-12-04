// author: LostPeter
// time:   2022-10-30

#ifndef _VULKAN_SCENE_MANAGER_H_
#define _VULKAN_SCENE_MANAGER_H_

#include "predefine.h"

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