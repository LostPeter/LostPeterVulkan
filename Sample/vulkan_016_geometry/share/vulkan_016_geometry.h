/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_016_GEOMETRY_H_
#define _VULKAN_016_GEOMETRY_H_

#include "VulkanWindow.h"
#include "FMath.h"
using namespace LostPeter; 

class Vulkan_016_Geometry : public VulkanWindow
{
public:
    Vulkan_016_Geometry(int width, int height, String name);

public:
    /////////////////////////// ModelMeshSub ////////////////////////
    struct ModelMesh;
    struct ModelMeshSub
    {
        ModelMesh* pMesh;
        String nameMeshSub;
        String nameOriginal;
        int indexMeshSub;

        //Vertex
        VulkanVertexType poTypeVertex;
        std::vector<Vertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<Vertex_Pos3Color4Normal3Tex4> vertices_Pos3Color4Normal3Tex4;
        std::vector<Vertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
        std::vector<Vertex_Pos3Color4Normal3Tangent3Tex4> vertices_Pos3Color4Normal3Tangent3Tex4;
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


        ModelMeshSub(ModelMesh* _pMesh, 
                     const String& _nameMeshSub,
                     const String& _nameOriginal,
                     int _indexMeshSub,
                     VulkanVertexType _poTypeVertex)
            : pMesh(_pMesh)
            , nameMeshSub(_nameMeshSub)
            , nameOriginal(_nameOriginal)
            , indexMeshSub(_indexMeshSub)

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
            Destroy();
        }

        void Destroy();

