/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-26
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_018_SUBPASS_H_
#define _VULKAN_018_SUBPASS_H_

#include "VulkanWindow.h"
#include "FMath.h"
using namespace LostPeterVulkan; 

class Vulkan_018_SubPass : public VulkanWindow
{
public:
    Vulkan_018_SubPass(int width, int height, String name);

public: 
    /////////////////////////// ModelObjectRend /////////////////////
    struct ModelObject;
    struct ModelObjectRend
    {
        String nameObjectRend;
        ModelObject* pModelObject;
        MeshSub* pMeshSub;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isLighting;
        bool isTransparent;

        //Texture
        TexturePtrShaderSortMap mapModelTexturesShaderSort;

        //Shader
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Computes;
        VkPipelineShaderStageCreateInfoMap mapShaderStageCreateInfos_Computes;

        //Uniform
        int countInstance;

        std::vector<ObjectConstants> objectCBs;
        VkBufferVector poBuffers_ObjectCB;
        VkDeviceMemoryVector poBuffersMemory_ObjectCB;
        std::vector<FMatrix4> instanceMatWorld;

        std::vector<MaterialConstants> materialCBs;
        VkBufferVector poBuffers_materialCB;
        VkDeviceMemoryVector poBuffersMemory_materialCB;

        std::vector<TessellationConstants> tessellationCBs;
        VkBufferVector poBuffers_tessellationCB;
        VkDeviceMemoryVector poBuffersMemory_tessellationCB;
        bool isUsedTessellation;

        //Pipeline Graphics
        VKPipelineGraphics* pPipelineGraphics;

        //Pipeline Computes
        VKPipelineComputePtrVector aPipelineComputes;
        
        //State
        VkDynamicStateVector cfg_aDynamicStates;
        VkPrimitiveTopology cfg_vkPrimitiveTopology;
        VkFrontFace cfg_vkFrontFace;
        VkPolygonMode cfg_vkPolygonMode;
        VkCullModeFlagBits cfg_vkCullModeFlagBits;
        VkBool32 cfg_isDepthBiasEnable;
        float cfg_DepthBiasConstantFactor;
        float cfg_DepthBiasClamp;
        float cfg_DepthBiasSlopeFactor;
        float cfg_LineWidth;
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
                        MeshSub* _pMeshSub)
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
            , cfg_isDepthBiasEnable(VK_FALSE)
            , cfg_DepthBiasConstantFactor(0.0f)
            , cfg_DepthBiasClamp(0.0f)
            , cfg_DepthBiasSlopeFactor(0.0f)
            , cfg_LineWidth(1.0f)
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
            cfg_aDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
            cfg_aDynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

