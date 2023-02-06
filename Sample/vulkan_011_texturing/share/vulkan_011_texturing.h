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
    /////////////////////////// ModelMesh ///////////////////////////
    struct ModelMesh
    {
        Vulkan_011_Texturing* pWindow;
        std::string nameMesh;
        std::string pathMesh;
        VulkanMeshType typeMesh;
        VulkanMeshGeometryType typeGeometryType;

        //Vertex
        VulkanVertexType poTypeVertex;
        std::vector<Vertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<Vertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
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


        ModelMesh(Vulkan_011_Texturing* _pWindow, 
                  const std::string& _nameMesh,
                  const std::string& _pathMesh,
                  VulkanMeshType _typeMesh,
                  VulkanMeshGeometryType _typeGeometryType,
                  VulkanVertexType _poTypeVertex)
            : pWindow(_pWindow)
            , nameMesh(_nameMesh)
            , pathMesh(_pathMesh)
            , typeMesh(_typeMesh)
            , typeGeometryType(_typeGeometryType)

            //Vertex
            , poTypeVertex(_poTypeVertex)
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
        {

        }

        ~ModelMesh()
        {
            Destroy();
        }

        void Destroy()
        {
            //Vertex
            this->pWindow->destroyBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
            this->poVertexBuffer = VK_NULL_HANDLE;
            this->poVertexBufferMemory = VK_NULL_HANDLE;

            //Index
            this->pWindow->destroyBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
            this->poIndexBuffer = VK_NULL_HANDLE;
            this->poIndexBufferMemory = VK_NULL_HANDLE;
        }


        bool LoadMesh(bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal);

    };
    typedef std::vector<ModelMesh*> ModelMeshPtrVector;
    typedef std::map<std::string, ModelMesh*> ModelMeshPtrMap;


    /////////////////////////// ModelTexture ////////////////////////
    struct ModelTexture
    {
        Vulkan_011_Texturing* pWindow;
        std::string nameTexture;
        std::vector<std::string> aPathTexture;
        VulkanTextureType typeTexture;
        VkFormat typeFormat; 
        VulkanTextureFilterType typeFilter;
        VulkanTextureAddressingType typeAddressing;
        VulkanTextureBorderColorType typeBorderColor;
        int refCount;
        int width;
        int height;
        int depth;

        uint32_t poMipMapCount;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;
        VkDescriptorImageInfo poTextureImageInfo;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        //Texture 3D
        uint8* pDataRGBA;

        //Texture Animation
        int texChunkMaxX;
        int texChunkMaxY;
        int texChunkIndex;
        int frameCurrent;


        ModelTexture(Vulkan_011_Texturing* _pWindow, 
                     const std::string& _nameTexture,
                     VulkanTextureType _typeTexture,
                     VkFormat _typeFormat,
                     VulkanTextureFilterType _typeFilter,
                     VulkanTextureAddressingType _typeAddressing,
                     VulkanTextureBorderColorType _typeBorderColor,
                     const std::vector<std::string>& _aPathTexture)
            : pWindow(_pWindow)
            , nameTexture(_nameTexture)
            , typeTexture(_typeTexture)
            , typeFormat(_typeFormat)
            , typeFilter(_typeFilter)
            , typeAddressing(_typeAddressing)
            , typeBorderColor(_typeBorderColor)
            , aPathTexture(_aPathTexture)
            , refCount(0)
            , width(0)
            , height(0)
            , depth(0)

            , poMipMapCount(1)
            , poTextureImage(VK_NULL_HANDLE)
            , poTextureImageMemory(VK_NULL_HANDLE)
            , poTextureImageView(VK_NULL_HANDLE)
            , poTextureSampler(VK_NULL_HANDLE)

            , stagingBuffer(VK_NULL_HANDLE)
            , stagingBufferMemory(VK_NULL_HANDLE)

            //Texture 3D
            , pDataRGBA(nullptr)

            //Texture Animation
            , texChunkMaxX(0)
            , texChunkMaxY(0)
            , texChunkIndex(0)
            , frameCurrent(0)
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
            if (this->stagingBuffer != VK_NULL_HANDLE)
            {
                this->pWindow->destroyBuffer(this->stagingBuffer, this->stagingBufferMemory);
            }
            this->pWindow->destroyTexture(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
            this->poTextureImage = VK_NULL_HANDLE;
            this->poTextureImageMemory = VK_NULL_HANDLE;
            this->poTextureImageView = VK_NULL_HANDLE;
            this->pWindow->destroyTextureSampler(this->poTextureSampler);
            this->poTextureSampler = VK_NULL_HANDLE;
            UTIL_DELETE_T(pDataRGBA)
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

        void LoadTexture(int width,
                         int height,
                         int depth)
        {
            this->width = width;
            this->height = height;
            this->depth = depth;

            if (this->typeTexture == Vulkan_Texture_1D)
            {
                this->pWindow->createTexture1D(this->aPathTexture[0], this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_1D, this->typeFormat, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, 1, this->poTextureImageView);
            }
            else if (this->typeTexture == Vulkan_Texture_2D)
            {
                this->pWindow->createTexture2D(this->aPathTexture[0], VK_IMAGE_TYPE_2D, VK_SAMPLE_COUNT_1_BIT, this->typeFormat, true, this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, this->typeFormat, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, 1, this->poTextureImageView);
            }
            else if (this->typeTexture == Vulkan_Texture_2DArray)
            {
                this->pWindow->createTexture2DArray(this->aPathTexture, VK_IMAGE_TYPE_2D, VK_SAMPLE_COUNT_1_BIT, this->typeFormat, true, this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_2D_ARRAY, this->typeFormat, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, (int)this->aPathTexture.size(), this->poTextureImageView);
            }
            else if (this->typeTexture == Vulkan_Texture_3D)
            {
                uint32_t size = width * height * depth;
                this->pDataRGBA = new uint8[size];
                memset(this->pDataRGBA, 0, (size_t)size);
                updateNoiseTextureData();
                this->pWindow->createTexture3D(this->typeFormat, this->pDataRGBA, size, width, height, depth, this->poTextureImage, this->poTextureImageMemory, this->stagingBuffer, this->stagingBufferMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_3D, this->typeFormat, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, 1, this->poTextureImageView);
            }
            else if (this->typeTexture == Vulkan_Texture_CubeMap)
            {
                this->pWindow->createTextureCubeMap(this->aPathTexture, this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                this->pWindow->createImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_CUBE, this->typeFormat, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, (int)this->aPathTexture.size(), this->poTextureImageView);
            }   
            else
            {
                std::string msg = "ModelTexture::LoadTexture: Wrong texture type !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            this->pWindow->createSampler(this->typeFilter, 
                                         this->typeAddressing,
                                         this->typeBorderColor,
                                         true,
                                         this->pWindow->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                         0.0f,
                                         static_cast<float>(this->poMipMapCount),
                                         0.0f,
                                         this->poTextureSampler);

            this->poTextureImageInfo = {};
            this->poTextureImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            this->poTextureImageInfo.imageView = this->poTextureImageView;
            this->poTextureImageInfo.sampler = this->poTextureSampler;
        }   

        void UpdateTexture();

    private:
        void updateNoiseTextureData();
        void updateNoiseTexture();
    };
    typedef std::vector<ModelTexture*> ModelTexturePtrVector;
    typedef std::map<std::string, ModelTexture*> ModelTexturePtrMap;


    /////////////////////////// ModelObject /////////////////////////
    struct ModelObject
    {
        ModelObject(Vulkan_011_Texturing* _pWindow)
            //Window
            : pWindow(_pWindow)

            //Name
            , nameModel("")
            , nameMesh("")
            , isShow(true)
            , isWireFrame(false)
            , isRotate(true)
            , isTransparent(false)
            , isLighting(true)

            //Mesh
            , pMesh(nullptr)

            //Uniform
            , countInstanceExt(0)
            , countInstance(1)

            //DescriptorSetLayout
            , nameDescriptorSetLayout("")

            //PipelineLayout
            , poPipelineLayout(VK_NULL_HANDLE)

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
            //Mesh
            this->pMesh = nullptr;

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
        std::string nameMesh;
        std::vector<int> aTextureChannels;
        std::vector<std::string> aPathTextures;
        std::map<int, std::vector<std::string>> mapPathTextures;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isTransparent;
        bool isLighting;

        //Mesh
        ModelMesh* pMesh;

        //Texture
        ModelTexturePtrVector m_aModelTextures;
        ModelTexturePtrMap m_mapModelTextures;

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

        //DescriptorSetLayout
        std::string nameDescriptorSetLayout;

        //PipelineLayout
        VkPipelineLayout poPipelineLayout;

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


    ////Mesh
        void SetMesh(ModelMesh* pMesh)
        {
            this->pMesh = pMesh;
        }
        ModelMesh* GetMesh()
        {
            return this->pMesh;
        }

        VkBuffer GetMeshVertexBuffer()
        {
            return this->pMesh->poVertexBuffer;
        }
        VkBuffer GetMeshIndexBuffer()
        {
            return this->pMesh->poIndexBuffer;
        }

    ////Textures
        void AddTexture(ModelTexture* pTexture)
        {
            this->m_aModelTextures.push_back(pTexture);
            this->m_mapModelTextures[pTexture->nameTexture] = pTexture;
        }
        int GetTextureCount()
        {
            return (int)this->m_aModelTextures.size();
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
    ModelMeshPtrVector m_aModelMesh;
    ModelMeshPtrMap m_mapModelMesh;    

    ModelTexturePtrVector m_aModelTexture;
    ModelTexturePtrMap m_mapModelTexture;

    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrVector m_aModelObjects_Render;
    ModelObjectPtrMap m_mapModelObjects;

    VkDescriptorSetLayoutVector m_aVkDescriptorSetLayouts;
    VkDescriptorSetLayoutMap m_mapVkDescriptorSetLayout;
    std::map<std::string, std::vector<std::string>> m_mapName2Layouts;

    VkShaderModuleVector m_aVkShaderModules;
    VkShaderModuleMap m_mapVkShaderModules;

    VkPipelineLayoutVector m_aVkPipelineLayouts;
    VkPipelineLayoutMap m_mapVkPipelineLayouts;

protected:
    //Create Pipeline

        //DescriptorSetLayout
        virtual void createDescriptorSetLayout_Custom();

    //Load Assets
        //Camera
        virtual void createCamera();

        //Geometry/Texture
        virtual void loadModel_Custom();

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

////ModelMesh
    void destroyModelMeshes();
    void createModelMeshes();
    ModelMesh* findModelMesh(const std::string& nameMesh);

////ModelTexture
    void destroyModelTextures();
    void createModelTextures();
    ModelTexture* findModelTexture(const std::string& nameTexture);

////DescriptorSetLayout
    void destroyDescriptorSetLayouts();
    void createDescriptorSetLayouts();
    VkDescriptorSetLayout findDescriptorSetLayout(const std::string& nameDescriptorSetLayout);
    std::vector<std::string>* findDescriptorSetLayoutNames(const std::string& nameDescriptorSetLayout);

////ShaderModule
    void destroyShaderModules();
    void createShaderModules();
    VkShaderModule findShaderModule(const std::string& pathShaderModule);

////PipelineLayout
    void destroyPipelineLayouts();
    void createPipelineLayouts();
    VkPipelineLayout findPipelineLayout(const std::string& namePipelineLayout);

    void drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject);
};


#endif