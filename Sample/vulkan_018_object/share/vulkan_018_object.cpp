/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-08-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_018_object.h"


/////////////////////////// Object //////////////////////////////
static const int g_Object_Count = 8;
static const char* g_Object_Configs[3 * g_Object_Count] = 
{
    //Object Name                      //Mesh Group         //Mesh Name                                                                    
    "object_skybox",                   "5001",              "cube", //object_skybox
    "object_mountain",                 "5002",              "mountain", //object_mountain   

    "object_rock",                     "5002",              "rock", //object_rock   
    "object_cliff",                    "5002",              "cliff", //object_cliff   

    "object_tree",                     "5002",              "tree", //object_tree        
    "object_tree_spruce",              "5002",              "tree_spruce", //object_tree_spruce
    
    "object_grass",                    "5002",              "grass", //object_grass        
    "object_flower",                   "5002",              "flower", //object_flower

};
static const char* g_Object_MeshSubsUsed[g_Object_Count] =
{
    "0", //object_skybox
    "0", //object_mountain

    "0", //object_rock
    "0", //object_cliff

    "0;1", //object_tree        
    "0;1", //object_tree_spruce

    "1;4;6;9", //object_grass
    "0;2;4;6;8;9;10;11", //object_flower

};  

static float g_Object_InstanceGap = 3.0f;
static int g_Object_InstanceExtCount[g_Object_Count] =
{
    0, //object_skybox
    0, //object_mountain 

    4, //object_rock 
    4, //object_cliff 

    4, //object_tree 
    4, //object_tree_spruce 

    4, //object_grass 
    4, //object_flower 

};
static bool g_Object_IsShows[] = 
{
    true, //object_skybox
    true, //object_mountain

    true, //object_rock
    true, //object_cliff

    true, //object_tree
    true, //object_tree_spruce

    true, //object_grass
    true, //object_flower

};
static bool g_Object_IsRotates[g_Object_Count] =
{
    false, //object_skybox
    false, //object_mountain

    false, //object_rock
    false, //object_cliff

    false, //object_tree
    false, //object_tree_spruce

    false, //object_grass
    false, //object_flower

};
static bool g_Object_IsLightings[g_Object_Count] =
{
    true, //object_skybox
    true, //object_mountain

    true, //object_rock
    true, //object_cliff

    true, //object_tree
    true, //object_tree_spruce

    true, //object_grass
    true, //object_flower

};
static bool g_Object_IsIndirectDraw[g_Object_Count] =
{
    false, //object_skybox
    false, //object_mountain

    false, //object_rock
    false, //object_cliff

    false, //object_tree
    false, //object_tree_spruce

    false, //object_grass
    true, //object_flower

};


/////////////////////////// ObjectRend //////////////////////////
static const int g_ObjectRend_Count = 20;
static const char* g_ObjectRend_Configs[7 * g_ObjectRend_Count] = 
{
    //Object Rend Name                     //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS            //Texture FS                                                                         //Texture CS
    "object_skybox-1",                     "",                     "",                              "",                         "",                    "6006:texturecubemap",                                                               "", //object_skybox-1
    "object_mountain-1",                   "",                     "",                              "",                         "",                    "6002:mountain_diffuse;6002:mountain_normal",                                        "", //object_mountain-1

    "object_rock-1",                       "",                     "",                              "",                         "",                    "6002:rock_diffuse;6002:rock_normal",                                                "", //object_rock-1
    "object_cliff-1",                      "",                     "",                              "",                         "",                    "6002:cliff_diffuse;6002:cliff_normal",                                              "", //object_cliff-1

    "object_tree-1",                       "",                     "",                              "",                         "",                    "6002:tree_diffuse",                                                                 "", //object_tree-1
    "object_tree-2",                       "",                     "",                              "",                         "",                    "6002:tree_diffuse",                                                                 "", //object_tree-2
    "object_tree_spruce-1",                "",                     "",                              "",                         "",                    "6002:tree_spruce_diffuse",                                                          "", //object_tree_spruce-1
    "object_tree_spruce-2",                "",                     "",                              "",                         "",                    "6002:tree_spruce_diffuse",                                                          "", //object_tree_spruce-2

    "object_grass-1",                      "",                     "",                              "",                         "",                    "6002:grass_field",                                                                  "", //object_grass-1
    "object_grass-2",                      "",                     "",                              "",                         "",                    "6002:grass_wheat",                                                                  "", //object_grass-2
    "object_grass-3",                      "",                     "",                              "",                         "",                    "6002:grass_tall",                                                                   "", //object_grass-3
    "object_grass-4",                      "",                     "",                              "",                         "",                    "6002:grass_field",                                                                  "", //object_grass-4
    "object_flower-1",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-1
    "object_flower-2",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-2
    "object_flower-3",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-3
    "object_flower-4",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-4
    "object_flower-5",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-5
    "object_flower-6",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-6
    "object_flower-7",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-7
    "object_flower-8",                     "",                     "",                              "",                         "",                    "6002:flower_atlas",                                                                 "", //object_flower-8

};
static const char* g_ObjectRend_NameShaderModules[6 * g_ObjectRend_Count] = 
{
    //vert                                                  //tesc                                          //tese                                      //geom                      //frag                                                  //comp
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                         "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //object_skybox-1
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_mountain-1
    
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_rock-1
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_cliff-1

    "vert_standard_mesh_opaque_tree_alphatest_lit",         "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tree_alphatest_lit",         "", //object_tree-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_tree-2
    "vert_standard_mesh_opaque_tree_alphatest_lit",         "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tree_alphatest_lit",         "", //object_tree_spruce-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_tree_spruce-2

    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-1
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-2
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-3
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-4
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-1
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-2
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-3
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-4
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-5
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-6
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-7
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-8

};
static const char* g_ObjectRend_NameDescriptorSetLayouts[2 * g_ObjectRend_Count] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_skybox-1
    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //object_mountain-1

    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //object_rock-1
    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //object_cliff-1

    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree-2
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree_spruce-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree_spruce-2

    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-2
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-3
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-4
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-2
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-3
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-4
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-5
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-6
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-7
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-8

};
static FVector3 g_ObjectRend_Tranforms[3 * g_ObjectRend_Count] = 
{   
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(  500.0f,    500.0f,    500.0f), //object_skybox-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_mountain-1
 
    FVector3(   0,  0.0,   1.5),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_rock-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    0.1f,      0.1f,      0.1f), //object_cliff-1

    FVector3(   0,  0.0, -10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree-1
    FVector3(   0,  0.0, -10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree-2
    FVector3(   0,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree_spruce-1
    FVector3(   0,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_tree_spruce-2

    FVector3(   0,  0.0,   2.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-1
    FVector3(   0,  0.0,   2.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-2
    FVector3(   0,  0.0,   5.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-3
    FVector3(   0,  0.0,   5.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_grass-4
    FVector3(   0,  0.0,  -1.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-1
    FVector3(   0,  0.0,  -1.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-2
    FVector3(   0,  0.0,  -2.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-3
    FVector3(   0,  0.0,  -2.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-4
    FVector3(   0,  0.0,  -3.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-5
    FVector3(   0,  0.0,  -3.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-6
    FVector3(   0,  0.0,  -4.0),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-7
    FVector3(   0,  0.0,  -4.5),    FVector3(     0,  0,  0),    FVector3(   50.0f,     50.0f,     50.0f), //object_flower-8

};
static bool g_ObjectRend_IsTransparents[g_ObjectRend_Count] = 
{
    false, //object_skybox-1
    false, //object_mountain-1

    false, //object_rock-1
    false, //object_cliff-1

    false, //object_tree-1
    false, //object_tree-2
    false, //object_tree_spruce-1
    false, //object_tree_spruce-2

    false, //object_grass-1
    false, //object_grass-2
    false, //object_grass-3
    false, //object_grass-4
    false, //object_flower-1
    false, //object_flower-2
    false, //object_flower-3
    false, //object_flower-4
    false, //object_flower-5
    false, //object_flower-6
    false, //object_flower-7
    false, //object_flower-8

};
static bool g_ObjectRend_IsTopologyPatchLists[g_ObjectRend_Count] =
{
    false, //object_skybox-1
    false, //object_mountain-1
    
    false, //object_rock-1
    false, //object_cliff-1

    false, //object_tree-1
    false, //object_tree-2
    false, //object_tree_spruce-1
    false, //object_tree_spruce-2

    false, //object_grass-1
    false, //object_grass-2
    false, //object_grass-3
    false, //object_grass-4
    false, //object_flower-1
    false, //object_flower-2
    false, //object_flower-3
    false, //object_flower-4
    false, //object_flower-5
    false, //object_flower-6
    false, //object_flower-7
    false, //object_flower-8

};



/////////////////////////// ModelObjectRend /////////////////////


/////////////////////////// ModelObjectRendIndirect /////////////
void Vulkan_018_Object::ModelObjectRendIndirect::Destroy()
{
    //Vertex
    this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
    this->poVertexBuffer = VK_NULL_HANDLE;
    this->poVertexBufferMemory = VK_NULL_HANDLE;

    //Index
    this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
    this->poIndexBuffer = VK_NULL_HANDLE;
    this->poIndexBufferMemory = VK_NULL_HANDLE;

    CleanupSwapChain();

    this->aRends.clear();
    this->aMeshSubs.clear();
    this->pRend = nullptr;
}

void Vulkan_018_Object::ModelObjectRendIndirect::CleanupSwapChain()
{
    size_t count = 0;

    //1> Uniform Buffer
    count = this->poBuffers_ObjectCB.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
    }
    this->objectCBs.clear();
    this->poBuffers_ObjectCB.clear();
    this->poBuffersMemory_ObjectCB.clear();

    count = this->poBuffers_materialCB.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_materialCB[i], this->poBuffersMemory_materialCB[i]);
    }
    this->materialCBs.clear();
    this->poBuffers_materialCB.clear();
    this->poBuffersMemory_materialCB.clear();

    count = this->poBuffers_tessellationCB.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_tessellationCB[i], this->poBuffersMemory_tessellationCB[i]);
    }
    this->tessellationCBs.clear();
    this->poBuffers_tessellationCB.clear();
    this->poBuffersMemory_tessellationCB.clear();

    //2> VkDescriptorSets
    this->poDescriptorSets.clear();

    //3> IndirectCommand Buffer
    if (this->poBuffer_indirectCommandCB != VK_NULL_HANDLE)
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffer_indirectCommandCB, this->poBuffersMemory_indirectCommandCB);
    }
    this->indirectCommandCBs.clear();
    this->poBuffer_indirectCommandCB = VK_NULL_HANDLE;
    this->poBuffersMemory_indirectCommandCB = VK_NULL_HANDLE;
}

void Vulkan_018_Object::ModelObjectRendIndirect::SetupVertexIndexBuffer(const ModelObjectRendPtrVector& _aRends)
{
    F_Assert(_aRends.size() > 0 && "Vulkan_018_Object::ModelObjectRendIndirect::SetupVertexIndexBuffer")
    this->aRends.clear();
    this->aRends = _aRends;
    this->pRend = _aRends[0];

    //1> Vertex/Index
    this->aMeshSubs.clear();
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        this->aMeshSubs.push_back(pMeshSub);
    }

    this->vertices_Pos3Color4Normal3Tex2.clear();
    this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
    this->indices.clear();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        pMeshSub->WriteVertexData(this->vertices_Pos3Color4Normal3Tex2, this->vertices_Pos3Color4Normal3Tangent3Tex2);
        pMeshSub->WriteIndexData(this->indices);
    }
    
    //Vertex
    if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
    {
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
    }
    else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
    {
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
    }
    else
    {
        F_Assert(false && "Vulkan_018_Object::ModelObjectRendIndirect::SetupVertexIndexBuffer: No vertex data !")
    }
    this->poIndexCount = this->indices.size();
    this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
    this->poIndexBuffer_Data =  &this->indices[0];

    //2> createVertexBuffer
    this->pRend->pModelObject->pWindow->createVertexBuffer(this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBuffer, this->poVertexBufferMemory);

    //3> createIndexBuffer
    if (this->poIndexBuffer_Size > 0 &&
        this->poIndexBuffer_Data != nullptr)
    {
        this->pRend->pModelObject->pWindow->createIndexBuffer(this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBuffer, this->poIndexBufferMemory);
    }
}

void Vulkan_018_Object::ModelObjectRendIndirect::SetupUniformIndirectCommandBuffer()
{
    VkDeviceSize bufferSize;
    size_t count_sci = this->pRend->pModelObject->pWindow->poSwapChainImages.size();

    //1> Uniform Buffer
    {
        //ObjectConstants
        bufferSize = sizeof(ObjectConstants) * this->objectCBs.size();
        this->poBuffers_ObjectCB.resize(count_sci);
        this->poBuffersMemory_ObjectCB.resize(count_sci);
        for (size_t j = 0; j < count_sci; j++) 
        {
            this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB[j], this->poBuffersMemory_ObjectCB[j]);
        }

        //MaterialConstants
        bufferSize = sizeof(MaterialConstants) * this->materialCBs.size();
        this->poBuffers_materialCB.resize(count_sci);
        this->poBuffersMemory_materialCB.resize(count_sci);
        for (size_t j = 0; j < count_sci; j++) 
        {
            this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_materialCB[j], this->poBuffersMemory_materialCB[j]);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            bufferSize = sizeof(TessellationConstants) * this->tessellationCBs.size();
            this->poBuffers_tessellationCB.resize(count_sci);
            this->poBuffersMemory_tessellationCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_tessellationCB[j], this->poBuffersMemory_tessellationCB[j]);
            }
        }
    }

    //2> IndirectCommand Buffer
    {
        bufferSize = sizeof(VkDrawIndexedIndirectCommand) * this->indirectCommandCBs.size();
        this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_indirectCommandCB, this->poBuffersMemory_indirectCommandCB);
    }
}

