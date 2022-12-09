// author: LostPeter
// time:   2022-11-15

#ifndef _VULKAN_004_MODEL_H_
#define _VULKAN_004_MODEL_H_

#include "VulkanWindow.h"
using namespace LostPeter; 

class Vulkan_004_Model : public VulkanWindow
{
public:
    Vulkan_004_Model(int width, int height, std::string name);

public:
    std::vector<Vertex_Pos3Color4Tex2> vertices;
    std::vector<uint32_t> indices;

protected:
    //Create Pipeline
    
    //Load Assets
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