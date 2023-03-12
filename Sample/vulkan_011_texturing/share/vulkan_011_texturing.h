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
#include "VulkanMath.h"
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
        bool isRenderTarget;
        bool isGraphicsComputeShared;
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
        VkImageLayout poTextureImageLayout;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        //Texture 3D
        uint8* pDataRGBA;

        //Texture Animation
        int texChunkMaxX;
        int texChunkMaxY;
        int texChunkIndex;
        int frameCurrent;

        //Texture RenderTarget
        glm::vec4 rtColorDefault;
        bool rtIsSetColor; 
        VkImageUsageFlags rtImageUsage;


        ModelTexture(Vulkan_011_Texturing* _pWindow, 
                     const std::string& _nameTexture,
                     VulkanTextureType _typeTexture,
                     bool _isRenderTarget,
                     bool _isGraphicsComputeShared,
                     VkFormat _typeFormat,
                     VulkanTextureFilterType _typeFilter,
                     VulkanTextureAddressingType _typeAddressing,
                     VulkanTextureBorderColorType _typeBorderColor,
                     const std::vector<std::string>& _aPathTexture)
            : pWindow(_pWindow)
            , nameTexture(_nameTexture)
            , typeTexture(_typeTexture)
            , isRenderTarget(_isRenderTarget)
            , isGraphicsComputeShared(_isGraphicsComputeShared)
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

            //Texture RenderTarget
            , rtColorDefault(0, 0, 0, 1)
            , rtIsSetColor(false)
            , rtImageUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT)
        {
            if (this->isRenderTarget)
                this->poTextureImageLayout = VK_IMAGE_LAYOUT_GENERAL;
            else
                this->poTextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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
                return VulkanMath::Rand(0, count - 1);
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

            if (!this->isRenderTarget)
            {
                if (this->typeTexture == Vulkan_Texture_1D)
                {
                    this->pWindow->createTexture1D(this->aPathTexture[0], 
                                                   this->poMipMapCount, 
                                                   this->poTextureImage, 
                                                   this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_1D, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   1, 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_2D)
                {
                    this->pWindow->createTexture2D(this->aPathTexture[0], 
                                                   VK_IMAGE_TYPE_2D, 
                                                   VK_SAMPLE_COUNT_1_BIT,
                                                   this->typeFormat, 
                                                   true, 
                                                   this->poMipMapCount, 
                                                   this->poTextureImage, 
                                                   this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_2D, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   1, 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_2DArray)
                {
                    this->pWindow->createTexture2DArray(this->aPathTexture, 
                                                        VK_IMAGE_TYPE_2D, 
                                                        VK_SAMPLE_COUNT_1_BIT, 
                                                        this->typeFormat, 
                                                        true, 
                                                        this->poMipMapCount, 
                                                        this->poTextureImage, 
                                                        this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   (int)this->aPathTexture.size(), 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_3D)
                {
                    uint32_t size = width * height * depth;
                    this->pDataRGBA = new uint8[size];
                    memset(this->pDataRGBA, 0, (size_t)size);
                    updateNoiseTextureData();
                    this->pWindow->createTexture3D(this->typeFormat, 
                                                   this->pDataRGBA, 
                                                   size, 
                                                   width, 
                                                   height, 
                                                   depth, 
                                                   this->poTextureImage, 
                                                   this->poTextureImageMemory, 
                                                   this->stagingBuffer, 
                                                   this->stagingBufferMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_3D, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   1, 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_CubeMap)
                {
                    this->pWindow->createTextureCubeMap(this->aPathTexture, 
                                                        this->poMipMapCount, 
                                                        this->poTextureImage, 
                                                        this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_CUBE, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   (int)this->aPathTexture.size(), 
                                                   this->poTextureImageView);
                }   
                else
                {
                    std::string msg = "ModelTexture::LoadTexture: Wrong texture type, Create from file, name: [" + this->nameTexture + "] !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
            else
            {
                if (this->typeTexture == Vulkan_Texture_1D)
                {
                    this->pWindow->createTextureRenderTarget1D(this->rtColorDefault, 
                                                               this->rtIsSetColor, 
                                                               this->width, 
                                                               this->poMipMapCount, 
                                                               VK_SAMPLE_COUNT_1_BIT, 
                                                               this->typeFormat, 
                                                               this->rtImageUsage,
                                                               this->isGraphicsComputeShared,
                                                               this->poTextureImage, 
                                                               this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_1D, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   1, 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_2D)
                {
                    this->pWindow->createTextureRenderTarget2D(this->rtColorDefault, 
                                                               this->rtIsSetColor, 
                                                               this->width, 
                                                               this->height,
                                                               this->poMipMapCount, 
                                                               VK_SAMPLE_COUNT_1_BIT, 
                                                               this->typeFormat, 
                                                               this->rtImageUsage,
                                                               this->isGraphicsComputeShared,
                                                               this->poTextureImage, 
                                                               this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_2D, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   1, 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_2DArray)
                {
                    this->pWindow->createTextureRenderTarget2DArray(this->rtColorDefault, 
                                                                    this->rtIsSetColor, 
                                                                    this->width, 
                                                                    this->height,
                                                                    this->poMipMapCount, 
                                                                    VK_IMAGE_TYPE_2D,
                                                                    VK_SAMPLE_COUNT_1_BIT, 
                                                                    this->typeFormat, 
                                                                    this->rtImageUsage,
                                                                    this->isGraphicsComputeShared,
                                                                    this->poTextureImage, 
                                                                    this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   (int)this->aPathTexture.size(), 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_3D)
                {
                    this->pWindow->createTextureRenderTarget3D(this->rtColorDefault, 
                                                               this->rtIsSetColor, 
                                                               this->width, 
                                                               this->height,
                                                               this->depth,
                                                               this->poMipMapCount, 
                                                               VK_SAMPLE_COUNT_1_BIT,
                                                               this->typeFormat, 
                                                               this->rtImageUsage,
                                                               this->isGraphicsComputeShared,
                                                               this->poTextureImage, 
                                                               this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_3D, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   1, 
                                                   this->poTextureImageView);
                }
                else if (this->typeTexture == Vulkan_Texture_CubeMap)
                {
                    this->pWindow->createTextureRenderTargetCubeMap(this->width, 
                                                                    this->height,
                                                                    this->poMipMapCount, 
                                                                    VK_SAMPLE_COUNT_1_BIT,
                                                                    this->typeFormat, 
                                                                    this->rtImageUsage,
                                                                    this->isGraphicsComputeShared,
                                                                    this->poTextureImage, 
                                                                    this->poTextureImageMemory);
                    this->pWindow->createImageView(this->poTextureImage, 
                                                   VK_IMAGE_VIEW_TYPE_CUBE, 
                                                   this->typeFormat, 
                                                   VK_IMAGE_ASPECT_COLOR_BIT, 
                                                   this->poMipMapCount, 
                                                   6, 
                                                   this->poTextureImageView);
                }
                else
                {
                    std::string msg = "ModelTexture::LoadTexture: Wrong texture type, Create render target, name: [" + this->nameTexture + "] !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
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
            this->poTextureImageInfo.imageLayout = this->poTextureImageLayout;
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
    typedef std::map<std::string, ModelTexturePtrVector> ModelTexturePtrShaderSortMap;


    /////////////////////////// PipelineGraphics ////////////////////
    struct PipelineGraphics
    {
        Vulkan_011_Texturing* pWindow;
        std::string nameDescriptorSetLayout;
        std::vector<std::string>* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline_WireFrame;
        VkPipeline poPipeline;
        std::vector<VkDescriptorSet> poDescriptorSets;

        PipelineGraphics(Vulkan_011_Texturing* _pWindow)
            : pWindow(_pWindow)
            , nameDescriptorSetLayout("")
            , poDescriptorSetLayoutNames(nullptr)
            , poDescriptorSetLayout(VK_NULL_HANDLE)
            , poPipelineLayout(VK_NULL_HANDLE)
            , poPipeline_WireFrame(VK_NULL_HANDLE)
            , poPipeline(VK_NULL_HANDLE)
        {

        }

        ~PipelineGraphics()
        {
            Destroy();
        }

        void Destroy()
        {
            CleanupSwapChain();
        }

        void CleanupSwapChain()
        {
            this->poDescriptorSetLayoutNames = nullptr;
            this->poDescriptorSetLayout = VK_NULL_HANDLE;
            this->poPipelineLayout = VK_NULL_HANDLE;
            if (this->poPipeline_WireFrame != VK_NULL_HANDLE)
            {
                this->pWindow->destroyVkPipeline(this->poPipeline_WireFrame);
            }
            this->poPipeline_WireFrame = VK_NULL_HANDLE;

            if (this->poPipeline != VK_NULL_HANDLE)
            {
                this->pWindow->destroyVkPipeline(this->poPipeline);
            }
            this->poPipeline = VK_NULL_HANDLE;
            this->poDescriptorSets.clear();
        }  
    };


    /////////////////////////// PipelineCompute /////////////////////
    struct PipelineCompute
    {
        Vulkan_011_Texturing* pWindow;
        std::string nameDescriptorSetLayout;
        std::vector<std::string>* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline;
        VkDescriptorSet poDescriptorSet;

        TextureCopyConstants* pTextureCopy;
        VkBuffer poBuffer_TextureCopy;  
        VkDeviceMemory poBufferMemory_TextureCopy;
        void CreateTextureCopy()
        {
            DestroyTextureCopy();
            this->pTextureCopy = new TextureCopyConstants();
            VkDeviceSize bufferSize = sizeof(TextureCopyConstants);
            this->pWindow->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
        }
        void DestroyTextureCopy()
        {
            UTIL_DELETE(this->pTextureCopy)
            if (this->poBuffer_TextureCopy != VK_NULL_HANDLE)
            {
                this->pWindow->destroyBuffer(this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
            }
            this->poBuffer_TextureCopy = VK_NULL_HANDLE;
            this->poBufferMemory_TextureCopy = VK_NULL_HANDLE;
        }

        PipelineCompute(Vulkan_011_Texturing* _pWindow)
            : pWindow(_pWindow)
            , nameDescriptorSetLayout("")
            , poDescriptorSetLayoutNames(nullptr)
            , poDescriptorSetLayout(VK_NULL_HANDLE)
            , poPipelineLayout(VK_NULL_HANDLE)
            , poPipeline(VK_NULL_HANDLE)
            
            , pTextureCopy(nullptr)
            , poBuffer_TextureCopy(VK_NULL_HANDLE)
            , poBufferMemory_TextureCopy(VK_NULL_HANDLE)
        {

        }

        ~PipelineCompute()
        {
            Destroy();
        }

        void Destroy()
        {
            CleanupSwapChain();
        }

        void CleanupSwapChain()
        {
            this->poDescriptorSetLayoutNames = nullptr;
            this->poDescriptorSetLayout = VK_NULL_HANDLE;
            this->poPipelineLayout = VK_NULL_HANDLE;
            if (this->poPipeline != VK_NULL_HANDLE)
            {
                this->pWindow->destroyVkPipeline(this->poPipeline);
            }       
            this->poPipeline = VK_NULL_HANDLE;
            this->poDescriptorSet = VK_NULL_HANDLE;

            DestroyTextureCopy();
        } 
    };
    typedef std::vector<PipelineCompute*> PipelineComputePtrVector;
    typedef std::map<std::string, PipelineCompute*> PipelineComputePtrMap;


    /////////////////////////// ModelObject /////////////////////////
    struct ModelObject
    {
        ModelObject(Vulkan_011_Texturing* _pWindow)
            //Window
            : pWindow(_pWindow)

            //Name
            , nameObject("")
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

            //Pipeline Graphics

            //Pipeline Computes
            
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
            this->pPipelineGraphics = new PipelineGraphics(_pWindow);
        }
        ~ModelObject()
        {
            //Mesh
            this->pMesh = nullptr;

            //Texture
            this->mapModelTexturesShaderSort.clear();

            //Clean
            CleanupSwapChain();
            UTIL_DELETE(pPipelineGraphics)
        }

        void CleanupSwapChain()
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

            //Shader
            this->aShaderStageCreateInfos_Graphics.clear();
            this->aShaderStageCreateInfos_Computes.clear();
            this->mapShaderStageCreateInfos_Computes.clear();

            //Pipeline Graphics
            this->pPipelineGraphics->CleanupSwapChain();

            //Pipeline Computes
            count = this->aPipelineComputes.size();
            for (size_t i = 0; i < count; i++)
            {
                PipelineCompute* p = this->aPipelineComputes[i];
                UTIL_DELETE(p)
            }
            this->aPipelineComputes.clear();
        }

        void recreateSwapChain()
        {

        }

        //Window
        Vulkan_011_Texturing* pWindow;

        //Name
        int indexModel;
        std::string nameObject;
        std::string nameMesh;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isTransparent;
        bool isLighting;

        //Mesh
        ModelMesh* pMesh;

        //Texture
        ModelTexturePtrShaderSortMap mapModelTexturesShaderSort;

        //Shader
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Computes;
        VkPipelineShaderStageCreateInfoMap mapShaderStageCreateInfos_Computes;

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

        //Pipeline Graphics
        PipelineGraphics* pPipelineGraphics;

        //Pipeline Computes
        PipelineComputePtrVector aPipelineComputes;

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
        void AddTexture(const std::string& nameShaderSort, ModelTexture* pTexture)
        {
            ModelTexturePtrVector* pVector = nullptr;
            ModelTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
            {
                ModelTexturePtrVector aMTs;
                this->mapModelTexturesShaderSort[nameShaderSort] = aMTs;
                itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            }
            itFind->second.push_back(pTexture);
        }
        ModelTexture* GetTexture(const std::string& nameShaderSort, int index)
        {
            ModelTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return itFind->second.at(index);
        }
        ModelTexturePtrVector* GetTextures(const std::string& nameShaderSort)
        {
            ModelTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return &(itFind->second);
        }

    //Pipeline Computes
        void AddPipelineCompute(PipelineCompute* pPipelineCompute)
        {
            this->aPipelineComputes.push_back(pPipelineCompute);
        }
        PipelineCompute* GetPipelineCompute(int index)
        {
            assert (index >= 0 && index < (int)this->aPipelineComputes.size() && "ModelObject::GetPipelineCompute");
            return this->aPipelineComputes[index];
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
        virtual void createCustomBeforePipeline();
        virtual void createGraphicsPipeline_Custom();
        virtual void createComputePipeline_Custom();

        //DescriptorSets
        virtual void createDescriptorSets_Custom();

    //Compute/Update
        virtual void updateCompute_Custom(VkCommandBuffer& commandBuffer);

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
    VkShaderModule findShaderModule(const std::string& nameShaderModule);
    bool createPipelineShaderStageCreateInfos(const std::string& nameShaderVert,
                                              const std::string& nameShaderTesc,
                                              const std::string& nameShaderTese,
                                              const std::string& nameShaderGeom,
                                              const std::string& nameShaderFrag,
                                              const std::string& nameShaderComp,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                              VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute);
    bool createPipelineShaderStageCreateInfos(const std::string& nameShaderVert,
                                              const std::string& nameShaderTesc,
                                              const std::string& nameShaderTese,
                                              const std::string& nameShaderGeom,
                                              const std::string& nameShaderFrag,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics);
    bool createPipelineShaderStageCreateInfos(const std::string& nameShaderComp,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                              VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute);

////PipelineLayout
    void destroyPipelineLayouts();
    void createPipelineLayouts();
    VkPipelineLayout findPipelineLayout(const std::string& namePipelineLayout);

    void drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject);
};


#endif