void Vulkan_018_Object::ModelObjectRendIndirect::UpdateUniformBuffer()
{
    this->objectCBs.clear();
    this->materialCBs.clear();
    this->tessellationCBs.clear();

    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        this->objectCBs.insert(this->objectCBs.end(), pR->objectCBs.begin(), pR->objectCBs.end());
        this->materialCBs.insert(this->materialCBs.end(), pR->materialCBs.begin(), pR->materialCBs.end());
        if (pRend->isUsedTessellation)
        {
            this->tessellationCBs.insert(this->tessellationCBs.end(), pR->tessellationCBs.begin(), pR->tessellationCBs.end());
        }
    }
}

void Vulkan_018_Object::ModelObjectRendIndirect::UpdateIndirectCommandBuffer()
{
    this->indirectCommandCBs.clear();

    int32_t vertexOffset = 0;
    uint32_t indexOffset = 0;
    uint32_t instanceOffset = 0;
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        VkDrawIndexedIndirectCommand indirectCommand = {};
        indirectCommand.indexCount = pMeshSub->poIndexCount;
        indirectCommand.instanceCount = pRend->pModelObject->countInstance;
        indirectCommand.firstIndex = indexOffset;
        indirectCommand.vertexOffset = vertexOffset;
        indirectCommand.firstInstance = instanceOffset;
        this->indirectCommandCBs.push_back(indirectCommand);

        indexOffset += pMeshSub->poIndexCount;
        vertexOffset += pMeshSub->poVertexCount;
        instanceOffset += pRend->pModelObject->countInstance;
    }
    this->countIndirectDraw = (uint32_t)this->indirectCommandCBs.size();
}


/////////////////////////// ModelObject /////////////////////////

static const String s_strNameDescriptorSet = "Cfg_DescriptorSet.xml";
static const String s_strNameDescriptorSetLayout = "Cfg_DescriptorSetLayout.xml";
static const String s_strNameScene = "Cfg_Scene.xml";


static uint32 s_nGroup_Sample = FPathManager::PathGroup_Editor + 1;
static const String s_strGroup_Sample = "Assets/Editor/Sample/Vulkan_018_Object";
static const String s_strNameMesh_Sample = "Cfg_Mesh.xml";
static const String s_strNameTexture_Sample = "Cfg_Texture.xml";
static const String s_strNameShader_Sample = "Cfg_Shader.xml";
static const String s_strNameMaterial_Sample = "Cfg_Material.xml";



Vulkan_018_Object::Vulkan_018_Object(int width, int height, String name)
    : VulkanWindow(width, height, name)
    , m_isDrawIndirect(false)
    , m_isDrawIndirectMulti(false)
    , m_pPathManager(nullptr)
    , m_pMeshManager(nullptr)
    , m_pTextureManager(nullptr)
    , m_pShaderManager(nullptr)
    , m_pVKDescriptorSetManager(nullptr)
    , m_pVKDescriptorSetLayoutManager(nullptr)
    , m_pVKPipelineLayoutManager(nullptr)
    , m_pVKPipelineManager(nullptr)
    , m_pMaterialDataManager(nullptr)
    , m_pMaterialManager(nullptr)
    , m_pSceneManagerEnumerator(nullptr)
    , m_pSceneDataManager(nullptr)
    , m_pSceneManager(nullptr)
    , m_pScene(nullptr)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = FVector3(0, -1, 0); //y-

    FPathManager::ms_bIsLog = true;
    m_pPathManager = new FPathManager();
    m_pPathManager->Init();
    m_pPathManager->RegisterUserGroup(FPathManager::PathGroup_Editor, s_nGroup_Sample, s_strGroup_Sample, true, false);
}

