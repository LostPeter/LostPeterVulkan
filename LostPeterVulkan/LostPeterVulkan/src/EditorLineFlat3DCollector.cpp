/****************************************************************************
 * LostPeterVulkan - Copyright (C) 2022 by LostPeter
 *
 * Author:   LostPeter
 * Time:     2023-10-04
 * Github:   https://github.com/LostPeter/LostPeterVulkan
 * Document: https://www.zhihu.com/people/lostpeter/posts
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 ****************************************************************************/

#include "../include/EditorLineFlat3DCollector.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/BufferStorage.h"

namespace LostPeterVulkan
{
    ////////////////////// BufferBaseLineFlat3D /////////////////////////
    EditorLineFlat3DCollector::BufferBaseLineFlat3D::BufferBaseLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh* p, bool bIsLineList)
        : pLineFlat3DCollector(pCollector)
        , pMesh(p)
        , nObjectCount(0)
        , isLineList(bIsLineList)
    {
        F_Assert(pLineFlat3DCollector != nullptr && pMesh != nullptr && "EditorLineFlat3DCollector::BufferBaseLineFlat3D::BufferBaseLineFlat3D")
    }
    EditorLineFlat3DCollector::BufferBaseLineFlat3D::~BufferBaseLineFlat3D()
    {

    }


    ////////////////////// BufferStorageLineFlat3D //////////////////////
    const int EditorLineFlat3DCollector::BufferStorageLineFlat3D::s_nStepCount = 128;
    EditorLineFlat3DCollector::BufferStorageLineFlat3D::BufferStorageLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh *p, bool bIsLineList)
        : BufferBaseLineFlat3D(pCollector, p, bIsLineList)
        , pBufferStorage(nullptr)
        , nObjectCountMax(MAX_OBJECT_LINEFLAT_3D_COUNT)
    {
        this->pPointerBufferPool = new ObjectManagedPool<PointerBuffer>();
        this->pPointerBufferPool->stepCount = 20;
        this->pPointerBufferPool->Reserve(100);
    } 
    EditorLineFlat3DCollector::BufferStorageLineFlat3D::~BufferStorageLineFlat3D()
    {
        Destroy();
    }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::Destroy()
    {
        Clear();
        F_DELETE(this->pBufferStorage)
        F_DELETE(this->pPointerBufferPool)
    }
    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::Init()
    {
        this->pBufferStorage = createBufferStorage(this->nObjectCountMax);
    }
        BufferStorage* EditorLineFlat3DCollector::BufferStorageLineFlat3D::createBufferStorage(int count)
        {
            String nameBuffer = "BufferStorage-" + this->pMesh->GetName() + FUtilString::SaveInt(count);
            BufferStorage* pBufferStorageNew = new BufferStorage(nameBuffer, count, sizeof(LineFlat3DObjectConstants));
            void* pBuffer = pBufferStorageNew->GetBuffer();

            if (this->pBufferStorage != nullptr)
            {
                memcpy(pBuffer, this->pBufferStorage->GetBuffer(), this->pBufferStorage->GetBufferSize());
            }
            F_DELETE(this->pBufferStorage)
            return pBufferStorageNew;
        }
        void EditorLineFlat3DCollector::BufferStorageLineFlat3D::increaseBufferStorage()
        {
            this->nObjectCountMax += s_nStepCount;
            this->pBufferStorage = createBufferStorage(this->nObjectCountMax);

            UpdateDescriptorSets();
        }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::Clear()
    {
        this->nObjectCount = 0;
        if (this->pBufferStorage == nullptr)
            this->nObjectCountMax = MAX_OBJECT_LINEFLAT_3D_COUNT;
        else 
            this->nObjectCountMax = this->pBufferStorage->nCount;
        this->pPointerBufferPool->Reset();
    }

    PointerBuffer* EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObject(const FMatrix4 &mat, const FColor &color, bool isUpdateBuffer /*= true*/)
    {
        LineFlat3DObjectConstants object;
        object.g_MatWorld = mat;
        object.color = color;
        return AddLineFlat3DObject(object, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObject(const LineFlat3DObjectConstants& object, bool isUpdateBuffer /*= true*/)
    {
        if (this->nObjectCount + 1 >= this->nObjectCountMax)
        {
            if (this->pLineFlat3DCollector->isBufferUniform)
            {
                F_LogError("*********************** EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObject: Object count is above max, cur: [%d], max: [%d] !", this->nObjectCount, this->nObjectCountMax);
                return nullptr;
            }
            else
            {
                increaseBufferStorage();
            }
        }

        pBufferStorage->CopyBuffer(this->nObjectCount * sizeof(LineFlat3DObjectConstants), sizeof(LineFlat3DObjectConstants), (void*)&object);
        this->nObjectCount++;

        if (isUpdateBuffer)
        {
            this->pBufferStorage->UpdateBuffer();
        }

        PointerBuffer* pPointer = this->pPointerBufferPool->Get(); 
        pPointer->Set(this, this->nObjectCount - 1, this->nObjectCount);
        return pPointer;
    }
    PointerBuffer* EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants> &objects, bool isUpdateBuffer /*= true*/)
    {
        int nStart = this->nObjectCount;
        size_t count = objects.size();
        for (size_t i = 0; i < count; i++)
        {
            if (this->nObjectCount + 1 >= this->nObjectCountMax)
            {
                if (this->pLineFlat3DCollector->isBufferUniform)
                {
                    F_LogError("*********************** EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObjects: Object count is above max, cur: [%d], max: [%d] !", this->nObjectCount, this->nObjectCountMax);
                    break;
                }
                else
                {
                    increaseBufferStorage();
                }
            }

            pBufferStorage->CopyBuffer(this->nObjectCount * sizeof(LineFlat3DObjectConstants), sizeof(LineFlat3DObjectConstants), (void*)&objects[i]);
            this->nObjectCount++;
        }
        if (this->nObjectCount - nStart <= 0)
            return nullptr;

        if (isUpdateBuffer)
        {
            this->pBufferStorage->UpdateBuffer();
        }

        PointerBuffer* pPointer = this->pPointerBufferPool->Get(); 
        pPointer->Set(this, nStart, this->nObjectCount);
        return pPointer;
    }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::RemoveLineFlat3DObject(PointerBuffer* pPointer, bool isUpdateBuffer /*= true*/)
    {
        if (pPointer == nullptr)
            return;

        int nIndexStart = pPointer->nPosStart;
        int nIndexEnd = pPointer->nPosEnd;
        int nCount = nIndexEnd - nIndexStart;
        this->pPointerBufferPool->Back(pPointer);
        if (nCount <= 0)
            return;
        for (std::list<PointerBuffer*>::iterator it = this->pPointerBufferPool->listUsed.begin();
             it != this->pPointerBufferPool->listUsed.end(); ++it)
        {
            PointerBuffer* p = (*it);
            if (p->nPosStart < nIndexEnd)
                continue;
            p->Offset(-nCount);
        }
        this->pBufferStorage->RemoveBufferGap(this->nObjectCount * this->pBufferStorage->nStride, 
                                              nIndexStart * this->pBufferStorage->nStride, 
                                              nIndexEnd * this->pBufferStorage->nStride,
                                              isUpdateBuffer);
        this->nObjectCount -= nCount;
    }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::UpdateBuffer()
    {   
        if (this->pBufferStorage == nullptr)
            return;

        this->pBufferStorage->UpdateBuffer();
    }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::CleanupSwapChain()
    {
        Base::GetWindowPtr()->destroyVkDescriptorSets(this->poDescriptorSets);
        this->poDescriptorSets.clear();
    }
    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::RecreateSwapChain()
    {
        if (this->poDescriptorSets.size() > 0)
            return;

        if (this->pLineFlat3DCollector->isBufferUniform)
        {
            Base::GetWindowPtr()->createVkDescriptorSets("DescriptorSets-Uniform-" + this->pMesh->GetName(), this->pLineFlat3DCollector->poDescriptorSetLayout_Uniform, this->poDescriptorSets);
        }
        else
        {
            Base::GetWindowPtr()->createVkDescriptorSets("DescriptorSets-Storage-" + this->pMesh->GetName(), this->pLineFlat3DCollector->poDescriptorSetLayout_Storage, this->poDescriptorSets);
        }
    }
    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::UpdateDescriptorSets()
    {
        if (this->pBufferStorage == nullptr ||
            this->poDescriptorSets.size() <= 0)
            return;

        if (this->pLineFlat3DCollector->isBufferUniform)
        {
            this->pLineFlat3DCollector->updateDescriptorSets(this->pLineFlat3DCollector->aNamesDescriptorSetLayout_Uniform, this->poDescriptorSets, this->pBufferStorage);
        }
        else
        {
            this->pLineFlat3DCollector->updateDescriptorSets(this->pLineFlat3DCollector->aNamesDescriptorSetLayout_Storage, this->poDescriptorSets, this->pBufferStorage);
        }
    }


    ////////////////////// EditorLineFlat3DCollector ////////////////////
    // Line
    const String EditorLineFlat3DCollector::c_strLine3D_Line = "geo_line_line_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Triangle = "geo_line_triangle_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Quad = "geo_line_quad_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Grid = "geo_line_grid_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Quad_Convex = "geo_line_quad_convex_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Quad_Concave = "geo_line_quad_concave_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Circle = "geo_line_circle_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_AABB = "geo_line_aabb_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Sphere = "geo_line_sphere_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Cylinder = "geo_line_cylinder_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Capsule = "geo_line_capsule_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Cone = "geo_line_cone_3d";
    const String EditorLineFlat3DCollector::c_strLine3D_Torus = "geo_line_torus_3d";

    // Flat
    const String EditorLineFlat3DCollector::c_strFlat3D_Triangle = "geo_flat_triangle_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Quad = "geo_flat_quad_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Quad_Convex = "geo_flat_quad_convex_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Quad_Concave = "geo_flat_quad_concave_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Circle = "geo_flat_circle_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_AABB = "geo_flat_aabb_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Sphere = "geo_flat_sphere_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Cylinder = "geo_flat_cylinder_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Capsule = "geo_flat_capsule_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Cone = "geo_flat_cone_3d";
    const String EditorLineFlat3DCollector::c_strFlat3D_Torus = "geo_flat_torus_3d";


    const String EditorLineFlat3DCollector::s_strNameShader_LineFlat3D_Vert = "vert_editor_line_3d";
    const String EditorLineFlat3DCollector::s_strNameShader_LineFlat3D_Frag = "frag_editor_line_3d";
    const String EditorLineFlat3DCollector::s_strNameShader_LineFlat3D_Ext_Vert = "vert_editor_line_3d_ext";
    const String EditorLineFlat3DCollector::s_strNameShader_LineFlat3D_Ext_Frag = "frag_editor_line_3d_ext";

    EditorLineFlat3DCollector::EditorLineFlat3DCollector()
        : EditorBase("EditorLineFlat3DCollector")
        , isBufferUniform(true)

        //PipelineGraphics-Uniform
        , nameDescriptorSetLayout_Uniform("Pass-ObjectLineFlat3D")
        , poDescriptorSetLayout_Uniform(VK_NULL_HANDLE)
        , poPipelineLayout_Uniform(VK_NULL_HANDLE)
        , poPipeline_Uniform_Line(VK_NULL_HANDLE)
        , poPipeline_Uniform_Flat(VK_NULL_HANDLE)
        , poPipeline_Uniform_Flat_Wire(VK_NULL_HANDLE)

        //PipelineGraphics-Storage
        , nameDescriptorSetLayout_Storage("Pass-BufferObjectLineFlat3D")
        , poDescriptorSetLayout_Storage(VK_NULL_HANDLE)
        , poPipelineLayout_Storage(VK_NULL_HANDLE)
        , poPipeline_Storage_Line(VK_NULL_HANDLE)
        , poPipeline_Storage_Flat(VK_NULL_HANDLE)
        , poPipeline_Storage_Flat_Wire(VK_NULL_HANDLE)

    {

    }

    EditorLineFlat3DCollector::~EditorLineFlat3DCollector()
    {
        Destroy();
    }

    void EditorLineFlat3DCollector::Destroy()
    {
        CleanupSwapChain();
        destroyMeshes();
        removeBufferLineFlat3DAll();
    }
        void EditorLineFlat3DCollector::destroyMeshes()
        {
            this->mapName2Mesh.clear();
        }
        void EditorLineFlat3DCollector::destroyBufferUniforms()
        {
            
        }
        void EditorLineFlat3DCollector::destroyPipelineGraphics()
        {
            destroyDescriptorSets_Graphics();

            //PipelineGraphics-Uniform
            if (this->poPipeline_Uniform_Line != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_Uniform_Line);
            }
            this->poPipeline_Uniform_Line = VK_NULL_HANDLE;
            if (this->poPipeline_Uniform_Flat != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_Uniform_Flat);
            }
            this->poPipeline_Uniform_Flat = VK_NULL_HANDLE;
            if (this->poPipeline_Uniform_Flat_Wire != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_Uniform_Flat_Wire);
            }
            this->poPipeline_Uniform_Flat_Wire = VK_NULL_HANDLE;
            
            //PipelineGraphics-Storage
            if (this->poPipeline_Storage_Line != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_Storage_Line);
            }
            this->poPipeline_Storage_Line = VK_NULL_HANDLE;
            if (this->poPipeline_Storage_Flat != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_Storage_Flat);
            }
            this->poPipeline_Storage_Flat = VK_NULL_HANDLE;
            if (this->poPipeline_Storage_Flat_Wire != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_Storage_Flat_Wire);
            }
            this->poPipeline_Storage_Flat_Wire = VK_NULL_HANDLE;
            
        }
        void EditorLineFlat3DCollector::destroyPipelineLayout()
        {
            //PipelineGraphics-Uniform  
            if (this->poPipelineLayout_Uniform != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipelineLayout(this->poPipelineLayout_Uniform);
            }
            this->poPipelineLayout_Uniform = VK_NULL_HANDLE;

            //PipelineGraphics-Storage
            if (this->poPipelineLayout_Storage != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipelineLayout(this->poPipelineLayout_Storage);
            }
            this->poPipelineLayout_Storage = VK_NULL_HANDLE;
        }
        void EditorLineFlat3DCollector::destroyDescriptorSetLayout()
        {
            //PipelineGraphics-Uniform
            this->aNamesDescriptorSetLayout_Uniform.clear();
            if (this->poDescriptorSetLayout_Uniform != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkDescriptorSetLayout(this->poDescriptorSetLayout_Uniform);
            }
            this->poDescriptorSetLayout_Uniform = VK_NULL_HANDLE;

            //PipelineGraphics-Storage
            this->aNamesDescriptorSetLayout_Storage.clear();
            if (this->poDescriptorSetLayout_Storage != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkDescriptorSetLayout(this->poDescriptorSetLayout_Storage);
            }
            this->poDescriptorSetLayout_Storage = VK_NULL_HANDLE;
        }

    void EditorLineFlat3DCollector::UpdateCBs()
    {

    }

    void EditorLineFlat3DCollector::Draw(VkCommandBuffer& commandBuffer)
    {
        size_t count = this->aBufferLineFlat3D.size();
        if (count <= 0)
            return;

        VulkanWindow* pWindow = Base::GetWindowPtr();
        for (size_t i = 0; i < count; i++)
        {
            BufferStorageLineFlat3D* pLineFlat3D =  this->aBufferLineFlat3D[i];
            int count_instance = pLineFlat3D->nObjectCount;
            if (count_instance <= 0)
                continue;
            
            VkPipeline vkPipeline = VK_NULL_HANDLE;
            VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

            if (this->isBufferUniform)
            {
                vkPipelineLayout = this->poPipelineLayout_Uniform;

                if (pLineFlat3D->isLineList)
                {
                    vkPipeline = this->poPipeline_Uniform_Line;
                }
                else
                {
                    if (pWindow->cfg_isWireFrame)
                        vkPipeline = this->poPipeline_Uniform_Flat_Wire;
                    else
                        vkPipeline = this->poPipeline_Uniform_Flat;
                }
            }
            else
            {
                vkPipelineLayout = this->poPipelineLayout_Storage;

                if (pLineFlat3D->isLineList)
                {
                    vkPipeline = this->poPipeline_Storage_Line;
                }
                else
                {
                    if (pWindow->cfg_isWireFrame)
                        vkPipeline = this->poPipeline_Storage_Flat_Wire;
                    else
                        vkPipeline = this->poPipeline_Storage_Flat;
                }
            }
            pWindow->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
            pWindow->bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &pLineFlat3D->poDescriptorSets[pWindow->poSwapChainImageIndex], 0, nullptr);
            
            MeshSub* pMeshSub = pLineFlat3D->pMesh->aMeshSubs[0];
            VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            pWindow->bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
            pWindow->bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            pWindow->drawIndexed(commandBuffer, pMeshSub->poIndexCount, count_instance, 0, 0, 0);
        }
    }

    void EditorLineFlat3DCollector::CleanupSwapChain()
    {
        EditorBase::CleanupSwapChain();

    }
    void EditorLineFlat3DCollector::RecreateSwapChain()
    {
        EditorBase::RecreateSwapChain();
    }

    void EditorLineFlat3DCollector::ChangeBufferMode(bool isUniform)
    {
        if (this->isBufferUniform == isUniform)
            return;

        this->isBufferUniform = isUniform;
        updateDescriptorSets_Graphics();
    }

    //Line 3D
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Line(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Line(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Line(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Line, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Triangle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Triangle(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Triangle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Triangle, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Quad(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Quad(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Quad(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Quad, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Grid(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Grid(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Grid(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Grid, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Quad_Convex(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Quad_Convex(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Quad_Convex(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Quad_Convex, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Quad_Concave(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Quad_Concave(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Quad_Concave(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Quad_Concave, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Circle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Circle(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Circle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Circle, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_AABB(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_AABB(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_AABB(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_AABB, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Sphere(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Sphere(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Sphere(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Sphere, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Cylinder(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Cylinder(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Cylinder(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Cylinder, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Capsule(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Capsule(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Capsule(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Capsule, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Cone(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Cone(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Cone(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Cone, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Torus(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddLine3D_Torus(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddLine3D_Torus(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strLine3D_Torus, true);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }

    //Flat 3D
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Triangle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Triangle(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Triangle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Triangle, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Quad(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Quad(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Quad(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Quad, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Quad_Convex(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Quad_Convex(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Quad_Convex(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Quad_Convex, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Quad_Concave(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Quad_Concave(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Quad_Concave(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Quad_Concave, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Circle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Circle(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Circle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Circle, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_AABB(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_AABB(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_AABB(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_AABB, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Sphere(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Sphere(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Sphere(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Sphere, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Cylinder(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Cylinder(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Cylinder(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Cylinder, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Capsule(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Capsule(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Capsule(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Capsule, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Cone(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Cone(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Cone(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Cone, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Torus(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        return AddFlat3D_Torus(FMath::FromTRS(vPos, vRotAngle, vScale), color, isUpdateBuffer);
    }
    PointerBuffer* EditorLineFlat3DCollector::AddFlat3D_Torus(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer /*= true*/)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getOrCreateBufferLineFlat3D(c_strFlat3D_Torus, false);
        return pBufferLineFlat3D->AddLineFlat3DObject(vMat, color, isUpdateBuffer);
    }

    void EditorLineFlat3DCollector::RemoveLineFlat3D(PointerBuffer* pPointer, bool isUpdateBuffer /*= true*/)
    {
        if (pPointer == nullptr)
            return;

        BufferStorageLineFlat3D* pBufferLineFlat3D = (BufferStorageLineFlat3D*)pPointer->pBase;
        pBufferLineFlat3D->RemoveLineFlat3DObject(pPointer, isUpdateBuffer);
    }
    void EditorLineFlat3DCollector::UpdateBuffer(BufferStorageLineFlat3D* pBufferLineFlat3D)
    {
        if (pBufferLineFlat3D == nullptr)
            return;
        pBufferLineFlat3D->UpdateBuffer();
    }
    void EditorLineFlat3DCollector::UpdateBuffer(const String& nameMesh)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = getBufferLineFlat3D(nameMesh);
        UpdateBuffer(pBufferLineFlat3D);
    }

    void EditorLineFlat3DCollector::initConfigs()
    {
        //Mesh
        {
            this->mapName2Mesh.clear();

            // Line
            this->mapName2Mesh[c_strLine3D_Line] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Line);
            this->mapName2Mesh[c_strLine3D_Triangle] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Triangle);
            this->mapName2Mesh[c_strLine3D_Quad] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Quad);
            this->mapName2Mesh[c_strLine3D_Grid] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Grid);
            this->mapName2Mesh[c_strLine3D_Quad_Convex] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Quad_Convex);
            this->mapName2Mesh[c_strLine3D_Quad_Concave] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Quad_Concave);
            this->mapName2Mesh[c_strLine3D_Circle] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Circle);
            this->mapName2Mesh[c_strLine3D_AABB] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_AABB);
            this->mapName2Mesh[c_strLine3D_Sphere] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Sphere);
            this->mapName2Mesh[c_strLine3D_Cylinder] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Cylinder);
            this->mapName2Mesh[c_strLine3D_Capsule] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Capsule);
            this->mapName2Mesh[c_strLine3D_Cone] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Cone);
            this->mapName2Mesh[c_strLine3D_Torus] = Base::GetWindowPtr()->FindMesh_Internal(c_strLine3D_Torus);

            // Flat
            this->mapName2Mesh[c_strFlat3D_Triangle] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Triangle);
            this->mapName2Mesh[c_strFlat3D_Quad] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Quad);
            this->mapName2Mesh[c_strFlat3D_Quad_Convex] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Quad_Convex);
            this->mapName2Mesh[c_strFlat3D_Quad_Concave] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Quad_Concave);
            this->mapName2Mesh[c_strFlat3D_Circle] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Circle);
            this->mapName2Mesh[c_strFlat3D_AABB] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_AABB);
            this->mapName2Mesh[c_strFlat3D_Sphere] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Sphere);
            this->mapName2Mesh[c_strFlat3D_Cylinder] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Cylinder);
            this->mapName2Mesh[c_strFlat3D_Capsule] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Capsule);
            this->mapName2Mesh[c_strFlat3D_Cone] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Cone);
            this->mapName2Mesh[c_strFlat3D_Torus] = Base::GetWindowPtr()->FindMesh_Internal(c_strFlat3D_Torus);
        }

        //Shader
        {
            //Line3D
            {
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_LineFlat3D_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = "Assets/Shader/editor_line_3d.vert.spv";
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_LineFlat3D_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = "Assets/Shader/editor_line_3d.frag.spv";
                this->aShaderModuleInfos.push_back(siFrag);
            }
            //Line3D Ext
            {
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_LineFlat3D_Ext_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = "Assets/Shader/editor_line_3d_ext.vert.spv";
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_LineFlat3D_Ext_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = "Assets/Shader/editor_line_3d_ext.frag.spv";
                this->aShaderModuleInfos.push_back(siFrag);
            }
        }
    }
    void EditorLineFlat3DCollector::initBufferUniforms()
    {
        initBuffer(c_strLine3D_AABB, false, true);
        initBuffer(c_strLine3D_Sphere, false, true);
        initBuffer(c_strFlat3D_AABB, false, false);
        initBuffer(c_strFlat3D_Sphere, false, false);
    }
        EditorLineFlat3DCollector::BufferStorageLineFlat3D* EditorLineFlat3DCollector::initBuffer(const String &nameMesh, bool isBindDescriptor, bool bIsLineList)
        {
            BufferStorageLineFlat3D* pBufferLineFlat3D = getBufferLineFlat3D(nameMesh);
            if (pBufferLineFlat3D != nullptr)
                return pBufferLineFlat3D;
            pBufferLineFlat3D = insertBufferLineFlat3D(nameMesh, bIsLineList);

            if (isBindDescriptor)
            {
                pBufferLineFlat3D->RecreateSwapChain();
                pBufferLineFlat3D->UpdateDescriptorSets();
            }
            return pBufferLineFlat3D;
        }
    void EditorLineFlat3DCollector::initDescriptorSetLayout()
    {
        //PipelineGraphics-Uniform
        this->aNamesDescriptorSetLayout_Uniform = FUtilString::Split(this->nameDescriptorSetLayout_Uniform, "-");
        this->poDescriptorSetLayout_Uniform = Base::GetWindowPtr()->CreateDescriptorSetLayout(this->nameDescriptorSetLayout_Uniform, &this->aNamesDescriptorSetLayout_Uniform);
        if (this->poDescriptorSetLayout_Uniform == VK_NULL_HANDLE)
        {
            String msg = "*********************** EditorLineFlat3DCollector::initDescriptorSetLayout: Can not create VkDescriptorSetLayout by name: " + this->nameDescriptorSetLayout_Uniform;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //PipelineGraphics-Storage
        this->aNamesDescriptorSetLayout_Storage = FUtilString::Split(this->nameDescriptorSetLayout_Storage, "-");
        this->poDescriptorSetLayout_Storage = Base::GetWindowPtr()->CreateDescriptorSetLayout(this->nameDescriptorSetLayout_Storage, &this->aNamesDescriptorSetLayout_Storage);
        if (this->poDescriptorSetLayout_Storage == VK_NULL_HANDLE)
        {
            String msg = "*********************** EditorLineFlat3DCollector::initDescriptorSetLayout: Can not create VkDescriptorSetLayout by name: " + this->nameDescriptorSetLayout_Storage;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
    void EditorLineFlat3DCollector::initPipelineLayout()
    {
        //PipelineGraphics-Uniform
        VkDescriptorSetLayoutVector aDescriptorSetLayout_Uniform;
        aDescriptorSetLayout_Uniform.push_back(this->poDescriptorSetLayout_Uniform);
        this->poPipelineLayout_Uniform = Base::GetWindowPtr()->createVkPipelineLayout("PipelineLayout-Uniform-" + this->name, aDescriptorSetLayout_Uniform);
        if (this->poPipelineLayout_Uniform == VK_NULL_HANDLE)
        {
            String msg = "*********************** EditorLineFlat3DCollector::initPipelineLayout: Can not create VkPipelineLayout by descriptorSetLayout name: " + this->nameDescriptorSetLayout_Uniform;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //PipelineGraphics-Storage
        VkDescriptorSetLayoutVector aDescriptorSetLayout_Storage;
        aDescriptorSetLayout_Storage.push_back(this->poDescriptorSetLayout_Storage);
        this->poPipelineLayout_Storage = Base::GetWindowPtr()->createVkPipelineLayout("PipelineLayout-" + this->name, aDescriptorSetLayout_Storage);
        if (this->poPipelineLayout_Storage == VK_NULL_HANDLE)
        {
            String msg = "*********************** EditorLineFlat3DCollector::initPipelineLayout: Can not create VkPipelineLayout by descriptorSetLayout name: " + this->nameDescriptorSetLayout_Storage;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
    void EditorLineFlat3DCollector::initPipelineGraphics()
    {
        createDescriptorSets_Graphics();
        updateDescriptorSets_Graphics();

        VkDynamicStateVector aDynamicStates =
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };

        VkStencilOpState stencilOpFront; 
        VkStencilOpState stencilOpBack;

        VkViewportVector aViewports;
        aViewports.push_back(Base::GetWindowPtr()->poViewport);
        VkRect2DVector aScissors;
        aScissors.push_back(Base::GetWindowPtr()->poScissor);

        //PipelineGraphics-Uniform
        {
            VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Uniform;
            if (!Base::GetWindowPtr()->CreatePipelineShaderStageCreateInfos(s_strNameShader_LineFlat3D_Vert,
                                                                            "",
                                                                            "",
                                                                            "",
                                                                            s_strNameShader_LineFlat3D_Frag,
                                                                            this->mapShaderModules,
                                                                            aShaderStageCreateInfos_Uniform))
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            this->poPipeline_Uniform_Line = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-Uniform-Line-" + this->name,
                                                                                           aShaderStageCreateInfos_Uniform,
                                                                                           false, 0, 3,
                                                                                           Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4), 
                                                                                           Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4),
                                                                                           Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout_Uniform, aViewports, aScissors, aDynamicStates,
                                                                                           VK_PRIMITIVE_TOPOLOGY_LINE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                           VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                           VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                           VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                           VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                           VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_Uniform_Line == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Failed to create pipeline graphics for [EditorLineFlat3DCollector-Uniform-Line] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorLineFlat3DCollector::initPipelineGraphics: [EditorLineFlat3DCollector-Uniform-Line] Create pipeline graphics success !");

            this->poPipeline_Uniform_Flat = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-Uniform-Flat-" + this->name,
                                                                                           aShaderStageCreateInfos_Uniform,
                                                                                           false, 0, 3,
                                                                                           Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4), 
                                                                                           Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4),
                                                                                           Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout_Uniform, aViewports, aScissors, aDynamicStates,
                                                                                           VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                           VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                           VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                           VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                           VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                           VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_Uniform_Flat == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Failed to create pipeline graphics for [EditorLineFlat3DCollector-Uniform-Flat] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorLineFlat3DCollector::initPipelineGraphics: [EditorLineFlat3DCollector-Uniform-Flat] Create pipeline graphics success !");

            this->poPipeline_Uniform_Flat_Wire = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-Uniform-Flat-Wire-" + this->name,
                                                                                                aShaderStageCreateInfos_Uniform,
                                                                                                false, 0, 3,
                                                                                                Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4), 
                                                                                                Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4),
                                                                                                Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout_Uniform, aViewports, aScissors, aDynamicStates,
                                                                                                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                                VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                                VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_Uniform_Flat_Wire == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Failed to create pipeline graphics for [EditorLineFlat3DCollector-Uniform-Flat-Wire] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorLineFlat3DCollector::initPipelineGraphics: [EditorLineFlat3DCollector-Uniform-Flat-Wire] Create pipeline graphics success !");
        }

        //PipelineGraphics-Storage
        {
            VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Storage;
            if (!Base::GetWindowPtr()->CreatePipelineShaderStageCreateInfos(s_strNameShader_LineFlat3D_Ext_Vert,
                                                                            "",
                                                                            "",
                                                                            "",
                                                                            s_strNameShader_LineFlat3D_Ext_Frag,
                                                                            this->mapShaderModules,
                                                                            aShaderStageCreateInfos_Storage))
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            this->poPipeline_Storage_Line = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-Storage-Line-" + this->name,
                                                                                           aShaderStageCreateInfos_Storage,
                                                                                           false, 0, 3,
                                                                                           Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4), 
                                                                                           Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4),
                                                                                           Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout_Storage, aViewports, aScissors, aDynamicStates,
                                                                                           VK_PRIMITIVE_TOPOLOGY_LINE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                           VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                           VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                           VK_FALSE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                           VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                           VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_Storage_Line == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Failed to create pipeline graphics for [EditorLineFlat3DCollector-Storage-Line] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorLineFlat3DCollector::initPipelineGraphics: [EditorLineFlat3DCollector-Storage-Line] Create pipeline graphics success !");

            this->poPipeline_Storage_Flat = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-Storage-Flat-" + this->name,
                                                                                           aShaderStageCreateInfos_Storage,
                                                                                           false, 0, 3,
                                                                                           Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4), 
                                                                                           Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4),
                                                                                           Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout_Storage, aViewports, aScissors, aDynamicStates,
                                                                                           VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                           VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                           VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                           VK_FALSE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                           VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                           VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_Storage_Flat == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Failed to create pipeline graphics for [EditorLineFlat3DCollector-Storage-Flat] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorLineFlat3DCollector::initPipelineGraphics: [EditorLineFlat3DCollector-Storage-Flat] Create pipeline graphics success !");

            this->poPipeline_Storage_Flat_Wire = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-Storage-Flat-Wire-" + this->name,
                                                                                                aShaderStageCreateInfos_Storage,
                                                                                                false, 0, 3,
                                                                                                Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4), 
                                                                                                Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4),
                                                                                                Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout_Storage, aViewports, aScissors, aDynamicStates,
                                                                                                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                                VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                                VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                VK_FALSE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_Storage_Flat_Wire == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorLineFlat3DCollector::initPipelineGraphics: Failed to create pipeline graphics for [EditorLineFlat3DCollector-Storage-Flat-Wire] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorLineFlat3DCollector::initPipelineGraphics: [EditorLineFlat3DCollector-Storage-Flat-Wire] Create pipeline graphics success !");
        }
    }   
    void EditorLineFlat3DCollector::destroyDescriptorSets_Graphics()
    {
        size_t count = this->aBufferLineFlat3D.size();
        for (size_t i = 0; i < count; i++)
        {
            BufferStorageLineFlat3D* p = this->aBufferLineFlat3D[i];
            p->CleanupSwapChain();
        }
    }
    void EditorLineFlat3DCollector::createDescriptorSets_Graphics()
    {
        size_t count = this->aBufferLineFlat3D.size();
        for (size_t i = 0; i < count; i++)
        {
            BufferStorageLineFlat3D* p = this->aBufferLineFlat3D[i];
            p->RecreateSwapChain();
        }
    }
    void EditorLineFlat3DCollector::updateDescriptorSets_Graphics()
    {
        size_t count = this->aBufferLineFlat3D.size();
        for (size_t i = 0; i < count; i++)
        {
            BufferStorageLineFlat3D* p = this->aBufferLineFlat3D[i];
            p->UpdateDescriptorSets();
        }
    }
        void EditorLineFlat3DCollector::updateDescriptorSets(const StringVector& aNamesDescriptorSetLayout, const VkDescriptorSetVector& vkDescriptorSets, BufferStorage* pBufferStorage)
        {
            F_Assert(pBufferStorage != nullptr && "EditorLineFlat3DCollector::updateDescriptorSets")
            uint32_t count_ds = (uint32_t)vkDescriptorSets.size();
            for (uint32_t i = 0; i < count_ds; i++)
            {
                VkWriteDescriptorSetVector descriptorWrites;

                uint32_t count_names = (uint32_t)aNamesDescriptorSetLayout.size();
                for (uint32_t j = 0; j < count_names; j++)
                {
                    const String& nameDescriptorSet = aNamesDescriptorSetLayout[j];
                    if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                    {
                        VkDescriptorBufferInfo bufferInfo_Pass = {};
                        bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i];
                        bufferInfo_Pass.offset = 0;
                        bufferInfo_Pass.range = sizeof(PassConstants);
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_Pass);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectLineFlat3D)) //ObjectLineFlat3D
                    {
                        VkDescriptorBufferInfo bufferInfo_ObjectLineFlat3D = {};
                        bufferInfo_ObjectLineFlat3D.buffer = pBufferStorage->poBuffer_Storage;
                        bufferInfo_ObjectLineFlat3D.offset = 0;
                        bufferInfo_ObjectLineFlat3D.range = pBufferStorage->GetBufferSize();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_ObjectLineFlat3D);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferObjectLineFlat3D)) //BufferObjectLineFlat3D
                    {
                        VkDescriptorBufferInfo bufferInfo_BufferObjectLineFlat3D = {};
                        bufferInfo_BufferObjectLineFlat3D.buffer = pBufferStorage->poBuffer_Storage;
                        bufferInfo_BufferObjectLineFlat3D.offset = 0;
                        bufferInfo_BufferObjectLineFlat3D.range = pBufferStorage->GetBufferSize();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_BufferObjectLineFlat3D);
                    }
                    else
                    {
                        String msg = "*********************** EditorLineFlat3DCollector::updateDescriptorSets: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                }
                Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
            }
        }

    Mesh *EditorLineFlat3DCollector::getMesh(const String &nameMesh)
    {
        MeshPtrMap::iterator itFind = this->mapName2Mesh.find(nameMesh);
        if (itFind == this->mapName2Mesh.end())
        {
            F_Assert(false && "EditorLineFlat3DCollector::getMesh") return nullptr;
        }
        return itFind->second;
    }

    bool EditorLineFlat3DCollector::hasBufferLineFlat3D(const String &nameMesh)
    {
        return getBufferLineFlat3D(nameMesh) != nullptr;
    }
    EditorLineFlat3DCollector::BufferStorageLineFlat3D* EditorLineFlat3DCollector::getBufferLineFlat3D(const String &nameMesh)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::getBufferLineFlat3D")

        Mesh2BufferStorageLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferLineFlat3D.find(pMesh);
        if (itFind == this->mapMesh2BufferLineFlat3D.end())
            return nullptr;
        return itFind->second;
    }
    EditorLineFlat3DCollector::BufferStorageLineFlat3D* EditorLineFlat3DCollector::getOrCreateBufferLineFlat3D(const String& nameMesh, bool bIsLineList)
    {
        BufferStorageLineFlat3D* pBufferLineFlat3D = initBuffer(nameMesh, true, bIsLineList);
        return pBufferLineFlat3D;
    }
    EditorLineFlat3DCollector::BufferStorageLineFlat3D* EditorLineFlat3DCollector::insertBufferLineFlat3D(const String &nameMesh, bool bIsLineList)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::insertBufferLineFlat3D")

        Mesh2BufferStorageLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferLineFlat3D.find(pMesh);
        if (itFind != this->mapMesh2BufferLineFlat3D.end())
        {
            return itFind->second;
        }

        BufferStorageLineFlat3D* pBufferLineFlat3D = new BufferStorageLineFlat3D(this, pMesh, bIsLineList);
        pBufferLineFlat3D->Init();

        this->aBufferLineFlat3D.push_back(pBufferLineFlat3D);
        this->mapMesh2BufferLineFlat3D[pMesh] = pBufferLineFlat3D;
        return pBufferLineFlat3D;
    }
    void EditorLineFlat3DCollector::removeBufferLineFlat3D(const String &nameMesh)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::removeBufferLineFlat3D")

        Mesh2BufferStorageLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferLineFlat3D.find(pMesh);
        if (itFind == this->mapMesh2BufferLineFlat3D.end())
            return;
        BufferStorageLineFlat3D* pBufferLineFlat3D = itFind->second;

        BufferStorageLineFlat3DPtrVector::iterator itFindV = std::find(this->aBufferLineFlat3D.begin(), this->aBufferLineFlat3D.end(), pBufferLineFlat3D);
        if (itFindV != this->aBufferLineFlat3D.end())
        {
            this->aBufferLineFlat3D.erase(itFindV);
        }
        this->mapMesh2BufferLineFlat3D.erase(itFind);
        F_DELETE(pBufferLineFlat3D)
    }
    void EditorLineFlat3DCollector::removeBufferLineFlat3DAll()
    {
        for (BufferStorageLineFlat3DPtrVector::iterator it = this->aBufferLineFlat3D.begin();
             it != this->aBufferLineFlat3D.end(); ++it)
        {
            F_DELETE(*it)
        }
        this->aBufferLineFlat3D.clear();
        this->mapMesh2BufferLineFlat3D.clear();
    }

}; // LostPeterVulkan