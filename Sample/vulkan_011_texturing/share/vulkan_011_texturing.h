/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-01-02
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_011_TEXTURING_H_
#define _VULKAN_011_TEXTURING_H_

#include "VulkanWindow.h"
#include "MathUtil.h"
using namespace LostPeter; 

class Vulkan_011_Texturing : public VulkanWindow
{
public:
    Vulkan_011_Texturing(int width, int height, std::string name);

public:
    struct ModelTexture
    {
        Vulkan_011_Texturing* pWindow;
        std::string nameTexture;
        std::vector<std::string> aPathTexture;
        VulkanTextureType typeTexture;
        int refCount;

        uint32_t poMipMapCount;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;

        ModelTexture(Vulkan_011_Texturing* _pWindow, 
                     const std::string& _nameTexture,
                     VulkanTextureType _typeTexture,
                     const std::vector<std::string>& _aPathTexture)
            : pWindow(_pWindow)
            , nameTexture(_nameTexture)
            , typeTexture(_typeTexture)
            , aPathTexture(_aPathTexture)
            , refCount(0)

            , poMipMapCount(1)
            , poTextureImage(VK_NULL_HANDLE)
            , poTextureImageMemory(VK_NULL_HANDLE)
            , poTextureImageView(VK_NULL_HANDLE)
            , poTextureSampler(VK_NULL_HANDLE)
        {

        }

        ~ModelTexture()
        {
            Destroy();
        }

        int GetRef() { return this->refCount; }
        int AddRef() { return ++ this->refCount; }
        int DelRef() { return -- this->refCount; }
        bool HasRef() { return this->refCount > 0; }
        bool CanDel() { return !HasRef(); }

