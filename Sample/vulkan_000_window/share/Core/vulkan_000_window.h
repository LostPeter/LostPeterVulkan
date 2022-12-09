// author: LostPeter
// time:   2022-10-30

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