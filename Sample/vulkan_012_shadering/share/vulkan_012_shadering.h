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
using namespace LostPeterVulkan; 

class Vulkan_012_Shadering : public VulkanWindow
{
public:
    Vulkan_012_Shadering(int width, int height, String name);

public:
    /////////////////////////// ModelMesh ///////////////////////////
    struct ModelMesh
    {
        Vulkan_012_Shadering* pWindow;
        String nameMesh;
        String pathMesh;
        FMeshType typeMesh;
        FMeshGeometryType typeGeometryType;

        //Vertex/Index
        FMeshVertexType poTypeVertex;
        std::vector<FVertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
        uint32_t poVertexCount;
        size_t poVertexBuffer_Size;
        void* poVertexBuffer_Data;
        std::vector<uint32_t> indices;
        uint32_t poIndexCount;
        size_t poIndexBuffer_Size;
        void* poIndexBuffer_Data;
        VKBufferVertex* pBufferVertex;
		VKBufferVertexIndex* pBufferVertexIndex;


        ModelMesh(Vulkan_012_Shadering* _pWindow, 
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

            //Vertex/Index
            , poTypeVertex(_poTypeVertex)
            , poVertexCount(0)
            , poVertexBuffer_Size(0)
            , poVertexBuffer_Data(nullptr)
            , poIndexCount(0)
            , poIndexBuffer_Size(0)
            , poIndexBuffer_Data(nullptr)
            , pBufferVertex(nullptr)
            , pBufferVertexIndex(nullptr)
        {

        }

        ~ModelMesh()
        {
            Destroy();
        }

        void Destroy()
        {
            //Vertex/Index
			F_DELETE(this->pBufferVertex)
			F_DELETE(this->pBufferVertexIndex)
        }


        bool LoadMesh(bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal);

    };
    typedef std::vector<ModelMesh*> ModelMeshPtrVector;
    typedef std::map<String, ModelMesh*> ModelMeshPtrMap;

    
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

			this->objectCBs.resize(MAX_OBJECT_COUNT);
			this->instanceMatWorld.resize(MAX_OBJECT_COUNT);
			this->materialCBs.resize(MAX_MATERIAL_COUNT);
			this->tessellationCBs.resize(MAX_OBJECT_COUNT);
			this->geometryCBs.resize(MAX_OBJECT_COUNT);
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
				F_DELETE(this->poBuffers_ObjectCB[i])
            }
			this->poBuffers_ObjectCB.clear();
            this->objectCBs.clear();

            count = this->poBuffers_materialCB.size();
            for (size_t i = 0; i < count; i++) 
            {
				F_DELETE(this->poBuffers_materialCB[i])
            }
			this->poBuffers_materialCB.clear();
            this->materialCBs.clear();

            count = this->poBuffers_tessellationCB.size();
            for (size_t i = 0; i < count; i++) 
            {
				F_DELETE(this->poBuffers_tessellationCB[i])
            }
			this->poBuffers_tessellationCB.clear();
            this->tessellationCBs.clear();

            count = this->poBuffers_geometryCB.size();
            for (size_t i = 0; i < count; i++) 
            {
				F_DELETE(this->poBuffers_geometryCB[i])
            }
			this->poBuffers_geometryCB.clear();
            this->geometryCBs.clear();

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
        ModelMesh* pMesh;

        //Texture
        VKTexturePtrShaderSortMap mapModelTexturesShaderSort;

        //Shader
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Computes;
        VkPipelineShaderStageCreateInfoMap mapShaderStageCreateInfos_Computes;

        //Uniform
        int countInstanceExt;
        int countInstance;

        std::vector<ObjectConstants> objectCBs;
        VKBufferUniformPtrVector poBuffers_ObjectCB;
        std::vector<FMatrix4> instanceMatWorld;

        std::vector<MaterialConstants> materialCBs;
        VKBufferUniformPtrVector poBuffers_materialCB;

        std::vector<TessellationConstants> tessellationCBs;
        VKBufferUniformPtrVector poBuffers_tessellationCB;
        bool isUsedTessellation;

        std::vector<GeometryConstants> geometryCBs;
        VKBufferUniformPtrVector poBuffers_geometryCB;
        bool isUsedGeometry;

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


    ////Mesh
        void SetMesh(ModelMesh* pMesh)
        {
            this->pMesh = pMesh;
        }
        ModelMesh* GetMesh()
        {
            return this->pMesh;
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

    //Pipeline Computes
        void AddPipelineCompute(VKPipelineCompute* pPipelineCompute)
        {
            this->aPipelineComputes.push_back(pPipelineCompute);
        }
        VKPipelineCompute* GetPipelineCompute(int index)
        {
            F_Assert(index >= 0 && index < (int)this->aPipelineComputes.size() && "ModelObject::GetPipelineCompute")
            return this->aPipelineComputes[index];
        }

    };
    typedef std::vector<ModelObject*> ModelObjectPtrVector;
    typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
    ModelMeshPtrVector m_aModelMesh;
    ModelMeshPtrMap m_mapModelMesh;    

    VKTexturePtrVector m_aModelTexture;
    VKTexturePtrMap m_mapModelTexture;

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
        virtual void updateCompute_BeforeRender_Custom(VkCommandBuffer& commandBuffer);

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
    void destroyMeshes();
    void createMeshes();
    ModelMesh* findMesh(const String& nameMesh);

////Texture
    void destroyTextures();
    void createTextures();
    VKTexture* findTexture(const String& nameTexture);

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

    void drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject);
};


#endif