        void Destroy()
        {
            this->pWindow->destroyTexture(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
            this->poTextureImage = VK_NULL_HANDLE;
            this->poTextureImageMemory = VK_NULL_HANDLE;
            this->poTextureImageView = VK_NULL_HANDLE;
            this->pWindow->destroyTextureSampler(this->poTextureSampler);
            this->poTextureSampler = VK_NULL_HANDLE;   
        }

        int RandomTextureIndex()
        {
            if (this->typeTexture == Vulkan_Texture_2DArray)
            {
                int count = (int)this->aPathTexture.size();
                return MathUtil::Rand(0, count - 1);
            }
            return 0;
        }

        void LoadTexture()
        {
            if (this->typeTexture == Vulkan_Texture_1D)
            {
                this->pWindow->createTexture1D(this->aPathTexture[0], this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_1D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, this->poTextureImageView);
                this->pWindow->createSampler(this->poMipMapCount, this->poTextureSampler);
            }
            else if (this->typeTexture == Vulkan_Texture_2D)
            {
                this->pWindow->createTexture2D(this->aPathTexture[0], this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, this->poTextureImageView);
                this->pWindow->createSampler(this->poMipMapCount, this->poTextureSampler);
            }
            else if (this->typeTexture == Vulkan_Texture_2DArray)
            {
                this->pWindow->createTexture2DArray(this->aPathTexture, this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_2D_ARRAY, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, this->poTextureImageView);
                this->pWindow->createSampler(this->poMipMapCount, this->poTextureSampler);
            }
            else if (this->typeTexture == Vulkan_Texture_3D)
            {
                
            }
            else if (this->typeTexture == Vulkan_Texture_CubeMap)
            {
                
            }   
            else
            {
                std::string msg = "ModelTexture::LoadTexture: Wrong texture type !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    };
    typedef std::vector<ModelTexture*> ModelTexturePtrVector;
    typedef std::map<std::string, ModelTexture*> ModelTexturePtrMap;

    struct ModelObject
    {
        ModelObject(Vulkan_011_Texturing* _pWindow)
            //Window
            : pWindow(_pWindow)

            //Name
            , nameModel("")
            , pathModel("")
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
            , countInstanceExt(0)
            , countInstance(1)

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
            this->pWindow->destroyBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
            this->poVertexBuffer = VK_NULL_HANDLE;
            this->poVertexBufferMemory = VK_NULL_HANDLE;

            //Index
            this->pWindow->destroyBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
            this->poIndexBuffer = VK_NULL_HANDLE;
            this->poIndexBufferMemory = VK_NULL_HANDLE;

            //Texture
            this->m_aModelTextures.clear();
            this->m_mapModelTextures.clear();
            
            cleanupSwapChain();
        }

        void cleanupSwapChain()
        {
            //Uniform
            size_t count = this->poBuffers_ObjectCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pWindow->destroyBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
            }
            this->objectCBs.clear();
            this->poBuffers_ObjectCB.clear();
            this->poBuffersMemory_ObjectCB.clear();

            count = this->poBuffers_materialCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pWindow->destroyBuffer(this->poBuffers_materialCB[i], this->poBuffersMemory_materialCB[i]);
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

        //Window
        Vulkan_011_Texturing* pWindow;

        //Name
        int indexModel;
        std::string nameModel;
        std::string pathModel;
        std::vector<int> aTextureChannels;
        std::vector<std::string> aPathTextures;
        std::map<int, std::vector<std::string>> mapPathTextures;
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
        std::vector<glm::mat4> instanceMatWorld;

        std::vector<MaterialConstants> materialCBs;
        std::vector<VkBuffer> poBuffers_materialCB;
        std::vector<VkDeviceMemory> poBuffersMemory_materialCB;

        //Texture
        ModelTexturePtrVector m_aModelTextures;
        ModelTexturePtrMap m_mapModelTextures;

        //Pipeline
        VkPipeline poPipelineGraphics_WireFrame;
        VkPipeline poPipelineGraphics;

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


        void AddTexture(ModelTexture* pTexture)
        {
            this->m_aModelTextures.push_back(pTexture);
            this->m_mapModelTextures[pTexture->nameTexture] = pTexture;
        }
        ModelTexture* GetTexture(int index)
        {
            return this->m_aModelTextures[index];
        }

        VkImage GetTextureImage(int index)
        {
            return this->m_aModelTextures[index]->poTextureImage;
        }
        VkImageView GetTextureImageView(int index)
        {
            return this->m_aModelTextures[index]->poTextureImageView;
        }
        VkSampler GetTextureSampler(int index)
        {
            return this->m_aModelTextures[index]->poTextureSampler;
        }
    };
    typedef std::vector<ModelObject*> ModelObjectPtrVector;
    typedef std::map<std::string, ModelObject*> ModelObjectPtrMap;

public:
    ModelTexturePtrVector m_aModelTexture;
    ModelTexturePtrMap m_mapModelTexture;

    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrVector m_aModelObjects_Render;
    ModelObjectPtrMap m_mapModelObjects;

    VkDescriptorSetLayoutVector m_aVkDescriptorSetLayouts;
    VkDescriptorSetLayoutMap m_mapVkDescriptorSetLayout;
    
    VkShaderModuleVector m_aVkShaderModules;
    VkShaderModuleMap m_mapVkShaderModules;

protected:
    //Create Pipeline

        //DescriptorSetLayout
        virtual void createDescriptorSetLayout_Custom();

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

        virtual void cleanupSwapChain_Custom();
        virtual void recreateSwapChain_Custom();

private:
    void rebuildInstanceCBs(bool isCreateVkBuffer);

    void destroyModelTextures();
    void createModelTextures();
    ModelTexture* findModelTexture(const std::string& nameTexture);

    void destroyDescriptorSetLayouts();
    void createDescriptorSetLayouts();
    VkDescriptorSetLayout findDescriptorSetLayout(const std::string& nameDescriptorSetLayout);

    void destroyShaderModules();
    void createShaderModules();
    VkShaderModule findShaderModule(const std::string& pathShaderModule);

    void drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject);
};


#endif