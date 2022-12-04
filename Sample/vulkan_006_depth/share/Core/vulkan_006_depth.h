// author: LostPeter
// time:   2022-11-28

#ifndef _VULKAN_006_DEPTH_H_
#define _VULKAN_006_DEPTH_H_

#include "vulkanwindow.h"
using namespace LostPeter; 

class Vulkan_006_Depth : public VulkanWindow
{
public:
    Vulkan_006_Depth(int width, int height, std::string name);

public:
    struct ModelObject
    {
        ModelObject(VkDevice device)
            : poDevice(device)
            , nameModel("")
            , pathModel("")
            , pathTexture("")
            , isShow(true)
            , isRotate(true)
            , isWireFrame(false)
            , poVertexCount(0)
            , poVertexBuffer_Size(0)
            , poVertexBuffer_Data(nullptr)
            , poVertexBuffer(VK_NULL_HANDLE)
            , poVertexBufferMemory(VK_NULL_HANDLE)
            , poIndexCount(0)
            , poIndexBuffer_Size(0)
            , poIndexBuffer_Data(nullptr)
            , poIndexBuffer(VK_NULL_HANDLE)
            , poIndexBufferMemory(VK_NULL_HANDLE)
            , poMatWorld(1.0f)
            , poMipLevels(0)
            , poTextureImage(VK_NULL_HANDLE)
            , poTextureImageMemory(VK_NULL_HANDLE)
            , poTextureImageView(VK_NULL_HANDLE)
            , poTextureSampler(VK_NULL_HANDLE)
        {

        }
        ~ModelObject()
        {
            //Vertex
            if (this->poVertexBuffer != VK_NULL_HANDLE)
            {
                vkDestroyBuffer(this->poDevice, this->poVertexBuffer, nullptr);
                vkFreeMemory(this->poDevice, this->poVertexBufferMemory, nullptr);
            }
            this->poVertexBuffer = VK_NULL_HANDLE;
            this->poVertexBufferMemory = VK_NULL_HANDLE;
            //Index
            if (this->poIndexBuffer != VK_NULL_HANDLE)
            {
                vkDestroyBuffer(this->poDevice, this->poIndexBuffer, nullptr);
                vkFreeMemory(this->poDevice, this->poIndexBufferMemory, nullptr);
            }
            this->poIndexBuffer = VK_NULL_HANDLE;
            this->poIndexBufferMemory = VK_NULL_HANDLE;
            //Uniform
            if (this->poUniformBuffer != VK_NULL_HANDLE)
            {
                vkDestroyBuffer(this->poDevice, this->poUniformBuffer, nullptr);
                vkFreeMemory(this->poDevice, this->poUniformBufferMemory, nullptr);
            }
            this->poUniformBuffer = VK_NULL_HANDLE;
            this->poUniformBufferMemory = VK_NULL_HANDLE;
            //Texture
            if (this->poTextureSampler != VK_NULL_HANDLE)
            {
                vkDestroySampler(this->poDevice, this->poTextureSampler, nullptr);
            }
            this->poTextureSampler = VK_NULL_HANDLE;
            if (this->poTextureImageView != VK_NULL_HANDLE)
            {
                vkDestroyImageView(this->poDevice, this->poTextureImageView, nullptr);
            }
            this->poTextureImageView = VK_NULL_HANDLE;
            if (this->poTextureImage != VK_NULL_HANDLE)
            {
                vkDestroyImage(this->poDevice, this->poTextureImage, nullptr);
                vkFreeMemory(this->poDevice, this->poTextureImageMemory, nullptr);
            }
            this->poTextureImage = VK_NULL_HANDLE;
            this->poTextureImageMemory = VK_NULL_HANDLE;
            //
        }

        //Device
        VkDevice poDevice;

        //Name
        std::string nameModel;
        std::string pathModel;
        std::string pathTexture;
        bool isShow;
        bool isRotate;
        bool isWireFrame;

        //Vertex
        std::vector<Vertex_Pos3Color3Tex2> vertices;
        uint32_t poVertexCount;
        size_t poVertexBuffer_Size;
        void* poVertexBuffer_Data;
        VkBuffer poVertexBuffer;
        VkDeviceMemory poVertexBufferMemory;
        //Index
        std::vector<uint32_t> indices;
        uint32_t poIndexCount;
        size_t poIndexBuffer_Size;
        void* poIndexBuffer_Data;
        VkBuffer poIndexBuffer;
        VkDeviceMemory poIndexBufferMemory;

        //Uniform
        VkBuffer poUniformBuffer;
        VkDeviceMemory poUniformBufferMemory;
        glm::mat4 poMatWorld;

        //Texture
        uint32_t poMipLevels;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;


        //


    };
    typedef std::vector<ModelObject*> ModelObjectPtrVector;
    typedef std::map<std::string, ModelObject*> ModelObjectPtrMap;

public:
    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrMap m_mapModelObjects;

protected:
    //Create Pipeline
    
    //Load Assets
        //Camera
        virtual void createCamera();

        //Geometry/Texture
        virtual void loadModel_Custom();
            bool loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal);
            bool loadModel_Texture(ModelObject* pModelObject);

        //ConstBuffers
        virtual void buildObjectCB();
        virtual void buildMaterialCB();
        virtual void buildInstanceCB();

        //Pipeline
        virtual void createGraphicsPipeline();

    //Render/Update
        virtual void updateCBs_ObjectsContent();
        virtual void updateCBs_MaterialsContent();
        virtual void updateCBs_InstancesContent();

        virtual bool beginRenderImgui();
        virtual void endRenderImgui();

        virtual void drawMesh_Custom(VkCommandBuffer& commandBuffer);

    //cleanup
        virtual void cleanupCustom();
};


#endif