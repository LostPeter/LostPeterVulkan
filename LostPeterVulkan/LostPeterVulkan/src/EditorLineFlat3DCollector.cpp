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
#include "../include/BufferStorage.h"

namespace LostPeterVulkan
{
    ////////////////////// BufferBaseLineFlat3D /////////////////////////
    EditorLineFlat3DCollector::BufferBaseLineFlat3D::BufferBaseLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh* p)
        : pLineFlat3DCollector(pCollector)
        , pMesh(p)
        , pLineFlat3DObject(nullptr)
        , nObjectCount(0)
    {
        F_Assert(pLineFlat3DCollector != nullptr && pMesh != nullptr && "EditorLineFlat3DCollector::BufferBaseLineFlat3D::BufferBaseLineFlat3D")
    }
    EditorLineFlat3DCollector::BufferBaseLineFlat3D::~BufferBaseLineFlat3D()
    {

    }


    ////////////////////// BufferStorageLineFlat3D //////////////////////
    const int EditorLineFlat3DCollector::BufferStorageLineFlat3D::s_nStepCount = 128;
    EditorLineFlat3DCollector::BufferStorageLineFlat3D::BufferStorageLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh *p)
        : BufferBaseLineFlat3D(pCollector, p)
        , pBufferStorage(nullptr)
        , nObjectCountMax(MAX_OBJECT_LINEFLAT_3D_COUNT)
    {
        
    } 
    EditorLineFlat3DCollector::BufferStorageLineFlat3D::~BufferStorageLineFlat3D()
    {
        Destroy();
    }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::Destroy()
    {
        Clear();
        F_DELETE(this->pBufferStorage)
    }
    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::Init()
    {
        this->pBufferStorage = createBufferStorage(this->nObjectCountMax);
    }
        BufferStorage* EditorLineFlat3DCollector::BufferStorageLineFlat3D::createBufferStorage(int count)
        {
            String nameBuffer = "BufferStorage-" + this->pMesh->GetName() + FUtilString::SaveInt(count);
            BufferStorage* pBufferStorageNew = new BufferStorage(nameBuffer, count, sizeof(LineFlat3DObjectConstants));
            void *pBuffer = pBufferStorageNew->GetBuffer();
            this->pLineFlat3DObject = (LineFlat3DObjectConstants *)pBuffer;

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
        }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::Clear()
    {
        this->nObjectCount = 0;
        this->nObjectCountMax = MAX_OBJECT_LINEFLAT_3D_COUNT;
    }

    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObject(const FMatrix4 &mat, const FColor &color, bool isUpdateBuffer /*= true*/)
    {
        LineFlat3DObjectConstants object;
        object.g_MatWorld = mat;
        object.color = color;
        AddLineFlat3DObject(object, isUpdateBuffer);
    }
    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObject(const LineFlat3DObjectConstants &object, bool isUpdateBuffer /*= true*/)
    {
        if (this->nObjectCount + 1 >= this->nObjectCountMax)
        {
            if (this->pLineFlat3DCollector->isBufferUniform)
            {
                F_LogError("*********************** EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObject: Object count is above max, cur: [%d], max: [%d] !", this->nObjectCount, this->nObjectCountMax);
                return;
            }
            else
            {
                increaseBufferStorage();
            }
        }

        this->pLineFlat3DObject[this->nObjectCount] = object;
        this->nObjectCount++;

        if (isUpdateBuffer)
        {
            this->pBufferStorage->UpdateBuffer();
        }
    }
    void EditorLineFlat3DCollector::BufferStorageLineFlat3D::AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants> &objects, bool isUpdateBuffer /*= true*/)
    {
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

            this->pLineFlat3DObject[this->nObjectCount] = objects[i];
            this->nObjectCount++;
        }

        if (isUpdateBuffer)
        {
            this->pBufferStorage->UpdateBuffer();
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
    const String EditorLineFlat3DCollector::s_strNameShader_LineFlat3D_Ext_Vert = "vert_editor_line_3d";
    const String EditorLineFlat3DCollector::s_strNameShader_LineFlat3D_Ext_Frag = "frag_editor_line_3d";

    EditorLineFlat3DCollector::EditorLineFlat3DCollector()
        : EditorBase("EditorLineFlat3DCollector")
        , isBufferUniform(true)

        //PipelineGraphics-Uniform
        , nameDescriptorSetLayout_Uniform("Pass-ObjectLineFlat3D")
        , poDescriptorSetLayoutNames_Uniform(nullptr)
        , poDescriptorSetLayout_Uniform(VK_NULL_HANDLE)
        , poPipelineLayout_Uniform(VK_NULL_HANDLE)
        , poPipeline_Uniform(VK_NULL_HANDLE)
        , poDescriptorSet_Uniform(VK_NULL_HANDLE)

        //PipelineGraphics-Storage
        , nameDescriptorSetLayout_Storage("Pass-ObjectLineFlat3D")
        , poDescriptorSetLayoutNames_Storage(nullptr)
        , poDescriptorSetLayout_Storage(VK_NULL_HANDLE)
        , poPipelineLayout_Storage(VK_NULL_HANDLE)
        , poPipeline_Storage(VK_NULL_HANDLE)
        , poDescriptorSet_Storage(VK_NULL_HANDLE)

    {

    }

    EditorLineFlat3DCollector::~EditorLineFlat3DCollector()
    {

    }

    void EditorLineFlat3DCollector::Destroy()
    {
        destroyBufferUniforms();
        destroyShaders();
        destroyMeshes();
    }
        void EditorLineFlat3DCollector::destroyMeshes()
        {
            this->mapName2Mesh.clear();
        }
        void EditorLineFlat3DCollector::destroyBufferUniforms()
        {
            removeBufferLineFlat3DAll();
        }
        void EditorLineFlat3DCollector::destroyPipelineGraphics()
        {

        }
        void EditorLineFlat3DCollector::destroyPipelineLayout()
        {

        }
        void EditorLineFlat3DCollector::destroyDescriptorSetLayout()
        {
            
        }

    void EditorLineFlat3DCollector::UpdateCBs()
    {

    }

    void EditorLineFlat3DCollector::Draw(VkCommandBuffer& commandBuffer)
    {
        if (this->isBufferUniform)
        {
            //for ()
        }
        else
        {

        }
    }

    //Line 3D
    bool EditorLineFlat3DCollector::AddLine3D_Line()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Triangle()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Quad()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Grid()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Quad_Convex()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Quad_Concave()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Circle()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_AABB()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Sphere()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Cylinder()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Capsule()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Cone()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddLine3D_Torus()
    {
        return true;
    }

    //Flat 3D
    bool EditorLineFlat3DCollector::AddFlat3D_Triangle()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Quad()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Quad_Convex()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Quad_Concave()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Circle()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_AABB()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Sphere()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Cylinder()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Capsule()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Cone()
    {
        return true;
    }
    bool EditorLineFlat3DCollector::AddFlat3D_Torus()
    {
        return true;
    }


    void EditorLineFlat3DCollector::initConfigs()
    {

    }
    void EditorLineFlat3DCollector::initMeshes()
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
    void EditorLineFlat3DCollector::initShaders()
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

        EditorBase::initShaders();
    }
    void EditorLineFlat3DCollector::initBufferUniforms()
    {
        initBuffer(c_strLine3D_AABB);
        initBuffer(c_strLine3D_Sphere);
        initBuffer(c_strFlat3D_AABB);
        initBuffer(c_strFlat3D_Sphere);
    }
        void EditorLineFlat3DCollector::initBuffer(const String &nameMesh)
        {
            if (hasBufferLineFlat3D(nameMesh))
                return;
            insertBufferLineFlat3D(nameMesh);
        }
    void EditorLineFlat3DCollector::initDescriptorSetLayout()
    {

    }
    void EditorLineFlat3DCollector::initPipelineLayout()
    {

    }
    void EditorLineFlat3DCollector::initPipelineGraphics()
    {

    }
    void EditorLineFlat3DCollector::updateDescriptorSets_Graphics()
    {

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
    EditorLineFlat3DCollector::BufferStorageLineFlat3D *EditorLineFlat3DCollector::getBufferLineFlat3D(const String &nameMesh)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::getBufferLineFlat3D")

        Mesh2BufferStorageLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferLineFlat3D.find(pMesh);
        if (itFind == this->mapMesh2BufferLineFlat3D.end())
            return nullptr;
        return itFind->second;
    }
    EditorLineFlat3DCollector::BufferStorageLineFlat3D *EditorLineFlat3DCollector::insertBufferLineFlat3D(const String &nameMesh)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::insertBufferLineFlat3D")

        Mesh2BufferStorageLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferLineFlat3D.find(pMesh);
        if (itFind != this->mapMesh2BufferLineFlat3D.end())
        {
            return itFind->second;
        }

        BufferStorageLineFlat3D *pBufferLineFlat3D = new BufferStorageLineFlat3D(this, pMesh);
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
        BufferStorageLineFlat3D *pBufferLineFlat3D = itFind->second;

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