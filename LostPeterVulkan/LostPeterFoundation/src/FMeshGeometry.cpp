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
    ///////////////////////////////////////// FMeshCreateParam ///////////////////////////////////////////
    FMeshCreateParam::FMeshCreateParam(bool _flipV, bool _rightHand)
        : flipV(_flipV)
        , rightHand(_rightHand)
    {

    }
    FMeshCreateParam::~FMeshCreateParam()
    {

    }

    //Line
    String FMeshCreateParam_Line::ms_nameType = "MeshLine";
    FMeshCreateParam_Line::FMeshCreateParam_Line()
        : FMeshCreateParam(false, false)
    {

    }
    FMeshCreateParam_Line::~FMeshCreateParam_Line()
    {

    }
    String FMeshCreateParam_Line::ToName()
    {
        return FUtilString::FormatString("%s_%d_%d", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0,
                                         rightHand ? 1 : 0);
    }

    //LineQuad
    String FMeshCreateParam_LineQuad::ms_nameType = "MeshLineQuad";
    FMeshCreateParam_LineQuad::FMeshCreateParam_LineQuad()
        : FMeshCreateParam(false, false)
        , centerX(0)
        , centerY(0)
        , width(1)
        , height(1)
    {

    }
    FMeshCreateParam_LineQuad::FMeshCreateParam_LineQuad(float _centerX,
                                                         float _centerY,
                                                         float _width,
                                                         float _height)
        : FMeshCreateParam(false, false)
        , centerX(_centerX)
        , centerY(_centerY)
        , width(_width)
        , height(_height)
    {

    }
    FMeshCreateParam_LineQuad::~FMeshCreateParam_LineQuad()
    {

    }
    String FMeshCreateParam_LineQuad::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%f", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         centerX,
                                         centerY,
                                         width,
                                         height);
    }

    //LineAABB
    String FMeshCreateParam_LineAABB::ms_nameType = "MeshLineAABB";
    FMeshCreateParam_LineAABB::FMeshCreateParam_LineAABB()
        : FMeshCreateParam(false, false)
    {

    }
    FMeshCreateParam_LineAABB::FMeshCreateParam_LineAABB(float _width,
                                                         float _height,
                                                         float _depth)
        : FMeshCreateParam(false, false)
        , width(_width)
        , height(_height)
        , depth(_depth)
    {

    }
    FMeshCreateParam_LineAABB::~FMeshCreateParam_LineAABB()
    {

    }
    String FMeshCreateParam_LineAABB::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         width,
                                         height,
                                         depth);
    }

    //LineSphere
    String FMeshCreateParam_LineSphere::ms_nameType = "MeshLineSphere";
    FMeshCreateParam_LineSphere::FMeshCreateParam_LineSphere()
        : FMeshCreateParam(false, false)
    {

    }
    FMeshCreateParam_LineSphere::FMeshCreateParam_LineSphere(float _radius,
                                                             uint32 _sliceCount,
                                                             uint32 _stackCount)
        : FMeshCreateParam(false, false)
        , radius(_radius)
        , sliceCount(_sliceCount)
        , stackCount(_stackCount)
    {

    }
    FMeshCreateParam_LineSphere::~FMeshCreateParam_LineSphere()
    {

    }
    String FMeshCreateParam_LineSphere::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%u-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         radius,
                                         sliceCount,
                                         stackCount);
    }

    //Triangle
    String FMeshCreateParam_Triangle::ms_nameType = "MeshTriangle";
    FMeshCreateParam_Triangle::FMeshCreateParam_Triangle()
        : FMeshCreateParam(true, false)
    {

    }
    FMeshCreateParam_Triangle::FMeshCreateParam_Triangle(bool _flipV,
                                                         bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
    {

    }
    FMeshCreateParam_Triangle::~FMeshCreateParam_Triangle()
    {

    }
    String FMeshCreateParam_Triangle::ToName()
    {
        return FUtilString::FormatString("%s_%d_%d", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0,
                                         rightHand ? 1 : 0);
    }

    //Quad
    String FMeshCreateParam_Quad::ms_nameType = "MeshQuad";
    FMeshCreateParam_Quad::FMeshCreateParam_Quad()
        : FMeshCreateParam(false, false)
        , centerX(0)
        , centerY(0)
        , width(1)
        , height(1)
        , depth(0)
    {

    }
    FMeshCreateParam_Quad::FMeshCreateParam_Quad(float _centerX,
                                                 float _centerY,
                                                 float _width,
                                                 float _height,
                                                 float _depth,
                                                 bool _flipV,
                                                 bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
        , centerX(_centerX)
        , centerY(_centerY)
        , width(_width)
        , height(_height)
        , depth(_depth)
    {

    }
    FMeshCreateParam_Quad::~FMeshCreateParam_Quad()
    {
        
    }
    String FMeshCreateParam_Quad::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%f", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         centerX,
                                         centerY,
                                         width,
                                         height,
                                         depth);
    }

    //Grid
    String FMeshCreateParam_Grid::ms_nameType = "MeshGrid";
    FMeshCreateParam_Grid::FMeshCreateParam_Grid()
        : FMeshCreateParam(false, false)
        , width(1)
        , height(1)
        , m(10)
        , n(10)
    {

    }
    FMeshCreateParam_Grid::FMeshCreateParam_Grid(float _width,
                                                 float _height,
                                                 uint32 _m,
                                                 uint32 _n,
                                                 bool _flipV,
                                                 bool _rightHand)    
        : FMeshCreateParam(_flipV, _rightHand)
        , width(_width)
        , height(_height)
        , m(_m)
        , n(_n)
    {

    }
    FMeshCreateParam_Grid::~FMeshCreateParam_Grid()
    {
        
    }
    String FMeshCreateParam_Grid::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         width,
                                         height,
                                         m,
                                         n);
    }

    //Circle
    String FMeshCreateParam_Circle::ms_nameType = "MeshCircle";
    FMeshCreateParam_Circle::FMeshCreateParam_Circle()
        : FMeshCreateParam(false, false)
        , radius(1)
        , segment(100)
    {

    }
    FMeshCreateParam_Circle::FMeshCreateParam_Circle(float _radius,
                                                     uint32 _segment,
                                                     bool _flipV,
                                                     bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
        , radius(_radius)
        , segment(_segment)
    {

    }
    FMeshCreateParam_Circle::~FMeshCreateParam_Circle()
    {
        
    }
    String FMeshCreateParam_Circle::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         radius,
                                         segment);
    }

    //AABB
    String FMeshCreateParam_AABB::ms_nameType = "MeshAABB";
    FMeshCreateParam_AABB::FMeshCreateParam_AABB()
        : FMeshCreateParam(false, false)
        , width(1)
        , height(1)
        , depth(1)
        , numSubdivisions(0)
    {

    }
    FMeshCreateParam_AABB::FMeshCreateParam_AABB(float _width,
                                                 float _height,
                                                 float _depth,
                                                 uint32 _numSubdivisions,
                                                 bool _flipV,
                                                 bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
        , width(_width)
        , height(_height)
        , depth(_depth)
        , numSubdivisions(_numSubdivisions)
    {

    }
    FMeshCreateParam_AABB::~FMeshCreateParam_AABB()
    {
        
    }
    String FMeshCreateParam_AABB::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         width,
                                         height,
                                         depth,
                                         numSubdivisions);
    }

    //Sphere
    String FMeshCreateParam_Sphere::ms_nameType = "MeshSphere";
    FMeshCreateParam_Sphere::FMeshCreateParam_Sphere()
        : FMeshCreateParam(false, false)
        , radius(1)
        , sliceCount(30)
        , stackCount(30)
    {

    }
    FMeshCreateParam_Sphere::FMeshCreateParam_Sphere(float _radius,
                                                     uint32 _sliceCount,
                                                     uint32 _stackCount,
                                                     bool _flipV,
                                                     bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
        , radius(_radius)
        , sliceCount(_sliceCount)
        , stackCount(_stackCount)
    {

    }
    FMeshCreateParam_Sphere::~FMeshCreateParam_Sphere()
    {
        
    }
    String FMeshCreateParam_Sphere::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%u-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         radius,
                                         sliceCount,
                                         stackCount);
    }

    //GeoSphere
    String FMeshCreateParam_GeoSphere::ms_nameType = "MeshGeoSphere";
    FMeshCreateParam_GeoSphere::FMeshCreateParam_GeoSphere()
        : FMeshCreateParam(false, false)
        , radius(1)
        , numSubdivisions(5)
    {

    }
    FMeshCreateParam_GeoSphere::FMeshCreateParam_GeoSphere(float _radius,
                                                           uint32 _numSubdivisions,
                                                           bool _flipV,
                                                           bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
        , radius(_radius)
        , numSubdivisions(_numSubdivisions)
    {

    }
    FMeshCreateParam_GeoSphere::~FMeshCreateParam_GeoSphere()
    {
        
    }
    String FMeshCreateParam_GeoSphere::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         radius,
                                         numSubdivisions);
    }

    //Cylinder
    String FMeshCreateParam_Cylinder::ms_nameType = "MeshCylinder";
    FMeshCreateParam_Cylinder::FMeshCreateParam_Cylinder()
        : FMeshCreateParam(false, false)
        , bottomRadius(0.5f)
        , topRadius(0.5f)
        , height(1.0f)
        , heightOffset(0.0f)
        , sliceCount(50)
        , stackCount(50)
    {

    }
    FMeshCreateParam_Cylinder::FMeshCreateParam_Cylinder(float _bottomRadius,
                                                         float _topRadius,
                                                         float _height,
                                                         float _heightOffset,
                                                         uint32 _sliceCount,
                                                         uint32 _stackCount,
                                                         bool _flipV,
                                                         bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
        , bottomRadius(_bottomRadius)
        , topRadius(_topRadius)
        , height(_height)
        , heightOffset(_heightOffset)
        , sliceCount(_sliceCount)
        , stackCount(_stackCount)
    {

    }
    FMeshCreateParam_Cylinder::~FMeshCreateParam_Cylinder()
    {
        
    }
    String FMeshCreateParam_Cylinder::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%u-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         bottomRadius,
                                         topRadius,
                                         height,
                                         heightOffset,
                                         sliceCount,
                                         stackCount);
    }

    //Capsule
    String FMeshCreateParam_Capsule::ms_nameType = "MeshCapsule";
    FMeshCreateParam_Capsule::FMeshCreateParam_Capsule()
        : FMeshCreateParam(false, false)
        , radius(0.5f)
        , height(1.0f)
        , numRings(5)
        , numSegments(50)
        , numSegHeight(30)
    {

    }
    FMeshCreateParam_Capsule::FMeshCreateParam_Capsule(float _radius,
                                                       float _height,
                                                       uint32 _numRings,
                                                       uint32 _numSegments,
                                                       uint32 _numSegHeight,
                                                       bool _flipV,
                                                       bool _rightHand)
        : FMeshCreateParam(_flipV, _rightHand)
        , radius(_radius)
        , height(_height)
        , numRings(_numRings)
        , numSegments(_numSegments)
        , numSegHeight(_numSegHeight)
    {

    }
    FMeshCreateParam_Capsule::~FMeshCreateParam_Capsule()
    {
        
    }
    String FMeshCreateParam_Capsule::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         radius,
                                         height,
                                         numRings,
                                         numSegments,
                                         numSegHeight);
    }

    //Cone
    String FMeshCreateParam_Cone::ms_nameType = "MeshCone";
    FMeshCreateParam_Cone::FMeshCreateParam_Cone()
        : FMeshCreateParam(false, false)
        , radius(0.5f)
        , height(1.0f)
        , numSegBase(50)
        , numSegHeight(30)
    {

    }
    FMeshCreateParam_Cone::FMeshCreateParam_Cone(float _radius,
                                                 float _height,
                                                 uint32 _numSegBase,
                                                 uint32 _numSegHeight,
                                                 bool _flipV,
                                                 bool _rightHand)
        : FMeshCreateParam(false, false)
        , radius(_radius)
        , height(_height)
        , numSegBase(_numSegBase)
        , numSegHeight(_numSegHeight)
    {

    }
    FMeshCreateParam_Cone::~FMeshCreateParam_Cone()
    {
        
    }
    String FMeshCreateParam_Cone::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u",
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         radius,
                                         height,
                                         numSegBase,
                                         numSegHeight);
    }

    //Torus
    String FMeshCreateParam_Torus::ms_nameType = "MeshTorus";
    FMeshCreateParam_Torus::FMeshCreateParam_Torus()
        : FMeshCreateParam(false, false)
        , radius(0.5f)
        , sectionRadius(0.2f)
        , numSegSection(50)
        , numSegCircle(20)
    {

    }
    FMeshCreateParam_Torus::FMeshCreateParam_Torus(float _radius,
                                                   float _sectionRadius,
                                                   uint32 _numSegSection,
                                                   uint32 _numSegCircle,
                                                   bool _flipV,
                                                   bool _rightHand)
        : FMeshCreateParam(false, false)
        , radius(_radius)
        , sectionRadius(_sectionRadius)
        , numSegSection(_numSegSection)
        , numSegCircle(_numSegCircle)
    {

    }
    FMeshCreateParam_Torus::~FMeshCreateParam_Torus()
    {
        
    }
    String FMeshCreateParam_Torus::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u",
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         radius,
                                         sectionRadius,
                                         numSegSection,
                                         numSegCircle);
    }

    //SkyBox
    String FMeshCreateParam_SkyBox::ms_nameType = "MeshSkyBox";
    FMeshCreateParam_SkyBox::FMeshCreateParam_SkyBox()
        : FMeshCreateParam(false, false)
    {

    }
    FMeshCreateParam_SkyBox::~FMeshCreateParam_SkyBox()
    {
        
    }
    String FMeshCreateParam_SkyBox::ToName()
    {
        return "";
    }   

    //SkyDome
    String FMeshCreateParam_SkyDome::ms_nameType = "MeshSkyDome";
    FMeshCreateParam_SkyDome::FMeshCreateParam_SkyDome()
        : FMeshCreateParam(false, false)
    {

    }
    FMeshCreateParam_SkyDome::~FMeshCreateParam_SkyDome()
    {
        
    }
    String FMeshCreateParam_SkyDome::ToName()
    {
        return "";
    }

    //Terrain
    String FMeshCreateParam_Terrain::ms_nameType = "MeshTerrain";
    FMeshCreateParam_Terrain::FMeshCreateParam_Terrain()
        : FMeshCreateParam(false, false)
        , offsetX(0)
        , offsetZ(0)
        , width(1024)
        , height(1024)
        , vertexX(1025)
        , vertexZ(1025)
        , pHeight(nullptr)
        , heightDataGap(1)
    {

    }
    FMeshCreateParam_Terrain::FMeshCreateParam_Terrain(float _offsetX,
                                                       float _offsetZ,
                                                       float _width,
                                                       float _height,
                                                       uint32 _vertexX,
                                                       uint32 _vertexZ,
                                                       float* _pHeight,
                                                       uint32 _heightDataGap,
                                                       bool _flipV,
                                                       bool _rightHand)    
        : FMeshCreateParam(_flipV, _rightHand)
        , offsetX(_offsetX)
        , offsetZ(_offsetZ)
        , width(_width)
        , height(_height)
        , vertexX(_vertexX)
        , vertexZ(_vertexZ)
        , pHeight(_pHeight)
        , heightDataGap(_heightDataGap)
    {

    }
    FMeshCreateParam_Terrain::~FMeshCreateParam_Terrain()
    {
        
    }
    String FMeshCreateParam_Terrain::ToName()
    {
        return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%u-%u", 
                                         ms_nameType.c_str(), 
                                         flipV ? 1 : 0, 
                                         rightHand ? 1 : 0,
                                         offsetX,
                                         offsetZ,
                                         width,
                                         height,
                                         vertexX,
                                         vertexZ);
    }

    ///////////////////////////////////////// FMeshGeometry //////////////////////////////////////////////
    bool FMeshGeometry::CreateGeometry(FMeshData& meshData, FMeshGeometryType eMeshGeometry)
    {
        switch ((int)eMeshGeometry)
        {
        case F_MeshGeometry_Line:
            {
                FMeshCreateParam_Line param_Line;
                FMeshGeometry::CreateLine(meshData, &param_Line);
                return true;
            }
        case F_MeshGeometry_LineQuad:
            {
                FMeshCreateParam_LineQuad param_LineQuad;
                FMeshGeometry::CreateLineQuad(meshData, &param_LineQuad);
                return true;
            }
        case F_MeshGeometry_LineAABB:
            {
                FMeshCreateParam_LineAABB param_LineAABB;
                FMeshGeometry::CreateLineAABB(meshData, &param_LineAABB);
                return true;
            }
        case F_MeshGeometry_LineSphere:
            {
                FMeshCreateParam_LineSphere param_LineSphere;
                FMeshGeometry::CreateLineSphere(meshData, &param_LineSphere);
                return true;
            }
        case F_MeshGeometry_Triangle:
            {
                FMeshCreateParam_Triangle param_Triangle;
                FMeshGeometry::CreateTriangle(meshData, &param_Triangle);
                return true;
            }
        case F_MeshGeometry_Quad:
            {
                FMeshCreateParam_Quad param_Quad;
                FMeshGeometry::CreateQuad(meshData, &param_Quad);
                return true;
            }
        case F_MeshGeometry_Grid:
            {
                FMeshCreateParam_Grid param_Grid;
                FMeshGeometry::CreateGrid(meshData, &param_Grid);
                return true;
            }
        case F_MeshGeometry_Circle:
            {
                FMeshCreateParam_Circle param_Circle;
                FMeshGeometry::CreateCircle(meshData, &param_Circle);
                return true;
            }
        case F_MeshGeometry_AABB:
            {
                FMeshCreateParam_AABB param_AABB;
                FMeshGeometry::CreateAABB(meshData, &param_AABB);
                return true;
            }
        case F_MeshGeometry_Sphere:
            {
                FMeshCreateParam_Sphere param_Sphere;
                FMeshGeometry::CreateSphere(meshData, &param_Sphere);
                return true;
            }
         case F_MeshGeometry_GeoSphere:
            {
                FMeshCreateParam_GeoSphere param_GeoSphere;
                FMeshGeometry::CreateGeosphere(meshData, &param_GeoSphere);
                return true;
            }
        case F_MeshGeometry_Cylinder:
            {
                FMeshCreateParam_Cylinder param_Cylinder;
                FMeshGeometry::CreateCylinder(meshData, &param_Cylinder);
                return true;
            }
        case F_MeshGeometry_Capsule:
            {
                FMeshCreateParam_Capsule param_Capsule;
                FMeshGeometry::CreateCapsule(meshData, &param_Capsule);
                return true;
            }
        case F_MeshGeometry_Cone:
            {
                FMeshCreateParam_Cone param_Cone;
                FMeshGeometry::CreateCone(meshData, &param_Cone);
                return true;
            }
        case F_MeshGeometry_Torus:
            {
                FMeshCreateParam_Torus param_Torus;
                FMeshGeometry::CreateTorus(meshData, &param_Torus);
                return true;
            }
        case F_MeshGeometry_SkyBox:
            {
                FMeshCreateParam_SkyBox param_SkyBox;
                FMeshGeometry::CreateSkyBox(meshData, &param_SkyBox);
                return true;
            }
        case F_MeshGeometry_SkyDome:
            {
                FMeshCreateParam_SkyDome param_SkyDome;
                FMeshGeometry::CreateSkyDome(meshData, &param_SkyDome);
                return true;
            }
        case F_MeshGeometry_Terrain:
            {
                FMeshCreateParam_Terrain param_Terrain;
                FMeshGeometry::CreateTerrain(meshData, &param_Terrain);
                return true;
            }
        }
        return false;
    }
    bool FMeshGeometry::CreateGeometry(FMeshData& meshData, FMeshGeometryType eMeshGeometry, FMeshCreateParam* pParam)
    {   
        if (pParam == nullptr)
        {
            return CreateGeometry(meshData, eMeshGeometry);
        }

        switch ((int)eMeshGeometry)
        {
        case F_MeshGeometry_Line:
            {
                FMeshCreateParam_Line* pParam_Line = static_cast<FMeshCreateParam_Line*>(pParam);
                FMeshGeometry::CreateLine(meshData, pParam_Line);
                return true;
            }
        case F_MeshGeometry_LineQuad:
            {
                FMeshCreateParam_LineQuad* pParam_LineQuad = static_cast<FMeshCreateParam_LineQuad*>(pParam);
                FMeshGeometry::CreateLineQuad(meshData, pParam_LineQuad);
                return true;
            }
        case F_MeshGeometry_LineAABB:
            {
                FMeshCreateParam_LineAABB* pParam_LineAABB = static_cast<FMeshCreateParam_LineAABB*>(pParam);
                FMeshGeometry::CreateLineAABB(meshData, pParam_LineAABB);
                return true;
            }
        case F_MeshGeometry_LineSphere:
            {
                FMeshCreateParam_LineSphere* pParam_LineSphere = static_cast<FMeshCreateParam_LineSphere*>(pParam);
                FMeshGeometry::CreateLineSphere(meshData, pParam_LineSphere);
                return true;
            }
        case F_MeshGeometry_Triangle:
            {
                FMeshCreateParam_Triangle* pParam_Triangle = static_cast<FMeshCreateParam_Triangle*>(pParam);
                FMeshGeometry::CreateTriangle(meshData, pParam_Triangle);
                return true;
            }
        case F_MeshGeometry_Quad:
            {
                FMeshCreateParam_Quad* pParam_Quad = static_cast<FMeshCreateParam_Quad*>(pParam);
                FMeshGeometry::CreateQuad(meshData, pParam_Quad);
                return true;
            }
        case F_MeshGeometry_Grid:
            {
                FMeshCreateParam_Grid* pParam_Grid = static_cast<FMeshCreateParam_Grid*>(pParam);
                FMeshGeometry::CreateGrid(meshData, pParam_Grid);
                return true;
            }
        case F_MeshGeometry_Circle:
            {
                FMeshCreateParam_Circle* pParam_Circle = static_cast<FMeshCreateParam_Circle*>(pParam);
                FMeshGeometry::CreateCircle(meshData, pParam_Circle);
                return true;
            }
        case F_MeshGeometry_AABB:
            {
                FMeshCreateParam_AABB* pParam_AABB = static_cast<FMeshCreateParam_AABB*>(pParam);
                FMeshGeometry::CreateAABB(meshData, pParam_AABB);
                return true;
            }
        case F_MeshGeometry_Sphere:
            {
                FMeshCreateParam_Sphere* pParam_Sphere = static_cast<FMeshCreateParam_Sphere*>(pParam);
                FMeshGeometry::CreateSphere(meshData, pParam_Sphere);
                return true;
            }
        case F_MeshGeometry_GeoSphere:
            {
                FMeshCreateParam_GeoSphere* pParam_GeoSphere = static_cast<FMeshCreateParam_GeoSphere*>(pParam);
                FMeshGeometry::CreateGeosphere(meshData, pParam_GeoSphere);
                return true;
            }
        case F_MeshGeometry_Cylinder:
            {
                FMeshCreateParam_Cylinder* pParam_Cylinder = static_cast<FMeshCreateParam_Cylinder*>(pParam);
                FMeshGeometry::CreateCylinder(meshData, pParam_Cylinder);
                return true;
            }
        case F_MeshGeometry_Capsule:
            {
                FMeshCreateParam_Capsule* pParam_Capsule = static_cast<FMeshCreateParam_Capsule*>(pParam);
                FMeshGeometry::CreateCapsule(meshData, pParam_Capsule);
                return true;
            }
        case F_MeshGeometry_Cone:
            {
                FMeshCreateParam_Cone* pParam_Cone = static_cast<FMeshCreateParam_Cone*>(pParam);
                FMeshGeometry::CreateCone(meshData, pParam_Cone);
                return true;
            }
        case F_MeshGeometry_Torus:
            {
                FMeshCreateParam_Torus* pParam_Torus = static_cast<FMeshCreateParam_Torus*>(pParam);
                FMeshGeometry::CreateTorus(meshData, pParam_Torus);
                return true;
            }
        case F_MeshGeometry_SkyBox:
            {
                FMeshCreateParam_SkyBox* pParam_SkyBox = static_cast<FMeshCreateParam_SkyBox*>(pParam);
                FMeshGeometry::CreateSkyBox(meshData, pParam_SkyBox);
                return true;
            }
        case F_MeshGeometry_SkyDome:
            {
                FMeshCreateParam_SkyDome* pParam_SkyDome = static_cast<FMeshCreateParam_SkyDome*>(pParam);
                FMeshGeometry::CreateSkyDome(meshData, pParam_SkyDome);
                return true;
            }
        case F_MeshGeometry_Terrain:
            {
                FMeshCreateParam_Terrain* pParam_Terrain = static_cast<FMeshCreateParam_Terrain*>(pParam);
                FMeshGeometry::CreateTerrain(meshData, pParam_Terrain);
                return true;
            }
        }
        return false;
    }


    void FMeshGeometry::CreateLine(FMeshData& meshData)
    {
        //FMeshVertex
        AddVertex(meshData, FMeshVertex( 0.0f,  0.0f,  0.0f,
                                         0.0f,  0.0f,  1.0f,
                                         1.0f,  0.0f,  0.0f,
                                         0.0f,  0.0f));
        AddVertex(meshData, FMeshVertex( 1.0f,  0.0f,  0.0f,
                                         0.0f,  0.0f,  1.0f,
                                         1.0f,  0.0f,  0.0f,
                                         0.0f,  0.0f));

        //Index
        AddIndexLine(meshData, 0, 1);
    }


    void FMeshGeometry::CreateLineQuad(FMeshData& meshData,
                                       float centerX,
                                       float centerY,
                                       float width,
                                       float height)
    {
        //  0       3
        //   --------
        //   |\     |
        //   |  \   |
        //   |    \ |
        //   --------
        //  1        2

        //FMeshVertex
        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY + height/2, 0,
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f));

        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY - height/2, 0,
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY - height/2, 0,
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY + height/2, 0,
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f));

        //Index
        AddIndexLine(meshData, 0, 1);
        AddIndexLine(meshData, 1, 2);
        AddIndexLine(meshData, 2, 3);
        AddIndexLine(meshData, 3, 0);
    }


    void FMeshGeometry::CreateLineAABB(FMeshData& meshData,
                                       float width,
                                       float height,
                                       float depth)
    {
        
    }


    void FMeshGeometry::CreateLineSphere(FMeshData& meshData,
                                         float radius,
                                         uint32 sliceCount,
                                         uint32 stackCount)
    {
        
    }


    void FMeshGeometry::CreateTriangle(FMeshData& meshData, 
                                       bool flipV,
                                       bool rightHand)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //FMeshVertex
        AddVertex(meshData, FMeshVertex( 0.0f,  0.5f,  0.0f,
                                         0.0f,  0.0f,  1.0f,
                                         1.0f,  0.0f,  0.0f,
                                         0.5f,  flipV ? 1.0f : 0.0f));

        AddVertex(meshData, FMeshVertex(-0.5f, -0.5f,  0.0f,
                                         0.0f,  0.0f,  1.0f,
                                         1.0f,  0.0f,  0.0f,
                                         0.0f,  flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex( 0.5f, -0.5f,  0.0f,
                                         0.0f,  0.0f,  1.0f,
                                         1.0f,  0.0f,  0.0f,
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

    void FMeshGeometry::CreateQuad(FMeshData& meshData, 
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
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, flipV ? 1.0f : 0.0f));

        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        1.0f, flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY + height/2, depth,
                                        0.0f, 0.0f, 1.0f,
                                        1.0f, 0.0f, 0.0f,
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

    void FMeshGeometry::CreateGrid(FMeshData& meshData, 
                                   float width, 
                                   float height, 
                                   uint32 m, 
                                   uint32 n,
                                   bool flipV,
                                   bool rightHand)
    {
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
            float y = halfH - i * dz;
            for (uint32 j = 0; j < n; ++j)
            {
                float x = -halfW + j * dx;

                FMeshVertex vertex(   x,    y, 0.0f,
                                   0.0f, 0.0f, 1.0f,
                                   1.0f, 0.0f, 0.0f,
                                   j * du, flipV ? (1.0f - i * dv) : (i * dv));
                SetVertex(meshData, i * n + j, vertex);
                //meshData.aabb.Merge(vertex.pos);
            }
        }

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
                                     i * n + j,
                                     (i + 1) * n + j,
                                     (i + 1) * n + j + 1);

                    SetIndexTriangle(meshData, k + 3,
                                     (i + 1) * n + j + 1,
                                     i * n + j + 1,
                                     i * n + j);
                }
                else
                {
                    SetIndexTriangle(meshData, k + 0,
                                     i * n + j,
                                     i * n + j + 1,
                                     (i + 1) * n + j + 1);

                    SetIndexTriangle(meshData, k + 3,
                                     (i + 1) * n + j + 1,
                                     (i + 1) * n + j,
                                     i * n + j);
                }

                k += 6;
            }
        }
    }

    void FMeshGeometry::CreateCircle(FMeshData& meshData,
                                     float radius,
                                     uint32 segment,
                                     bool flipV,
                                     bool rightHand)
    {
        uint32 vertexCount = segment + 1;
        uint32 faceCount = segment;

        //FMeshVertex
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        ResizeVertexCount(meshData, vertexCount);
        SetVertex(meshData, 
                  0, 
                  FMeshVertex(0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f,
                              1.0f, 0.0f, 0.0f,
                              0.5f, 0.5f));
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            float ux = cos(thetaStep * i);
            float uy = sin(thetaStep * i);

            SetVertex(meshData, 
                      index,
                      FMeshVertex(radius * ux,  radius * uy, 0.0f,
                                  0.0f, 0.0f, 1.0f,
                                  1.0f, 0.0f, 0.0f,
                                  (1.0f + ux) / 2.0f,  flipV ? (1.0f - (1.0f - uy) / 2.0f) : (1.0f - uy) / 2.0f));
            //meshData.aabb.Merge(vertex.pos);
            index++;
        }

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, i*3 + 0,
                                     0,
                                     i + 1,
                                     i + 2);
                }
                else
                {
                    SetIndexTriangle(meshData, i*3 + 0,
                                     0,
                                     i + 2,
                                     i + 1);
                }
            }
            else
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, i*3 + 0,
                                     0,
                                     i,
                                     1);
                }
                else
                {
                    SetIndexTriangle(meshData, i*3 + 0,
                                     0,
                                     1,
                                     i);
                }
            }
        }
    }

    void FMeshGeometry::CreateAABB(FMeshData& meshData, 
                                   float width, 
                                   float height, 
                                   float depth, 
                                   uint32 numSubdivisions,
                                   bool flipV,
                                   bool rightHand)
    {
        //FMeshVertex
        FMeshVertex v[24];
        float w2 = 0.5f * width;
        float h2 = 0.5f * height;
        float d2 = 0.5f * depth;
        //Front
        v[0]  = FMeshVertex(-w2, +h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[1]  = FMeshVertex(-w2, -h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[2]  = FMeshVertex(+w2, -h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[3]  = FMeshVertex(+w2, +h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Back
        v[4]  = FMeshVertex(-w2, -h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[5]  = FMeshVertex(-w2, +h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[6]  = FMeshVertex(+w2, +h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[7]  = FMeshVertex(+w2, -h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Top
        v[8]  = FMeshVertex(-w2, +h2, -d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[9]  = FMeshVertex(-w2, +h2, +d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[10] = FMeshVertex(+w2, +h2, +d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[11] = FMeshVertex(+w2, +h2, -d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Bottom
        v[12] = FMeshVertex(-w2, -h2, +d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[13] = FMeshVertex(-w2, -h2, -d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[14] = FMeshVertex(+w2, -h2, -d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[15] = FMeshVertex(+w2, -h2, +d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Left
        v[16] = FMeshVertex(-w2, +h2, -d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[17] = FMeshVertex(-w2, -h2, -d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[18] = FMeshVertex(-w2, -h2, +d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[19] = FMeshVertex(-w2, +h2, +d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Right
        v[20] = FMeshVertex(+w2, +h2, +d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[21] = FMeshVertex(+w2, -h2, +d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[22] = FMeshVertex(+w2, -h2, -d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[23] = FMeshVertex(+w2, +h2, -d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   1.0f, flipV ? 1.0f : 0.0f);

        for (int i = 0; i < 24; i++)
        {
            AddVertex(meshData, v[i]);
        }

        //Index
        uint32 i[36];
        if (rightHand)
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
        else
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
        AddIndices(meshData, 36, i);

        //Subdivisions.
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        for (uint32 i = 0; i < numSubdivisions; ++i)
        {
            Subdivide(meshData, rightHand);
        }
    }

    void FMeshGeometry::CreateSphere(FMeshData& meshData, 
                                     float radius, 
                                     uint32 sliceCount, 
                                     uint32 stackCount,
                                     bool flipV,
                                     bool rightHand)
    {
        //FMeshVertex
        FMeshVertex topVertex(0.0f, +radius, 0.0f,  0.0f, +1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 1.0f : 0.0f);
        FMeshVertex bottomVertex(0.0f, -radius, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 0.0f : 1.0f);
        AddVertex(meshData, topVertex);

        float phiStep = FMath::ms_fPI / stackCount;
        float thetaStep = 2.0f * FMath::ms_fPI / sliceCount;
        for (uint32 i = 1; i <= stackCount - 1; ++i)
        {
            float phi = i * phiStep;
            for (uint32 j = 0; j <= sliceCount; ++j)
            {
                float theta = j * thetaStep;

                FMeshVertex v;
                //pos
                v.pos.x = radius * sinf(phi) * cosf(theta);
                v.pos.y = radius * cosf(phi);
                v.pos.z = radius * sinf(phi) * sinf(theta);
                //color
                v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                //normal
                v.normal = FMath::Normalize(v.pos);
                //tangent
                v.tangent.x = -radius * sinf(phi) * sinf(theta);
                v.tangent.y = 0.0f;
                v.tangent.z = +radius * sinf(phi) * cosf(theta);
                v.tangent = FMath::Normalize(v.tangent);
                //texCoord
                v.texCoord.x = theta / FMath::ms_fPI_Two;
                v.texCoord.y = flipV ? (1.0f - phi / FMath::ms_fPI) : (phi / FMath::ms_fPI);

                AddVertex(meshData, v);
            }
        }
        AddVertex(meshData, bottomVertex);

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

    void FMeshGeometry::CreateGeosphere(FMeshData& meshData, 
                                        float radius, 
                                        uint32 numSubdivisions,
                                        bool flipV,
                                        bool rightHand)
    {
        //FMeshVertex
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        const float X = 0.525731f;
        const float Z = 0.850651f;
        FVector3 pos[12] =
        {
            FVector3(   -X,  0.0f,     Z),  FVector3(    X,  0.0f,     Z),
            FVector3(   -X,  0.0f,    -Z),  FVector3(    X,  0.0f,    -Z),
            FVector3( 0.0f,     Z,     X),  FVector3( 0.0f,     Z,    -X),
            FVector3( 0.0f,    -Z,     X),  FVector3( 0.0f,    -Z,    -X),
            FVector3(    Z,     X,  0.0f),  FVector3(   -Z,     X,  0.0f),
            FVector3(    Z,    -X,  0.0f),  FVector3(   -Z,    -X,  0.0f)
        };

        uint32 k[60] =
        {
            1,  4, 0,   4, 9, 0,   4,  5, 9,   8, 5,  4,   1, 8, 4,
            1, 10, 8,  10, 3, 8,   8,  3, 5,   3, 2,  5,   3, 7, 2,
            3, 10, 7,  10, 6, 7,   6, 11, 7,   6, 0, 11,   6, 1, 0,
            10, 1, 6,  11, 0, 9,   2, 11, 9,   5, 2,  9,  11, 2, 7
        };

        ResizeVertexCount(meshData, 12);
        AddIndices(meshData, 60, k);

        for (uint32 i = 0; i < 12; ++i)
        {
            FMeshVertex& v = GetVertex(meshData, i);
            v.pos = pos[i];
        }
        for (uint32 i = 0; i < numSubdivisions; ++i)
        {
            Subdivide(meshData, rightHand);
        }

        uint32 countVertex = GetVertexCount(meshData);
        for (uint32 i = 0; i < countVertex; ++i)
        {
            FMeshVertex& v = GetVertex(meshData, i);

            v.normal = FMath::Normalize(v.pos);
            v.pos = radius * v.normal;

            float theta = atan2f(v.pos.z, v.pos.x);
            if (theta < 0.0f)
                theta += FMath::ms_fPI_Two;

            float phi = acosf(v.pos.y / radius);

            v.texCoord.x = theta / FMath::ms_fPI_Two;
            v.texCoord.y = flipV ? (1.0f - phi / FMath::ms_fPI) : (phi / FMath::ms_fPI);

            v.tangent.x = -radius * sinf(phi) * sinf(theta);
            v.tangent.y = 0.0f;
            v.tangent.z = +radius * sinf(phi) * cosf(theta);
            v.tangent = FMath::Normalize(v.tangent);

            //meshData.aabb.Merge(meshData.vertices[i].pos);
        }
    }

    void s_BuildCylinderTopCap(FMeshData& meshData, 
                               float bottomRadius, 
                               float topRadius, 
                               float height, 
                               uint32 sliceCount, 
                               uint32 stackCount, 
                               bool flipV,
                               bool rightHand)
    {
        uint32 baseIndex = FMeshGeometry::GetVertexCount(meshData);

        float y = 0.5f * height;
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
                                  uint32 sliceCount, 
                                  uint32 stackCount, 
                                  bool flipV,
                                  bool rightHand)
    {
        uint32 baseIndex = FMeshGeometry::GetVertexCount(meshData);
        float y = -0.5f * height;

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

    void FMeshGeometry::CreateCylinder(FMeshData& meshData, 
                                       float bottomRadius, 
                                       float topRadius, 
                                       float height, 
                                       float heightOffset,
                                       uint32 sliceCount, 
                                       uint32 stackCount,
                                       bool flipV,
                                       bool rightHand)
    {
        // Cylinder can be parameterized as follows, where we introduce v
        // parameter that goes in the same direction as the v tex-coord
        // so that the bitangent goes in the same direction as the v tex-coord.
        //  Let r0 be the bottom radius and let r1 be the top radius.
        //  y(v) = h - hv for v in [0,1].
        //  r(v) = r1 + (r0-r1)v
        //
        //  x(t, v) = r(v)*cos(t)
        //  y(t, v) = h - hv
        //  z(t, v) = r(v)*sin(t)
        // 
        //  dx/dt = -r(v)*sin(t)
        //  dy/dt = 0
        //  dz/dt = +r(v)*cos(t)
        //
        //  dx/dv = (r0-r1)*cos(t)
        //  dy/dv = -h
        //  dz/dv = (r0-r1)*sin(t)

        //FMeshVertex
        float stackHeight = height / stackCount;
        float radiusStep = (topRadius - bottomRadius) / stackCount;
        uint32 ringCount = stackCount + 1;
        for (uint32 i = 0; i < ringCount; ++i)
        {
            float y = (-0.5f + heightOffset) * height + i * stackHeight;
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

        s_BuildCylinderTopCap(meshData, bottomRadius, topRadius, height, sliceCount, stackCount, flipV, rightHand);
        s_BuildCylinderBottomCap(meshData, bottomRadius, topRadius, height, sliceCount, stackCount, flipV, rightHand);
    }

    void FMeshGeometry::CreateCapsule(FMeshData& meshData,
                                      float radius,
                                      float height,
                                      uint32 numRings,
                                      uint32 numSegments,
                                      uint32 numSegHeight,
                                      bool flipV,
                                      bool rightHand)
    {
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
                FVector3 pos(x0, 0.5f * height + y0, z0);
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
                FVector3 pos(x0, 0.5f * height - i * deltamHeight, z0);
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
                FVector3 pos(x0, -0.5f * height + y0, z0);
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

    void FMeshGeometry::CreateCone(FMeshData& meshData,
                                   float radius,
                                   float height,
                                   uint32 numSegBase,
                                   uint32 numSegHeight,
                                   bool flipV,
                                   bool rightHand)
    {
        uint32 nVertexCount = (numSegHeight + 1) * (numSegBase + 1) + numSegBase + 2;
        uint32 nIndexCount = numSegHeight * numSegBase * 6 + 3 * numSegBase;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float deltaAngle = (FMath::ms_fPI_Two / numSegBase);
        float deltaHeight = height/(float)numSegHeight;
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
                FVector3 pos(x0, i * deltaHeight, z0);
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
                  FMath::ms_v3Zero,
                  FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                  FMath::ms_v3UnitNegY,
                  FMath::ms_v3UnitX,
                  flipV ? FMath::ms_v2UnitX : FMath::ms_v2One);
        offset++;
        for (uint32 j = 0; j <= numSegBase; j++)
        {
            float x0 = radius * cosf(j * deltaAngle);
            float z0 = radius * sinf(j * deltaAngle);
            FVector3 pos(x0, 0.0f, z0);
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

    void FMeshGeometry::CreateTorus(FMeshData& meshData,
                                    float radius,
                                    float sectionRadius,
                                    uint32 numSegSection,
                                    uint32 numSegCircle,
                                    bool flipV,
                                    bool rightHand)
    {
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

    void FMeshGeometry::CreateSkyBox(FMeshData& meshData,
                                     bool flipV,
                                     bool rightHand)
    {

    }

    void FMeshGeometry::CreateSkyDome(FMeshData& meshData,
                                      bool flipV,
                                      bool rightHand)
    {

    }

    void FMeshGeometry::CreateTerrain(FMeshData& meshData,
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

    void FMeshGeometry::Subdivide(FMeshData& meshData, bool rightHand)
    {
        FMeshData inputCopy = meshData;

        ResizeVertexCount(meshData, 0);
        ResizeIndexCount(meshData, 0);

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