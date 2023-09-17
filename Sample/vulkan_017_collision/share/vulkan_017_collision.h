/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-07-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_017_COLLISION_H_
#define _VULKAN_017_COLLISION_H_

#include "VulkanWindow.h"
#include "FMath.h"
using namespace LostPeter; 

class Vulkan_017_Collision : public VulkanWindow
{
public:
    Vulkan_017_Collision(int width, int height, String name);

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
        bool isGeometry3D;
        bool isGeometryLine;
        bool isGeometryFlat;

        //Texture
        TexturePtrShaderSortMap mapModelTexturesShaderSort;

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

        std::vector<LineFlat2DObjectConstants> objectCBs_LineFlat2D;
        std::vector<VkBuffer> poBuffers_ObjectCB_LineFlat2D;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB_LineFlat2D;

        std::vector<LineFlat3DObjectConstants> objectCBs_LineFlat3D;
        std::vector<VkBuffer> poBuffers_ObjectCB_LineFlat3D;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB_LineFlat3D;

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
            , isGeometry3D(true)
            , isGeometryLine(false)
            , isGeometryFlat(false)

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
            this->pPipelineGraphics = new PipelineGraphics("PipelineG-Model");
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
            //Object
            size_t count = this->poBuffers_ObjectCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
            }
            this->objectCBs.clear();
            this->poBuffers_ObjectCB.clear();
            this->poBuffersMemory_ObjectCB.clear();

            //ObjectLineFlat2D
            count = this->poBuffers_ObjectCB_LineFlat2D.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB_LineFlat2D[i], this->poBuffersMemory_ObjectCB_LineFlat2D[i]);
            }
            this->objectCBs_LineFlat2D.clear();
            this->poBuffers_ObjectCB_LineFlat2D.clear();
            this->poBuffersMemory_ObjectCB_LineFlat2D.clear();

            //ObjectLineFlat3D
            count = this->poBuffers_ObjectCB_LineFlat3D.size();
            for (size_t i = 0; i < count; i++) 
            {
                this->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB_LineFlat3D[i], this->poBuffersMemory_ObjectCB_LineFlat3D[i]);
            }
            this->objectCBs_LineFlat3D.clear();
            this->poBuffers_ObjectCB_LineFlat3D.clear();
            this->poBuffersMemory_ObjectCB_LineFlat3D.clear();

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

        void RecreateSwapChain();

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
        MeshSubPtrVector aMeshSubs;
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

        std::vector<LineFlat2DObjectConstants> objectCBs_LineFlat2D;
        std::vector<VkBuffer> poBuffers_ObjectCB_LineFlat2D;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB_LineFlat2D;

        std::vector<LineFlat3DObjectConstants> objectCBs_LineFlat3D;
        std::vector<VkBuffer> poBuffers_ObjectCB_LineFlat3D;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB_LineFlat3D;

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
        Vulkan_017_Collision* pWindow;
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
        bool isInstanceDynamic;

        //Mesh
        Mesh* pMesh;
        std::vector<int> aMeshSubUsed;

        //ModelObjectRend
        ModelObjectRendPtrVector aRends;
        ModelObjectRendIndirect* pRendIndirect;

        ModelObject(Vulkan_017_Collision* _pWindow,
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
            , isInstanceDynamic(true)

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
    bool m_isDrawIndirect;
    bool m_isDrawIndirectMulti;

    //Line2D
    ModelObjectRend* pRend_LineLine2D;
    ModelObjectRend* pRend_LineTriangle2D;
    ModelObjectRend* pRend_LineQuad2D;
    ModelObjectRend* pRend_LineGrid2D;
    ModelObjectRend* pRend_LineQuadConvex2D;
    ModelObjectRend* pRend_LineQuadConcave2D;
    ModelObjectRend* pRend_LineCircle2D;
    //Flat2D
    ModelObjectRend* pRend_FlatTriangle2D;
    ModelObjectRend* pRend_FlatQuad2D;
    ModelObjectRend* pRend_FlatQuadConvex2D;
    ModelObjectRend* pRend_FlatQuadConcave2D;
    ModelObjectRend* pRend_FlatCircle2D;
    //Line3D
    ModelObjectRend* pRend_LineLine3D;
    ModelObjectRend* pRend_LineTriangle3D;
    ModelObjectRend* pRend_LineQuad3D;
    ModelObjectRend* pRend_LineGrid3D;
    ModelObjectRend* pRend_LineQuadConvex3D;
    ModelObjectRend* pRend_LineQuadConcave3D;
    ModelObjectRend* pRend_LineCircle3D;
    ModelObjectRend* pRend_LineAABB3D;
    ModelObjectRend* pRend_LineSphere3D;
    ModelObjectRend* pRend_LineCylinder3D;
    ModelObjectRend* pRend_LineCapsule3D;
    ModelObjectRend* pRend_LineCone3D;
    ModelObjectRend* pRend_LineTorus3D;
    //Flat3D
    ModelObjectRend* pRend_FlatTriangle3D;
    ModelObjectRend* pRend_FlatQuad3D;
    ModelObjectRend* pRend_FlatQuadConvex3D;
    ModelObjectRend* pRend_FlatQuadConcave3D;
    ModelObjectRend* pRend_FlatCircle3D;
    ModelObjectRend* pRend_FlatAABB3D;
    ModelObjectRend* pRend_FlatSphere3D;
    ModelObjectRend* pRend_FlatCylinder3D;
    ModelObjectRend* pRend_FlatCapsule3D;
    ModelObjectRend* pRend_FlatCone3D;
    ModelObjectRend* pRend_FlatTorus3D;
    //Entity
    ModelObjectRend* pRend_EntityTriangle;
    ModelObjectRend* pRend_EntityQuad;
    ModelObjectRend* pRend_EntityGrid;
    ModelObjectRend* pRend_EntityCircle;
    ModelObjectRend* pRend_EntityAABB;
    ModelObjectRend* pRend_EntitySphere;
    ModelObjectRend* pRend_EntityGeoSphere;
    ModelObjectRend* pRend_EntityCylinder;
    ModelObjectRend* pRend_EntityCapsule;
    ModelObjectRend* pRend_EntityCone;
    ModelObjectRend* pRend_EntityTorus;
    ModelObjectRend* pRend_EntitySkyBox;
    ModelObjectRend* pRend_EntitySkyDome;
    ModelObjectRend* pRend_EntityTerrain;

    VkDescriptorSetLayoutVector m_aVkDescriptorSetLayouts;
    VkDescriptorSetLayoutMap m_mapVkDescriptorSetLayout;
    std::map<String, StringVector> m_mapName2Layouts;
    
    VkShaderModuleVector m_aVkShaderModules;
    VkShaderModuleMap m_mapVkShaderModules;

    VkPipelineLayoutVector m_aVkPipelineLayouts;
    VkPipelineLayoutMap m_mapVkPipelineLayouts;

public:
    virtual void OnMouseLeftDown(double x, double y);
    virtual void OnMouseLeftUp(double x, double y);
    virtual void OnMouseMove(int button, double x, double y);
    virtual void OnMouseHover(double x, double y);

public:
    static FColor s_color_Select;
    static FColor s_color_Hover;

    //Line2D
    static FColor s_color_LineLine2D;               //LineLine2D
    static FColor s_color_LineTriangle2D;           //LineTriangle2D
    static FColor s_color_LineQuad2D;               //LineQuad2D
    static FColor s_color_LineGrid2D;               //LineGrid2D
    static FColor s_color_LineQuadConvex2D;         //LineQuad2D Convex
    static FColor s_color_LineQuadConcave2D;        //LineQuad2D Concave
    static FColor s_color_LineCircle2D;             //LineCircle2D

    //Flat2D
    static FColor s_color_FlatTriangle2D;           //FlatTriangle2D
    static FColor s_color_FlatQuad2D;               //FlatQuad2D
    static FColor s_color_FlatQuadConvex2D;         //FlatQuad2D Convex
    static FColor s_color_FlatQuadConcave2D;        //FlatQuad2D Concave
    static FColor s_color_FlatCircle2D;             //FlatCircle2D

    //Line3D
    static FColor s_color_LineLine3D;               //LineLine3D
    static FColor s_color_LineTriangle3D;           //LineTriangle3D
    static FColor s_color_LineQuad3D;               //LineQuad3D
    static FColor s_color_LineGrid3D;               //LineGrid3D
    static FColor s_color_LineQuadConvex3D;         //LineQuad3D Convex
    static FColor s_color_LineQuadConcave3D;        //LineQuad3D Concave
    static FColor s_color_LineCircle3D;             //LineCircle3D
    static FColor s_color_LineAABB3D;               //LineAABB3D
    static FColor s_color_LineSphere3D;             //LineSphere3D
    static FColor s_color_LineCylinder3D;           //LineCylinder3D
    static FColor s_color_LineCapsule3D;            //LineCapsule3D
    static FColor s_color_LineCone3D;               //LineCone3D
    static FColor s_color_LineTorus3D;              //LineTorus3D

    //Flat3D
    static FColor s_color_FlatTriangle3D;           //FlatTriangle3D
    static FColor s_color_FlatQuad3D;               //FlatQuad3D
    static FColor s_color_FlatQuadConvex3D;         //FlatQuad3D Convex
    static FColor s_color_FlatQuadConcave3D;        //FlatQuad3D Concave
    static FColor s_color_FlatCircle3D;             //FlatCircle3D
    static FColor s_color_FlatAABB3D;               //FlatAABB3D
    static FColor s_color_FlatSphere3D;             //FlatSphere3D
    static FColor s_color_FlatCylinder3D;           //FlatCylinder3D
    static FColor s_color_FlatCapsule3D;            //FlatCapsule3D
    static FColor s_color_FlatCone3D;               //FlatCone3D
    static FColor s_color_FlatTorus3D;              //FlatTorus3D

    //Entity
    static FColor s_color_EntityTriangle;           //EntityTriangle
    static FColor s_color_EntityQuad;               //EntityQuad
    static FColor s_color_EntityGrid;               //EntityGrid
    static FColor s_color_EntityCircle;             //EntityCircle
    static FColor s_color_EntityAABB;               //EntityAABB
    static FColor s_color_EntitySphere;             //EntitySphere
    static FColor s_color_EntityGeoSphere;          //EntityGeoSphere
    static FColor s_color_EntityCylinder;           //EntityCylinder
    static FColor s_color_EntityCapsule;            //EntityCapsule
    static FColor s_color_EntityCone;               //EntityCone
    static FColor s_color_EntityTorus;              //EntityTorus
    static FColor s_color_EntitySkyBox;             //EntitySkyBox
    static FColor s_color_EntitySkyDome;            //EntitySkyDome
    static FColor s_color_EntityTerrain;            //EntityTerrain

    FPointI Convert2PointI(double x, double y);
    FPointF Convert2PointF(double x, double y);
    void IsCollision(double x, double y, bool isHover);

    //Line2D
    bool IsCollision_LineLine2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineTriangle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineQuad2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineGrid2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineQuadConvex2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineQuadConcave2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineCircle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);

    //Flat2D
    bool IsCollision_FlatTriangle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatQuad2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatQuadConvex2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatQuadConcave2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatCircle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover);

    //Line3D
    bool IsCollision_LineLine3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineTriangle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineQuad3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineGrid3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineQuadConvex3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineQuadConcave3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineCircle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineAABB3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineSphere3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineCylinder3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineCapsule3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineCone3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_LineTorus3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);

    //Flat3D
    bool IsCollision_FlatTriangle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatQuad3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatQuadConvex3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatQuadConcave3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatCircle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatAABB3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatSphere3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatCylinder3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatCapsule3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatCone3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_FlatTorus3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);

    //Entity
    bool IsCollision_EntityTriangle(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityQuad(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityGrid(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityCircle(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityAABB(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntitySphere(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityGeoSphere(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityCylinder(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityCapsule(double x, double y, const FRay& ray,  ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityCone(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityTorus(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntitySkyBox(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntitySkyDome(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    bool IsCollision_EntityTerrain(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover);
    
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

    void drawModelObjectRendIndirects(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends);
    void drawModelObjectRendIndirect(VkCommandBuffer& commandBuffer, ModelObjectRendIndirect* pRendIndirect);

    void drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends);
    void drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend);
};


#endif