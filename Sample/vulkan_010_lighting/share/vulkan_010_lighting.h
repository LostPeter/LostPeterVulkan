/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_010_LIGHTING_H_
#define _VULKAN_010_LIGHTING_H_

#include "VulkanWindow.h"
using namespace LostPeter; 

class Vulkan_010_Lighting : public VulkanWindow
{
public:
    Vulkan_010_Lighting(int width, int height, String name);

public:
    struct ModelObject
    {
        ModelObject(Vulkan_010_Lighting* _pWindow)
            //Common
            : pWindow(_pWindow)

            //Name
            , nameModel("")
            , pathModel("")
            , pathTexture("")
            , isShow(true)
            , isWireFrame(false)
            , isRotate(true)
            , isTransparent(false)

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
            , countInstanceExt(5)
            , countInstance(11)
            
            //Texture
            , poMipMapCount(0)
            , poTextureImage(VK_NULL_HANDLE)
            , poTextureImageMemory(VK_NULL_HANDLE)
            , poTextureImageView(VK_NULL_HANDLE)
            , poTextureSampler(VK_NULL_HANDLE)

            //Pipeline
            , poPipelineGraphics_WireFrame(VK_NULL_HANDLE)
            , poPipelineGraphics(VK_NULL_HANDLE)

            //State
            , cfg_vkPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            , cfg_vkFrontFace(VK_FRONT_FACE_CLOCKWISE)
            , cfg_vkPolygonMode(VK_POLYGON_MODE_FILL)
            , cfg_vkCullModeFlagBits(VK_CULL_MODE_BACK_BIT)
            , cfg_isDepthTest(VK_TRUE)
            , cfg_isDepthWrite(VK_TRUE)
            , cfg_DepthCompareOp(VK_COMPARE_OP_LESS_OR_EQUAL)  
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
            this->pWindow->destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
            this->poVertexBuffer = VK_NULL_HANDLE;
            this->poVertexBufferMemory = VK_NULL_HANDLE;

            //Index
            this->pWindow->destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
            this->poIndexBuffer = VK_NULL_HANDLE;
            this->poIndexBufferMemory = VK_NULL_HANDLE;

            //Texture
            this->pWindow->destroyVkImage(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
            this->pWindow->destroyVkImageSampler(this->poTextureSampler);
            this->poTextureImage = VK_NULL_HANDLE;
            this->poTextureImageMemory = VK_NULL_HANDLE;
            this->poTextureImageView = VK_NULL_HANDLE;
            this->poTextureSampler = VK_NULL_HANDLE;

            CleanupSwapChain();
        }

        void CleanupSwapChain()
        {
            //Uniform
            size_t count = this->poBuffers_ObjectCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
            }
            this->objectCBs.clear();
            this->poBuffers_ObjectCB.clear();
            this->poBuffersMemory_ObjectCB.clear();

            count = this->poBuffers_materialCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pWindow->destroyVkBuffer(this->poBuffers_materialCB[i], this->poBuffersMemory_materialCB[i]);
            }
            this->materialCBs.clear();
            this->poBuffers_materialCB.clear();
            this->poBuffersMemory_materialCB.clear();

            //Pipeline
            this->pWindow->destroyVkPipeline(this->poPipelineGraphics_WireFrame);
            this->poPipelineGraphics_WireFrame = VK_NULL_HANDLE;
            this->pWindow->destroyVkPipeline(this->poPipelineGraphics);
            this->poPipelineGraphics = VK_NULL_HANDLE;
        }

        void recreateSwapChain()
        {

        }

        //Common
        Vulkan_010_Lighting* pWindow;

        //Name
        String nameModel;
        String pathModel;
        String pathTexture;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isTransparent;

        //Vertex
        std::vector<Vertex_Pos3Color4Normal3Tex2> vertices;
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
        int countInstanceExt;
        int countInstance;

        std::vector<ObjectConstants> objectCBs;
        std::vector<VkBuffer> poBuffers_ObjectCB;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB;
        std::vector<FMatrix4> instanceMatWorld;

        std::vector<MaterialConstants> materialCBs;
        std::vector<VkBuffer> poBuffers_materialCB;
        std::vector<VkDeviceMemory> poBuffersMemory_materialCB;

        //Texture
        uint32_t poMipMapCount;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;

        //Pipeline
        VkPipeline poPipelineGraphics_WireFrame;
        VkPipeline poPipelineGraphics;

        //DescriptorSets
        VkDescriptorSetVector poDescriptorSets;

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
    typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrVector m_aModelObjects_Render;
    ModelObjectPtrMap m_mapModelObjects;
    
    VkShaderModuleVector m_aVkShaderModules;
    VkShaderModuleMap m_mapVkShaderModules;

protected:
    //Create Pipeline
    
    //Load Assets
        //Camera
        virtual void createCamera();

        //Geometry/Texture
        virtual void loadModel_Custom();
            bool loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTranformLocal, const FMatrix4& matTransformLocal);
            bool loadModel_Texture(ModelObject* pModelObject);

        //ConstBuffers
        virtual void createCustomCB();

        //Pipeline
        virtual void createCustomBeforePipeline();
        virtual void createGraphicsPipeline_Custom();

        //DescriptorSets
        virtual void createDescriptorSets_Custom();

    //Render/Update
        virtual void updateCBs_Custom();

        virtual bool beginRenderImgui();
            virtual void modelConfig();
            
        virtual void endRenderImgui();

        virtual void drawMeshDefault_Custom(VkCommandBuffer& commandBuffer);

    //cleanup
        virtual void cleanupCustom();

        virtual void cleanupSwapChain_Custom();
        virtual void recreateSwapChain_Custom();

private:
    void rebuildInstanceCBs(bool isCreateVkBuffer);

    void destroyShaderModules();
    void createShaderModules();
    VkShaderModule findShaderModule(const String& pathShaderModule);

    void drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject);
};


#endif