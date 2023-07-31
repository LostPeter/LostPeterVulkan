/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FMeshGeometry.h"
#include "../include/FMath.h"
#include "../include/FUtilString.h"

namespace LostPeterFoundation
{
    ////////////////////////////////// Line //////////////////////////////////
    String FMeshCreateParam_Line::ms_nameType = "MeshLine"; //Line
    String FMeshCreateParam_LineTriangle::ms_nameType = "MeshLineTriangle"; //LineTriangle
    String FMeshCreateParam_LineQuad::ms_nameType = "MeshLineQuad"; //LineQuad
    String FMeshCreateParam_LineGrid::ms_nameType = "MeshLineGrid"; //LineGrid
    String FMeshCreateParam_LineCircle::ms_nameType = "MeshLineCircle"; //LineCircle
    String FMeshCreateParam_LineAABB::ms_nameType = "MeshLineAABB"; //LineAABB    
    String FMeshCreateParam_LineSphere::ms_nameType = "MeshLineSphere"; //LineSphere
    String FMeshCreateParam_LineCylinder::ms_nameType = "MeshLineCylinder"; //LineCylinder
    String FMeshCreateParam_LineCapsule::ms_nameType = "MeshLineCapsule"; //LineCapsule
    String FMeshCreateParam_LineCone::ms_nameType = "MeshLineCone"; //LineCone
    String FMeshCreateParam_LineTorus::ms_nameType = "MeshLineTorus"; //LineTorus


    ////////////////////////////////// Flat //////////////////////////////////
    String FMeshCreateParam_FlatTriangle::ms_nameType = "MeshFlatTriangle"; //FlatTriangle
    String FMeshCreateParam_FlatQuad::ms_nameType = "MeshFlatQuad"; //FlatQuad
    String FMeshCreateParam_FlatCircle::ms_nameType = "MeshFlatCircle"; //FlatCircle
    String FMeshCreateParam_FlatAABB::ms_nameType = "MeshFlatAABB"; //FlatAABB
    String FMeshCreateParam_FlatSphere::ms_nameType = "MeshFlatSphere"; //FlatSphere
    String FMeshCreateParam_FlatCylinder::ms_nameType = "MeshFlatCylinder"; //FlatCylinder
    String FMeshCreateParam_FlatCapsule::ms_nameType = "MeshFlatCapsule"; //FlatCapsule
    String FMeshCreateParam_FlatCone::ms_nameType = "MeshFlatCone"; //FlatCone
    String FMeshCreateParam_FlatTorus::ms_nameType = "MeshFlatTorus"; //FlatTorus


    ////////////////////////////////// Entity ////////////////////////////////
    String FMeshCreateParam_EntityTriangle::ms_nameType = "MeshEntityTriangle"; //EntityTriangle
    String FMeshCreateParam_EntityQuad::ms_nameType = "MeshEntityQuad"; //EntityQuad
    String FMeshCreateParam_EntityGrid::ms_nameType = "MeshEntityGrid"; //EntityGrid   
    String FMeshCreateParam_EntityCircle::ms_nameType = "MeshEntityCircle";  //EntityCircle
    String FMeshCreateParam_EntityAABB::ms_nameType = "MeshEntityAABB"; //EntityAABB
    String FMeshCreateParam_EntitySphere::ms_nameType = "MeshEntitySphere"; //EntitySphere
    String FMeshCreateParam_EntityGeoSphere::ms_nameType = "MeshEntityGeoSphere"; //EntityGeoSphere
    String FMeshCreateParam_EntityCylinder::ms_nameType = "MeshEntityCylinder"; //EntityCylinder
    String FMeshCreateParam_EntityCapsule::ms_nameType = "MeshEntityCapsule"; //EntityCapsule
    String FMeshCreateParam_EntityCone::ms_nameType = "MeshEntityCone"; //EntityCone
    String FMeshCreateParam_EntityTorus::ms_nameType = "MeshEntityTorus"; //EntityTorus
    String FMeshCreateParam_EntitySkyBox::ms_nameType = "MeshEntitySkyBox"; //EntitySkyBox
    String FMeshCreateParam_EntitySkyDome::ms_nameType = "MeshEntitySkyDome"; //EntitySkyDome
    String FMeshCreateParam_EntityTerrain::ms_nameType = "MeshEntityTerrain"; //EntityTerrain
    

