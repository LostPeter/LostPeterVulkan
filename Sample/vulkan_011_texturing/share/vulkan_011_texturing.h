/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-01-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_011_TEXTURING_H_
#define _VULKAN_011_TEXTURING_H_

#include "VulkanWindow.h"
#include "FMath.h"
using namespace LostPeterVulkan; 

class Vulkan_011_Texturing : public VulkanWindow
{
public:
    Vulkan_011_Texturing(int width, int height, String name);

public:
    /////////////////////////// ModelMesh ///////////////////////////
    struct ModelMesh
    {
        Vulkan_011_Texturing* pWindow;
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


        ModelMesh(Vulkan_011_Texturing* _pWindow, 
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
            , isUsedTessellation(false)
			, poBuffer_TextureCopy(nullptr)
			, isUsedCompute(false)
			, frameRand(0)

            //Pipeline Graphics
			, poStatePipelineGraphics(nullptr)
			, nameDescriptorSetLayout("")

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
        ~ModelObject()
        {
            //Mesh
            this->pMesh = nullptr;

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

        //Window
        Vulkan_011_Texturing* pWindow;

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

		std::vector<TextureCopyConstants> textureCopyCBs;
		VKBufferUniform* poBuffer_TextureCopy;
		bool isUsedCompute;
		int frameRand;

        //Pipeline Graphics
        VKStatePipelineGraphics* poStatePipelineGraphics;
		String nameDescriptorSetLayout;

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
        void AddStatePipelineCompute(VKStatePipelineCompute* pStatePipelineCompute)
        {
            this->aStatePipelineComputes.push_back(pStatePipelineCompute);
        }
        VKStatePipelineCompute* GetStatePipelineCompute(int index)
        {
            F_Assert (index >= 0 && index < (int)this->aStatePipelineComputes.size() && "ModelObject::GetStatePipelineCompute")
            return this->aStatePipelineComputes[index];
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

    DescriptorSetLayoutPtrVector m_aDescriptorSetLayouts;
    DescriptorSetLayoutPtrMap m_mapDescriptorSetLayouts;

    VKShaderPtrVector m_aShaders;
	VKShaderPtrMap m_mapShaders;

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
    DescriptorSetLayout* findDescriptorSetLayout(const String& nameDescriptorSetLayout);

////Shader
    void destroyShaders();
    void createShaders();
    VKShader* findShader(const String& nameShader);

////Draw
    void drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject);
};


#endif