void Vulkan_018_Object::setUpEnabledFeatures()
{
    VulkanWindow::setUpEnabledFeatures();

    if (this->poPhysicalEnabledFeatures.multiDrawIndirect)
    {
        this->m_isDrawIndirectMulti = true;
    }
    else
    {
        this->m_isDrawIndirectMulti = false;
        F_LogError("*********************** Vulkan_018_Object::setUpEnabledFeatures: multiDrawIndirect is not supported !");
    }
}

void Vulkan_018_Object::createDescriptorSetLayout_Custom()
{
    VulkanWindow::createDescriptorSetLayout_Custom();
}

void Vulkan_018_Object::createCamera()
{
    this->pCamera = new FCamera();
    cameraReset();
}
void Vulkan_018_Object::cameraReset()
{
    VulkanWindow::cameraReset();

    this->pCamera->SetPos(FVector3(-25.0f, 13.0f, 4.0f));
    this->pCamera->SetEulerAngles(FVector3(35.0f, 90.0f, 0.0f));
    this->pCamera->SetFarZ(100000.0f);
}

void Vulkan_018_Object::loadModel_Custom()
{
    //1> Mesh
    m_pMeshManager = new MeshManager();
    m_pMeshManager->Init(s_nGroup_Sample, s_strNameMesh_Sample);
    m_pMeshManager->LoadMeshAll();

    //2> Texture
    m_pTextureManager = new TextureManager();
    m_pTextureManager->Init(s_nGroup_Sample, s_strNameTexture_Sample);
    m_pTextureManager->LoadTextureAll();

    //3> Shader
    m_pShaderManager = new ShaderManager();
    m_pShaderManager->Init(s_nGroup_Sample, s_strNameShader_Sample);
    m_pShaderManager->LoadShaderAll();
    
    //4> DescriptorSet
    m_pVKDescriptorSetManager = new VKDescriptorSetManager();
    m_pVKDescriptorSetManager->Init(FPathManager::PathGroup_Config, s_strNameDescriptorSet);
    m_pVKDescriptorSetManager->LoadVKDescriptorSetAll();

    //5> DescriptorSetLayout
    m_pVKDescriptorSetLayoutManager = new VKDescriptorSetLayoutManager();
    m_pVKDescriptorSetLayoutManager->Init(FPathManager::PathGroup_Config, s_strNameDescriptorSetLayout);
    m_pVKDescriptorSetLayoutManager->LoadVKDescriptorSetLayoutAll();

    //6> PipelineLayout
    m_pVKPipelineLayoutManager = new VKPipelineLayoutManager();
    m_pVKPipelineLayoutManager->Init();
    m_pVKPipelineLayoutManager->LoadVKPipelineLayoutAll();

    //7> Pipeline
    m_pVKPipelineManager = new VKPipelineManager();
    m_pVKPipelineManager->Init();

    //8> MaterialData/Material
    m_pMaterialDataManager = new MaterialDataManager();
    m_pMaterialManager = new MaterialManager();
    m_pMaterialManager->Init(s_nGroup_Sample, s_strNameMaterial_Sample);
    
    //9> SceneManagerEnumerator/SceneDataManager
    m_pSceneManagerEnumerator = new SceneManagerEnumerator();
    m_pSceneManagerEnumerator->Init();
    m_pSceneDataManager = new SceneDataManager();
    m_pSceneDataManager->Init(FPathManager::PathGroup_Config, s_strNameScene);
    m_pSceneDataManager->LoadSceneAll();
    
    //10> SceneManager/Scene
    m_pScene = m_pSceneDataManager->LoadScene(FPathManager::PathGroup_Scene, "Default");
    

    int nIndexObjectRend = 0;
    for (int i = 0; i < g_Object_Count; i++)
    {
        ModelObject* pModelObject = new ModelObject(this, i);

        //1> Object
        {
            pModelObject->indexModel = i;
            pModelObject->nameObject = g_Object_Configs[3 * i + 0];
            String nameGroup = g_Object_Configs[3 * i + 1];
            uint32 nGroup = FUtilString::ParserUInt(nameGroup);
            pModelObject->nameMesh = g_Object_Configs[3 * i + 2];
            //Mesh
            {
                Mesh* pMesh = MeshManager::GetSingleton().GetMesh(nGroup, pModelObject->nameMesh);
                F_Assert(pMesh != nullptr && "Vulkan_018_Object::loadModel_Custom")
                pModelObject->SetMesh(pMesh);
            }
            //MeshSub Used
            {
                String nameMeshSubUsed = g_Object_MeshSubsUsed[i];
                StringVector aMeshSubUsed = FUtilString::Split(nameMeshSubUsed, ";");
                pModelObject->aMeshSubUsed.clear();
                size_t count_mesh_sub_used = aMeshSubUsed.size();
                for (size_t j = 0; j < count_mesh_sub_used; j++)
                {
                    String& name = aMeshSubUsed[j];
                    int indexMeshSub = FUtilString::ParserInt(name);
                    pModelObject->aMeshSubUsed.push_back(indexMeshSub);
                }
            }
            pModelObject->isShow = g_Object_IsShows[i];
            pModelObject->isRotate = g_Object_IsRotates[i];
            pModelObject->isIndirectDraw = g_Object_IsIndirectDraw[i];
            pModelObject->countInstanceExt = g_Object_InstanceExtCount[i];
            pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;
        }

        //2> ObjectRend
        {
            size_t count_mesh_sub = pModelObject->pMesh->aMeshSubs.size();
            size_t count_mesh_sub_used = pModelObject->aMeshSubUsed.size();
            for (size_t j = 0; j < count_mesh_sub_used; j++)
            {
                int indexMeshSub = pModelObject->aMeshSubUsed[j];
                F_Assert(indexMeshSub >= 0 && indexMeshSub < count_mesh_sub && "Vulkan_018_Object::loadModel_Custom")

                MeshSub* pMeshSub = pModelObject->pMesh->aMeshSubs[indexMeshSub];
                String nameObjectRend = g_ObjectRend_Configs[7 * nIndexObjectRend + 0];
                ModelObjectRend* pRend = new ModelObjectRend(nameObjectRend, pModelObject, pMeshSub);

                //Texture VS
                {
                    String nameTextureVS = g_ObjectRend_Configs[7 * nIndexObjectRend + 1]; //Texture VS
                    if (!nameTextureVS.empty())
                    {
                        StringVector aTextureVS = FUtilString::Split(nameTextureVS, ";");
                        size_t count_tex = aTextureVS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            StringVector aArr = FUtilString::Split(aTextureVS[p], ":");
                            uint32 group = FUtilString::ParserUInt(aArr[0]);
                            String nameTex = aArr[1];
                            Texture* pTextureVS = TextureManager::GetSingleton().GetTexture(group, nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Vertex), pTextureVS);
                        }
                    }
                }
                //Texture TESC
                {
                    String nameTextureTESC = g_ObjectRend_Configs[7 * nIndexObjectRend + 2]; //Texture TESC
                    if (!nameTextureTESC.empty())
                    {
                        StringVector aTextureTESC = FUtilString::Split(nameTextureTESC, ";");
                        size_t count_tex = aTextureTESC.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            StringVector aArr = FUtilString::Split(aTextureTESC[p], ":");
                            uint32 group = FUtilString::ParserUInt(aArr[0]);
                            String nameTex = aArr[1];
                            Texture* pTextureTESC = TextureManager::GetSingleton().GetTexture(group, nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_TessellationControl), pTextureTESC);
                        }
                    }
                }
                //Texture TESE
                {
                    String nameTextureTESE = g_ObjectRend_Configs[7 * nIndexObjectRend + 3]; //Texture TESE
                    if (!nameTextureTESE.empty())
                    {
                        StringVector aTextureTESE = FUtilString::Split(nameTextureTESE, ";");
                        size_t count_tex = aTextureTESE.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            StringVector aArr = FUtilString::Split(aTextureTESE[p], ":");
                            uint32 group = FUtilString::ParserUInt(aArr[0]);
                            String nameTex = aArr[1];
                            Texture* pTextureTESE = TextureManager::GetSingleton().GetTexture(group, nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_TessellationEvaluation), pTextureTESE);
                        }
                    }
                }
                //Texture GS
                {
                    String nameTextureGS = g_ObjectRend_Configs[7 * nIndexObjectRend + 4]; //Texture GS
                    if (!nameTextureGS.empty())
                    {
                        StringVector aTextureGS = FUtilString::Split(nameTextureGS, ";");
                        size_t count_tex = aTextureGS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            StringVector aArr = FUtilString::Split(aTextureGS[p], ":");
                            uint32 group = FUtilString::ParserUInt(aArr[0]);
                            String nameTex = aArr[1];
                            Texture* pTextureGS = TextureManager::GetSingleton().GetTexture(group, nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureGS);
                        }
                    }
                }
                //Texture FS
                {
                    String nameTextureFS = g_ObjectRend_Configs[7 * nIndexObjectRend + 5]; //Texture FS
                    if (!nameTextureFS.empty())
                    {
                        StringVector aTextureFS = FUtilString::Split(nameTextureFS, ";");
                        size_t count_tex = aTextureFS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            StringVector aArr = FUtilString::Split(aTextureFS[p], ":");
                            uint32 group = FUtilString::ParserUInt(aArr[0]);
                            String nameTex = aArr[1];
                            Texture* pTextureFS = TextureManager::GetSingleton().GetTexture(group, nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureFS);
                        }
                    }
                }
                //Texture CS
                {
                    String nameTextureCS = g_ObjectRend_Configs[7 * nIndexObjectRend + 6]; //Texture CS
                    if (!nameTextureCS.empty())
                    {
                        StringVector aTextureCS = FUtilString::Split(nameTextureCS, ";");
                        size_t count_tex = aTextureCS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            StringVector aArr = FUtilString::Split(aTextureCS[p], ":");
                            uint32 group = FUtilString::ParserUInt(aArr[0]);
                            String nameTex = aArr[1];
                            Texture* pTextureCS = TextureManager::GetSingleton().GetTexture(group, nameTex);
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Compute), pTextureCS);
                        }
                    }
                }

                String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 1];
                String nameShaderTese = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 2];
                if (!nameShaderTesc.empty() || !nameShaderTese.empty())
                {
                    pRend->isUsedTessellation = true;
                    if (g_ObjectRend_IsTopologyPatchLists[nIndexObjectRend])
                    {
                        pRend->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
                    }
                }

                //Pipeline Graphics - DescriptorSetLayout
                pRend->pPipelineGraphics->nameDescriptorSetLayout = g_ObjectRend_NameDescriptorSetLayouts[2 * nIndexObjectRend + 0];

                //Pipeline Computes - DescriptorSetLayout
                String nameDescriptorSetLayout = g_ObjectRend_NameDescriptorSetLayouts[2 * nIndexObjectRend + 1];
                if (!nameDescriptorSetLayout.empty())
                {
                    StringVector aDescriptorSetLayout = FUtilString::Split(nameDescriptorSetLayout, ";");
                    size_t count_dsl = aDescriptorSetLayout.size();
                    for (size_t p = 0; p < count_dsl; p++)
                    {
                        const String& nameDescriptorSetLayout = aDescriptorSetLayout[p];
                        VKPipelineCompute* pPipelineCompute = new VKPipelineCompute("PipelineC-Object");
                        pPipelineCompute->nameDescriptorSetLayout = nameDescriptorSetLayout;
                        pRend->AddPipelineCompute(pPipelineCompute);
                    }
                }

                //Common
                pRend->isTransparent = g_ObjectRend_IsTransparents[nIndexObjectRend];

                pModelObject->AddObjectRend(pRend);
                m_aModelObjectRends_All.push_back(pRend);
                if (pRend->isTransparent)
                    m_aModelObjectRends_Transparent.push_back(pRend);
                else 
                    m_aModelObjectRends_Opaque.push_back(pRend);

                nIndexObjectRend ++;
            }

            m_aModelObjects.push_back(pModelObject);
            m_mapModelObjects[pModelObject->nameObject] = pModelObject;
        }
        
        //3> ObjectRendIndirectDraw
        if (pModelObject->isIndirectDraw)
        {
            size_t count_object_rend = pModelObject->aRends.size();
            if (count_object_rend > 0)
            {
                ModelObjectRend* pRend = pModelObject->aRends[i];
                String nameObjectRendIndirect = pModelObject->nameObject + " - RendIndirect";
                pModelObject->pRendIndirect = new ModelObjectRendIndirect(nameObjectRendIndirect);
                pModelObject->pRendIndirect->SetupVertexIndexBuffer(pModelObject->aRends);
            }
        }

    }
}
void Vulkan_018_Object::createIndirectCommands()
{

}

