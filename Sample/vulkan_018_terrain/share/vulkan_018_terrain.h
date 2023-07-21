/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_018_TERRAIN_H_
#define _VULKAN_018_TERRAIN_H_

#include "VulkanWindow.h"
#include "FMath.h"
using namespace LostPeter; 

class Vulkan_018_Terrain : public VulkanWindow
{
public:
    Vulkan_018_Terrain(int width, int height, String name);

public:
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
            F_Assert(index >= 0 && index < (int)this->aPipelineComputes.size() && "ModelObjectRend::GetPipelineCompute")
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
        std::vector<FVertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
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
        Vulkan_018_Terrain* pWindow;
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

        ModelObject(Vulkan_018_Terrain* _pWindow,
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