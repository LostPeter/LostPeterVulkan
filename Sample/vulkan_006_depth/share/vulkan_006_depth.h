/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_006_DEPTH_H_
#define _VULKAN_006_DEPTH_H_

#include "VulkanWindow.h"
using namespace LostPeterVulkan; 

class Vulkan_006_Depth : public VulkanWindow
{
public:
    Vulkan_006_Depth(int width, int height, String name);

public:
    struct ModelObject
    {
        ModelObject(Vulkan_006_Depth* _pWindow)
            //Common
            : pWindow(_pWindow)

            //Name
            , nameModel("")
            , pathModel("")
            , pathTexture("")
            , isShow(true)
            , isRotate(true)
            , isWireFrame(false)
            , isNoDepthTest(false)
            , isNoDepthWrite(false)
            , isNoDepthTestWrite(false)

            //Vertex/Index
            , poVertexCount(0)
            , poVertexBuffer_Size(0)
            , poVertexBuffer_Data(nullptr)
            , poIndexCount(0)
            , poIndexBuffer_Size(0)
            , poIndexBuffer_Data(nullptr)
            , pBufferVertex(nullptr)
            , pBufferVertexIndex(nullptr)

            //Uniform
            , poMatWorld(1.0f)
            
            //Texture
            , poMipMapCount(0)
            , poTextureImage(VK_NULL_HANDLE)
            , poTextureImageMemory(VK_NULL_HANDLE)
            , poTextureImageView(VK_NULL_HANDLE)
            , poTextureSampler(VK_NULL_HANDLE)

            //Pipeline
			, poStatePipelineGraphics(nullptr)
			, poStatePipelineGraphics_NoDepthTest(nullptr)
			, poStatePipelineGraphics_NoDepthWrite(nullptr)
			, poStatePipelineGraphics_NoDepthTestWrite(nullptr)

			//DescriptorSets
			, nameDescriptorSetLayout("Pass-Object-Material-Instance-TextureFS")
			, pDescriptorSetLayout(nullptr)

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

			this->pDescriptorSetLayout = new DescriptorSetLayout();
			this->pDescriptorSetLayout->Init(this->nameDescriptorSetLayout);
        }
        ~ModelObject()
        {
            //Vertex/Index
			F_DELETE(this->pBufferVertex)
			F_DELETE(this->pBufferVertexIndex)

            //Texture
            this->pWindow->destroyVkImage(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
            this->pWindow->destroyVkImageSampler(this->poTextureSampler);
            this->poTextureImage = VK_NULL_HANDLE;
            this->poTextureImageMemory = VK_NULL_HANDLE;
            this->poTextureImageView = VK_NULL_HANDLE;
            this->poTextureSampler = VK_NULL_HANDLE;

			CleanupSwapChain();
            
			F_DELETE(this->pDescriptorSetLayout)
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

			//Pipeline
			F_DELETE(this->poStatePipelineGraphics)
			F_DELETE(this->poStatePipelineGraphics_NoDepthTest)
			F_DELETE(this->poStatePipelineGraphics_NoDepthWrite)
			F_DELETE(this->poStatePipelineGraphics_NoDepthTestWrite)

			this->pDescriptorSetLayout->DestroyLayoutAndDescriptorSet();
		}

		void RecreateSwapChain()
        {

        }

        //Common
        Vulkan_006_Depth* pWindow;

        //Name
        String nameModel;
        String pathModel;
        String pathTexture;
        bool isShow;
        bool isRotate;
        bool isWireFrame;
        bool isNoDepthTest;
        bool isNoDepthWrite;
        bool isNoDepthTestWrite;

		//Vertex/Index
        std::vector<FVertex_Pos3Color4Tex2> vertices;
        uint32_t poVertexCount;
        size_t poVertexBuffer_Size;
        void* poVertexBuffer_Data;
        std::vector<uint32_t> indices;
        uint32_t poIndexCount;
        size_t poIndexBuffer_Size;
        void* poIndexBuffer_Data;
		VKBufferVertex* pBufferVertex;
		VKBufferVertexIndex* pBufferVertexIndex;

        //Uniform
        std::vector<ObjectConstants> objectCBs;
		VKBufferUniformPtrVector poBuffers_ObjectCB;
        FMatrix4 poMatWorld;

        //Texture
        uint32_t poMipMapCount;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;

        //Pipeline
		VKStatePipelineGraphics* poStatePipelineGraphics;
		VKStatePipelineGraphics* poStatePipelineGraphics_NoDepthTest;
		VKStatePipelineGraphics* poStatePipelineGraphics_NoDepthWrite;
		VKStatePipelineGraphics* poStatePipelineGraphics_NoDepthTestWrite;

		//DescriptorSets
		String nameDescriptorSetLayout;
		DescriptorSetLayout* pDescriptorSetLayout;

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
    };
    typedef std::vector<ModelObject*> ModelObjectPtrVector;
    typedef std::map<String, ModelObject*> ModelObjectPtrMap;

public:
    ModelObjectPtrVector m_aModelObjects;
    ModelObjectPtrMap m_mapModelObjects;

	FMeshVertexType typeVertex;
	String shaderVertex_Path;
	String shaderFragment_Path;
	VKShader* pShaderVertex;
	VKShader* pShaderFragment;

protected:
    //Create Pipeline
    
    //Load Assets
        //Camera
        virtual void createCamera();

        //Geometry/Texture
        virtual void loadModel_Custom();
            bool loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal);
            bool loadModel_Texture(ModelObject* pModelObject);

        //ConstBuffers
        virtual void createCustomCB();

        //Pipeline
        virtual void createGraphicsPipeline_Custom();

        //DescriptorSets
        virtual void createDescriptorSets_Custom();

    //Render/Update
        virtual void updateCBs_Custom();

        virtual bool beginRenderImgui();
            virtual void modelConfig();
            
        virtual void endRenderImgui();

        virtual void drawMeshDefault_Custom(VkCommandBuffer& commandBuffer);

    //cleanup
        virtual void cleanupCustom();

		virtual void cleanupSwapChain_Custom();
        virtual void recreateSwapChain_Custom();
};


#endif