void Vulkan_018_Object::loadTexture_Custom()
{

}

void Vulkan_018_Object::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_018_Object::rebuildInstanceCBs(bool isCreateVkBuffer)
{   
    VkDeviceSize bufferSize;
    size_t count_sci = this->poSwapChainImages.size();

    //1> ObjectRends
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        int indexObject = pRend->pModelObject->index;
        int count_instance = pRend->pModelObject->countInstance;
        bool isObjectLighting = g_Object_IsLightings[indexObject];

        pRend->instanceMatWorld.clear();
        pRend->objectCBs.clear();
        pRend->materialCBs.clear();
        for (int j = 0; j < count_instance; j++)
        {
            //ObjectConstants
            {
                ObjectConstants objectConstants;
                objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectRend_Tranforms[3 * i + 0] + FVector3((j - pRend->pModelObject->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                                 g_ObjectRend_Tranforms[3 * i + 1],
                                                                 g_ObjectRend_Tranforms[3 * i + 2]);
                pRend->objectCBs.push_back(objectConstants);
                pRend->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = FMath::RandomColor(false);
                materialConstants.factorDiffuse = FMath::RandomColor(false);
                materialConstants.factorSpecular = FMath::RandomColor(false);
                materialConstants.shininess = FMath::RandF(10.0f, 100.0f);
                materialConstants.alpha = FMath::RandF(0.2f, 0.9f);
                materialConstants.lighting = isObjectLighting;
                //Texture VS
                {
                    TexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                    if (pTextureVSs != nullptr)
                    {

                    }
                }
                //Texture FS
                {
                    TexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                    if (pTextureFSs != nullptr)
                    {

                    }
                }
                //Texture CS
                {
                    TexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                    if (pTextureCSs != nullptr)
                    {

                    }
                }
                pRend->materialCBs.push_back(materialConstants);
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants tessellationConstants;
                tessellationConstants.tessLevelOuter = 3.0f;
                tessellationConstants.tessLevelInner = 3.0f;
                tessellationConstants.tessAlpha = 1.0f;
                pRend->tessellationCBs.push_back(tessellationConstants);
            }
        }
        
        if (isCreateVkBuffer)
        {
            //ObjectConstants
            bufferSize = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
            pRend->poBuffers_ObjectCB.resize(count_sci);
            pRend->poBuffersMemory_ObjectCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB[j], pRend->poBuffersMemory_ObjectCB[j]);
            }

            //MaterialConstants
            bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
            pRend->poBuffers_materialCB.resize(count_sci);
            pRend->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_materialCB[j], pRend->poBuffersMemory_materialCB[j]);
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                bufferSize = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pRend->poBuffers_tessellationCB.resize(count_sci);
                pRend->poBuffersMemory_tessellationCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_tessellationCB[j], pRend->poBuffersMemory_tessellationCB[j]);
                }
            }
        }
    }

    //2> ObjectRendIndriect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        if (pModelObject->isIndirectDraw &&
            pModelObject->pRendIndirect != nullptr)
        {
            pModelObject->pRendIndirect->UpdateUniformBuffer();
            pModelObject->pRendIndirect->UpdateIndirectCommandBuffer();

            if (isCreateVkBuffer)
            {
                pModelObject->pRendIndirect->SetupUniformIndirectCommandBuffer();
            }
        }
    }
}