            this->pPipelineGraphics = new VKPipelineGraphics("PipelineGraphics-Model");
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
            size_t count = this->aPipelineComputes.size();
            for (size_t i = 0; i < count; i++)
            {
                VKPipelineCompute* p = this->aPipelineComputes[i];
                F_DELETE(p)
            }
            this->aPipelineComputes.clear();
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
                VKPipelineCompute* p = this->aPipelineComputes[i];
                p->CleanupSwapChain();
            }
        }

        void RecreateSwapChain()
        {

        }   

    ////Textures
        void AddTexture(const String& nameShaderSort, Texture* pTexture)
        {
            TexturePtrVector* pVector = nullptr;
            TexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
            {
                TexturePtrVector aMTs;
                this->mapModelTexturesShaderSort[nameShaderSort] = aMTs;
                itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            }
            itFind->second.push_back(pTexture);
        }
        Texture* GetTexture(const String& nameShaderSort, int index)
        {
            TexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return itFind->second.at(index);
        }
        TexturePtrVector* GetTextures(const String& nameShaderSort)
        {
            TexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return &(itFind->second);
        }

    //Pipeline Computes
        void AddPipelineCompute(VKPipelineCompute* pPipelineCompute)
        {
            this->aPipelineComputes.push_back(pPipelineCompute);
        }
        VKPipelineCompute* GetPipelineCompute(int index)
        {
            F_Assert(index >= 0 && index < (int)this->aPipelineComputes.size() && "ModelObjectRend::GetPipelineCompute")
            return this->aPipelineComputes[index];
        }

    };
    typedef std::vector<ModelObjectRend*> ModelObjectRendPtrVector;
    typedef std::map<String, ModelObjectRend*> ModelObjectRendPtrMap;


    /////////////////////////// ModelObject /////////////////////////
    struct ModelObject
    {
        //Window
        Vulkan_018_SubPass* pWindow;
        int index;

        //Name
        int indexModel;
        String nameObject;
        String nameMesh;
        bool isShow;
        bool isWireFrame;
        bool isRotate;
        bool isLighting;

        int countInstanceExt;
        int countInstance;

        //Mesh
        Mesh* pMesh;
        std::vector<int> aMeshSubUsed;

        //ModelObjectRend
        ModelObjectRendPtrVector aRends;

        ModelObject(Vulkan_018_SubPass* _pWindow,
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

            , countInstanceExt(0)
            , countInstance(1)

            //Mesh
            , pMesh(nullptr)

            //ModelObjectRend
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
        }

        void CleanupSwapChain()
        {
            size_t count = this->aRends.size();
            for (size_t i = 0; i < count; i++)
            {
                ModelObjectRend* pRend = this->aRends[i];
                pRend->CleanupSwapChain();
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
        }

    ////Mesh
        void SetMesh(Mesh* pMesh)
        {
            this->pMesh = pMesh;
        }
        Mesh* GetMesh()
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


    /////////////////////////// SubPassRenderPass ///////////////////
    class SubPassRenderPass : public Base
    {
    public:
        SubPassRenderPass(const String& _nameRenderPass);
        virtual ~SubPassRenderPass();

    public:
        //Attachment Color
        VkImageVector aColorImageLists;
        VkDeviceMemoryVector aColorImageMemoryLists;
        VkImageViewVector aColorImageViewLists;
        VkSampler sampler;
        FVector4Vector aColors;
        VkDescriptorImageInfoVector aImageInfos;

        //RenderPass
        VkRenderPass poRenderPass_SubPass;

        //FrameBuffer
        VkFramebuffer poFrameBuffer_SubPass;


    public:
        void Destroy();

        virtual void Init(uint32_t width, 
                          uint32_t height, 
                          int countColorAttachment);
    
    public:
        void CleanupSwapChain();
        void RecreateSwapChain();

    private:
        
    };


public:
    MeshPtrVector m_aModelMesh;
    MeshPtrMap m_mapModelMesh;    

    TexturePtrVector m_aModelTexture;
    TexturePtrMap m_mapModelTexture;

    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrMap m_mapModelObjects;
    ModelObjectRendPtrVector m_aModelObjectRends_All;
    ModelObjectRendPtrVector m_aModelObjectRends_Opaque;
    ModelObjectRendPtrVector m_aModelObjectRends_Transparent;

    SubPassRenderPass* m_pSubPassRenderPass;

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

        //RenderPass
        virtual void createRenderPass_Custom();

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
            void createDescriptorSets_Graphics(StringVector* pDescriptorSetLayoutNames,
                                               VkDescriptorSetVector& listDescriptorSets, 
                                               ModelObjectRend* pRend); 
    //Render/Update
        virtual void updateCBs_Custom();
        
        virtual void updateRenderPass_CustomBeforeDefault(VkCommandBuffer& commandBuffer);
            void drawCustomBeforeDefault(VkCommandBuffer& commandBuffer);
                void drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends);
                void drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend);
                void drawModelObjectRendPipeline(VkCommandBuffer& commandBuffer, 
                                                ModelObjectRend* pRend,
                                                MeshSub* pMeshSub,  
                                                VkPipelineLayout pipelineLayout,
                                                VkDescriptorSetVector& descriptorSets,
                                                VkPipeline pipeline, 
                                                VkPipeline pipeline_WireFrame);

            virtual void drawMeshDefault_CustomBeforeImgui(VkCommandBuffer& commandBuffer);

        virtual bool beginRenderImgui();
            virtual void cameraReset();
            virtual void modelConfig();

        virtual void endRenderImgui();


    //cleanup
        virtual void cleanupCustom();

        virtual void cleanupSwapChain_Custom();
        virtual void recreateSwapChain_Custom();

private:
    void rebuildInstanceCBs(bool isCreateVkBuffer);

////Mesh
    void destroyMeshes();
    void createMeshes();
    Mesh* findMesh(const String& nameMesh);

////Texture
    void destroyTextures();
    void createTextures();
    Texture* findTexture(const String& nameTexture);

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
};


#endif