    ///////////////////////////////////////// FMeshGeometry //////////////////////////////////////////////
    bool FMeshGeometry::CreateGeometry(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry)
    {
        if (F_IsMeshGeometryType_Entity(typeMeshGeometry))
        {
            F_Assert(pMeshData != nullptr && "FMeshGeometry::CreateGeometry");
            return CreateEntityGeometry(*pMeshData, typeMeshGeometry);
        }
        else if (F_IsMeshGeometryType_Flat(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometry");
            return CreateFlatGeometry(*pMeshDataPC, typeMeshGeometry);
        }
        else if (F_IsMeshGeometryType_Line(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometry");
            return CreateLineGeometry(*pMeshDataPC, typeMeshGeometry);
        }
        
        F_Assert(false && "FMeshGeometry::CreateGeometry: Wrong MeshGeometry type !");
        return false;
    }
	bool FMeshGeometry::CreateGeometryWithParam(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (F_IsMeshGeometryType_Entity(typeMeshGeometry))
        {
            F_Assert(pMeshData != nullptr && "FMeshGeometry::CreateGeometryWithParam");
            return CreateEntityGeometryWithParam(*pMeshData, typeMeshGeometry, pParam);
        }
        else if (F_IsMeshGeometryType_Flat(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometryWithParam");
            return CreateFlatGeometryWithParam(*pMeshDataPC, typeMeshGeometry, pParam);
        }
        else if (F_IsMeshGeometryType_Line(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometryWithParam");
            return CreateLineGeometryWithParam(*pMeshDataPC, typeMeshGeometry, pParam);
        }
        
        F_Assert(false && "FMeshGeometry::CreateGeometryWithParam: Wrong MeshGeometry type !");
        return false;
    }


    ////////////////////////////////// Line //////////////////////////////////
    bool FMeshGeometry::CreateLineGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_Line:
            {
                FMeshCreateParam_Line param_Line;
                FMeshGeometry::CreateLine(meshDataPC, &param_Line);
                return true;
            }
        case F_MeshGeometry_LineTriangle:
            {
                FMeshCreateParam_LineTriangle param_LineTriangle;
                FMeshGeometry::CreateLineTriangle(meshDataPC, &param_LineTriangle);
                return true;
            }
        case F_MeshGeometry_LineQuad:
            {
                FMeshCreateParam_LineQuad param_LineQuad;
                FMeshGeometry::CreateLineQuad(meshDataPC, &param_LineQuad);
                return true;
            }
        case F_MeshGeometry_LineGrid:
            {
                FMeshCreateParam_LineGrid param_LineGrid;
                FMeshGeometry::CreateLineGrid(meshDataPC, &param_LineGrid);
                return true;
            }
        case F_MeshGeometry_LineCircle:
            {
                FMeshCreateParam_LineCircle param_LineCircle;
                FMeshGeometry::CreateLineCircle(meshDataPC, &param_LineCircle);
                return true;
            }
        case F_MeshGeometry_LineAABB:
            {
                FMeshCreateParam_LineAABB param_LineAABB;
                FMeshGeometry::CreateLineAABB(meshDataPC, &param_LineAABB);
                return true;
            }
        case F_MeshGeometry_LineSphere:
            {
                FMeshCreateParam_LineSphere param_LineSphere;
                FMeshGeometry::CreateLineSphere(meshDataPC, &param_LineSphere);
                return true;
            }
        case F_MeshGeometry_LineCylinder:
            {
                FMeshCreateParam_LineCylinder param_LineCylinder;
                FMeshGeometry::CreateLineCylinder(meshDataPC, &param_LineCylinder);
                return true;
            }
        case F_MeshGeometry_LineCapsule:
            {
                FMeshCreateParam_LineCapsule param_LineCapsule;
                FMeshGeometry::CreateLineCapsule(meshDataPC, &param_LineCapsule);
                return true;
            }
        case F_MeshGeometry_LineCone:
            {
                FMeshCreateParam_LineCone param_LineCone;
                FMeshGeometry::CreateLineCone(meshDataPC, &param_LineCone);
                return true;
            }
        case F_MeshGeometry_LineTorus:
            {
                FMeshCreateParam_LineTorus param_LineTorus;
                FMeshGeometry::CreateLineTorus(meshDataPC, &param_LineTorus);
                return true;
            }
        }
        return false;
    }
	bool FMeshGeometry::CreateLineGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (pParam == nullptr)
        {
            return CreateLineGeometry(meshDataPC, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_Line:
            {
                FMeshCreateParam_Line* pParam_Line = static_cast<FMeshCreateParam_Line*>(pParam);
                FMeshGeometry::CreateLine(meshDataPC, pParam_Line);
                return true;
            }
        case F_MeshGeometry_LineTriangle:
            {
                FMeshCreateParam_LineTriangle* pParam_LineTriangle = static_cast<FMeshCreateParam_LineTriangle*>(pParam);
                FMeshGeometry::CreateLineTriangle(meshDataPC, pParam_LineTriangle);
                return true;
            }
        case F_MeshGeometry_LineQuad:
            {
                FMeshCreateParam_LineQuad* pParam_LineQuad = static_cast<FMeshCreateParam_LineQuad*>(pParam);
                FMeshGeometry::CreateLineQuad(meshDataPC, pParam_LineQuad);
                return true;
            }
        case F_MeshGeometry_LineGrid:
            {
                FMeshCreateParam_LineGrid* pParam_LineGrid = static_cast<FMeshCreateParam_LineGrid*>(pParam);
                FMeshGeometry::CreateLineGrid(meshDataPC, pParam_LineGrid);
                return true;
            }
        case F_MeshGeometry_LineCircle:
            {
                FMeshCreateParam_LineCircle* pParam_LineCircle = static_cast<FMeshCreateParam_LineCircle*>(pParam);
                FMeshGeometry::CreateLineCircle(meshDataPC, pParam_LineCircle);
                return true;
            }
        case F_MeshGeometry_LineAABB:
            {
                FMeshCreateParam_LineAABB* pParam_LineAABB = static_cast<FMeshCreateParam_LineAABB*>(pParam);
                FMeshGeometry::CreateLineAABB(meshDataPC, pParam_LineAABB);
                return true;
            }
        case F_MeshGeometry_LineSphere:
            {
                FMeshCreateParam_LineSphere* pParam_LineSphere = static_cast<FMeshCreateParam_LineSphere*>(pParam);
                FMeshGeometry::CreateLineSphere(meshDataPC, pParam_LineSphere);
                return true;
            }
        case F_MeshGeometry_LineCylinder:
            {
                FMeshCreateParam_LineCylinder* pParam_LineCylinder = static_cast<FMeshCreateParam_LineCylinder*>(pParam);
                FMeshGeometry::CreateLineCylinder(meshDataPC, pParam_LineCylinder);
                return true;
            }
        case F_MeshGeometry_LineCapsule:
            {
                FMeshCreateParam_LineCapsule* pParam_LineCapsule = static_cast<FMeshCreateParam_LineCapsule*>(pParam);
                FMeshGeometry::CreateLineCapsule(meshDataPC, pParam_LineCapsule);
                return true;
            }
        case F_MeshGeometry_LineCone:
            {
                FMeshCreateParam_LineCone* pParam_LineCone = static_cast<FMeshCreateParam_LineCone*>(pParam);
                FMeshGeometry::CreateLineCone(meshDataPC, pParam_LineCone);
                return true;
            }
        case F_MeshGeometry_LineTorus:
            {
                FMeshCreateParam_LineTorus* pParam_LineTorus= static_cast<FMeshCreateParam_LineTorus*>(pParam);
                FMeshGeometry::CreateLineTorus(meshDataPC, pParam_LineTorus);
                return true;
            }
        }
        return false;
    }

    //Line
    void FMeshGeometry::CreateLine(FMeshDataPC& meshDataPC,
                                   const FVector3& vStart,
                                   const FVector3& vEnd,
                                   const FVector4& vColor)
    {
        // 0 ----- 1

        //FMeshVertex
        AddVertex(meshDataPC, FMeshVertexPC(vStart, vColor));   //0
        AddVertex(meshDataPC, FMeshVertexPC(vEnd, vColor));     //1

        //Index
        AddIndexLine(meshDataPC, 0, 1);
    }

    //LineTriangle
    void FMeshGeometry::CreateLineTriangle(FMeshDataPC& meshDataPC,
                                           const FVector3& vTop,
                                           const FVector3& vLeft,
                                           const FVector3& vRight,
                                           const FVector4& vColor)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //FMeshVertex
        AddVertex(meshDataPC, FMeshVertexPC(vTop, vColor));     //0
        AddVertex(meshDataPC, FMeshVertexPC(vLeft, vColor));    //1
        AddVertex(meshDataPC, FMeshVertexPC(vRight, vColor));   //2
        
        //Index
        AddIndexLine(meshDataPC, 0, 1);
        AddIndexLine(meshDataPC, 1, 2);
        AddIndexLine(meshDataPC, 2, 0);
    }

    //LineQuad
    void FMeshGeometry::CreateLineQuad(FMeshDataPC& meshDataPC,
                                       const FVector3& vLeftTop,
                                       const FVector3& vLeftBottom,
                                       const FVector3& vRightBottom,
                                       const FVector3& vRightTop,
                                       const FVector4& vColor)
    {
        //  0       3
        //   --------
        //   |      |
        //   |      |
        //   |      |
        //   --------
        //  1        2

        //FMeshVertex
        AddVertex(meshDataPC, FMeshVertexPC(vLeftTop, vColor));     //0
        AddVertex(meshDataPC, FMeshVertexPC(vLeftBottom, vColor));  //1
        AddVertex(meshDataPC, FMeshVertexPC(vRightBottom, vColor)); //2
        AddVertex(meshDataPC, FMeshVertexPC(vRightTop, vColor));    //3

        //Index
        AddIndexLine(meshDataPC, 0, 1);
        AddIndexLine(meshDataPC, 1, 2);
        AddIndexLine(meshDataPC, 2, 3);
        AddIndexLine(meshDataPC, 3, 0);
    }

    //LineGrid
    void FMeshGeometry::CreateLineGrid(FMeshDataPC& meshDataPC)
    {
        
    }

    //LineCircle
    void FMeshGeometry::CreateLineCircle(FMeshDataPC& meshDataPC)
    {

    }

    //LineAABB
    void FMeshGeometry::CreateLineAABB(FMeshDataPC& meshDataPC,
                                       float width,
                                       float height,
                                       float depth)
    {
        
    }

    //LineSphere
    void FMeshGeometry::CreateLineSphere(FMeshDataPC& meshDataPC,
                                         float radius,
                                         uint32 sliceCount,
                                         uint32 stackCount)
    {
        
    }

    //LineCylinder
    void FMeshGeometry::CreateLineCylinder(FMeshDataPC& meshDataPC)
    {

    }

    //LineCapsule
    void FMeshGeometry::CreateLineCapsule(FMeshDataPC& meshDataPC)
    {

    }

    //LineCone
    void FMeshGeometry::CreateLineCone(FMeshDataPC& meshDataPC)
    {

    }

    //LineTorus
    void FMeshGeometry::CreateLineTorus(FMeshDataPC& meshDataPC)
    {

    }


    ////////////////////////////////// Flat //////////////////////////////////
    bool FMeshGeometry::CreateFlatGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_FlatTriangle:
            {
                FMeshCreateParam_FlatTriangle param_FlatTriangle;
                FMeshGeometry::CreateFlatTriangle(meshDataPC, &param_FlatTriangle);
                return true;
            }
        case F_MeshGeometry_FlatQuad:
            {
                FMeshCreateParam_FlatQuad param_FlatQuad;
                FMeshGeometry::CreateFlatQuad(meshDataPC, &param_FlatQuad);
                return true;
            }
        case F_MeshGeometry_FlatCircle:
            {
                FMeshCreateParam_FlatCircle param_FlatCircle;
                FMeshGeometry::CreateFlatCircle(meshDataPC, &param_FlatCircle);
                return true;
            }
        case F_MeshGeometry_FlatAABB:
            {
                FMeshCreateParam_FlatAABB param_FlatAABB;
                FMeshGeometry::CreateFlatAABB(meshDataPC, &param_FlatAABB);
                return true;
            }
        case F_MeshGeometry_FlatSphere:
            {
                FMeshCreateParam_FlatSphere param_FlatSphere;
                FMeshGeometry::CreateFlatSphere(meshDataPC, &param_FlatSphere);
                return true;
            }
        case F_MeshGeometry_FlatCylinder:
            {
                FMeshCreateParam_FlatCylinder param_FlatCylinder;
                FMeshGeometry::CreateFlatCylinder(meshDataPC, &param_FlatCylinder);
                return true;
            }
        case F_MeshGeometry_FlatCapsule:
            {
                FMeshCreateParam_FlatCapsule param_FlatCapsule;
                FMeshGeometry::CreateFlatCapsule(meshDataPC, &param_FlatCapsule);
                return true;
            }
        case F_MeshGeometry_FlatCone:
            {
                FMeshCreateParam_FlatCone param_FlatCone;
                FMeshGeometry::CreateFlatCone(meshDataPC, &param_FlatCone);
                return true;
            }
        case F_MeshGeometry_FlatTorus:
            {
                FMeshCreateParam_FlatTorus param_FlatTorus;
                FMeshGeometry::CreateFlatTorus(meshDataPC, &param_FlatTorus);
                return true;
            }
        }
        return false;
    }
	bool FMeshGeometry::CreateFlatGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (pParam == nullptr)
        {
            return CreateFlatGeometry(meshDataPC, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_FlatTriangle:
            {
                FMeshCreateParam_FlatTriangle* pParam_FlatTriangle = static_cast<FMeshCreateParam_FlatTriangle*>(pParam);
                FMeshGeometry::CreateFlatTriangle(meshDataPC, pParam_FlatTriangle);
                return true;
            }
        case F_MeshGeometry_FlatQuad:
            {
                FMeshCreateParam_FlatQuad* pParam_FlatQuad = static_cast<FMeshCreateParam_FlatQuad*>(pParam);
                FMeshGeometry::CreateFlatQuad(meshDataPC, pParam_FlatQuad);
                return true;
            }
        case F_MeshGeometry_FlatCircle:
            {
                FMeshCreateParam_FlatCircle* pParam_FlatCircle = static_cast<FMeshCreateParam_FlatCircle*>(pParam);
                FMeshGeometry::CreateFlatCircle(meshDataPC, pParam_FlatCircle);
                return true;
            }
        case F_MeshGeometry_FlatAABB:
            {
                FMeshCreateParam_FlatAABB* pParam_FlatAABB = static_cast<FMeshCreateParam_FlatAABB*>(pParam);
                FMeshGeometry::CreateFlatAABB(meshDataPC, pParam_FlatAABB);
                return true;
            }
        case F_MeshGeometry_FlatSphere:
            {
                FMeshCreateParam_FlatSphere* pParam_FlatSphere = static_cast<FMeshCreateParam_FlatSphere*>(pParam);
                FMeshGeometry::CreateFlatSphere(meshDataPC, pParam_FlatSphere);
                return true;
            }
        case F_MeshGeometry_FlatCylinder:
            {
                FMeshCreateParam_FlatCylinder* pParam_FlatCylinder = static_cast<FMeshCreateParam_FlatCylinder*>(pParam);
                FMeshGeometry::CreateFlatCylinder(meshDataPC, pParam_FlatCylinder);
                return true;
            }
        case F_MeshGeometry_FlatCapsule:
            {
                FMeshCreateParam_FlatCapsule* pParam_FlatCapsule = static_cast<FMeshCreateParam_FlatCapsule*>(pParam);
                FMeshGeometry::CreateFlatCapsule(meshDataPC, pParam_FlatCapsule);
                return true;
            }
        case F_MeshGeometry_FlatCone:
            {
                FMeshCreateParam_FlatCone* pParam_FlatCone = static_cast<FMeshCreateParam_FlatCone*>(pParam);
                FMeshGeometry::CreateFlatCone(meshDataPC, pParam_FlatCone);
                return true;
            }
        case F_MeshGeometry_FlatTorus:
            {
                FMeshCreateParam_FlatTorus* pParam_FlatTorus = static_cast<FMeshCreateParam_FlatTorus*>(pParam);
                FMeshGeometry::CreateFlatTorus(meshDataPC, pParam_FlatTorus);
                return true;
            }
        }
        return false;
    }

    //FlatTriangle
    void FMeshGeometry::CreateFlatTriangle(FMeshDataPC& meshDataPC)
    {

    }

    //FlatQuad
    void FMeshGeometry::CreateFlatQuad(FMeshDataPC& meshDataPC)
    {

    }

    //FlatCircle
    void FMeshGeometry::CreateFlatCircle(FMeshDataPC& meshDataPC)
    {

    }

    //FlatAABB
    void FMeshGeometry::CreateFlatAABB(FMeshDataPC& meshDataPC)
    {

    }

    //FlatSphere
    void FMeshGeometry::CreateFlatSphere(FMeshDataPC& meshDataPC)
    {

    }

    //FlatCylinder
    void FMeshGeometry::CreateFlatCylinder(FMeshDataPC& meshDataPC)
    {

    }

    //FlatCapsule
    void FMeshGeometry::CreateFlatCapsule(FMeshDataPC& meshDataPC)
    {

    }

    //FlatCone
    void FMeshGeometry::CreateFlatCone(FMeshDataPC& meshDataPC)
    {

    }

    //FlatTorus
    void FMeshGeometry::CreateFlatTorus(FMeshDataPC& meshDataPC)
    {

    }


    ////////////////////////////////// Entity ////////////////////////////////
    bool FMeshGeometry::CreateEntityGeometry(FMeshData& meshData, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_EntityTriangle:
            {
                FMeshCreateParam_EntityTriangle param_EntityTriangle;
                FMeshGeometry::CreateEntityTriangle(meshData, &param_EntityTriangle);
                return true;
            }
        case F_MeshGeometry_EntityQuad:
            {
                FMeshCreateParam_EntityQuad param_EntityQuad;
                FMeshGeometry::CreateEntityQuad(meshData, &param_EntityQuad);
                return true;
            }
        case F_MeshGeometry_EntityGrid:
            {
                FMeshCreateParam_EntityGrid param_EntityGrid;
                FMeshGeometry::CreateEntityGrid(meshData, &param_EntityGrid);
                return true;
            }
        case F_MeshGeometry_EntityCircle:
            {
                FMeshCreateParam_EntityCircle param_EntityCircle;
                FMeshGeometry::CreateEntityCircle(meshData, &param_EntityCircle);
                return true;
            }
        case F_MeshGeometry_EntityAABB:
            {
                FMeshCreateParam_EntityAABB param_EntityAABB;
                FMeshGeometry::CreateEntityAABB(meshData, &param_EntityAABB);
                return true;
            }
        case F_MeshGeometry_EntitySphere:
            {
                FMeshCreateParam_EntitySphere param_EntitySphere;
                FMeshGeometry::CreateEntitySphere(meshData, &param_EntitySphere);
                return true;
            }
         case F_MeshGeometry_EntityGeoSphere:
            {
                FMeshCreateParam_EntityGeoSphere param_EntityGeoSphere;
                FMeshGeometry::CreateEntityGeoSphere(meshData, &param_EntityGeoSphere);
                return true;
            }
        case F_MeshGeometry_EntityCylinder:
            {
                FMeshCreateParam_EntityCylinder param_EntityCylinder;
                FMeshGeometry::CreateEntityCylinder(meshData, &param_EntityCylinder);
                return true;
            }
        case F_MeshGeometry_EntityCapsule:
            {
                FMeshCreateParam_EntityCapsule param_EntityCapsule;
                FMeshGeometry::CreateEntityCapsule(meshData, &param_EntityCapsule);
                return true;
            }
        case F_MeshGeometry_EntityCone:
            {
                FMeshCreateParam_EntityCone param_EntityCone;
                FMeshGeometry::CreateEntityCone(meshData, &param_EntityCone);
                return true;
            }
        case F_MeshGeometry_EntityTorus:
            {
                FMeshCreateParam_EntityTorus param_EntityTorus;
                FMeshGeometry::CreateEntityTorus(meshData, &param_EntityTorus);
                return true;
            }
        case F_MeshGeometry_EntitySkyBox:
            {
                FMeshCreateParam_EntitySkyBox param_EntitySkyBox;
                FMeshGeometry::CreateEntitySkyBox(meshData, &param_EntitySkyBox);
                return true;
            }
        case F_MeshGeometry_EntitySkyDome:
            {
                FMeshCreateParam_EntitySkyDome param_EntitySkyDome;
                FMeshGeometry::CreateEntitySkyDome(meshData, &param_EntitySkyDome);
                return true;
            }
        case F_MeshGeometry_EntityTerrain:
            {
                FMeshCreateParam_EntityTerrain param_EntityTerrain;
                FMeshGeometry::CreateEntityTerrain(meshData, &param_EntityTerrain);
                return true;
            }
        }
        return false;
    }
    bool FMeshGeometry::CreateEntityGeometryWithParam(FMeshData& meshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {   
        if (pParam == nullptr)
        {
            return CreateEntityGeometry(meshData, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_EntityTriangle:
            {
                FMeshCreateParam_EntityTriangle* pParam_EntityTriangle = static_cast<FMeshCreateParam_EntityTriangle*>(pParam);
                FMeshGeometry::CreateEntityTriangle(meshData, pParam_EntityTriangle);
                return true;
            }
        case F_MeshGeometry_EntityQuad:
            {
                FMeshCreateParam_EntityQuad* pParam_EntityQuad = static_cast<FMeshCreateParam_EntityQuad*>(pParam);
                FMeshGeometry::CreateEntityQuad(meshData, pParam_EntityQuad);
                return true;
            }
        case F_MeshGeometry_EntityGrid:
            {
                FMeshCreateParam_EntityGrid* pParam_EntityGrid = static_cast<FMeshCreateParam_EntityGrid*>(pParam);
                FMeshGeometry::CreateEntityGrid(meshData, pParam_EntityGrid);
                return true;
            }
        case F_MeshGeometry_EntityCircle:
            {
                FMeshCreateParam_EntityCircle* pParam_EntityCircle = static_cast<FMeshCreateParam_EntityCircle*>(pParam);
                FMeshGeometry::CreateEntityCircle(meshData, pParam_EntityCircle);
                return true;
            }
        case F_MeshGeometry_EntityAABB:
            {
                FMeshCreateParam_EntityAABB* pParam_EntityAABB = static_cast<FMeshCreateParam_EntityAABB*>(pParam);
                FMeshGeometry::CreateEntityAABB(meshData, pParam_EntityAABB);
                return true;
            }
        case F_MeshGeometry_EntitySphere:
            {
                FMeshCreateParam_EntitySphere* pParam_EntitySphere = static_cast<FMeshCreateParam_EntitySphere*>(pParam);
                FMeshGeometry::CreateEntitySphere(meshData, pParam_EntitySphere);
                return true;
            }
        case F_MeshGeometry_EntityGeoSphere:
            {
                FMeshCreateParam_EntityGeoSphere* pParam_EntityGeoSphere = static_cast<FMeshCreateParam_EntityGeoSphere*>(pParam);
                FMeshGeometry::CreateEntityGeoSphere(meshData, pParam_EntityGeoSphere);
                return true;
            }
        case F_MeshGeometry_EntityCylinder:
            {
                FMeshCreateParam_EntityCylinder* pParam_EntityCylinder = static_cast<FMeshCreateParam_EntityCylinder*>(pParam);
                FMeshGeometry::CreateEntityCylinder(meshData, pParam_EntityCylinder);
                return true;
            }
        case F_MeshGeometry_EntityCapsule:
            {
                FMeshCreateParam_EntityCapsule* pParam_EntityCapsule = static_cast<FMeshCreateParam_EntityCapsule*>(pParam);
                FMeshGeometry::CreateEntityCapsule(meshData, pParam_EntityCapsule);
                return true;
            }
        case F_MeshGeometry_EntityCone:
            {
                FMeshCreateParam_EntityCone* pParam_EntityCone = static_cast<FMeshCreateParam_EntityCone*>(pParam);
                FMeshGeometry::CreateEntityCone(meshData, pParam_EntityCone);
                return true;
            }
        case F_MeshGeometry_EntityTorus:
            {
                FMeshCreateParam_EntityTorus* pParam_EntityTorus = static_cast<FMeshCreateParam_EntityTorus*>(pParam);
                FMeshGeometry::CreateEntityTorus(meshData, pParam_EntityTorus);
                return true;
            }
        case F_MeshGeometry_EntitySkyBox:
            {
                FMeshCreateParam_EntitySkyBox* pParam_EntitySkyBox = static_cast<FMeshCreateParam_EntitySkyBox*>(pParam);
                FMeshGeometry::CreateEntitySkyBox(meshData, pParam_EntitySkyBox);
                return true;
            }
        case F_MeshGeometry_EntitySkyDome:
            {
                FMeshCreateParam_EntitySkyDome* pParam_EntitySkyDome = static_cast<FMeshCreateParam_EntitySkyDome*>(pParam);
                FMeshGeometry::CreateEntitySkyDome(meshData, pParam_EntitySkyDome);
                return true;
            }
        case F_MeshGeometry_EntityTerrain:
            {
                FMeshCreateParam_EntityTerrain* pParam_EntityTerrain = static_cast<FMeshCreateParam_EntityTerrain*>(pParam);
                FMeshGeometry::CreateEntityTerrain(meshData, pParam_EntityTerrain);
                return true;
            }
        }
        return false;
    }


    void FMeshGeometry::CreateEntityTriangle(FMeshData& meshData, 
                                             bool flipV,
                                             bool rightHand)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //FMeshVertex
        AddVertex(meshData, FMeshVertex( 0.0f,  0.5f,   0.0f,
                                         0.0f,  0.0f,  -1.0f,
                                         1.0f,  0.0f,   0.0f,
                                         0.5f,  flipV ? 1.0f : 0.0f));

        AddVertex(meshData, FMeshVertex(-0.5f, -0.5f,   0.0f,
                                         0.0f,  0.0f,  -1.0f,
                                         1.0f,  0.0f,   0.0f,
                                         0.0f,  flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex( 0.5f, -0.5f,   0.0f,
                                         0.0f,  0.0f,  -1.0f,
                                         1.0f,  0.0f,   0.0f,
                                         1.0f,  flipV ? 0.0f : 1.0f));
        
        //Index
        if (rightHand)
        {
            AddIndexTriangle(meshData, 0, 1, 2);
        }
        else
        {
            AddIndexTriangle(meshData, 0, 2, 1);
        }
    }

    void FMeshGeometry::CreateEntityQuad(FMeshData& meshData, 
                                         float centerX, 
                                         float centerY, 
                                         float width, 
                                         float height, 
                                         float depth,
                                         bool flipV,
                                         bool rightHand)
    {
        //  0       3
        //   --------
        //   |\     |
        //   |  \   |
        //   |    \ |
        //   --------
        //  1        2

        //FMeshVertex
        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY + height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        0.0f, flipV ? 1.0f : 0.0f));

        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        0.0f, flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        1.0f, flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY + height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        1.0f, flipV ? 1.0f : 0.0f));

        //Index
        if (rightHand)
        {
            AddIndexTriangle(meshData, 0, 1, 2);
            AddIndexTriangle(meshData, 2, 3, 0);
        }
        else
        {
            AddIndexTriangle(meshData, 0, 3, 2);
            AddIndexTriangle(meshData, 2, 1, 0);
        }
    }

    void FMeshGeometry::CreateEntityGrid(FMeshData& meshData, 
                                         float width, 
                                         float height, 
                                         uint32 m, 
                                         uint32 n,
                                         bool flipV,
                                         bool rightHand)
    {
        // (n-1)*(m-2)                  (n-1)*(m-1)
        //      |
        //      |
        //      |
        //  n*2 ----------------        (n-1)*3
        //      |\     |\     |
        //      |  \   |  \   |
        //      |    \ |    \ |
        //  n   ----------------        (n-1)*2
        //      |\     |\(n+1)|(n+2)
        //      |  \   |  \   |
        //      |    \ |    \ |
        //      ------------------------
        //  0          1      2         (n-1)

        uint32 vertexCount = m * n;
        uint32 faceCount = (m - 1) * (n - 1) * 2;

        //FMeshVertex
        float halfW = 0.5f * width;
        float halfH = 0.5f * height;

        float dx = width / (n - 1);
        float dz = height / (m - 1);

        float du = 1.0f / (n - 1);
        float dv = 1.0f / (m - 1);

        ResizeVertexCount(meshData, vertexCount);
        for (uint32 i = 0; i < m; ++i)
        {
            float y = -halfH + i * dz;
            for (uint32 j = 0; j < n; ++j)
            {
                float x = -halfW + j * dx;

                FMeshVertex vertex(   x,    y,  0.0f,
                                   0.0f, 0.0f, -1.0f,
                                   1.0f, 0.0f,  0.0f,
                                   j * du, flipV ? (1.0f - i * dv) : (i * dv));
                SetVertex(meshData, i * n + j, vertex);
            }
        }
        meshData.RefreshAABB();

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        uint32 k = 0;
        for (uint32 i = 0; i < m - 1; ++i)
        {
            for (uint32 j = 0; j < n - 1; ++j)
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, k + 0,
                                     (i + 1) * n + j,
                                     i * n + j,
                                     i * n + j + 1);

                    SetIndexTriangle(meshData, k + 3,
                                     i * n + j + 1,
                                     (i + 1) * n + j + 1,
                                     (i + 1) * n + j);
                }
                else
                {
                    SetIndexTriangle(meshData, k + 0,
                                     (i + 1) * n + j,
                                     (i + 1) * n + j + 1,
                                     i * n + j + 1);

                    SetIndexTriangle(meshData, k + 3,
                                     i * n + j + 1,
                                     i * n + j,
                                     (i + 1) * n + j);
                }

                k += 6;
            }
        }
    }

    void FMeshGeometry::CreateEntityCircle(FMeshData& meshData,
                                           float radius,
                                           uint32 segment,
                                           bool flipV,
                                           bool rightHand)
    {
		//          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *
        
        uint32 vertexCount = segment + 1;
        uint32 faceCount = segment;

        //FMeshVertex
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        ResizeVertexCount(meshData, vertexCount);
        SetVertex(meshData, 
                  0, 
                  FMeshVertex(0.0f, 0.0f,  0.0f,
                              0.0f, 0.0f, -1.0f,
                              1.0f, 0.0f,  0.0f,
                              0.5f, 0.5f));
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            float ux = cos(thetaStep * i);
            float uy = sin(thetaStep * i);

            SetVertex(meshData, 
                      index,
                      FMeshVertex(radius * ux,  radius * uy, 0.0f,
                                  0.0f, 0.0f, -1.0f,
                                  1.0f, 0.0f,  0.0f,
                                  (1.0f + ux) / 2.0f,  flipV ? (1.0f - (1.0f - uy) / 2.0f) : (1.0f - uy) / 2.0f));
            index++;
        }
        meshData.RefreshAABB();

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     i + 1,
                                     i + 2);
                }
                else
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     i + 2,
                                     i + 1);
                }
            }
            else
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     i,
                                     1);
                }
                else
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     1,
                                     i);
                }
            }
        }
    }

    void FMeshGeometry::CreateEntityAABB(FMeshData& meshData, 
                                         float width, 
                                         float height, 
                                         float depth, 
                                         uint32 numSubdivisions,
                                         bool flipV,
                                         bool rightHand)
    {
        //     7+------+4			  0 -  9 - 19  -+-
		//     /|     /|			  1 - 12 - 18  ---
		//    / |    / |			  2 - 15 - 21  +--
		//   / 6+---/--+5	 y		  3 - 10 - 20  ++-
		// 0+------+3 /		 | z	  4 - 11 - 23  +++
		//  | /    | /    	 |/		  5 - 14 - 22  +-+
		//  |/     |/     	 *---x	  6 - 13 - 17  --+
		// 1+------+2        		  7 -  8 - 16  -++

        //FMeshVertex
        FMeshVertex v[24];
        float w2 = 0.5f * width;
        float h2 = 0.5f * height;
        float d2 = 0.5f * depth;
        //Front
        v[0]  = FMeshVertex(-w2, +h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //0 -+-
        v[1]  = FMeshVertex(-w2, -h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //1 ---
        v[2]  = FMeshVertex(+w2, -h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //2 +--
        v[3]  = FMeshVertex(+w2, +h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //3 ++-
        //Back
        v[4]  = FMeshVertex(+w2, +h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //4 +++
        v[5]  = FMeshVertex(+w2, -h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //5 +-+
        v[6]  = FMeshVertex(-w2, -h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //6 --+
        v[7]  = FMeshVertex(-w2, +h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //7 -++
        //Top
        v[8]  = FMeshVertex(-w2, +h2, +d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //8  - 7
        v[9]  = FMeshVertex(-w2, +h2, -d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //9  - 0
        v[10] = FMeshVertex(+w2, +h2, -d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //10 - 3
        v[11] = FMeshVertex(+w2, +h2, +d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //11 - 4
        //Bottom
        v[12] = FMeshVertex(-w2, -h2, -d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //12 - 1
        v[13] = FMeshVertex(-w2, -h2, +d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //13 - 6
        v[14] = FMeshVertex(+w2, -h2, +d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //14 - 5
        v[15] = FMeshVertex(+w2, -h2, -d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //15 - 2
        //Left
        v[16] = FMeshVertex(-w2, +h2, +d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   0.0f, flipV ? 1.0f : 0.0f); //16 - 7
        v[17] = FMeshVertex(-w2, -h2, +d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   0.0f, flipV ? 0.0f : 1.0f); //17 - 6
        v[18] = FMeshVertex(-w2, -h2, -d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   1.0f, flipV ? 0.0f : 1.0f); //18 - 1
        v[19] = FMeshVertex(-w2, +h2, -d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   1.0f, flipV ? 1.0f : 0.0f); //19 - 0
        //Right
        v[20] = FMeshVertex(+w2, +h2, -d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   0.0f, flipV ? 1.0f : 0.0f); //20 - 3
        v[21] = FMeshVertex(+w2, -h2, -d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   0.0f, flipV ? 0.0f : 1.0f); //21 - 2
        v[22] = FMeshVertex(+w2, -h2, +d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   1.0f, flipV ? 0.0f : 1.0f); //22 - 5
        v[23] = FMeshVertex(+w2, +h2, +d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   1.0f, flipV ? 1.0f : 0.0f); //23 - 4

        for (int i = 0; i < 24; i++)
        {
            AddVertex(meshData, v[i]);
        }

        //Index
        uint32 i[36];
        if (rightHand)
        {
            //Front 
            i[0]  = 0;  i[1]  = 1;  i[2]  = 2;
            i[3]  = 2;  i[4]  = 3;  i[5]  = 0;
            //Back
            i[6]  = 4;  i[7]  = 5;  i[8]  = 6;
            i[9]  = 6;  i[10] = 7;  i[11] = 4;
            //Top
            i[12] = 8;  i[13] = 9;  i[14] = 10;
            i[15] = 10; i[16] = 11; i[17] = 8;
            //Bottom
            i[18] = 12; i[19] = 13; i[20] = 14;
            i[21] = 14; i[22] = 15; i[23] = 12;
            //Left
            i[24] = 16; i[25] = 17; i[26] = 18;
            i[27] = 18; i[28] = 19; i[29] = 16;
            //Right
            i[30] = 20; i[31] = 21; i[32] = 22;
            i[33] = 22; i[34] = 23; i[35] = 20;
        }
        else
        {
            //Front 
            i[0]  = 0;  i[1]  = 3;  i[2]  = 2;
            i[3]  = 2;  i[4]  = 1;  i[5]  = 0;
            //Back
            i[6]  = 4;  i[7]  = 7;  i[8]  = 6;
            i[9]  = 6;  i[10] = 5;  i[11] = 4;
            //Top
            i[12] = 8;  i[13] = 11;  i[14] = 10;
            i[15] = 10; i[16] =  9; i[17] = 8;
            //Bottom
            i[18] = 12; i[19] = 15; i[20] = 14;
            i[21] = 14; i[22] = 13; i[23] = 12;
            //Left
            i[24] = 16; i[25] = 19; i[26] = 18;
            i[27] = 18; i[28] = 17; i[29] = 16;
            //Right
            i[30] = 20; i[31] = 23; i[32] = 22;
            i[33] = 22; i[34] = 21; i[35] = 20;
        }
        AddIndices(meshData, 36, i);

        //SubDivide
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        for (uint32 i = 0; i < numSubdivisions; ++i)
        {
            SubDivide(meshData, rightHand);
        }
    }

    void FMeshGeometry::CreateEntitySphere(FMeshData& meshData, 
                                           float radius, 
                                           uint32 sliceCount, 
                                           uint32 stackCount,
                                           bool flipV,
                                           bool rightHand)
    {       
        //           0 
        //           *   
	    //	    *    - 2  *  1          
	    //        -     -          y                   
	    //   *	     *       *     | z              
	    //	      -     - l-2      |/
	    //      *    -    *  l-1   *---x
	    //           *
        //           l=(stackCount-1)*(sliceCount+1)+1

        //FMeshVertex
        //Vertex Top
        FMeshVertex vertexTop(0.0f, +radius, 0.0f,  0.0f, +1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 1.0f : 0.0f);
        AddVertex(meshData, vertexTop);
        {
            //Vertex
            float phiStep = FMath::ms_fPI / stackCount;
            float thetaStep = 2.0f * FMath::ms_fPI / sliceCount;
            for (uint32 i = 1; i <= stackCount - 1; ++i)
            {
                float phi = i * phiStep;
                for (uint32 j = 0; j <= sliceCount; ++j)
                {
                    float theta = j * thetaStep;

                    FMeshVertex vertex;
                    //pos
                    vertex.pos.x = radius * sinf(phi) * cosf(theta);
                    vertex.pos.y = radius * cosf(phi);
                    vertex.pos.z = radius * sinf(phi) * sinf(theta);
                    //color
                    vertex.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                    //normal
                    vertex.normal = FMath::Normalize(vertex.pos);
                    //tangent
                    vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
                    vertex.tangent.y = 0.0f;
                    vertex.tangent.z = +radius * sinf(phi) * cosf(theta);
                    vertex.tangent = FMath::Normalize(vertex.tangent);
                    //texCoord
                    vertex.texCoord.x = theta / FMath::ms_fPI_Two;
                    vertex.texCoord.y = flipV ? (1.0f - phi / FMath::ms_fPI) : (phi / FMath::ms_fPI);

                    AddVertex(meshData, vertex);
                }
            }
        }
        //Vertex Bottom
        FMeshVertex vertexBottom(0.0f, -radius, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 0.0f : 1.0f);
        AddVertex(meshData, vertexBottom);

        //Index Top
        for (uint32 i = 1; i <= sliceCount; ++i)
        {
            if (rightHand)
            {
                AddIndexTriangle(meshData, 
                                 0,
                                 i,
                                 i + 1);
            }
            else
            {
                AddIndexTriangle(meshData, 
                                 0,
                                 i + 1,
                                 i);
            }
        }

        //Index Inner
        uint32 baseIndex = 1;
        uint32 ringVertexCount = sliceCount + 1;
        for (uint32 i = 0; i < stackCount - 2; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                if (rightHand)
                {
                    AddIndexTriangle(meshData, 
                                     baseIndex + i * ringVertexCount + j,
                                     baseIndex + (i + 1) * ringVertexCount + j,
                                     baseIndex + (i + 1) * ringVertexCount + j + 1);

                    AddIndexTriangle(meshData, 
                                     baseIndex + (i + 1) * ringVertexCount + j + 1,
                                     baseIndex + i * ringVertexCount + j + 1,
                                     baseIndex + i * ringVertexCount + j);
                }
                else
                {
                    AddIndexTriangle(meshData, 
                                     baseIndex + i * ringVertexCount + j,
                                     baseIndex + (i + 1) * ringVertexCount + j + 1,
                                     baseIndex + (i + 1) * ringVertexCount + j);

                    AddIndexTriangle(meshData, 
                                     baseIndex + (i + 1) * ringVertexCount + j + 1,
                                     baseIndex + i * ringVertexCount + j,
                                     baseIndex + i * ringVertexCount + j + 1);
                }
                
            }
        }

        //Index Bottom
        uint32 southPoleIndex = GetVertexCount(meshData) - 1;
        baseIndex = southPoleIndex - ringVertexCount;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                AddIndexTriangle(meshData, 
                                 southPoleIndex,
                                 baseIndex + i + 1,
                                 baseIndex + i);
            }
            else
            {
                AddIndexTriangle(meshData, 
                                 southPoleIndex,
                                 baseIndex + i,
                                 baseIndex + i + 1);
            }
        }
    }

    void FMeshGeometry::CreateEntityGeoSphere(FMeshData& meshData, 
                                              float radius, 
                                              uint32 numSubdivisions,
                                              bool flipV,
                                              bool rightHand)
    {
		//             * 4    
		//			 * 5 	    
		//         * 0  * 1           
        //    * 9              * 8    y
		//	 * 11	         * 10     | z
		//		   * 2	* 3           |/
		//             * 6            *---x
		//           * 7    

        //FMeshVertex
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        const float X = 0.525731f;
        const float Z = 0.850651f;
        FVector3 pos[12] =
        {
            FVector3(   -X,  0.0f,     Z),  //0
            FVector3(    X,  0.0f,     Z),  //1
            FVector3(   -X,  0.0f,    -Z),  //2
            FVector3(    X,  0.0f,    -Z),  //3
            FVector3( 0.0f,     Z,     X),  //4
            FVector3( 0.0f,     Z,    -X),  //5
            FVector3( 0.0f,    -Z,     X),  //6
            FVector3( 0.0f,    -Z,    -X),  //7
            FVector3(    Z,     X,  0.0f),  //8
            FVector3(   -Z,     X,  0.0f),  //9
            FVector3(    Z,    -X,  0.0f),  //10
            FVector3(   -Z,    -X,  0.0f)   //11
        };

        //Vertex
        ResizeVertexCount(meshData, 12);
        for (uint32 i = 0; i < 12; ++i)
        {
            FMeshVertex& vertex = GetVertex(meshData, i);
            vertex.pos = pos[i];
        }

        //Index
        if (rightHand)
        {
            uint32 k[60] =
            {
                1,  4,  0,    4, 9, 0,   4,  5, 9,   8, 5,  4,   1, 8, 4,
                1, 10,  8,   10, 3, 8,   8,  3, 5,   3, 2,  5,   3, 7, 2,
                3, 10,  7,   10, 6, 7,   6, 11, 7,   6, 0, 11,   6, 1, 0,
               10,  1,  6,   11, 0, 9,   2, 11, 9,   5, 2,  9,  11, 2, 7
            };
            AddIndices(meshData, 60, k);
        }
        else
        {
            uint32 k[60] =
            {
                1,  0,  4,    4, 0, 9,   4, 9,  5,   8,  4, 5,   1, 4, 8,
                1,  8, 10,   10, 8, 3,   8, 5,  3,   3,  5, 2,   3, 2, 7,
                3,  7, 10,   10, 7, 6,   6, 7, 11,   6, 11, 0,   6, 0, 1,
               10,  6,  1,   11, 9, 0,   2, 9, 11,   5,  9, 2,  11, 7, 2
            };
            AddIndices(meshData, 60, k);
        }

        //SubDivide
        for (uint32 i = 0; i < numSubdivisions; ++i)
        {
            SubDivide(meshData, rightHand);
        }

        uint32 countVertex = GetVertexCount(meshData);
        for (uint32 i = 0; i < countVertex; ++i)
        {
            FMeshVertex& vertex = GetVertex(meshData, i);

            vertex.normal = FMath::Normalize(vertex.pos);
            vertex.pos = radius * vertex.normal;

            float theta = atan2f(vertex.pos.z, vertex.pos.x);
            if (theta < 0.0f)
                theta += FMath::ms_fPI_Two;

            float phi = acosf(vertex.pos.y / radius);

            vertex.texCoord.x = theta / FMath::ms_fPI_Two;
            vertex.texCoord.y = flipV ? (1.0f - phi / FMath::ms_fPI) : (phi / FMath::ms_fPI);

            vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
            vertex.tangent.y = 0.0f;
            vertex.tangent.z = +radius * sinf(phi) * cosf(theta);
            vertex.tangent = FMath::Normalize(vertex.tangent);
        }
        meshData.RefreshAABB();
    }

    void s_BuildCylinderTopCap(FMeshData& meshData, 
                               float bottomRadius, 
                               float topRadius, 
                               float height, 
                               float heightOffset,
                               uint32 sliceCount, 
                               uint32 stackCount, 
                               bool flipV,
                               bool rightHand)
    {
        uint32 baseIndex = FMeshGeometry::GetVertexCount(meshData);
        float y = heightOffset + 0.5f * height;
        float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = topRadius * cosf(i * dTheta);
            float z = topRadius * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = flipV ? (1.0f - z / height + 0.5f) : (z / height + 0.5f);

            FMeshGeometry::AddVertex(meshData, FMeshVertex(x, y, z, 
                                                           0.0f, 1.0f, 0.0f, 
                                                           1.0f, 0.0f, 0.0f, 
                                                           u, v));
        }
        FMeshGeometry::AddVertex(meshData, FMeshVertex(0.0f, y, 0.0f, 
                                                       0.0f, 1.0f, 0.0f, 
                                                       1.0f, 0.0f, 0.0f, 
                                                       0.5f, 0.5f));

        uint32 centerIndex = FMeshGeometry::GetVertexCount(meshData) - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i,
                                                baseIndex + i + 1);
            }
            else
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i + 1,
                                                baseIndex + i);
            }
        }
    }

    void s_BuildCylinderBottomCap(FMeshData& meshData, 
                                  float bottomRadius, 
                                  float topRadius, 
                                  float height, 
                                  float heightOffset,
                                  uint32 sliceCount, 
                                  uint32 stackCount, 
                                  bool flipV,
                                  bool rightHand)
    {
        uint32 baseIndex = FMeshGeometry::GetVertexCount(meshData);
        float y = heightOffset - 0.5f * height;
        float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = bottomRadius * cosf(i * dTheta);
            float z = bottomRadius * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = flipV ? (1.0f - z / height + 0.5f) : (z / height + 0.5f);

            FMeshGeometry::AddVertex(meshData, FMeshVertex(x, y, z, 
                                                           0.0f, -1.0f, 0.0f, 
                                                           1.0f, 0.0f, 0.0f, 
                                                           u, v));
        }
        FMeshGeometry::AddVertex(meshData, FMeshVertex(0.0f, y, 0.0f, 
                                                       0.0f, -1.0f, 0.0f, 
                                                       1.0f,  0.0f, 0.0f, 
                                                       0.5f,  0.5f));

        uint32 centerIndex = FMeshGeometry::GetVertexCount(meshData) - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i + 1,
                                                baseIndex + i);
            }
            else
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i,
                                                baseIndex + i + 1);
            }
        }
    }

    void FMeshGeometry::CreateEntityCylinder(FMeshData& meshData, 
                                             float bottomRadius, 
                                             float topRadius, 
                                             float height, 
                                             float heightOffset,
                                             uint32 sliceCount, 
                                             uint32 stackCount,
                                             bool flipV,
                                             bool rightHand)
    {
        //       * 
        //    *     *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *     *
        //       *       

        //FMeshVertex
        float stackHeight = height / stackCount;
        float radiusStep = (topRadius - bottomRadius) / stackCount;
        uint32 ringCount = stackCount + 1;
        for (uint32 i = 0; i < ringCount; ++i)
        {
            float y = heightOffset - 0.5f * height + i * stackHeight;
            float r = bottomRadius + i * radiusStep;

            float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
            for (uint32 j = 0; j <= sliceCount; ++j)
            {
                FMeshVertex vertex;

                float c = cosf(j * dTheta);
                float s = sinf(j * dTheta);

                vertex.pos = FVector3(r * c, y, r * s);
                vertex.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

                vertex.texCoord.x = (float)j / sliceCount;
                vertex.texCoord.y = flipV ? ((float)i / stackCount) : (1.0f - (float)i / stackCount);

                vertex.tangent = FVector3(-s, 0.0f, c);

                float dr = bottomRadius - topRadius;
                FVector3 T = vertex.tangent;
                FVector3 B = FVector3(dr * c, -height, dr * s);
                vertex.normal = FMath::Normalize(FMath::Cross(T, B));

                AddVertex(meshData, vertex);
            }
        }

        uint32 ringVertexCount = sliceCount + 1;

        for (uint32 i = 0; i < stackCount; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                if (rightHand)
                {
                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j + 1,
                                     (i + 1) * ringVertexCount + j);

                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     i * ringVertexCount + j + 1,
                                     (i + 1) * ringVertexCount + j + 1);
                }
                else
                {
                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j + 1);

                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j + 1,
                                     i * ringVertexCount + j + 1);
                }
            }
        }

        s_BuildCylinderTopCap(meshData, 
                              bottomRadius, 
                              topRadius, 
                              height, 
                              heightOffset,
                              sliceCount, 
                              stackCount, 
                              flipV, 
                              rightHand);
        s_BuildCylinderBottomCap(meshData, 
                                 bottomRadius, 
                                 topRadius, 
                                 height, 
                                 heightOffset,
                                 sliceCount, 
                                 stackCount, 
                                 flipV, 
                                 rightHand);
    }

    void FMeshGeometry::CreateEntityCapsule(FMeshData& meshData,
                                            float radius,
                                            float height,
                                            float heightOffset,
                                            uint32 numRings,
                                            uint32 numSegments,
                                            uint32 numSegHeight,
                                            bool flipV,
                                            bool rightHand)
    {
        //      *** 
        //    *  *  *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *  *  *
        //      ***  

        //FMeshVertex
        uint32 nVertexCount = (2 * numRings + 2) * (numSegments + 1) + (numSegHeight - 1) * (numSegments + 1);
        uint32 nIndexCount = (2 * numRings + 1) * (numSegments + 1) * 6 + (numSegHeight - 1) * (numSegments + 1) * 6;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float fDeltaRingAngle = (FMath::ms_fPI_Half / numRings);
        float fDeltaSegAngle = (FMath::ms_fPI_Two / numSegments);

        float sphereRatio = radius / (2 * radius + height);
        float cylinderRatio = height / (2 * radius + height);
        int offset = 0;

        //1> Top half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf(ring * fDeltaRingAngle);
            float y0 = radius * cosf(ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 pos(x0, heightOffset + 0.5f * height + y0, z0);
                FVector3 normal = FMath::Normalize(FVector3(x0, y0, z0));
                FVector3 tangent = FMath::Normalize(FVector3(-z0, y0, x0));
                float u = (float)seg / (float)numSegments;
                float v = flipV ? (1.0f - (float)ring / (float)numRings * sphereRatio) : (float)ring / (float)numRings * sphereRatio; 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (rightHand)
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                }
                else
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + numSegments, offset);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + 1);
                }

                offset ++;
            }
        }

        //2> Cylinder part
        float deltaAngle = (FMath::ms_fPI_Two / numSegments);
        float deltamHeight = height/(float)numSegHeight;
        for (uint32 i = 1; i < numSegHeight; i++)
        {
            for (uint32 j = 0; j<=numSegments; j++)
            {
                float x0 = radius * cosf(j * deltaAngle);
                float z0 = radius * sinf(j * deltaAngle);
                FVector3 pos(x0, heightOffset + 0.5f * height - i * deltamHeight, z0);
                FVector3 normal = FMath::Normalize(FVector3(x0, 0, z0));
                FVector3 tangent = FMath::Normalize(FVector3(-z0, 0, x0));
                float u = j / (float)numSegments;
                float v = flipV ? (1.0f - i / (float)numSegHeight * cylinderRatio + sphereRatio) : (i / (float)numSegHeight * cylinderRatio + sphereRatio); 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (rightHand)
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                }
                else
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + numSegments, offset);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + 1);
                }

                offset ++;
            }
        }

        //3> Bottom half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);
            float y0 =  radius * cosf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 pos(x0, heightOffset - 0.5f * height + y0, z0);
                FVector3 normal = FMath::Normalize(FVector3(x0, y0, z0));
                FVector3 tangent = FMath::Normalize(FVector3(-z0, y0, x0));
                float u = (float)seg / (float)numSegments;
                float v = flipV ? (1.0f - ((float)ring / (float)numRings * sphereRatio + cylinderRatio + sphereRatio)) : ((float)ring / (float)numRings * sphereRatio + cylinderRatio + sphereRatio); 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (ring != numRings)
                {
                    if (rightHand)
                    {
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                    }
                    else
                    {
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset + numSegments, offset);
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + 1);
                    }
                }

                offset ++;
            } 
        } 
    }

    void FMeshGeometry::CreateEntityCone(FMeshData& meshData,
                                         float radius,
                                         float height,
                                         float heightOffset,
                                         uint32 numSegBase,
                                         uint32 numSegHeight,
                                         bool flipV,
                                         bool rightHand)
    {
        //               *
        //             *   *
        //           *       *
        //         *           *
        //       *       +       *
        //     *   +           +   *
        //    *          *           *
        //      +                  +
        //          +          +
        //               +

        //FMeshVertex
        uint32 nVertexCount = (numSegHeight + 1) * (numSegBase + 1) + numSegBase + 2;
        uint32 nIndexCount = numSegHeight * numSegBase * 6 + 3 * numSegBase;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float deltaAngle = (FMath::ms_fPI_Two / numSegBase);
        float deltaHeight = height / (float)numSegHeight;
        int offset = 0;

        FVector3 refNormal = FMath::Normalize(FVector3(radius, height, 0.f));
        
        //1> Cone
        for (uint32 i = 0; i <= numSegHeight; i++)
        {
            float r0 = radius * (1 - i / (float)numSegHeight);
            for (uint32 j = 0; j <= numSegBase; j++)
            {
                float x0 = r0 * cosf(j * deltaAngle);
                float z0 = r0 * sinf(j * deltaAngle);
                FVector3 pos(x0, heightOffset + i * deltaHeight, z0);
                FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(-deltaAngle * j, FMath::ms_v3UnitY);
                FVector3 normal = FMath::Transform(qRot, refNormal);
                FVector3 tangent = FMath::Normalize(FVector3(-z0, i * deltaHeight, x0));
                float u = j / (float)numSegBase;
                float v = flipV ? (1.0f - i / (float)numSegHeight) : i / (float)numSegHeight;

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (i != numSegHeight && j != numSegBase)
                {
                    if (rightHand)
                    {
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset + numSegBase + 1, offset);
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset, offset + 1);
                    }
                    else
                    {
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset, offset + numSegBase + 1);
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset + 1, offset);
                    }
                }

                offset ++;
            }
        }

        //2> Low Cap
        int centerIndex = offset;
        AddVertex(meshData, 
                  heightOffset + FMath::ms_v3Zero,
                  FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                  FMath::ms_v3UnitNegY,
                  FMath::ms_v3UnitX,
                  flipV ? FMath::ms_v2UnitX : FMath::ms_v2One);
        offset++;
        for (uint32 j = 0; j <= numSegBase; j++)
        {
            float x0 = radius * cosf(j * deltaAngle);
            float z0 = radius * sinf(j * deltaAngle);
            FVector3 pos(x0, heightOffset, z0);
            float u = j / (float)numSegBase;
            float v = flipV ? 1.0f : 0.0f;

            AddVertex(meshData, 
                      pos,
                      FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                      FMath::ms_v3UnitNegY,
                      FMath::ms_v3UnitX,
                      FVector2(u, v));

            if (j != numSegBase)
            {
                if (rightHand)
                {
                    AddIndexTriangle(meshData, centerIndex, offset + 1, offset);
                }
                else
                {
                    AddIndexTriangle(meshData, centerIndex, offset, offset + 1);
                }
            }
            offset++;
        }
    }

    void FMeshGeometry::CreateEntityTorus(FMeshData& meshData,
                                          float radius,
                                          float sectionRadius,
                                          uint32 numSegSection,
                                          uint32 numSegCircle,
                                          bool flipV,
                                          bool rightHand)
    {
        //         + +
        //       + * *  +  
        //     + * - -  *  +
        //    + * -    - * +
        //     + * - -  *  + 
        //       + * *  +
        //         +  +

        //FMeshVertex
        uint32 nVertexCount = (numSegCircle + 1) * (numSegSection + 1);
        uint32 nIndexCount = (numSegCircle) * (numSegSection + 1) * 6;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float deltaSection = (FMath::ms_fPI_Two / numSegSection);
        float deltaCircle = (FMath::ms_fPI_Two / numSegCircle);
        int offset = 0;

        for (uint32 i = 0; i <= numSegCircle; i++)
        {   
            for (uint32 j = 0; j<= numSegSection; j++)
            {
                FVector3 c0(radius, 0.0, 0.0);
                FVector3 v0(radius + sectionRadius * cosf(j * deltaSection), sectionRadius * sinf(j * deltaSection), 0.0);
                FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(i * deltaCircle, FMath::ms_v3UnitY);
                FVector3 vPos = FMath::Transform(qRot, v0);
                FVector3 c = FMath::Transform(qRot, c0);
                float u = i / (float)numSegCircle;
                float v = flipV ? (1.0f - j / (float)numSegSection) : (j / (float)numSegSection);

                AddVertex(meshData, 
                          vPos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          FMath::Normalize(vPos - c),
                          FMath::ms_v3UnitX,
                          FVector2(u, v));

                if (i != numSegCircle)
                {
                    if (rightHand)
                    {
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset + numSegSection, offset);
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset, offset + 1);
                    }
                    else
                    {
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset, offset + numSegSection);
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset + 1, offset);
                    }
                }
                offset ++;
            }
        }       
    }

    void FMeshGeometry::CreateEntitySkyBox(FMeshData& meshData,
                                           bool flipV,
                                           bool rightHand)
    {

    }

    void FMeshGeometry::CreateEntitySkyDome(FMeshData& meshData,
                                            bool flipV,
                                            bool rightHand)
    {

    }

    void FMeshGeometry::CreateEntityTerrain(FMeshData& meshData,
                                            float offsetX,
                                            float offsetZ,
                                            float width,
                                            float height,
                                            uint32 vertexX,
                                            uint32 vertexZ,
                                            float* pHeight,
                                            uint32 heightDataGap,
                                            bool flipV,
                                            bool rightHand)
    {
        uint32 vertexCount = vertexX * vertexZ;
        uint32 faceCount = (vertexX - 1) * (vertexZ - 1) * 2;

        //FMeshVertex
        float halfW = 0.5f * width;
        float halfH = 0.5f * height;

        float dx = width / (vertexX - 1);
        float dz = height / (vertexZ - 1);

        float du = 1.0f / (vertexX - 1);
        float dv = 1.0f / (vertexZ - 1);

        ResizeVertexCount(meshData, vertexCount);
        for (uint32 i = 0; i < vertexZ; ++i)
        {
            float z = halfH - i * dz + offsetZ;
            for (uint32 j = 0; j < vertexX; ++j)
            {
                float height = 0.0f;
                if (pHeight != nullptr)
                {
                    height = pHeight[i * vertexX * heightDataGap + j * heightDataGap];
                }
                
                float x = -halfW + j * dx + offsetX;
                FMeshVertex vertex = FMeshVertex(x, height, z,
                                                 0.0f, 0.0f, 1.0f,
                                                 1.0f, 0.0f, 0.0f,
                                                 j * du, flipV ? (1.0f - i * dv) : (i * dv));
                SetVertex(meshData, i * vertexZ + j, vertex);
            }
        }
        meshData.RefreshAABB();

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        uint32 k = 0;
        for (uint32 i = 0; i < vertexZ - 1; ++i)
        {
            for (uint32 j = 0; j < vertexX - 1; ++j)
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, k + 0,
                                     i * vertexZ + j,
                                     (i + 1) * vertexZ + j,
                                     (i + 1) * vertexZ + j + 1);
                    
                    SetIndexTriangle(meshData, k + 3,
                                     (i + 1) * vertexZ + j + 1,
                                     i * vertexZ + j + 1,
                                     i * vertexZ + j);
                }
                else
                {
                    SetIndexTriangle(meshData, k + 0,
                                     i * vertexZ + j,
                                     i * vertexZ + j + 1,
                                     (i + 1) * vertexZ + j + 1);
                    
                    SetIndexTriangle(meshData, k + 3,
                                     (i + 1) * vertexZ + j + 1,
                                     (i + 1) * vertexZ + j,
                                     i * vertexZ + j);
                }

                k += 6;
            }
        }
    }

    void FMeshGeometry::MidPoint(const FMeshVertex& v0, const FMeshVertex& v1, FMeshVertex& m)
    {
        m.pos = 0.5f * (v0.pos + v1.pos);
        m.color = 0.5f * (v0.color + v1.color);
        m.normal = FMath::Normalize(0.5f * (v0.normal + v1.normal));
        m.tangent = FMath::Normalize(0.5f * (v0.tangent + v1.tangent));
        m.texCoord = 0.5f * (v0.texCoord + v1.texCoord);
    }

    void FMeshGeometry::SubDivide(FMeshData& meshData, bool rightHand)
    {
        //      (0)
        //       v0
        //       *
        //      / \
        //  (3)/   \(5)
        //  m0*-----*m2
        //   / \   / \
        //  /   \ /   \
        // *-----*-----*
        // v1    m1    v2
        //(1)    (4)   (2)

        FMeshData inputCopy = meshData;
        ResizeVertexCount(meshData, 0);
        ResizeIndexCount(meshData, 0);
        uint32 numTri = GetIndexCount(inputCopy) / 3;
        for (uint32 i = 0; i < numTri; ++i)
        {
            const FMeshVertex& v0 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 0));
            const FMeshVertex& v1 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 1));
            const FMeshVertex& v2 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 2));

            //mid
            FMeshVertex m0,m1,m2;
            MidPoint(v0, v1, m0);
            MidPoint(v1, v2, m1);
            MidPoint(v0, v2, m2);

            //new
            AddVertex(meshData, v0); // 0
            AddVertex(meshData, v1); // 1
            AddVertex(meshData, v2); // 2
            AddVertex(meshData, m0); // 3
            AddVertex(meshData, m1); // 4
            AddVertex(meshData, m2); // 5

            if (rightHand)
            {
                AddIndexTriangle(meshData, 
                                 i * 6 + 0,
                                 i * 6 + 3,
                                 i * 6 + 5);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 4,
                                 i * 6 + 5);

                AddIndexTriangle(meshData, 
                                 i * 6 + 5,
                                 i * 6 + 4,
                                 i * 6 + 2);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 1,
                                 i * 6 + 4);
            }
            else
            {
                AddIndexTriangle(meshData, 
                                 i * 6 + 0,
                                 i * 6 + 5,
                                 i * 6 + 3);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 5,
                                 i * 6 + 4);

                AddIndexTriangle(meshData, 
                                 i * 6 + 5,
                                 i * 6 + 2,
                                 i * 6 + 4);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 4,
                                 i * 6 + 1);
            }
        }
    }


    //FMeshDataPC
    uint32 FMeshGeometry::GetVertexCount(FMeshDataPC& meshDataPC)
    {
        return meshDataPC.GetVertexCount();
    }
    void FMeshGeometry::ReserveVertexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ReserveVertexCount(count);
    }
    void FMeshGeometry::ResizeVertexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ResizeVertexCount(count);
    }
    FMeshVertexPC& FMeshGeometry::GetVertex(FMeshDataPC& meshDataPC, uint32 index)
    {
        return meshDataPC.GetVertex(index);
    }
    uint32 FMeshGeometry::AddVertex(FMeshDataPC& meshDataPC, const FMeshVertexPC& vertex)
    {
        meshDataPC.AddVertex(vertex);
        return GetVertexCount(meshDataPC);
    }
    uint32 FMeshGeometry::AddVertex(FMeshDataPC& meshDataPC, 
                                    const FVector3& vPos,
                                    const FVector4& color)
    {
        return AddVertex(meshDataPC, FMeshVertexPC(vPos, color));
    }
    void FMeshGeometry::SetVertex(FMeshDataPC& meshDataPC, int index, const FMeshVertexPC& vertex)
    {
        meshDataPC.SetVertex(index, vertex);
    }

    uint32 FMeshGeometry::GetIndexCount(FMeshDataPC& meshDataPC)
    {
        return meshDataPC.GetIndexCount();
    }
    void FMeshGeometry::ReserveIndexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ReserveIndexCount(count);
    }
    void FMeshGeometry::ResizeIndexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ResizeIndexCount(count);
    }
    uint32 FMeshGeometry::GetIndex(FMeshDataPC& meshDataPC, uint32 index)
    {
        return meshDataPC.GetIndex(index);
    }   
    uint32 FMeshGeometry::AddIndex(FMeshDataPC& meshDataPC, uint32 value)
    {
        meshDataPC.AddIndex(value);
        return GetIndexCount(meshDataPC);
    }
    void FMeshGeometry::AddIndices(FMeshDataPC& meshDataPC, uint32 count, uint32* pIndex)
    {
        meshDataPC.AddIndices(count, pIndex);
    }
    void FMeshGeometry::SetIndex(FMeshDataPC& meshDataPC, uint32 index, uint32 value)
    {
        meshDataPC.SetIndex(index, value);
    }
    void FMeshGeometry::AddIndexLine(FMeshDataPC& meshDataPC, uint32 index1, uint32 index2)
    {
        meshDataPC.AddIndexLine(index1, index2);
    }
    void FMeshGeometry::AddIndexTriangle(FMeshDataPC& meshDataPC, uint32 index1, uint32 index2, uint32 index3)
    {
        meshDataPC.AddIndexTriangle(index1, index2, index3);
    }
    void FMeshGeometry::SetIndexTriangle(FMeshDataPC& meshDataPC, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
    {
        meshDataPC.SetIndexTriangle(indexStart, index1, index2, index3);
    }

    
    //FMeshData
    uint32 FMeshGeometry::GetVertexCount(FMeshData& meshData)
    {
        return meshData.GetVertexCount();
    }
    void FMeshGeometry::ReserveVertexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ReserveVertexCount(count);
    }
    void FMeshGeometry::ResizeVertexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ResizeVertexCount(count);
    }
    FMeshVertex& FMeshGeometry::GetVertex(FMeshData& meshData, uint32 index)
    {
        return meshData.GetVertex(index);
    }
    uint32 FMeshGeometry::AddVertex(FMeshData& meshData, const FMeshVertex& vertex)
    {
        meshData.AddVertex(vertex);
        return GetVertexCount(meshData);
    }
    uint32 FMeshGeometry::AddVertex(FMeshData& meshData,
                                    const FVector3& vPos,
                                    const FVector4& color, 
                                    const FVector3& normal, 
                                    const FVector3& tangent, 
                                    const FVector2& texCoord)
    {
        return AddVertex(meshData, FMeshVertex(vPos, color, normal, tangent, texCoord));
    }
    void FMeshGeometry::SetVertex(FMeshData& meshData, int index, const FMeshVertex& vertex)
    {
        meshData.SetVertex(index, vertex);
    }

    uint32 FMeshGeometry::GetIndexCount(FMeshData& meshData)
    {
        return meshData.GetIndexCount();
    }
    void FMeshGeometry::ReserveIndexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ReserveIndexCount(count);
    }
    void FMeshGeometry::ResizeIndexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ResizeIndexCount(count);
    }
    uint32 FMeshGeometry::GetIndex(FMeshData& meshData, uint32 index)
    {
        return meshData.GetIndex(index);
    }
    uint32 FMeshGeometry::AddIndex(FMeshData& meshData, uint32 value)
    {
        meshData.AddIndex(value);
        return GetIndexCount(meshData);
    }
    void FMeshGeometry::AddIndices(FMeshData& meshData, uint32 count, uint32* pIndex)
    {
        meshData.AddIndices(count, pIndex);
    }
    void FMeshGeometry::SetIndex(FMeshData& meshData, uint32 index, uint32 value)
    {
        meshData.SetIndex(index, value);
    }
    void FMeshGeometry::AddIndexLine(FMeshData& meshData, uint32 index1, uint32 index2)
    {
        meshData.AddIndexLine(index1, index2);
    }
    void FMeshGeometry::AddIndexTriangle(FMeshData& meshData, uint32 index1, uint32 index2, uint32 index3)
    {
        meshData.AddIndexTriangle(index1, index2, index3);
    }
    void FMeshGeometry::SetIndexTriangle(FMeshData& meshData, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
    {
        meshData.SetIndexTriangle(indexStart, index1, index2, index3);
    }

}; //LostPeterFoundation