void Vulkan_018_Object::createCustomBeforePipeline()
{
    
}   
void Vulkan_018_Object::createGraphicsPipeline_Custom()
{
    //1> Viewport
    VkViewportVector aViewports;
    aViewports.push_back(this->poViewport);
    VkRect2DVector aScissors;
    aScissors.push_back(this->poScissor);

    //2> Pipeline
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //[1] Shaders
        String nameShaderVert = g_ObjectRend_NameShaderModules[6 * i + 0];
        String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * i + 1];
        String nameShaderTese = g_ObjectRend_NameShaderModules[6 * i + 2];
        String nameShaderGeom = g_ObjectRend_NameShaderModules[6 * i + 3];
        String nameShaderFrag = g_ObjectRend_NameShaderModules[6 * i + 4];
        if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                  nameShaderTesc,
                                                  nameShaderTese,
                                                  nameShaderGeom,
                                                  nameShaderFrag,
                                                  ShaderManager::GetSingleton().GetVkShaderModuleMap(),
                                                  pRend->aShaderStageCreateInfos_Graphics))
        {
            String msg = "*********************** Vulkan_018_Object::createGraphicsPipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Graphics
        {
            VKDescriptorSetLayout* pVKDescriptorSetLayout = VKDescriptorSetLayoutManager::GetSingleton().GetVKDescriptorSetLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pVKDescriptorSetLayout == nullptr)
            {
                String msg = "*********************** Vulkan_018_Object::createGraphicsPipeline_Custom: Can not find VKDescriptorSetLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poDescriptorSetLayoutNames = &pVKDescriptorSetLayout->GetNamesDescriptorSet();
            pRend->pPipelineGraphics->poDescriptorSetLayout = pVKDescriptorSetLayout->GetVKDescriptorSetLayout(); 
            if (pRend->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_Object::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            VKPipelineLayout* pVKPipelineLayout = VKPipelineLayoutManager::GetSingleton().GetVKPipelineLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pVKPipelineLayout == nullptr)
            {
                String msg = "*********************** Vulkan_018_Object::createGraphicsPipeline_Custom: Can not find VKPipelineLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poPipelineLayout = pVKPipelineLayout->GetVkPipelineLayout();

            //pPipelineGraphics->poPipeline_WireFrame
            pRend->pPipelineGraphics->poPipeline_WireFrame = createVkGraphicsPipeline(pRend->aShaderStageCreateInfos_Graphics,
                                                                                      pRend->isUsedTessellation, 0, 3,
                                                                                      Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                      pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pRend->cfg_vkCullModeFlagBits, this->cfg_LineWidth,
                                                                                      pRend->cfg_isDepthTest, pRend->cfg_isDepthWrite, pRend->cfg_DepthCompareOp,
                                                                                      pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                      pRend->cfg_isBlend, pRend->cfg_BlendColorFactorSrc, pRend->cfg_BlendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                                      pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                                      pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_Object::createGraphicsPipeline_Custom: Failed to create pipeline graphics wire frame: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_018_Object::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics wire frame success !", pRend->nameObjectRend.c_str());

            //pPipelineGraphics->poPipeline
            VkBool32 isDepthTestEnable = pRend->cfg_isDepthTest;
            VkBool32 isDepthWriteEnable = pRend->cfg_isDepthWrite;
            VkBool32 isBlend = pRend->cfg_isBlend;
            VkBlendFactor blendColorFactorSrc = pRend->cfg_BlendColorFactorSrc; 
            VkBlendFactor blendColorFactorDst = pRend->cfg_BlendColorFactorDst; 
            if (pRend->isTransparent)
            {
                isDepthTestEnable = VK_FALSE;
                isDepthWriteEnable = VK_FALSE;

                isBlend = VK_TRUE;
                blendColorFactorSrc = VK_BLEND_FACTOR_SRC_ALPHA;
                blendColorFactorDst = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            }
            pRend->pPipelineGraphics->poPipeline = createVkGraphicsPipeline(pRend->aShaderStageCreateInfos_Graphics,
                                                                            pRend->isUsedTessellation, 0, 3,
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                            this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                            pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_LineWidth,
                                                                            isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                            pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                            isBlend, blendColorFactorSrc, blendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                            pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                            pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_Object::createGraphicsPipeline_Custom: Failed to create pipeline graphics: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_018_Object::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics graphics success !", pRend->nameObjectRend.c_str());
        }
    }
}
void Vulkan_018_Object::createComputePipeline_Custom()
{
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        size_t count_pipeline = pRend->aPipelineComputes.size();
        if (count_pipeline <= 0)
            continue;

        //[1] Shaders
        String nameShaderComp = g_ObjectRend_NameShaderModules[6 * i + 5];
        if (!CreatePipelineShaderStageCreateInfos(nameShaderComp,
                                                  ShaderManager::GetSingleton().GetVkShaderModuleMap(),
                                                  pRend->aShaderStageCreateInfos_Computes,
                                                  pRend->mapShaderStageCreateInfos_Computes))
        {
            String msg = "*********************** Vulkan_018_Object::createComputePipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Compute
        if (count_pipeline != pRend->aShaderStageCreateInfos_Computes.size())
        {
            String msg = "*********************** Vulkan_018_Object::createComputePipeline_Custom: Pipeline count is not equal shader count !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        for (size_t j = 0; j < count_pipeline; j ++)
        {
            VKPipelineCompute* p = pRend->aPipelineComputes[j];
            VkPipelineShaderStageCreateInfo& shaderStageCreateInfo = pRend->aShaderStageCreateInfos_Computes[j];

            VKDescriptorSetLayout* pVKDescriptorSetLayout = VKDescriptorSetLayoutManager::GetSingleton().GetVKDescriptorSetLayout(p->nameDescriptorSetLayout);
            if (pVKDescriptorSetLayout == nullptr)
            {
                String msg = "*********************** Vulkan_018_Object::createComputePipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poDescriptorSetLayoutNames = &pVKDescriptorSetLayout->GetNamesDescriptorSet();
            p->poDescriptorSetLayout = pVKDescriptorSetLayout->GetVKDescriptorSetLayout();
            if (p->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_Object::createComputePipeline_Custom: Can not find DescriptorSetLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            VKPipelineLayout* pVKPipelineLayout = VKPipelineLayoutManager::GetSingleton().GetVKPipelineLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pVKPipelineLayout == nullptr)
            {
                String msg = "*********************** Vulkan_018_Object::createComputePipeline_Custom: Can not find VKPipelineLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poPipelineLayout = pVKPipelineLayout->GetVkPipelineLayout();

            p->poPipeline = createVkComputePipeline(shaderStageCreateInfo, p->poPipelineLayout, 0);
            if (p->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_018_Object::createComputePipeline_Custom: Create compute pipeline failed, PipelineLayout name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
    }   
}


void Vulkan_018_Object::createDescriptorSets_Custom()
{
    //1> Object Rend
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //Pipeline Graphics
        {
            createVkDescriptorSets(pRend->pPipelineGraphics->poDescriptorSetLayout, pRend->pPipelineGraphics->poDescriptorSets);
            createDescriptorSets_Graphics(pRend->pPipelineGraphics->poDescriptorSets, pRend, nullptr);
        }   
        
        //Pipeline Computes
        size_t count_comp_rend = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp_rend; j++)
        {       
            VKPipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
            createDescriptorSets_Compute(pPipelineCompute, pRend);
        }
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        if (pModelObject->pRendIndirect != nullptr)
        {
            createVkDescriptorSets(pModelObject->pRendIndirect->pRend->pPipelineGraphics->poDescriptorSetLayout, pModelObject->pRendIndirect->poDescriptorSets);
            createDescriptorSets_Graphics(pModelObject->pRendIndirect->poDescriptorSets, pModelObject->pRendIndirect->pRend, pModelObject->pRendIndirect);
        }
    }
}
void Vulkan_018_Object::createDescriptorSets_Graphics(VkDescriptorSetVector& poDescriptorSets, 
                                                            ModelObjectRend* pRend, 
                                                            ModelObjectRendIndirect* pRendIndirect)
{
    StringVector* pDescriptorSetLayoutNames = pRend->pPipelineGraphics->poDescriptorSetLayoutNames;
    F_Assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_018_Object::createDescriptorSets_Graphics")
    size_t count_ds = poDescriptorSets.size();
    for (size_t j = 0; j < count_ds; j++)
    {   
        VkWriteDescriptorSetVector descriptorWrites;
        int nIndexTextureVS = 0;
        int nIndexTextureTESC = 0;
        int nIndexTextureTESE = 0;
        int nIndexTextureFS = 0;

        size_t count_names = pDescriptorSetLayoutNames->size();
        for (size_t p = 0; p < count_names; p++)
        {
            String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
            if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Pass);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
            {
                VkDescriptorBufferInfo bufferInfo_Object = {};
                bufferInfo_Object.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_ObjectCB[j] : pRend->poBuffers_ObjectCB[j];
                bufferInfo_Object.offset = 0;
                bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Object);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Material)) //Material
            {
                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_materialCB[j] : pRend->poBuffers_materialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Material);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Instance)) //Instance
            {
                VkDescriptorBufferInfo bufferInfo_Instance = {};
                bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j]; 
                bufferInfo_Instance.offset = 0;
                bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Instance);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Tessellation)) //Tessellation
            {
                VkDescriptorBufferInfo bufferInfo_Tessellation = {};
                bufferInfo_Tessellation.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_tessellationCB[j] : pRend->poBuffers_tessellationCB[j];
                bufferInfo_Tessellation.offset = 0;
                bufferInfo_Tessellation.range = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Tessellation);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureVS)) //TextureVS
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Vertex), nIndexTextureVS);
                nIndexTextureVS ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESC))//TextureTESC
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_TessellationControl), nIndexTextureTESC);
                nIndexTextureTESC ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESE))//TextureTESE
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_TessellationEvaluation), nIndexTextureTESE);
                nIndexTextureTESE ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS)) //TextureFS
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Fragment), nIndexTextureFS);
                nIndexTextureFS ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else
            {
                String msg = "*********************** Vulkan_018_Object::createDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        updateVkDescriptorSets(descriptorWrites);
    }
}
void Vulkan_018_Object::createDescriptorSets_Compute(VKPipelineCompute* pPipelineCompute, 
                                                           ModelObjectRend* pRend)
{
    StringVector* pDescriptorSetLayoutNames = pPipelineCompute->poDescriptorSetLayoutNames;
    F_Assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_018_Object::createDescriptorSets_Compute")
    createVkDescriptorSet(pPipelineCompute->poDescriptorSetLayout, pPipelineCompute->poDescriptorSet);

    VkWriteDescriptorSetVector descriptorWrites;
    int nIndexTextureCS = 0;
    size_t count_names = pDescriptorSetLayoutNames->size();
    for (size_t p = 0; p < count_names; p++)
    {
        String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
        if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCopy)) //TextureCopy
        {
            pPipelineCompute->CreateTextureCopy();

            VkDescriptorBufferInfo bufferInfo_TextureCopy = {};
            bufferInfo_TextureCopy.buffer = pPipelineCompute->poBuffer_TextureCopy;
            bufferInfo_TextureCopy.offset = 0;
            bufferInfo_TextureCopy.range = sizeof(TextureCopyConstants);
            pushVkDescriptorSet_Uniform(descriptorWrites,
                                        pPipelineCompute->poDescriptorSet,
                                        p,
                                        0,
                                        1,
                                        bufferInfo_TextureCopy);
        }   
        else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
        {
            Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
            nIndexTextureCS ++;
            pPipelineCompute->pTextureSource = pTexture;
            pushVkDescriptorSet_Image(descriptorWrites,
                                      pPipelineCompute->poDescriptorSet,
                                      p,
                                      0,
                                      1,
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      pTexture->poTextureImageInfo);
        }
        else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRW)) //TextureCSRW
        {
            Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
            nIndexTextureCS ++;
            pPipelineCompute->pTextureTarget = pTexture;
            pushVkDescriptorSet_Image(descriptorWrites,
                                      pPipelineCompute->poDescriptorSet,
                                      p,
                                      0,
                                      1,
                                      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                      pTexture->poTextureImageInfo);
        }
        else
        {
            String msg = "*********************** Vulkan_018_Object::createDescriptorSets_Compute: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }  
    updateVkDescriptorSets(descriptorWrites);
}

