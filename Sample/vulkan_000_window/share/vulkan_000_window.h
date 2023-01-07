/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-30
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_000_WINDOW_H_
#define _VULKAN_000_WINDOW_H_

#include "VulkanWindow.h"
using namespace LostPeter; 

class Vulkan_000_Window : public VulkanWindow
{
public:
    Vulkan_000_Window(int width, int height, std::string name);

public:

protected:
    //Create Pipeline
    virtual void createPipeline();

    //Load Assets
    virtual void loadAssets();

public:
    


};


#endif