        uint32_t GetVertexSize() 
        {
            if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
                return sizeof(Vertex_Pos3Color4Normal3Tex2);
            else if(this->vertices_Pos3Color4Normal3Tex4.size() > 0)
                return sizeof(Vertex_Pos3Color4Normal3Tex4);
            else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
                return sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2);
            else if (this->vertices_Pos3Color4Normal3Tangent3Tex4.size() > 0)
                return sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4);
            else
            {
                assert(false && "ModelMeshSub::GetVertexSize: wrong vertex type !");
                return 0;
            }
        }
        uint32_t GetIndexSize()
        {
            return sizeof(uint32_t);
        }

        bool CreateMeshSub(MeshData& meshData, bool isTranformLocal, const FMatrix4& matTransformLocal);

        void WriteVertexData(std::vector<Vertex_Pos3Color4Normal3Tex2>& aPos3Color4Normal3Tex4,
                             std::vector<Vertex_Pos3Color4Normal3Tangent3Tex2>& aPos3Color4Normal3Tangent3Tex4);
        void WriteIndexData(std::vector<uint32_t>& indexData);
    };
    typedef std::vector<ModelMeshSub*> ModelMeshSubPtrVector;
    typedef std::map<String, ModelMeshSub*> ModelMeshSubPtrMap;


    /////////////////////////// ModelMesh ///////////////////////////
    struct ModelMesh
    {
        Vulkan_016_Geometry* pWindow;
        String nameMesh;
        String pathMesh;
        VulkanMeshType typeMesh;
        VulkanMeshGeometryType typeGeometryType;
        VulkanVertexType typeVertex;

        ModelMeshSubPtrVector aMeshSubs;
        ModelMeshSubPtrMap mapMeshSubs;

        ModelMesh(Vulkan_016_Geometry* _pWindow, 
                  const String& _nameMesh,
                  const String& _pathMesh,
                  VulkanMeshType _typeMesh,
                  VulkanMeshGeometryType _typeGeometryType,
                  VulkanVertexType _typeVertex)
            : pWindow(_pWindow)
            , nameMesh(_nameMesh)
            , pathMesh(_pathMesh)
            , typeMesh(_typeMesh)
            , typeGeometryType(_typeGeometryType)
            , typeVertex(_typeVertex)
        {

        }

        ~ModelMesh()
        {
            Destroy();
        }

        void Destroy()
        {
            int count = (int)this->aMeshSubs.size();
            for (int i = 0; i < count; i++)
            {
                ModelMeshSub* pMeshSub = this->aMeshSubs[i];
                pMeshSub->Destroy();
                delete pMeshSub;
            }
            this->aMeshSubs.clear();
            this->mapMeshSubs.clear();
        }

        bool AddMeshSub(ModelMeshSub* pMeshSub);
        bool LoadMesh(bool isFlipY, bool isTranformLocal, const FMatrix4& matTransformLocal);
    };
    typedef std::vector<ModelMesh*> ModelMeshPtrVector;
    typedef std::map<String, ModelMesh*> ModelMeshPtrMap;


    /////////////////////////// ModelTexture ////////////////////////
    struct ModelTexture
    {
        Vulkan_016_Geometry* pWindow;
        String nameTexture;
        StringVector aPathTexture;
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
        FVector4 rtColorDefault;
        bool rtIsSetColor; 
        VkImageUsageFlags rtImageUsage;


        ModelTexture(Vulkan_016_Geometry* _pWindow, 
                     const String& _nameTexture,
                     VulkanTextureType _typeTexture,
                     bool _isRenderTarget,
                     bool _isGraphicsComputeShared,
                     VkFormat _typeFormat,
                     VulkanTextureFilterType _typeFilter,
                     VulkanTextureAddressingType _typeAddressing,
                     VulkanTextureBorderColorType _typeBorderColor,
                     const StringVector& _aPathTexture)
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
                this->pWindow->destroyVkBuffer(this->stagingBuffer, this->stagingBufferMemory);
            }
            this->pWindow->destroyVkImage(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
            this->poTextureImage = VK_NULL_HANDLE;
            this->poTextureImageMemory = VK_NULL_HANDLE;
            this->poTextureImageView = VK_NULL_HANDLE;
            this->pWindow->destroyVkImageSampler(this->poTextureSampler);
            this->poTextureSampler = VK_NULL_HANDLE;
            F_DELETE_T(pDataRGBA)
        }

        int RandomTextureIndex()
        {
            if (this->typeTexture == Vulkan_Texture_2DArray)
            {
                int count = (int)this->aPathTexture.size();
                return FMath::Rand(0, count - 1);
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                                                  this->typeFormat, true, 
                                                  this->poMipMapCount, 
                                                  this->poTextureImage, 
                                                  this->poTextureImageMemory);
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
                                                     VK_IMAGE_VIEW_TYPE_CUBE, 
                                                     this->typeFormat, 
                                                     VK_IMAGE_ASPECT_COLOR_BIT, 
                                                     this->poMipMapCount, 
                                                     (int)this->aPathTexture.size(), 
                                                     this->poTextureImageView);
                }   
                else
                {
                    String msg = "ModelTexture::LoadTexture: Wrong texture type, Create from file, name: [" + this->nameTexture + "] !";
                    F_LogError(msg.c_str());
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
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
                    this->pWindow->createVkImageView(this->poTextureImage, 
                                                     VK_IMAGE_VIEW_TYPE_CUBE, 
                                                     this->typeFormat, 
                                                     VK_IMAGE_ASPECT_COLOR_BIT, 
                                                     this->poMipMapCount, 
                                                     6, 
                                                     this->poTextureImageView);
                }
                else
                {
                    String msg = "ModelTexture::LoadTexture: Wrong texture type, Create render target, name: [" + this->nameTexture + "] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }

            this->pWindow->createVkSampler(this->typeFilter, 
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
    typedef std::map<String, ModelTexture*> ModelTexturePtrMap;
    typedef std::map<String, ModelTexturePtrVector> ModelTexturePtrShaderSortMap;


    /////////////////////////// PipelineGraphics ////////////////////
    struct PipelineGraphics
    {
        Vulkan_016_Geometry* pWindow;
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline_WireFrame;
        VkPipeline poPipeline;
        VkDescriptorSetVector poDescriptorSets;

        PipelineGraphics(Vulkan_016_Geometry* _pWindow)
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
        Vulkan_016_Geometry* pWindow;
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline;
        VkDescriptorSet poDescriptorSet;
        
        ModelTexture* pTextureSource;
        ModelTexture* pTextureTarget;
        TextureCopyConstants* pTextureCopy;
        VkBuffer poBuffer_TextureCopy;  
        VkDeviceMemory poBufferMemory_TextureCopy;
        int frameRand;
        void CreateTextureCopy()
        {
            DestroyTextureCopy();
            this->pTextureCopy = new TextureCopyConstants();
            VkDeviceSize bufferSize = sizeof(TextureCopyConstants);
            this->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
        }
        void DestroyTextureCopy()
        {
            F_DELETE(this->pTextureCopy)
            if (this->poBuffer_TextureCopy != VK_NULL_HANDLE)
            {
                this->pWindow->destroyVkBuffer(this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
            }
            this->poBuffer_TextureCopy = VK_NULL_HANDLE;
            this->poBufferMemory_TextureCopy = VK_NULL_HANDLE;
        }

        PipelineCompute(Vulkan_016_Geometry* _pWindow)
            : pWindow(_pWindow)
            , nameDescriptorSetLayout("")
            , poDescriptorSetLayoutNames(nullptr)
            , poDescriptorSetLayout(VK_NULL_HANDLE)
            , poPipelineLayout(VK_NULL_HANDLE)
            , poPipeline(VK_NULL_HANDLE)
            , pTextureSource(nullptr)
            , pTextureTarget(nullptr)
            
            , pTextureCopy(nullptr)
            , poBuffer_TextureCopy(VK_NULL_HANDLE)
            , poBufferMemory_TextureCopy(VK_NULL_HANDLE)
            , frameRand(0)
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
    typedef std::map<String, PipelineCompute*> PipelineComputePtrMap;

    
    /////////////////////////// ModelObjectRend /////////////////////
    struct ModelObject;
    struct ModelObjectRend
    {
        String nameObjectRend;
        ModelObject* pModelObject;
        ModelMeshSub* pMeshSub;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isLighting;
        bool isTransparent;

        //Texture
        ModelTexturePtrShaderSortMap mapModelTexturesShaderSort;

        //Shader
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Computes;
        VkPipelineShaderStageCreateInfoMap mapShaderStageCreateInfos_Computes;

        //Uniform
        int countInstance;

        std::vector<ObjectConstants> objectCBs;
        std::vector<VkBuffer> poBuffers_ObjectCB;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB;
        std::vector<FMatrix4> instanceMatWorld;

        std::vector<MaterialConstants> materialCBs;
        std::vector<VkBuffer> poBuffers_materialCB;
        std::vector<VkDeviceMemory> poBuffersMemory_materialCB;

        std::vector<TessellationConstants> tessellationCBs;
        std::vector<VkBuffer> poBuffers_tessellationCB;
        std::vector<VkDeviceMemory> poBuffersMemory_tessellationCB;
        bool isUsedTessellation;

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


        ModelObjectRend(const String& _nameObjectRend,
                        ModelObject* _pModelObject,
                        ModelMeshSub* _pMeshSub)
            : nameObjectRend(_nameObjectRend)
            , pModelObject(_pModelObject)
            , pMeshSub(_pMeshSub)
            , isShow(true)
            , isWireFrame(false)
            , isRotate(false)
            , isLighting(true)
            , isTransparent(false)

            //Uniform
            , countInstance(1)
            , isUsedTessellation(false)

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
            this->pPipelineGraphics = new PipelineGraphics(_pModelObject->pWindow);
        }
        ~ModelObjectRend()
        {
            //MeshSub
            this->pMeshSub = nullptr;

            //Texture
            this->mapModelTexturesShaderSort.clear();

            //Clean
            CleanupSwapChain();
            F_DELETE(pPipelineGraphics)
        }

        void CleanupSwapChain()
        {
            //Uniform
            size_t count = this->poBuffers_ObjectCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
            }
            this->objectCBs.clear();
            this->poBuffers_ObjectCB.clear();
            this->poBuffersMemory_ObjectCB.clear();

            count = this->poBuffers_materialCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_materialCB[i], this->poBuffersMemory_materialCB[i]);
            }
            this->materialCBs.clear();
            this->poBuffers_materialCB.clear();
            this->poBuffersMemory_materialCB.clear();

            count = this->poBuffers_tessellationCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_tessellationCB[i], this->poBuffersMemory_tessellationCB[i]);
            }
            this->tessellationCBs.clear();
            this->poBuffers_tessellationCB.clear();
            this->poBuffersMemory_tessellationCB.clear();

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
                F_DELETE(p)
            }
            this->aPipelineComputes.clear();
        }

        void RecreateSwapChain()
        {

        }   

    ////Textures
        void AddTexture(const String& nameShaderSort, ModelTexture* pTexture)
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
        ModelTexture* GetTexture(const String& nameShaderSort, int index)
        {
            ModelTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return itFind->second.at(index);
        }
        ModelTexturePtrVector* GetTextures(const String& nameShaderSort)
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
            assert (index >= 0 && index < (int)this->aPipelineComputes.size() && "ModelObjectRend::GetPipelineCompute");
            return this->aPipelineComputes[index];
        }

    };
    typedef std::vector<ModelObjectRend*> ModelObjectRendPtrVector;
    typedef std::map<String, ModelObjectRend*> ModelObjectRendPtrMap;


    /////////////////////////// ModelObjectRendIndirect /////////////
    struct ModelObjectRendIndirect
    {
        String nameObjectRendIndirect;
        ModelObjectRendPtrVector aRends;
        ModelMeshSubPtrVector aMeshSubs;
        ModelObjectRend* pRend;

        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isLighting;
        bool isTransparent;

        //Vertex
        std::vector<Vertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<Vertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
        size_t poVertexCount;
        size_t poVertexBuffer_Size;
        void* poVertexBuffer_Data;
        VkBuffer poVertexBuffer;
        VkDeviceMemory poVertexBufferMemory;
        
        //Index
        std::vector<uint32_t> indices;
        size_t poIndexCount;
        size_t poIndexBuffer_Size;
        void* poIndexBuffer_Data;
        VkBuffer poIndexBuffer;
        VkDeviceMemory poIndexBufferMemory;

        //Uniform
        std::vector<ObjectConstants> objectCBs;
        std::vector<VkBuffer> poBuffers_ObjectCB;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB;

        std::vector<MaterialConstants> materialCBs;
        std::vector<VkBuffer> poBuffers_materialCB;
        std::vector<VkDeviceMemory> poBuffersMemory_materialCB;

        std::vector<TessellationConstants> tessellationCBs;
        std::vector<VkBuffer> poBuffers_tessellationCB;
        std::vector<VkDeviceMemory> poBuffersMemory_tessellationCB;
        bool isUsedTessellation;

        //VkDescriptorSets
        VkDescriptorSetVector poDescriptorSets;

        //IndirectCommand 
        std::vector<VkDrawIndexedIndirectCommand> indirectCommandCBs;
        uint32_t countIndirectDraw;
        VkBuffer poBuffer_indirectCommandCB;
        VkDeviceMemory poBuffersMemory_indirectCommandCB;


        ModelObjectRendIndirect(const String& _nameObjectRendIndirect)
            : nameObjectRendIndirect(_nameObjectRendIndirect)
            , pRend(nullptr)

            , isShow(true)
            , isWireFrame(false)
            , isRotate(false)
            , isLighting(true)
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

            //IndirectCommand
            , countIndirectDraw(0)
            , poBuffer_indirectCommandCB(VK_NULL_HANDLE)
            , poBuffersMemory_indirectCommandCB(VK_NULL_HANDLE)
        {
            
        }

        ~ModelObjectRendIndirect()
        {
            Destroy();
        }

        void Destroy();

        void CleanupSwapChain();

        void RecreateSwapChain()
        {

        }   

        void SetupVertexIndexBuffer(const ModelObjectRendPtrVector& _aRends);
        void SetupUniformIndirectCommandBuffer();

        void UpdateUniformBuffer();
        void UpdateIndirectCommandBuffer();
    };
    typedef std::vector<ModelObjectRendIndirect*> ModelObjectRendIndirectPtrVector;
    typedef std::map<String, ModelObjectRendIndirect*> ModelObjectRendIndirectPtrMap;
    

    /////////////////////////// ModelObject /////////////////////////
    struct ModelObject
    {
        //Window
        Vulkan_016_Geometry* pWindow;
        int index;

        //Name
        int indexModel;
        String nameObject;
        String nameMesh;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isLighting;
        bool isIndirectDraw;
        bool isIndirectDrawMulti;

        int countInstanceExt;
        int countInstance;

        //Mesh
        ModelMesh* pMesh;
        std::vector<int> aMeshSubUsed;

        //ModelObjectRend
        ModelObjectRendPtrVector aRends;
        ModelObjectRendIndirect* pRendIndirect;

        ModelObject(Vulkan_016_Geometry* _pWindow,
                    int _index)
            //Window
            : pWindow(_pWindow)
            , index(_index)

            //Name
            , nameObject("")
            , nameMesh("")
            , isShow(true)
            , isWireFrame(false)
            , isRotate(false)
            , isLighting(true)
            , isIndirectDraw(false)
            , isIndirectDrawMulti(true)

            , countInstanceExt(0)
            , countInstance(1)

            //Mesh
            , pMesh(nullptr)

            //ModelObjectRend
            , pRendIndirect(nullptr)
        {
            
        }
        ~ModelObject()
        {
            
        }

        void Destroy()
        {
            //Mesh
            this->pMesh = nullptr;
            this->aMeshSubUsed.clear();

            //ObjectRend
            CleanupSwapChain();
            size_t count = this->aRends.size();
            for (size_t i = 0; i < count; i++)
            {
                ModelObjectRend* pRend = this->aRends[i];
                delete pRend;
            }
            this->aRends.clear();
            F_DELETE(pRendIndirect)
        }

        void CleanupSwapChain()
        {
            size_t count = this->aRends.size();
            for (size_t i = 0; i < count; i++)
            {
                ModelObjectRend* pRend = this->aRends[i];
                pRend->CleanupSwapChain();
            }
            if (pRendIndirect != nullptr)
            {
                pRendIndirect->CleanupSwapChain();
            }
        }

        void RecreateSwapChain()
        {
            size_t count = this->aRends.size();
            for (size_t i = 0; i < count; i++)
            {
                ModelObjectRend* pRend = this->aRends[i];
                pRend->RecreateSwapChain();
            }
            if (pRendIndirect != nullptr)
            {
                pRendIndirect->RecreateSwapChain();
            }
        }

    ////Mesh
        void SetMesh(ModelMesh* pMesh)
        {
            this->pMesh = pMesh;
        }
        ModelMesh* GetMesh()
        {
            return this->pMesh;
        }

    ////ModelObjectRend
        void AddObjectRend(ModelObjectRend* pRend)
        {
            this->aRends.push_back(pRend);
        }

    };
    typedef std::vector<ModelObject*> ModelObjectPtrVector;
    typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
    ModelMeshPtrVector m_aModelMesh;
    ModelMeshPtrMap m_mapModelMesh;    

    ModelTexturePtrVector m_aModelTexture;
    ModelTexturePtrMap m_mapModelTexture;

    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrMap m_mapModelObjects;
    ModelObjectRendPtrVector m_aModelObjectRends_All;
    ModelObjectRendPtrVector m_aModelObjectRends_Opaque;
    ModelObjectRendPtrVector m_aModelObjectRends_Transparent;
    bool m_isDrawIndirect;
    bool m_isDrawIndirectMulti;

    VkDescriptorSetLayoutVector m_aVkDescriptorSetLayouts;
    VkDescriptorSetLayoutMap m_mapVkDescriptorSetLayout;
    std::map<String, StringVector> m_mapName2Layouts;
    
    VkShaderModuleVector m_aVkShaderModules;
    VkShaderModuleMap m_mapVkShaderModules;

    VkPipelineLayoutVector m_aVkPipelineLayouts;
    VkPipelineLayoutMap m_mapVkPipelineLayouts;