void Vulkan_018_Object::updateCompute_Custom(VkCommandBuffer& commandBuffer)
{
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_comp = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            VKPipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
            if (pPipelineCompute->pTextureSource != nullptr &&
                pPipelineCompute->pTextureTarget != nullptr &&
                pPipelineCompute->pTextureCopy != nullptr)
            {
                bool isRand = false;
                if (++pPipelineCompute->frameRand > 30)
                {
                    isRand = true;
                    pPipelineCompute->frameRand = 0;
                }

                pPipelineCompute->pTextureCopy->texInfo.x = pPipelineCompute->pTextureSource->width;
                pPipelineCompute->pTextureCopy->texInfo.y = pPipelineCompute->pTextureSource->height;
                pPipelineCompute->pTextureCopy->texInfo.z = 0;
                pPipelineCompute->pTextureCopy->texInfo.w = 0;
                if (isRand)
                {
                    pPipelineCompute->pTextureCopy->texOffset.x = FMath::Rand(0, 1) * pPipelineCompute->pTextureSource->width;
                    pPipelineCompute->pTextureCopy->texOffset.y = FMath::Rand(0, 1) * pPipelineCompute->pTextureSource->height;
                    pPipelineCompute->pTextureCopy->texOffset.z = 0;
                    pPipelineCompute->pTextureCopy->texOffset.w = 0;

                    int seed = FMath::Rand(0, 10000);
                    int start = seed % 4;
                    pPipelineCompute->pTextureCopy->texIndexArray.x = start;
                    pPipelineCompute->pTextureCopy->texIndexArray.y = ++start % 4;
                    pPipelineCompute->pTextureCopy->texIndexArray.z = ++start % 4;
                    pPipelineCompute->pTextureCopy->texIndexArray.w = ++start % 4;
                }
                pPipelineCompute->pTextureCopy->texClearColor.x = 0;
                pPipelineCompute->pTextureCopy->texClearColor.y = 0;
                pPipelineCompute->pTextureCopy->texClearColor.z = 0;
                pPipelineCompute->pTextureCopy->texClearColor.w = 1;

                VkDeviceMemory& memory = pPipelineCompute->poBufferMemory_TextureCopy;
                updateVKBuffer(0, sizeof(TextureCopyConstants), pPipelineCompute->pTextureCopy, memory);

                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pPipelineCompute->poPipeline);
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pPipelineCompute->poPipelineLayout, 0, 1, &pPipelineCompute->poDescriptorSet, 0, 0);
                
                uint32_t groupX = (uint32_t)(pPipelineCompute->pTextureTarget->width / 8);
                uint32_t groupY = (uint32_t)(pPipelineCompute->pTextureTarget->height / 8);
                dispatch(commandBuffer, groupX, groupY, 1);
            }
        }
    }
}

void Vulkan_018_Object::updateCBs_Custom()
{
    //1> Object Rend
    float time = this->pTimer->GetTimeSinceStart();
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_object = pRend->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pRend->objectCBs[j];
            if (pRend->pModelObject->isRotate || pRend->isRotate || this->cfg_isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pRend->instanceMatWorld[j], 
                                                  time * glm::radians(90.0f), 
                                                  FVector3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pRend->instanceMatWorld[j];
            }

            //MaterialConstants
            MaterialConstants& materialCB = pRend->materialCBs[j];
            
            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants& tessellationCB = pRend->tessellationCBs[j];
            }
        }

        //ObjectConstants
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pRend->objectCBs.data(), memory);
        }

        //MaterialConstants
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pRend->materialCBs.data(), memory);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(TessellationConstants) * count_object, pRend->tessellationCBs.data(), memory);
        }
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        ModelObjectRendIndirect* pRendIndirect = pModelObject->pRendIndirect;
        if (pRendIndirect != nullptr)
        {
            pRendIndirect->UpdateUniformBuffer();
            size_t count_object = pRendIndirect->objectCBs.size();
            
            //ObjectConstants
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pRendIndirect->objectCBs.data(), memory);
            }

            //MaterialConstants
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pRendIndirect->materialCBs.data(), memory);
            }

            //TessellationConstants
            if (pRendIndirect->pRend->isUsedTessellation)
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(TessellationConstants) * count_object, pRendIndirect->tessellationCBs.data(), memory);
            }

            //IndirectCommand
            {
                size_t count_indirectcommand = pRendIndirect->indirectCommandCBs.size();
                updateVKBuffer(0, sizeof(VkDrawIndexedIndirectCommand) * count_indirectcommand, pRendIndirect->indirectCommandCBs.data(), pRendIndirect->poBuffersMemory_indirectCommandCB);
            }
        }
    }
}

void Vulkan_018_Object::updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer)
{
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_comp = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            VKPipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
            if (pPipelineCompute->pTextureSource != nullptr &&
                pPipelineCompute->pTextureTarget != nullptr &&
                pPipelineCompute->pTextureCopy != nullptr)
            {
                VkImageMemoryBarrier imageMemoryBarrier = {};
                imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
                imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
                imageMemoryBarrier.image = pPipelineCompute->pTextureTarget->poTextureImage;
                imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

                vkCmdPipelineBarrier(commandBuffer,
                                     VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                     0,
                                     0,
                                     nullptr,
                                     0,
                                     nullptr,
                                     1,
                                     &imageMemoryBarrier);
            }
        }
    }
}

