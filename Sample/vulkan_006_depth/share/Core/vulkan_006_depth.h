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
            //Device
            : poDevice(device)

            //Name
            , nameModel("")
            , pathModel("")
            , pathTexture("")
            , isShow(true)
            , isRotate(true)
            , isWireFrame(false)
            , isNoDepthTest(false)
            , isNoDepthWrite(false)
            , isNoDepthTestWrite(false)

            //Vertex
            , poVertexCount(0)
            , poVertexBuffer_Size(0)
            , poVertexBuffer_Data(nullptr)
            , poVertexBuffer(VK_NULL_HANDLE)
            , poVertexBufferMemory(VK_NULL_HANDLE)

            //Index
            , poIndexCount(0)
            , poIndexBuffer_Size(0)
            , poIndexBuffer_Data(nullptr)
            , poIndexBuffer(VK_NULL_HANDLE)
            , poIndexBufferMemory(VK_NULL_HANDLE)

            //Uniform
            , poMatWorld(1.0f)
            
            //Texture
            , poMipLevels(0)
            , poTextureImage(VK_NULL_HANDLE)
            , poTextureImageMemory(VK_NULL_HANDLE)
            , poTextureImageView(VK_NULL_HANDLE)
            , poTextureSampler(VK_NULL_HANDLE)

            //Pipeline
            , poPipelineGraphics(VK_NULL_HANDLE)
            , poPipelineGraphics_WireFrame(VK_NULL_HANDLE)
            , poPipelineGraphics_NoDepthTest(VK_NULL_HANDLE)
            , poPipelineGraphics_NoDepthWrite(VK_NULL_HANDLE)
            , poPipelineGraphics_NoDepthTestWrite(VK_NULL_HANDLE)

            //State
            , cfg_vkPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            , cfg_vkFrontFace(VK_FRONT_FACE_CLOCKWISE)
            , cfg_vkPolygonMode(VK_POLYGON_MODE_FILL)
            , cfg_vkCullModeFlagBits(VK_CULL_MODE_BACK_BIT)
            , cfg_isDepthTest(VK_TRUE)
            , cfg_isDepthWrite(VK_TRUE)
            , cfg_DepthCompareOp(VK_COMPARE_OP_LESS) 
            , cfg_isStencilTest(VK_FALSE)
            , cfg_isBlend(VK_FALSE)
            , cfg_BlendColorFactorSrc(VK_BLEND_FACTOR_ONE)
            , cfg_BlendColorFactorDst(VK_BLEND_FACTOR_ZERO)
            , cfg_BlendColorOp(VK_BLEND_OP_ADD)
            , cfg_BlendAlphaFactorSrc(VK_BLEND_FACTOR_ONE)
            , cfg_BlendAlphaFactorDst(VK_BLEND_FACTOR_ZERO)
            , cfg_BlendAlphaOp(VK_BLEND_OP_ADD)
            , cfg_ColorWriteMask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
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
            size_t count = this->poBuffers_ObjectCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                vkDestroyBuffer(this->poDevice, this->poBuffers_ObjectCB[i], nullptr);
                vkFreeMemory(this->poDevice, this->poBuffersMemory_ObjectCB[i], nullptr);
            }
            this->objectCBs.clear();
            this->poBuffers_ObjectCB.clear();
            this->poBuffersMemory_ObjectCB.clear();

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

            //Pipeline
            if (this->poPipelineGraphics != nullptr)
            {
                vkDestroyPipeline(this->poDevice, this->poPipelineGraphics, nullptr);
            }
            this->poPipelineGraphics = VK_NULL_HANDLE;
            if (this->poPipelineGraphics_WireFrame != nullptr)
            {
                vkDestroyPipeline(this->poDevice, this->poPipelineGraphics_WireFrame, nullptr);
            }
            this->poPipelineGraphics_WireFrame = VK_NULL_HANDLE;
            if (this->poPipelineGraphics_NoDepthTest != nullptr)
            {
                vkDestroyPipeline(this->poDevice, this->poPipelineGraphics_NoDepthTest, nullptr);
            }
            this->poPipelineGraphics_NoDepthTest = VK_NULL_HANDLE;
            if (this->poPipelineGraphics_NoDepthWrite != nullptr)
            {
                vkDestroyPipeline(this->poDevice, this->poPipelineGraphics_NoDepthWrite, nullptr);
            }
            this->poPipelineGraphics_NoDepthWrite = VK_NULL_HANDLE;
            if (this->poPipelineGraphics_NoDepthTestWrite != nullptr)
            {
                vkDestroyPipeline(this->poDevice, this->poPipelineGraphics_NoDepthTestWrite, nullptr);
            }
            this->poPipelineGraphics_NoDepthTestWrite = VK_NULL_HANDLE;
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
        bool isNoDepthTest;
        bool isNoDepthWrite;
        bool isNoDepthTestWrite;

        //Vertex
        std::vector<Vertex_Pos3Color4Tex2> vertices;
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
        std::vector<ObjectConstants> objectCBs;
        std::vector<VkBuffer> poBuffers_ObjectCB;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB;
        glm::mat4 poMatWorld;

        //Texture
        uint32_t poMipLevels;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;

        //Pipeline
        VkPipeline poPipelineGraphics;
        VkPipeline poPipelineGraphics_WireFrame;
        VkPipeline poPipelineGraphics_NoDepthTest;
        VkPipeline poPipelineGraphics_NoDepthWrite;
        VkPipeline poPipelineGraphics_NoDepthTestWrite;

        //DescriptorSets
        std::vector<VkDescriptorSet> poDescriptorSets;

        //State
        VkPrimitiveTopology cfg_vkPrimitiveTopology;
        VkFrontFace cfg_vkFrontFace;
        VkPolygonMode cfg_vkPolygonMode;
        VkCullModeFlagBits cfg_vkCullModeFlagBits;
        VkBool32 cfg_isDepthTest;
        VkBool32 cfg_isDepthWrite; 
        VkCompareOp cfg_DepthCompareOp; 
        VkBool32 cfg_isStencilTest;
        VkStencilOpState cfg_StencilOpFront; 
        VkStencilOpState cfg_StencilOpBack; 
        VkBool32 cfg_isBlend;
        VkBlendFactor cfg_BlendColorFactorSrc; 
        VkBlendFactor cfg_BlendColorFactorDst; 
        VkBlendOp cfg_BlendColorOp;
        VkBlendFactor cfg_BlendAlphaFactorSrc;
        VkBlendFactor cfg_BlendAlphaFactorDst; 
        VkBlendOp cfg_BlendAlphaOp;
        VkColorComponentFlags cfg_ColorWriteMask;
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
        virtual void createCustomCB();

        //Pipeline
        virtual void createPipeline_Custom();

        //DescriptorSets
        virtual void createDescriptorSets_Custom();

    //Render/Update
        virtual void updateCBs_Custom();

        virtual bool beginRenderImgui();
        virtual void endRenderImgui();

        virtual void drawMesh_Custom(VkCommandBuffer& commandBuffer);

    //cleanup
        virtual void cleanupCustom();
};


#endif