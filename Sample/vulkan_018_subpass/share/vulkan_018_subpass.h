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
        VKTexturePtrShaderSortMap mapModelTexturesShaderSort;

        //Shader
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Computes;
        VkPipelineShaderStageCreateInfoMap mapShaderStageCreateInfos_Computes;

        //Uniform
        int countInstance;

        std::vector<ObjectConstants> objectCBs;
        VKBufferUniformPtrVector poBuffers_ObjectCB;
        std::vector<FMatrix4> instanceMatWorld;

        std::vector<MaterialConstants> materialCBs;
        VKBufferUniformPtrVector poBuffers_materialCB;

        std::vector<TessellationConstants> tessellationCBs;
        VKBufferUniformPtrVector poBuffers_tessellationCB;
        bool isUsedTessellation;

		std::vector<TextureCopyConstants> textureCopyCBs;
		VKBufferUniform* poBuffer_TextureCopy;
		bool isUsedCompute;
		int frameRand;

		//hasNextSubpass
		bool hasNextSubpass;

        //Pipeline Graphics
        VKStatePipelineGraphics* poStatePipelineGraphics;
        String nameDescriptorSetLayout;
		VKStatePipelineGraphics* poStatePipelineGraphics2;
        String nameDescriptorSetLayout2;

        //Pipeline Computes
        VKStatePipelineComputePtrVector aStatePipelineComputes;
        
        //State
        VkDynamicStateVector poDynamicStates;
        VkPrimitiveTopology poPrimitiveTopology;
        VkFrontFace poFrontFace;
        VkPolygonMode poPolygonMode;
        VkCullModeFlagBits poCullModeFlagBits;
        VkBool32 poDepthBiasEnabled;
        float poDepthBiasConstantFactor;
        float poDepthBiasClamp;
        float poDepthBiasSlopeFactor;
        float poLineWidth;

		VkBool32 poDepthEnabled;
        VkBool32 poDepthIsTest;
        VkBool32 poDepthIsWrite; 
        VkCompareOp poDepthCompareOp; 

        VkBool32 poStencilEnabled;
        VkStencilOpState poStencilOpFront; 
        VkStencilOpState poStencilOpBack; 

        VkBool32 poBlendEnabled;
        VkBlendFactor poBlendColorFactorSrc; 
        VkBlendFactor poBlendColorFactorDst; 
        VkBlendOp poBlendColorOp;
        VkBlendFactor poBlendAlphaFactorSrc;
        VkBlendFactor poBlendAlphaFactorDst; 
        VkBlendOp poBlendAlphaOp;
        VkColorComponentFlags poColorWriteMask;


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
			, poBuffer_TextureCopy(nullptr)
			, isUsedCompute(false)
			, frameRand(0)

			//hasNextSubpass
			, hasNextSubpass(false)

            //Pipeline Graphics
			, poStatePipelineGraphics(nullptr)
			, nameDescriptorSetLayout("")
			, poStatePipelineGraphics2(nullptr)
			, nameDescriptorSetLayout2("")

            //Pipeline Computes

            //State
            , poPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            , poFrontFace(VK_FRONT_FACE_CLOCKWISE)
            , poPolygonMode(VK_POLYGON_MODE_FILL)
            , poCullModeFlagBits(VK_CULL_MODE_BACK_BIT)
            , poDepthBiasEnabled(VK_FALSE)
            , poDepthBiasConstantFactor(0.0f)
            , poDepthBiasClamp(0.0f)
            , poDepthBiasSlopeFactor(0.0f)
            , poLineWidth(1.0f)
			, poDepthEnabled(VK_TRUE)
            , poDepthIsTest(VK_TRUE)
            , poDepthIsWrite(VK_TRUE)
            , poDepthCompareOp(VK_COMPARE_OP_LESS_OR_EQUAL)  
            , poStencilEnabled(VK_FALSE)
            , poBlendEnabled(VK_FALSE)
            , poBlendColorFactorSrc(VK_BLEND_FACTOR_ONE)
            , poBlendColorFactorDst(VK_BLEND_FACTOR_ZERO)
            , poBlendColorOp(VK_BLEND_OP_ADD)
            , poBlendAlphaFactorSrc(VK_BLEND_FACTOR_ONE)
            , poBlendAlphaFactorDst(VK_BLEND_FACTOR_ZERO)
            , poBlendAlphaOp(VK_BLEND_OP_ADD)
            , poColorWriteMask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
        {
            this->poDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
            this->poDynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

			this->objectCBs.resize(MAX_OBJECT_COUNT);
			this->instanceMatWorld.resize(MAX_OBJECT_COUNT);
			this->materialCBs.resize(MAX_MATERIAL_COUNT);
			this->tessellationCBs.resize(MAX_OBJECT_COUNT);
        }
        ~ModelObjectRend()
        {
            //MeshSub
            this->pMeshSub = nullptr;

            //Texture
            this->mapModelTexturesShaderSort.clear();

            //Clean
            CleanupSwapChain();
            
			//Pipeline Computes
            size_t count = this->aStatePipelineComputes.size();
            for (size_t i = 0; i < count; i++)
            {
                VKStatePipelineCompute* p = this->aStatePipelineComputes[i];
                F_DELETE(p)
            }
			this->aStatePipelineComputes.clear();
        }

        void CleanupSwapChain()
        {
            //Uniform
            size_t count = this->poBuffers_ObjectCB.size();
            for (size_t i = 0; i < count; i++) 
            {
				F_DELETE(this->poBuffers_ObjectCB[i])
            }
			this->poBuffers_ObjectCB.clear();

            count = this->poBuffers_materialCB.size();
            for (size_t i = 0; i < count; i++) 
            {
				F_DELETE(this->poBuffers_materialCB[i])
            }
			this->poBuffers_materialCB.clear();

            count = this->poBuffers_tessellationCB.size();
            for (size_t i = 0; i < count; i++) 
            {
				F_DELETE(this->poBuffers_tessellationCB[i])
            }
			this->poBuffers_tessellationCB.clear();

			F_DELETE(this->poBuffer_TextureCopy)

            //Shader
            this->aShaderStageCreateInfos_Graphics.clear();
            this->aShaderStageCreateInfos_Computes.clear();
            this->mapShaderStageCreateInfos_Computes.clear();

            //Pipeline Graphics
            F_DELETE(this->poStatePipelineGraphics)
			F_DELETE(this->poStatePipelineGraphics2)

            //Pipeline Computes
            count = this->aStatePipelineComputes.size();
            for (size_t i = 0; i < count; i++)
            {
                VKStatePipelineCompute* p = this->aStatePipelineComputes[i];
                p->Destroy();
            }
        }

        void RecreateSwapChain()
        {

        }   

    ////Textures
        void AddTexture(const String& nameShaderSort, VKTexture* pTexture)
        {
            VKTexturePtrVector* pVector = nullptr;
            VKTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
            {
                VKTexturePtrVector aMTs;
                this->mapModelTexturesShaderSort[nameShaderSort] = aMTs;
                itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            }
            itFind->second.push_back(pTexture);
        }
        VKTexture* GetTexture(const String& nameShaderSort, int index)
        {
            VKTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return itFind->second.at(index);
        }
        VKTexturePtrVector* GetTextures(const String& nameShaderSort)
        {
            VKTexturePtrShaderSortMap::iterator itFind = this->mapModelTexturesShaderSort.find(nameShaderSort);
            if (itFind == this->mapModelTexturesShaderSort.end())
                return nullptr;
            return &(itFind->second);
        }

    ////Pipeline Computes
        void AddStatePipelineCompute(VKStatePipelineCompute* pStatePipelineCompute)
        {
            this->aStatePipelineComputes.push_back(pStatePipelineCompute);
        }
        VKStatePipelineCompute* GetStatePipelineCompute(int index)
        {
            F_Assert(index >= 0 && index < (int)this->aStatePipelineComputes.size() && "ModelObjectRend::GetStatePipelineCompute")
            return this->aStatePipelineComputes[index];
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

    VKTexturePtrVector m_aModelTexture;
    VKTexturePtrMap m_mapModelTexture;

    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrMap m_mapModelObjects;
    ModelObjectRendPtrVector m_aModelObjectRends_All;
    ModelObjectRendPtrVector m_aModelObjectRends_Opaque;
    ModelObjectRendPtrVector m_aModelObjectRends_Transparent;

    SubPassRenderPass* m_pSubPassRenderPass;

    DescriptorSetLayoutPtrVector m_aDescriptorSetLayouts;
    DescriptorSetLayoutPtrMap m_mapDescriptorSetLayouts;
    
    VKShaderPtrVector m_aShaders;
	VKShaderPtrMap m_mapShaders;

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
            void createDescriptorSets_Graphics(VKStatePipelineGraphics* pStatePipelineGraphics, 
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
                                                 VKStatePipelineGraphics* pStatePipelineGraphics);

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
    VKTexture* findTexture(const String& nameTexture);

////DescriptorSetLayout
    void destroyDescriptorSetLayouts();
    void createDescriptorSetLayouts();
    DescriptorSetLayout* findDescriptorSetLayout(const String& nameDescriptorSetLayout);

////Shader
    void destroyShaders();
    void createShaders();
    VKShader* findShader(const String& nameShader);
};


#endif