bool Vulkan_018_Object::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_018_Object", &windowOpened, 0);
    {
        //0> Common
        commonConfig();

        //1> Camera
        cameraConfig();

        //2> Light
        lightConfig();

        //3> PassConstants
        passConstantsConfig();

        //4> Model
        modelConfig();

        //5> Enum
        enumConfig();
    }
    ImGui::End();

    return true;
}
void Vulkan_018_Object::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
        //m_isDrawIndirect
        if (ImGui::Checkbox("Is DrawIndirect", &this->m_isDrawIndirect))
        {
            
        }
        //m_isDrawIndirectMulti
        if (ImGui::Checkbox("Is DrawIndirectMulti", &this->m_isDrawIndirectMulti))
        {
            
        }

        float fGap = g_Object_InstanceGap;
        if (ImGui::DragFloat("Instance Gap: ", &fGap, 0.1f, 1.0f, 100.0f))
        {
            g_Object_InstanceGap = fGap;
            rebuildInstanceCBs(false);
        }

        size_t count_object = this->m_aModelObjects.size();
        for (size_t i = 0; i < count_object; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];
            ModelObjectRendIndirect* pRendIndirect = pModelObject->pRendIndirect;
            size_t count_object_rend = pModelObject->aRends.size();

            //1> ModelObject
            String nameObject = FUtilString::SaveInt(i) + " - " + pModelObject->nameObject;
            if (ImGui::CollapsingHeader(nameObject.c_str()))
            {
                //isShow
                String nameIsShow = "Is Show - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isShow = pModelObject->isShow;
                    }
                }
                //isWireFrame
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isWireFrame = pModelObject->isWireFrame;
                    }
                }
                //isRotate
                String nameIsRotate = "Is Rotate - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isRotate = pModelObject->isRotate;
                    }
                }
                //isLighting
                String nameIsLighting = "Is Lighting - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        for (int p = 0; p < pModelObject->countInstance; p++)
                        {
                            MaterialConstants& mat = pRend->materialCBs[p];
                            mat.lighting = pModelObject->isLighting;
                        }
                    }
                }

                String nameInstances = "Instance - " + pModelObject->nameObject;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 3);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                //2> ModelObjectRend
                if (pRendIndirect != nullptr && this->m_isDrawIndirect)
                {
                    String& nameObjectRendIndirect = pRendIndirect->nameObjectRendIndirect;
                    if (ImGui::CollapsingHeader(nameObjectRendIndirect.c_str()))
                    {
                        ImGui::Text("Rend Count: [%d], Vertex: [%d], Index: [%d]", 
                            (int)pRendIndirect->aRends.size(), 
                            (int)pRendIndirect->poVertexCount,
                            (int)pRendIndirect->poIndexCount);
                        //isShow
                        String nameIsShowRend = "Is Show - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsShowRend.c_str(), &pRendIndirect->isShow))
                        {
                            if (pRendIndirect->isShow)
                            {
                                pModelObject->isShow = true;
                            }
                        }
                        //isWireFrame
                        String nameIsWireFrameRend = "Is WireFrame - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsWireFrameRend.c_str(), &pRendIndirect->isWireFrame))
                        {
                            if (!pRendIndirect->isWireFrame)
                            {
                                pModelObject->isWireFrame = false;
                            }
                        }
                        //isRotate
                        String nameIsRotateRend = "Is Rotate - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsRotateRend.c_str(), &pRendIndirect->isRotate))
                        {
                            
                        }
                        //isLighting
                        String nameIsLightingRend = "Is Lighting - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsLightingRend.c_str(), &pRendIndirect->isLighting))
                        {
                            if (pRendIndirect->isLighting)
                            {
                                pModelObject->isLighting = true;
                            }
                            for (size_t p = 0; p < pRendIndirect->materialCBs.size(); p++)
                            {
                                MaterialConstants& mat = pRendIndirect->materialCBs[p];
                                mat.lighting = pRendIndirect->isLighting;
                            }
                        }
                        //isTransparent
                        String nameIsTransparent = "Is Transparent(Read Only) - " + nameObjectRendIndirect;
                        bool isTransparent = pRendIndirect->isTransparent;
                        ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);

                        //countIndirectDraw
                        String nameCountIndirectDraw = "Count IndirectDraw - " + nameObjectRendIndirect;
                        int countIndirectDraw = (int)pRendIndirect->countIndirectDraw;
                        if (ImGui::DragInt(nameCountIndirectDraw.c_str(), &countIndirectDraw, 1, 0, (int)pRendIndirect->indirectCommandCBs.size()))
                        {
                            pRendIndirect->countIndirectDraw = (uint32_t)countIndirectDraw;
                        }

                        //ObjectRend
                        size_t count_rend = pRendIndirect->aRends.size();
                        for (int j = 0; j < count_rend; j++)
                        {
                            ModelObjectRend* pRend = pRendIndirect->aRends[j];

                            ImGui::Text("[%d], Vertex: [%d], Index: [%d], - [%s]", 
                            j,
                            (int)pRend->pMeshSub->poVertexCount,
                            (int)pRend->pMeshSub->poIndexCount,
                            pRend->nameObjectRend.c_str());
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        String& nameObjectRend = pRend->nameObjectRend;
                        if (ImGui::CollapsingHeader(nameObjectRend.c_str()))
                        {
                            ImGui::Text("Vertex: [%d], Index: [%d]", (int)pRend->pMeshSub->poVertexCount, (int)pRend->pMeshSub->poIndexCount);
                            //isShow
                            String nameIsShowRend = "Is Show - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsShowRend.c_str(), &pRend->isShow))
                            {
                                if (pRend->isShow)
                                {
                                    pModelObject->isShow = true;
                                }
                            }
                            //isWireFrame
                            String nameIsWireFrameRend = "Is WireFrame - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsWireFrameRend.c_str(), &pRend->isWireFrame))
                            {
                                if (!pRend->isWireFrame)
                                {
                                    pModelObject->isWireFrame = false;
                                }
                            }
                            //isRotate
                            String nameIsRotateRend = "Is Rotate - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsRotateRend.c_str(), &pRend->isRotate))
                            {
                                
                            }
                            //isLighting
                            String nameIsLightingRend = "Is Lighting - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsLightingRend.c_str(), &pRend->isLighting))
                            {
                                if (pRend->isLighting)
                                {
                                    pModelObject->isLighting = true;
                                }
                                for (int p = 0; p < pModelObject->countInstance; p++)
                                {
                                    MaterialConstants& mat = pRend->materialCBs[p];
                                    mat.lighting = pModelObject->isLighting;
                                }
                            }
                            //isTransparent
                            String nameIsTransparent = "Is Transparent(Read Only) - " + nameObjectRend;
                            bool isTransparent = pRend->isTransparent;
                            ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);

                            String nameWorld = "Model Object - " + nameObjectRend;
                            if (ImGui::CollapsingHeader(nameWorld.c_str()))
                            {
                                int count_instance = pModelObject->countInstance;
                                for (int p = 0; p < count_instance; p++)
                                {
                                    ObjectConstants& obj = pRend->objectCBs[p];
                                    MaterialConstants& mat = pRend->materialCBs[p];

                                    String nameModelInstance = FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                    if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                                    {
                                        //ObjectConstants
                                        String nameObject = FUtilString::SaveInt(p) + " - Object - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameObject.c_str()))
                                        {
                                            const FMatrix4& mat4World = obj.g_MatWorld;
                                            String nameTable = FUtilString::SaveInt(p) + " - matWorld - " + nameObjectRend;
                                            if (ImGui::BeginTable(nameTable.c_str(), 4))
                                            {
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][0]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][1]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][2]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][3]);

                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][0]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][1]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][2]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][3]);

                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][0]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][1]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][2]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][3]);

                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][0]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][1]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][2]);
                                                ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][3]);

                                                ImGui::EndTable();
                                            }
                                        }
                                        
                                        //MaterialConstants
                                        String nameMaterial = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                                        {
                                            //factorAmbient
                                            String nameFactorAmbient = "FactorAmbient - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorDiffuse
                                            String nameFactorDiffuse = "FactorDiffuse - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorSpecular
                                            String nameFactorSpecular = "FactorSpecular - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //shininess
                                            String nameShininess = "Shininess - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //alpha
                                            String nameAlpha = "Alpha - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //lighting
                                            String nameLighting = "Lighting - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            bool isLighting = mat.lighting == 1.0f ? true : false;
                                            if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                            {
                                                mat.lighting = isLighting ? 1.0f : 0.0f;
                                            }

                                            //Texture VS
                                            {
                                                TexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                                                if (pTextureVSs != nullptr)
                                                {

                                                }
                                            }
                                            //Texture FS
                                            {
                                                TexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                                                if (pTextureFSs != nullptr)
                                                {
                                                    size_t count_texture = pTextureFSs->size();
                                                    for (size_t q = 0; q < count_texture; q++)
                                                    {
                                                        Texture* pTexture = (*pTextureFSs)[q];

                                                        String nameMaterial_Texture = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend + " - TextureFS - " + FUtilString::SaveInt(q);
                                                        if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                                        {
                                                            //texWidth
                                                            String nameWidth = "Width - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int width = pTexture->width;
                                                            ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                            //texHeight
                                                            String nameHeight = "Height - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int height = pTexture->height;
                                                            ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                            //texDepth
                                                            String nameDepth = "Depth - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int depth = pTexture->depth;
                                                            ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                                            //indexTextureArray
                                                            String nameIndexTextureArray = "IndexTextureArray - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            if (pTexture->typeTexture == F_Texture_2DArray)
                                                            {
                                                                int count_tex = (int)pTexture->aPathTexture.size();
                                                                int indexTextureArray = (int)mat.aTexLayers[q].indexTextureArray;
                                                                if (ImGui::DragInt(nameIndexTextureArray.c_str(), &indexTextureArray, 1, 0, count_tex - 1))
                                                                {
                                                                    mat.aTexLayers[p].indexTextureArray = indexTextureArray;
                                                                }
                                                            }
                                                            else 
                                                            {
                                                                if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[q].indexTextureArray, 0.001f, 0.0f, 1.0f))
                                                                {

                                                                }
                                                            }

                                                            //texSpeedU
                                                            String nameTexSpeedU = "TexSpeedU - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedV
                                                            String nameTexSpeedV = "texSpeedV - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedW
                                                            String nameTexSpeedW = "texSpeedW - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeedW, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }

                                                            //texChunkMaxX
                                                            String nameTexChunkMaxX = "texChunkMaxX - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxX = mat.aTexLayers[q].texChunkMaxX;
                                                            ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                                            //texChunkMaxY
                                                            String nameTexChunkMaxY = "texChunkMaxY - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxY = mat.aTexLayers[q].texChunkMaxY;
                                                            ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                                            //texChunkIndexX
                                                            String nameTexChunkIndexX = "texChunkIndexX - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexX = mat.aTexLayers[q].texChunkIndexX;
                                                            ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                                            //texChunkIndexY
                                                            String nameTexChunkIndexY = "texChunkIndexY - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexY = mat.aTexLayers[q].texChunkIndexY;
                                                            ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                                        }
                                                    }
                                                }
                                            }
                                            //Texture CS
                                            {
                                                TexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                                                if (pTextureCSs != nullptr)
                                                {

                                                }
                                            }

                                            ImGui::Spacing();
                                        }

                                        //TessellationConstants
                                        String nameTessellation = FUtilString::SaveInt(p) + " - Tessellation - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameTessellation.c_str()))
                                        {
                                            if (pRend->isUsedTessellation)
                                            {
                                                TessellationConstants& tess = pRend->tessellationCBs[j];
                                                //tessLevelOuter
                                                String nameTessLevelOuter = "tessLevelOuter - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelOuter.c_str(), &tess.tessLevelOuter, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessLevelInner
                                                String nameTessLevelInner = "tessLevelInner - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelInner.c_str(), &tess.tessLevelInner, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessAlpha
                                                String nameTessAlpha = "tessAlpha - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                                {
                                                    
                                                }
                                                //tessStrength
                                                String nameTessStrength = "tessStrength - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessStrength.c_str(), &tess.tessStrength, 0.025f, 0.1f, 100.0f))
                                                {
                                                    
                                                }
                                            }

                                            ImGui::Spacing();
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
                
            }
        }
    }
    ImGui::Separator();
    ImGui::Spacing();
}
void Vulkan_018_Object::enumConfig()
{
    if (ImGui::CollapsingHeader("Enum"))
    {
        enumPath();
        enumMesh();
        enumTexture();
        enumShader();
        enumVKDescriptorSet();
        enumVKDescriptorSetLayout();
        enumVKPipelineLayout();
        enumVKPipeline();
        enumMaterial();
        enumScene();
        enumSceneManager();
    }
    ImGui::Separator();
    ImGui::Spacing();
}