protected:
    //Create Pipeline
        virtual void setUpEnabledFeatures();

        //DescriptorSetLayout
        virtual void createDescriptorSetLayout_Custom();

    //Load Assets
        //Camera
        virtual void createCamera();

        //Geometry/Texture
        virtual void loadModel_Custom();
            void createIndirectCommands();

        //ConstBuffers
        virtual void createCustomCB();

        //Pipeline
        virtual void createCustomBeforePipeline();
        virtual void createGraphicsPipeline_Custom();
        virtual void createComputePipeline_Custom();

        //DescriptorSets
        virtual void createDescriptorSets_Custom();
            void createDescriptorSets_Graphics(VkDescriptorSetVector& poDescriptorSets, 
                                               ModelObjectRend* pRend, 
                                               ModelObjectRendIndirect* pRendIndirect);
            void createDescriptorSets_Compute(PipelineCompute* pPipelineCompute, 
                                              ModelObjectRend* pRend);

    //Compute/Update
        virtual void updateCompute_Custom(VkCommandBuffer& commandBuffer);

    //Render/Update
        virtual void updateCBs_Custom();

        virtual void updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer);

        virtual bool beginRenderImgui();
            virtual void cameraReset();
            virtual void modelConfig();

        virtual void endRenderImgui();

        virtual void drawMeshDefault_Custom(VkCommandBuffer& commandBuffer);

    //cleanup
        virtual void cleanupCustom();

        virtual void cleanupSwapChain_Custom();
        virtual void recreateSwapChain_Custom();

