// author: LostPeter
// time:   2022-11-12

#ifndef _VULKAN_003_TEXTURE_H_
#define _VULKAN_003_TEXTURE_H_

#include "VulkanWindow.h"
using namespace LostPeter; 

class Vulkan_003_Texture : public VulkanWindow
{
public:
    Vulkan_003_Texture(int width, int height, std::string name);

public:

protected:
    std::vector<Vertex_Pos2Color4Tex2> vertices;

public:
    //Create Pipeline

    //Load Assets
        //Geometry
            virtual void loadModel_Custom();

    //Render/Update
        virtual bool beginRenderImgui();
        virtual void endRenderImgui();

public:
    
};


#endif