void Vulkan_018_Object::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_018_Object::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
{   
    if (this->m_isDrawIndirect)
    {
        //1> Opaque
        {
            drawModelObjectRendIndirects(commandBuffer, this->m_aModelObjectRends_Opaque);
        }
        //2> Transparent
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Transparent);
        }
    }
    else
    {
        //1> Opaque
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Opaque);
        }
        //2> Transparent
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Transparent);
        }
    }
}
void Vulkan_018_Object::drawModelObjectRendIndirects(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
{
    ModelObjectRendIndirect* pRendIndirect_Last = nullptr;
    size_t count_rend = aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = aRends[i];
        ModelObjectRendIndirect* pRendIndirect = pRend->pModelObject->pRendIndirect;
        if (pRendIndirect != nullptr)
        {
            if (pRendIndirect_Last != nullptr && pRendIndirect_Last == pRendIndirect)
                continue;
            if (!pRendIndirect->isShow)
            {
                pRendIndirect_Last = nullptr;
                continue;
            }
            drawModelObjectRendIndirect(commandBuffer, pRendIndirect);
            pRendIndirect_Last = pRendIndirect;
        }
        else
        {
            if (!pRend->isShow)
                continue;
            drawModelObjectRend(commandBuffer, pRend);
        }
    }
}   
void Vulkan_018_Object::drawModelObjectRendIndirect(VkCommandBuffer& commandBuffer, ModelObjectRendIndirect* pRendIndirect)
{
    ModelObjectRend* pRend = pRendIndirect->pRend;
    ModelObject* pModelObject = pRend->pModelObject;

    VkBuffer vertexBuffers[] = { pRendIndirect->poVertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
    if (pRendIndirect->poIndexBuffer != nullptr)
    {
        bindIndexBuffer(commandBuffer, pRendIndirect->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    if (pModelObject->isWireFrame || pRendIndirect->isWireFrame || this->cfg_isWireFrame)
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline_WireFrame);
        if (pRendIndirect->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRendIndirect->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
    }
    else
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline);
        if (pRendIndirect->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRendIndirect->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
    }

    uint32_t drawCount = pRendIndirect->countIndirectDraw;
    if (m_isDrawIndirectMulti)
    {
        drawIndexedIndirect(commandBuffer, pRendIndirect->poBuffer_indirectCommandCB, 0, drawCount, sizeof(VkDrawIndexedIndirectCommand));
    }
    else
    {
        for (uint32_t i = 0; i < drawCount; i++)
        {
            drawIndexedIndirect(commandBuffer, pRendIndirect->poBuffer_indirectCommandCB, i * sizeof(VkDrawIndexedIndirectCommand), 1, sizeof(VkDrawIndexedIndirectCommand));
        }
    }
}

void Vulkan_018_Object::drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
{
    size_t count_rend = aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = aRends[i];
        if (!pRend->isShow)
            continue;
        drawModelObjectRend(commandBuffer, pRend);
    }
}
void Vulkan_018_Object::drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend)
{
    ModelObject* pModelObject = pRend->pModelObject;
    MeshSub* pMeshSub = pRend->pMeshSub;

    VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
    if (pMeshSub->poIndexBuffer != nullptr)
    {
        bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    if (pModelObject->isWireFrame || pRend->isWireFrame || this->cfg_isWireFrame)
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline_WireFrame);
        if (pRend->pPipelineGraphics->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRend->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
        if (pMeshSub->poIndexBuffer != nullptr)
        {
            drawIndexed(commandBuffer, pMeshSub->poIndexCount, pModelObject->countInstance, 0, 0, 0);
        }
        else
        {
            draw(commandBuffer, pMeshSub->poVertexCount, pModelObject->countInstance, 0, 0);
        }
    }
    else
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline);
        if (pRend->pPipelineGraphics->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRend->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
        if (pMeshSub->poIndexBuffer != nullptr)
        {
            drawIndexed(commandBuffer, pMeshSub->poIndexCount, pModelObject->countInstance, 0, 0, 0);
        }
        else
        {
            draw(commandBuffer, pMeshSub->poVertexCount, pModelObject->countInstance, 0, 0);
        }
    }
}

void Vulkan_018_Object::cleanupCustom()
{   
    F_DELETE(m_pScene)
    F_DELETE(m_pSceneManager)
    F_DELETE(m_pSceneDataManager)
    F_DELETE(m_pSceneManagerEnumerator)
    F_DELETE(m_pMaterialManager)
    F_DELETE(m_pMaterialDataManager)
    F_DELETE(m_pVKPipelineManager)
    F_DELETE(m_pVKPipelineLayoutManager)
    F_DELETE(m_pVKDescriptorSetLayoutManager)
    F_DELETE(m_pVKDescriptorSetManager)
    F_DELETE(m_pShaderManager)
    F_DELETE(m_pTextureManager)
    F_DELETE(m_pMeshManager)
    F_DELETE(m_pPathManager)

    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        F_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_mapModelObjects.clear();
    this->m_aModelObjectRends_All.clear();
    this->m_aModelObjectRends_Opaque.clear();
    this->m_aModelObjectRends_Transparent.clear();
}

void Vulkan_018_Object::cleanupSwapChain_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }
}

void Vulkan_018_Object::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->RecreateSwapChain();
    }
}