private:
    void rebuildInstanceCBs(bool isCreateVkBuffer);

////ModelMesh
    void destroyModelMeshes();
    void createModelMeshes();
    ModelMesh* findModelMesh(const String& nameMesh);

////ModelTexture
    void destroyModelTextures();
    void createModelTextures();
    ModelTexture* findModelTexture(const String& nameTexture);

////DescriptorSetLayout
    void destroyDescriptorSetLayouts();
    void createDescriptorSetLayouts();
    VkDescriptorSetLayout findDescriptorSetLayout(const String& nameDescriptorSetLayout);
    StringVector* findDescriptorSetLayoutNames(const String& nameDescriptorSetLayout);

////ShaderModule
    void destroyShaderModules();
    void createShaderModules();
    VkShaderModule findShaderModule(const String& nameShaderModule);
    bool createPipelineShaderStageCreateInfos(const String& nameShaderVert,
                                              const String& nameShaderTesc,
                                              const String& nameShaderTese,
                                              const String& nameShaderGeom,
                                              const String& nameShaderFrag,
                                              const String& nameShaderComp,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                              VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute);
    bool createPipelineShaderStageCreateInfos(const String& nameShaderVert,
                                              const String& nameShaderTesc,
                                              const String& nameShaderTese,
                                              const String& nameShaderGeom,
                                              const String& nameShaderFrag,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics);
    bool createPipelineShaderStageCreateInfos(const String& nameShaderComp,
                                              VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                              VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute);

////PipelineLayout
    void destroyPipelineLayouts();
    void createPipelineLayouts();
    VkPipelineLayout findPipelineLayout(const String& namePipelineLayout);

    void drawModelObjectRendIndirects(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends);
    void drawModelObjectRendIndirect(VkCommandBuffer& commandBuffer, ModelObjectRendIndirect* pRendIndirect);

    void drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends);
    void drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend);
};


#endif