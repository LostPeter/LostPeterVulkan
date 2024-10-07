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
#include "../include/BufferUniform.h"

namespace LostPeterVulkan
{
    ////////////////////// BufferUniformLineFlat3D //////////////////////
    EditorLineFlat3DCollector::BufferUniformLineFlat3D::BufferUniformLineFlat3D(Mesh *p)
        : pMesh(p), pBufferUniform(nullptr), pLineFlat3DObject(nullptr){
                                                 F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::BufferUniformLineFlat3D::BufferUniformLineFlat3D")} EditorLineFlat3DCollector::BufferUniformLineFlat3D::~BufferUniformLineFlat3D()
    {
        Destroy();
    }

    void EditorLineFlat3DCollector::BufferUniformLineFlat3D::Destroy()
    {
        Clear();
        F_DELETE(this->pBufferUniform)
    }
    void EditorLineFlat3DCollector::BufferUniformLineFlat3D::Init()
    {
        this->pBufferUniform = new BufferUniform("BufferUniform-" + this->pMesh->GetName(), MAX_OBJECT_LINEFLAT_3D_COUNT, sizeof(LineFlat3DObjectConstants));
        void *pBuffer = this->pBufferUniform->GetBuffer();
        this->pLineFlat3DObject = (LineFlat3DObjectConstants *)pBuffer;
    }

    void EditorLineFlat3DCollector::BufferUniformLineFlat3D::Clear()
    {
        this->nObjectCount = 0;
    }

    void EditorLineFlat3DCollector::BufferUniformLineFlat3D::AddLineFlat3DObject(const FMatrix4 &mat, const FColor &color, bool isUpdateBuffer /*= true*/)
    {
        LineFlat3DObjectConstants object;
        object.g_MatWorld = mat;
        object.color = color;
        AddLineFlat3DObject(object, isUpdateBuffer);
    }
    void EditorLineFlat3DCollector::BufferUniformLineFlat3D::AddLineFlat3DObject(const LineFlat3DObjectConstants &object, bool isUpdateBuffer /*= true*/)
    {
        if (this->nObjectCount + 1 >= MAX_OBJECT_LINEFLAT_3D_COUNT)
            return;

        this->pLineFlat3DObject[this->nObjectCount] = object;
        this->nObjectCount++;

        if (isUpdateBuffer)
        {
            this->pBufferUniform->UpdateBuffer();
        }
    }
    void EditorLineFlat3DCollector::BufferUniformLineFlat3D::AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants> &objects, bool isUpdateBuffer /*= true*/)
    {
        size_t count = objects.size();
        for (size_t i = 0; i < count; i++)
        {
            if (this->nObjectCount + 1 >= MAX_OBJECT_LINEFLAT_3D_COUNT)
                break;

            this->pLineFlat3DObject[this->nObjectCount] = objects[i];
            this->nObjectCount++;
        }

        if (isUpdateBuffer)
        {
            this->pBufferUniform->UpdateBuffer();
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

    EditorLineFlat3DCollector::EditorLineFlat3DCollector()
        : EditorBase("EditorLineFlat3DCollector")
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
        void EditorLineFlat3DCollector::destroyShaders()
        {
        }
        void EditorLineFlat3DCollector::destroyBufferUniforms()
        {
            removeBufferUniformLineFlat3DAll();
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
    void EditorLineFlat3DCollector::initMesh(const String &nameMesh)
    {
    }
    void EditorLineFlat3DCollector::initShaders()
    {
    }
    void EditorLineFlat3DCollector::initBufferUniforms()
    {
    }
    void EditorLineFlat3DCollector::initBufferUniform(const String &nameMesh)
    {
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

    bool EditorLineFlat3DCollector::hasBufferUniformLineFlat3D(const String &nameMesh)
    {
        return getBufferUniformLineFlat3D(nameMesh) != nullptr;
    }
    EditorLineFlat3DCollector::BufferUniformLineFlat3D *EditorLineFlat3DCollector::getBufferUniformLineFlat3D(const String &nameMesh)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::getBufferUniformLineFlat3D")

            Mesh2BufferUniformLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferUniformLineFlat3D.find(pMesh);
        if (itFind == this->mapMesh2BufferUniformLineFlat3D.end())
            return nullptr;
        return itFind->second;
    }
    EditorLineFlat3DCollector::BufferUniformLineFlat3D *EditorLineFlat3DCollector::insertBufferUniformLineFlat3D(const String &nameMesh)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::insertBufferUniformLineFlat3D")

            Mesh2BufferUniformLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferUniformLineFlat3D.find(pMesh);
        if (itFind != this->mapMesh2BufferUniformLineFlat3D.end())
        {
            return itFind->second;
        }

        BufferUniformLineFlat3D *pBufferUniformLineFlat3D = new BufferUniformLineFlat3D(pMesh);
        pBufferUniformLineFlat3D->Init();

        this->aBufferUniformLineFlat3D.push_back(pBufferUniformLineFlat3D);
        this->mapMesh2BufferUniformLineFlat3D[pMesh] = pBufferUniformLineFlat3D;
        return pBufferUniformLineFlat3D;
    }
    void EditorLineFlat3DCollector::removeBufferUniformLineFlat3D(const String &nameMesh)
    {
        Mesh *pMesh = getMesh(nameMesh);
        F_Assert(pMesh != nullptr && "EditorLineFlat3DCollector::removeBufferUniformLineFlat3D")

            Mesh2BufferUniformLineFlat3DPtrMap::iterator itFind = this->mapMesh2BufferUniformLineFlat3D.find(pMesh);
        if (itFind == this->mapMesh2BufferUniformLineFlat3D.end())
            return;
        BufferUniformLineFlat3D *pBufferUniformLineFlat3D = itFind->second;

        BufferUniformLineFlat3DPtrVector::iterator itFindV = std::find(this->aBufferUniformLineFlat3D.begin(), this->aBufferUniformLineFlat3D.end(), pBufferUniformLineFlat3D);
        if (itFindV != this->aBufferUniformLineFlat3D.end())
        {
            this->aBufferUniformLineFlat3D.erase(itFindV);
        }
        this->mapMesh2BufferUniformLineFlat3D.erase(itFind);
        F_DELETE(pBufferUniformLineFlat3D)
    }
    void EditorLineFlat3DCollector::removeBufferUniformLineFlat3DAll()
    {
        for (BufferUniformLineFlat3DPtrVector::iterator it = this->aBufferUniformLineFlat3D.begin();
             it != this->aBufferUniformLineFlat3D.end(); ++it)
        {
            F_DELETE(*it)
        }
        this->aBufferUniformLineFlat3D.clear();
        this->mapMesh2BufferUniformLineFlat3D.clear();
    }

}; // LostPeterVulkan