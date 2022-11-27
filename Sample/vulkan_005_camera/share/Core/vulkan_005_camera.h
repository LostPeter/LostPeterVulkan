// author: LostPeter
// time:   2022-11-21

#ifndef _VULKAN_005_CAMERA_H_
#define _VULKAN_005_CAMERA_H_

#include "vulkanwindow.h"
using namespace LibUtil; 

class Vulkan_005_Camera : public VulkanWindow
{
public:
    Vulkan_005_Camera(int width, int height, std::string name);

public:
    std::vector<Vertex_Pos3Color3Tex2> vertices;
    std::vector<uint32_t> indices;

protected:
    //Create Pipeline
    
    //Load Assets
        //Camera
        virtual void createCamera();

        //Geometry
        virtual void loadModel_Assimp();

    //Render/Update
        virtual bool beginRenderImgui();
        virtual void endRenderImgui();

private:
    void resetSetting(int index);
    void changeModel(int index);
};


#endif