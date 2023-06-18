/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-01-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_012_SHADERING_H_
#define _VULKAN_012_SHADERING_H_

#include "VulkanWindow.h"
#include "FMath.h"
using namespace LostPeter; 

class Vulkan_012_Shadering : public VulkanWindow
{
public:
    Vulkan_012_Shadering(int width, int height, String name);

public:
    /////////////////////////// ModelMeshRaw ////////////////////////
    struct ModelMeshRaw
    {
        Vulkan_012_Shadering* pWindow;
        String nameMesh;
        String pathMesh;
        FMeshType typeMesh;
        FMeshGeometryType typeGeometryType;

        //Vertex
        FMeshVertexType poTypeVertex;
        std::vector<FVertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
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


        ModelMeshRaw(Vulkan_012_Shadering* _pWindow, 
                     const String& _nameMesh,
                     const String& _pathMesh,
                     FMeshType _typeMesh,
                     FMeshGeometryType _typeGeometryType,
                     FMeshVertexType _poTypeVertex)
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

        ~ModelMeshRaw()
        {
            Destroy();
        }

        void Destroy()
        {
            //Vertex
            this->pWindow->destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
            this->poVertexBuffer = VK_NULL_HANDLE;
            this->poVertexBufferMemory = VK_NULL_HANDLE;

            //Index
            this->pWindow->destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
            this->poIndexBuffer = VK_NULL_HANDLE;
            this->poIndexBufferMemory = VK_NULL_HANDLE;
        }


        bool LoadMesh(bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal);

    };
    typedef std::vector<ModelMeshRaw*> ModelMeshRawPtrVector;
    typedef std::map<String, ModelMeshRaw*> ModelMeshRawPtrMap;

    
    /////////////////////////// ModelObject /////////////////////////
    struct ModelObject
    {
        ModelObject(Vulkan_012_Shadering* _pWindow)
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
            , isUsedTessellation(false)
            , isUsedGeometry(false)

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
            F_DELETE(pPipelineGraphics)
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

            count = this->poBuffers_tessellationCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pWindow->destroyVkBuffer(this->poBuffers_tessellationCB[i], this->poBuffersMemory_tessellationCB[i]);
            }
            this->tessellationCBs.clear();
            this->poBuffers_tessellationCB.clear();
            this->poBuffersMemory_tessellationCB.clear();

            count = this->poBuffers_geometryCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pWindow->destroyVkBuffer(this->poBuffers_geometryCB[i], this->poBuffersMemory_geometryCB[i]);
            }
            this->geometryCBs.clear();
            this->poBuffers_geometryCB.clear();
            this->poBuffersMemory_geometryCB.clear();

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

        void recreateSwapChain()
        {

        }

        //Window
        Vulkan_012_Shadering* pWindow;

        //Name
        int indexModel;
        String nameObject;
        String nameMesh;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isTransparent;
        bool isLighting;

        //Mesh
        ModelMeshRaw* pMesh;

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
        std::vector<FMatrix4> instanceMatWorld;

        std::vector<MaterialConstants> materialCBs;
        std::vector<VkBuffer> poBuffers_materialCB;
        std::vector<VkDeviceMemory> poBuffersMemory_materialCB;

        std::vector<TessellationConstants> tessellationCBs;
        std::vector<VkBuffer> poBuffers_tessellationCB;
        std::vector<VkDeviceMemory> poBuffersMemory_tessellationCB;
        bool isUsedTessellation;

        std::vector<GeometryConstants> geometryCBs;
        std::vector<VkBuffer> poBuffers_geometryCB;
        std::vector<VkDeviceMemory> poBuffersMemory_geometryCB;
        bool isUsedGeometry;

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
        void SetMesh(ModelMeshRaw* pMesh)
        {
            this->pMesh = pMesh;
        }
        ModelMeshRaw* GetMesh()
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
            F_Assert(index >= 0 && index < (int)this->aPipelineComputes.size() && "ModelObject::GetPipelineCompute")
            return this->aPipelineComputes[index];
        }

    };
    typedef std::vector<ModelObject*> ModelObjectPtrVector;
    typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
    ModelMeshRawPtrVector m_aModelMesh;
    ModelMeshRawPtrMap m_mapModelMesh;    

    ModelTexturePtrVector m_aModelTexture;
    ModelTexturePtrMap m_mapModelTexture;

    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrVector m_aModelObjects_Render;
    ModelObjectPtrMap m_mapModelObjects;

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

////ModelMeshRaw
    void destroyModelMeshes();
    void createModelMeshes();
    ModelMeshRaw* findModelMesh(const String& nameMesh);

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

    void drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject);
};


#endif