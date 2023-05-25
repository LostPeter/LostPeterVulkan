/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanMeshGeometry.h"
#include "../include/VulkanMath.h"
#include "../include/VulkanUtilString.h"

namespace LostPeter
{
	///////////////////////////////////////// VulkanMeshCreateParam ///////////////////////////////////////////
    VulkanMeshCreateParam::VulkanMeshCreateParam(bool _flipV, bool _rightHand)
        : flipV(_flipV)
        , rightHand(_rightHand)
    {

    }
    VulkanMeshCreateParam::~VulkanMeshCreateParam()
    {

    }

    //Triangle
    String VulkanMeshCreateParam_Triangle::ms_nameType = "MeshTriangle";
    VulkanMeshCreateParam_Triangle::VulkanMeshCreateParam_Triangle()
        : VulkanMeshCreateParam(true, false)
    {

    }
    VulkanMeshCreateParam_Triangle::VulkanMeshCreateParam_Triangle(bool _flipV,
                                                                   bool _rightHand)
        : VulkanMeshCreateParam(_flipV, _rightHand)
    {

    }
    VulkanMeshCreateParam_Triangle::~VulkanMeshCreateParam_Triangle()
    {

    }
    String VulkanMeshCreateParam_Triangle::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d_%d", 
											  ms_nameType.c_str(), 
											  flipV ? 1 : 0,
											  rightHand ? 1 : 0);
    }

    //Quad
    String VulkanMeshCreateParam_Quad::ms_nameType = "MeshQuad";
    VulkanMeshCreateParam_Quad::VulkanMeshCreateParam_Quad()
        : VulkanMeshCreateParam(false, false)
        , centerX(0)
        , centerY(0)
        , width(1)
        , height(1)
        , depth(0)
    {

    }
    VulkanMeshCreateParam_Quad::VulkanMeshCreateParam_Quad(float _centerX,
                                                           float _centerY,
                                                           float _width,
                                                           float _height,
                                                           float _depth,
                                                           bool _flipV,
                                                           bool _rightHand)
        : VulkanMeshCreateParam(_flipV, _rightHand)
        , centerX(_centerX)
        , centerY(_centerY)
        , width(_width)
        , height(_height)
        , depth(_depth)
    {

    }
    VulkanMeshCreateParam_Quad::~VulkanMeshCreateParam_Quad()
    {
        
    }
    String VulkanMeshCreateParam_Quad::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%f", 
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
    String VulkanMeshCreateParam_Grid::ms_nameType = "MeshGrid";
    VulkanMeshCreateParam_Grid::VulkanMeshCreateParam_Grid()
        : VulkanMeshCreateParam(false, false)
        , width(1)
        , height(1)
        , m(10)
        , n(10)
    {

    }
    VulkanMeshCreateParam_Grid::VulkanMeshCreateParam_Grid(float _width,
                                                           float _height,
                                                           uint32 _m,
                                                           uint32 _n,
                                                           bool _flipV,
                                                           bool _rightHand)    
        : VulkanMeshCreateParam(_flipV, _rightHand)
        , width(_width)
        , height(_height)
        , m(_m)
        , n(_n)
    {

    }
    VulkanMeshCreateParam_Grid::~VulkanMeshCreateParam_Grid()
    {
        
    }
    String VulkanMeshCreateParam_Grid::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u", 
											  ms_nameType.c_str(), 
											  flipV ? 1 : 0, 
											  rightHand ? 1 : 0,
											  width,
											  height,
											  m,
											  n);
    }

    //Circle
    String VulkanMeshCreateParam_Circle::ms_nameType = "MeshCircle";
    VulkanMeshCreateParam_Circle::VulkanMeshCreateParam_Circle()
        : VulkanMeshCreateParam(false, false)
        , radius(1)
        , segment(100)
    {

    }
    VulkanMeshCreateParam_Circle::VulkanMeshCreateParam_Circle(float _radius,
                                                               uint32 _segment,
                                                               bool _flipV,
                                                               bool _rightHand)
        : VulkanMeshCreateParam(_flipV, _rightHand)
        , radius(_radius)
        , segment(_segment)
    {

    }
    VulkanMeshCreateParam_Circle::~VulkanMeshCreateParam_Circle()
    {
        
    }
    String VulkanMeshCreateParam_Circle::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%u", 
											  ms_nameType.c_str(), 
											  flipV ? 1 : 0, 
											  rightHand ? 1 : 0,
											  radius,
											  segment);
    }

    //AABB
    String VulkanMeshCreateParam_AABB::ms_nameType = "MeshAABB";
    VulkanMeshCreateParam_AABB::VulkanMeshCreateParam_AABB()
        : VulkanMeshCreateParam(false, false)
        , width(1)
        , height(1)
        , depth(1)
        , numSubdivisions(0)
    {

    }
    VulkanMeshCreateParam_AABB::VulkanMeshCreateParam_AABB(float _width,
                                                           float _height,
                                                           float _depth,
                                                           uint32 _numSubdivisions,
                                                           bool _flipV,
                                                           bool _rightHand)
        : VulkanMeshCreateParam(_flipV, _rightHand)
        , width(_width)
        , height(_height)
        , depth(_depth)
        , numSubdivisions(_numSubdivisions)
    {

    }
    VulkanMeshCreateParam_AABB::~VulkanMeshCreateParam_AABB()
    {
        
    }
    String VulkanMeshCreateParam_AABB::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u", 
											  ms_nameType.c_str(), 
											  flipV ? 1 : 0, 
											  rightHand ? 1 : 0,
											  width,
											  height,
											  depth,
											  numSubdivisions);
    }

    //Sphere
    String VulkanMeshCreateParam_Sphere::ms_nameType = "MeshSphere";
    VulkanMeshCreateParam_Sphere::VulkanMeshCreateParam_Sphere()
        : VulkanMeshCreateParam(false, false)
        , radius(1)
        , sliceCount(30)
        , stackCount(30)
    {

    }
    VulkanMeshCreateParam_Sphere::VulkanMeshCreateParam_Sphere(float _radius,
                                                               uint32 _sliceCount,
                                                               uint32 _stackCount,
                                                               bool _flipV,
                                                               bool _rightHand)
        : VulkanMeshCreateParam(_flipV, _rightHand)
        , radius(_radius)
        , sliceCount(_sliceCount)
        , stackCount(_stackCount)
    {

    }
    VulkanMeshCreateParam_Sphere::~VulkanMeshCreateParam_Sphere()
    {
        
    }
    String VulkanMeshCreateParam_Sphere::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%u-%u", 
											  ms_nameType.c_str(), 
											  flipV ? 1 : 0, 
											  rightHand ? 1 : 0,
											  radius,
											  sliceCount,
											  stackCount);
    }

    //GeoSphere
    String VulkanMeshCreateParam_GeoSphere::ms_nameType = "MeshGeoSphere";
    VulkanMeshCreateParam_GeoSphere::VulkanMeshCreateParam_GeoSphere()
        : VulkanMeshCreateParam(false, false)
        , radius(1)
        , numSubdivisions(5)
    {

    }
    VulkanMeshCreateParam_GeoSphere::VulkanMeshCreateParam_GeoSphere(float _radius,
                                                                     uint32 _numSubdivisions,
                                                                     bool _flipV,
                                                                     bool _rightHand)
        : VulkanMeshCreateParam(_flipV, _rightHand)
        , radius(_radius)
        , numSubdivisions(_numSubdivisions)
    {

    }
    VulkanMeshCreateParam_GeoSphere::~VulkanMeshCreateParam_GeoSphere()
    {
        
    }
    String VulkanMeshCreateParam_GeoSphere::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%u", 
											  ms_nameType.c_str(), 
											  flipV ? 1 : 0, 
											  rightHand ? 1 : 0,
											  radius,
											  numSubdivisions);
    }

    //Cylinder
    String VulkanMeshCreateParam_Cylinder::ms_nameType = "MeshCylinder";
    VulkanMeshCreateParam_Cylinder::VulkanMeshCreateParam_Cylinder()
        : VulkanMeshCreateParam(false, false)
        , bottomRadius(0.5f)
        , topRadius(0.5f)
        , height(2.0f)
        , sliceCount(50)
        , stackCount(50)
    {

    }
    VulkanMeshCreateParam_Cylinder::VulkanMeshCreateParam_Cylinder(float _bottomRadius,
                                                                   float _topRadius,
                                                                   float _height,
                                                                   uint32 _sliceCount,
                                                                   uint32 _stackCount,
                                                                   bool _flipV,
                                                                   bool _rightHand)
        : VulkanMeshCreateParam(_flipV, _rightHand)
        , bottomRadius(_bottomRadius)
        , topRadius(_topRadius)
        , height(_height)
        , sliceCount(_sliceCount)
        , stackCount(_stackCount)
    {

    }
    VulkanMeshCreateParam_Cylinder::~VulkanMeshCreateParam_Cylinder()
    {
        
    }
    String VulkanMeshCreateParam_Cylinder::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u-%u", 
										 	  ms_nameType.c_str(), 
											  flipV ? 1 : 0, 
											  rightHand ? 1 : 0,
											  bottomRadius,
											  topRadius,
											  height,
											  sliceCount,
											  stackCount);
    }

    //Capsule
    String VulkanMeshCreateParam_Capsule::ms_nameType = "MeshCapsule";
    VulkanMeshCreateParam_Capsule::VulkanMeshCreateParam_Capsule()
        : VulkanMeshCreateParam(false, false)
    {

    }
    VulkanMeshCreateParam_Capsule::~VulkanMeshCreateParam_Capsule()
    {
        
    }
    String VulkanMeshCreateParam_Capsule::ToName()
    {
        return "";
    }

    //Cone
    String VulkanMeshCreateParam_Cone::ms_nameType = "MeshCone";
    VulkanMeshCreateParam_Cone::VulkanMeshCreateParam_Cone()
        : VulkanMeshCreateParam(false, false)
    {

    }
    VulkanMeshCreateParam_Cone::~VulkanMeshCreateParam_Cone()
    {
        
    }
    String VulkanMeshCreateParam_Cone::ToName()
    {
        return "";
    }

    //Torus
    String VulkanMeshCreateParam_Torus::ms_nameType = "MeshTorus";
    VulkanMeshCreateParam_Torus::VulkanMeshCreateParam_Torus()
        : VulkanMeshCreateParam(false, false)
    {

    }
    VulkanMeshCreateParam_Torus::~VulkanMeshCreateParam_Torus()
    {
        
    }
    String VulkanMeshCreateParam_Torus::ToName()
    {
        return "";
    }

    //SkyBox
    String VulkanMeshCreateParam_SkyBox::ms_nameType = "MeshSkyBox";
    VulkanMeshCreateParam_SkyBox::VulkanMeshCreateParam_SkyBox()
        : VulkanMeshCreateParam(false, false)
    {

    }
    VulkanMeshCreateParam_SkyBox::~VulkanMeshCreateParam_SkyBox()
    {
        
    }
    String VulkanMeshCreateParam_SkyBox::ToName()
    {
        return "";
    }   

    //SkyDome
    String VulkanMeshCreateParam_SkyDome::ms_nameType = "MeshSkyDome";
    VulkanMeshCreateParam_SkyDome::VulkanMeshCreateParam_SkyDome()
        : VulkanMeshCreateParam(false, false)
    {

    }
    VulkanMeshCreateParam_SkyDome::~VulkanMeshCreateParam_SkyDome()
    {
        
    }
    String VulkanMeshCreateParam_SkyDome::ToName()
    {
        return "";
    }

    //Terrain
    String VulkanMeshCreateParam_Terrain::ms_nameType = "MeshTerrain";
    VulkanMeshCreateParam_Terrain::VulkanMeshCreateParam_Terrain()
        : VulkanMeshCreateParam(false, false)
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
    VulkanMeshCreateParam_Terrain::VulkanMeshCreateParam_Terrain(float _offsetX,
                                                                 float _offsetZ,
                                                                 float _width,
                                                                 float _height,
                                                                 uint32 _vertexX,
                                                                 uint32 _vertexZ,
                                                                 float* _pHeight,
                                                                 uint32 _heightDataGap,
                                                                 bool _flipV,
                                                                 bool _rightHand)    
        : VulkanMeshCreateParam(_flipV, _rightHand)
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
    VulkanMeshCreateParam_Terrain::~VulkanMeshCreateParam_Terrain()
    {
        
    }
    String VulkanMeshCreateParam_Terrain::ToName()
    {
        return VulkanUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%u-%u", 
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

    ///////////////////////////////////////// VulkanMeshGeometry //////////////////////////////////////////////
    bool VulkanMeshGeometry::CreateGeometry(MeshData& meshData, VulkanMeshGeometryType eMeshGeometry)
    {
        switch ((int)eMeshGeometry)
        {
        case Vulkan_MeshGeometry_Triangle:
            {
                VulkanMeshCreateParam_Triangle param_Triangle;
                VulkanMeshGeometry::CreateTriangle(meshData, &param_Triangle);
                return true;
            }
        case Vulkan_MeshGeometry_Quad:
            {
                VulkanMeshCreateParam_Quad param_Quad;
                VulkanMeshGeometry::CreateQuad(meshData, &param_Quad);
                return true;
            }
        case Vulkan_MeshGeometry_Grid:
            {
                VulkanMeshCreateParam_Grid param_Grid;
                VulkanMeshGeometry::CreateGrid(meshData, &param_Grid);
                return true;
            }
        case Vulkan_MeshGeometry_Circle:
            {
                VulkanMeshCreateParam_Circle param_Circle;
                VulkanMeshGeometry::CreateCircle(meshData, &param_Circle);
                return true;
            }
        case Vulkan_MeshGeometry_AABB:
            {
                VulkanMeshCreateParam_AABB param_AABB;
                VulkanMeshGeometry::CreateAABB(meshData, &param_AABB);
                return true;
            }
        case Vulkan_MeshGeometry_Sphere:
            {
                VulkanMeshCreateParam_Sphere param_Sphere;
                VulkanMeshGeometry::CreateSphere(meshData, &param_Sphere);
                return true;
            }
         case Vulkan_MeshGeometry_GeoSphere:
            {
                VulkanMeshCreateParam_GeoSphere param_GeoSphere;
                VulkanMeshGeometry::CreateGeosphere(meshData, &param_GeoSphere);
                return true;
            }
        case Vulkan_MeshGeometry_Cylinder:
            {
                VulkanMeshCreateParam_Cylinder param_Cylinder;
                VulkanMeshGeometry::CreateCylinder(meshData, &param_Cylinder);
                return true;
            }
        case Vulkan_MeshGeometry_Capsule:
            {
                VulkanMeshCreateParam_Capsule param_Capsule;
                VulkanMeshGeometry::CreateCapsule(meshData, &param_Capsule);
                return true;
            }
        case Vulkan_MeshGeometry_Cone:
            {
                VulkanMeshCreateParam_Cone param_Cone;
                VulkanMeshGeometry::CreateCone(meshData, &param_Cone);
                return true;
            }
        case Vulkan_MeshGeometry_Torus:
            {
                VulkanMeshCreateParam_Torus param_Torus;
                VulkanMeshGeometry::CreateTorus(meshData, &param_Torus);
                return true;
            }
        case Vulkan_MeshGeometry_SkyBox:
            {
                VulkanMeshCreateParam_SkyBox param_SkyBox;
                VulkanMeshGeometry::CreateSkyBox(meshData, &param_SkyBox);
                return true;
            }
        case Vulkan_MeshGeometry_SkyDome:
            {
                VulkanMeshCreateParam_SkyDome param_SkyDome;
                VulkanMeshGeometry::CreateSkyDome(meshData, &param_SkyDome);
                return true;
            }
        case Vulkan_MeshGeometry_Terrain:
            {
                VulkanMeshCreateParam_Terrain param_Terrain;
                VulkanMeshGeometry::CreateTerrain(meshData, &param_Terrain);
                return true;
            }
        }
        return false;
    }
    bool VulkanMeshGeometry::CreateGeometry(MeshData& meshData, VulkanMeshGeometryType eMeshGeometry, VulkanMeshCreateParam* pParam)
    {   
        switch ((int)eMeshGeometry)
        {
        case Vulkan_MeshGeometry_Triangle:
            {
                VulkanMeshCreateParam_Triangle* pParam_Triangle = static_cast<VulkanMeshCreateParam_Triangle*>(pParam);
                VulkanMeshGeometry::CreateTriangle(meshData, pParam_Triangle);
                return true;
            }
        case Vulkan_MeshGeometry_Quad:
            {
                VulkanMeshCreateParam_Quad* pParam_Quad = static_cast<VulkanMeshCreateParam_Quad*>(pParam);
                VulkanMeshGeometry::CreateQuad(meshData, pParam_Quad);
                return true;
            }
        case Vulkan_MeshGeometry_Grid:
            {
                VulkanMeshCreateParam_Grid* pParam_Grid = static_cast<VulkanMeshCreateParam_Grid*>(pParam);
                VulkanMeshGeometry::CreateGrid(meshData, pParam_Grid);
                return true;
            }
        case Vulkan_MeshGeometry_Circle:
            {
                VulkanMeshCreateParam_Circle* pParam_Circle = static_cast<VulkanMeshCreateParam_Circle*>(pParam);
                VulkanMeshGeometry::CreateCircle(meshData, pParam_Circle);
                return true;
            }
        case Vulkan_MeshGeometry_AABB:
            {
                VulkanMeshCreateParam_AABB* pParam_AABB = static_cast<VulkanMeshCreateParam_AABB*>(pParam);
                VulkanMeshGeometry::CreateAABB(meshData, pParam_AABB);
                return true;
            }
        case Vulkan_MeshGeometry_Sphere:
            {
                VulkanMeshCreateParam_Sphere* pParam_Sphere = static_cast<VulkanMeshCreateParam_Sphere*>(pParam);
                VulkanMeshGeometry::CreateSphere(meshData, pParam_Sphere);
                return true;
            }
        case Vulkan_MeshGeometry_GeoSphere:
            {
                VulkanMeshCreateParam_GeoSphere* pParam_GeoSphere = static_cast<VulkanMeshCreateParam_GeoSphere*>(pParam);
                VulkanMeshGeometry::CreateGeosphere(meshData, pParam_GeoSphere);
                return true;
            }
        case Vulkan_MeshGeometry_Cylinder:
            {
                VulkanMeshCreateParam_Cylinder* pParam_Cylinder = static_cast<VulkanMeshCreateParam_Cylinder*>(pParam);
                VulkanMeshGeometry::CreateCylinder(meshData, pParam_Cylinder);
                return true;
            }
        case Vulkan_MeshGeometry_Capsule:
            {
                VulkanMeshCreateParam_Capsule* pParam_Capsule = static_cast<VulkanMeshCreateParam_Capsule*>(pParam);
                VulkanMeshGeometry::CreateCapsule(meshData, pParam_Capsule);
                return true;
            }
        case Vulkan_MeshGeometry_Cone:
            {
                VulkanMeshCreateParam_Cone* pParam_Cone = static_cast<VulkanMeshCreateParam_Cone*>(pParam);
                VulkanMeshGeometry::CreateCone(meshData, pParam_Cone);
                return true;
            }
        case Vulkan_MeshGeometry_Torus:
            {
                VulkanMeshCreateParam_Torus* pParam_Torus = static_cast<VulkanMeshCreateParam_Torus*>(pParam);
                VulkanMeshGeometry::CreateTorus(meshData, pParam_Torus);
                return true;
            }
        case Vulkan_MeshGeometry_SkyBox:
            {
                VulkanMeshCreateParam_SkyBox* pParam_SkyBox = static_cast<VulkanMeshCreateParam_SkyBox*>(pParam);
                VulkanMeshGeometry::CreateSkyBox(meshData, pParam_SkyBox);
                return true;
            }
        case Vulkan_MeshGeometry_SkyDome:
            {
                VulkanMeshCreateParam_SkyDome* pParam_SkyDome = static_cast<VulkanMeshCreateParam_SkyDome*>(pParam);
                VulkanMeshGeometry::CreateSkyDome(meshData, pParam_SkyDome);
                return true;
            }
        case Vulkan_MeshGeometry_Terrain:
            {
                VulkanMeshCreateParam_Terrain* pParam_Terrain = static_cast<VulkanMeshCreateParam_Terrain*>(pParam);
                VulkanMeshGeometry::CreateTerrain(meshData, pParam_Terrain);
                return true;
            }
        }
        return false;
    }


    void VulkanMeshGeometry::CreateTriangle(MeshData& meshData, 
                                            bool flipV,
                                            bool rightHand)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //MeshVertex
        meshData.AddVertex(MeshVertex(
            0.0f,  0.5f,  0.0f,
            0.0f,  0.0f,  1.0f,
            1.0f,  0.0f,  0.0f,
            0.5f,  flipV ? 1.0f : 0.0f));

        meshData.AddVertex(MeshVertex(
           -0.5f, -0.5f,  0.0f,
            0.0f,  0.0f,  1.0f,
            1.0f,  0.0f,  0.0f,
            0.0f,  flipV ? 0.0f : 1.0f));

        meshData.AddVertex(MeshVertex(
            0.5f, -0.5f,  0.0f,
            0.0f,  0.0f,  1.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  flipV ? 0.0f : 1.0f));

        //Index
        if (rightHand)
        {
            meshData.AddIndexTriangle(0, 1, 2);
        }
        else
        {
            meshData.AddIndexTriangle(0, 2, 1);
        }
    }

    void VulkanMeshGeometry::CreateQuad(MeshData& meshData, 
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

        //MeshVertex
        meshData.AddVertex(MeshVertex(
            centerX - width/2, centerY + height/2, depth,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, flipV ? 1.0f : 0.0f));

        meshData.AddVertex(MeshVertex(
            centerX - width/2, centerY - height/2, depth,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, flipV ? 0.0f : 1.0f));

        meshData.AddVertex(MeshVertex(
            centerX + width/2, centerY - height/2, depth,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, flipV ? 0.0f : 1.0f));

        meshData.AddVertex(MeshVertex(
            centerX + width/2, centerY + height/2, depth,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, flipV ? 1.0f : 0.0f));

        //Index
        if (rightHand)
        {
            meshData.AddIndexTriangle(0, 1, 2);
            meshData.AddIndexTriangle(2, 3, 0);
        }
        else
        {
            meshData.AddIndexTriangle(0, 3, 2);
            meshData.AddIndexTriangle(2, 1, 0);
        }
    }

    void VulkanMeshGeometry::CreateGrid(MeshData& meshData, 
                                        float width, 
                                        float height, 
                                        uint32 m, 
                                        uint32 n,
                                        bool flipV,
                                        bool rightHand)
    {
        uint32 vertexCount = m * n;
        uint32 faceCount = (m - 1) * (n - 1) * 2;

        //MeshVertex
        float halfW = 0.5f * width;
        float halfH = 0.5f * height;

        float dx = width / (n - 1);
        float dz = height / (m - 1);

        float du = 1.0f / (n - 1);
        float dv = 1.0f / (m - 1);

        meshData.vertices.resize(vertexCount);
        for (uint32 i = 0; i < m; ++i)
        {
            float y = halfH - i * dz;
            for (uint32 j = 0; j < n; ++j)
            {
                float x = -halfW + j * dx;

                MeshVertex vertex = MeshVertex(
                    x, y, 0.0f,
                    0.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 0.0f,
                    j * du, flipV ? (1.0f - i * dv) : (i * dv));
                meshData.vertices[i * n + j] = vertex;
                //meshData.aabb.Merge(vertex.pos);
            }
        }

        //Index
        meshData.indices32.resize(faceCount * 3);
        uint32 k = 0;
        for (uint32 i = 0; i < m - 1; ++i)
        {
            for (uint32 j = 0; j < n - 1; ++j)
            {
                if (rightHand)
                {
                    meshData.indices32[k + 0] = i * n + j;
                    meshData.indices32[k + 1] = (i + 1) * n + j;
                    meshData.indices32[k + 2] = (i + 1) * n + j + 1;
                    
                    meshData.indices32[k + 3] = (i + 1) * n + j + 1;
                    meshData.indices32[k + 4] = i * n + j + 1;
                    meshData.indices32[k + 5] = i * n + j;
                }
                else
                {
                    meshData.indices32[k + 0] = i * n + j;
                    meshData.indices32[k + 1] = i * n + j + 1;
                    meshData.indices32[k + 2] = (i + 1) * n + j + 1;
                    
                    meshData.indices32[k + 3] = (i + 1) * n + j + 1;
                    meshData.indices32[k + 4] = (i + 1) * n + j;
                    meshData.indices32[k + 5] = i * n + j;
                }

                k += 6;
            }
        }
    }

    void VulkanMeshGeometry::CreateCircle(MeshData& meshData,
                                          float radius,
                                          uint32 segment,
                                          bool flipV,
                                          bool rightHand)
    {
        uint32 vertexCount = segment + 1;
        uint32 faceCount = segment;

        //MeshVertex
        float thetaStep = 2.0f * VulkanMath::ms_fPI / segment;
        meshData.vertices.resize(vertexCount);
        meshData.vertices[0] = MeshVertex(
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 0.0f,
                    0.5f, 0.5f);
        //meshData.aabb.Merge(meshData.vertices[0].pos);
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            float ux = cos(thetaStep * i);
            float uy = sin(thetaStep * i);
            MeshVertex vertex = MeshVertex(
                    radius * ux,  radius * uy, 0.0f,
                    0.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 0.0f,
                    (1.0f + ux) / 2.0f,  flipV ? (1.0f - (1.0f - uy) / 2.0f) : (1.0f - uy) / 2.0f);
            meshData.vertices[index] = vertex;
            //meshData.aabb.Merge(vertex.pos);
            index++;
        }

        //Index
        meshData.indices32.resize(faceCount * 3);
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                if (rightHand)
                {
                    meshData.indices32[i*3 + 0] = 0;
                    meshData.indices32[i*3 + 1] = i + 1;
                    meshData.indices32[i*3 + 2] = i + 2;
                }
                else
                {
                    meshData.indices32[i*3 + 0] = 0;
                    meshData.indices32[i*3 + 1] = i + 2;
                    meshData.indices32[i*3 + 2] = i + 1;
                }
            }
            else
            {
                if (rightHand)
                {
                    meshData.indices32[i*3 + 0] = 0;
                    meshData.indices32[i*3 + 1] = i;
                    meshData.indices32[i*3 + 2] = 1;
                }
                else
                {
                    meshData.indices32[i*3 + 0] = 0;
                    meshData.indices32[i*3 + 1] = 1;
                    meshData.indices32[i*3 + 2] = i;
                }
            }
        }
    }

    void VulkanMeshGeometry::CreateAABB(MeshData& meshData, 
                                        float width, 
                                        float height, 
                                        float depth, 
                                        uint32 numSubdivisions,
                                        bool flipV,
                                        bool rightHand)
    {
        //MeshVertex
        MeshVertex v[24];
        float w2 = 0.5f * width;
        float h2 = 0.5f * height;
        float d2 = 0.5f * depth;
        //Front
        v[0]  = MeshVertex(-w2, +h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[1]  = MeshVertex(-w2, -h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[2]  = MeshVertex(+w2, -h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[3]  = MeshVertex(+w2, +h2, +d2,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Back
        v[4]  = MeshVertex(-w2, -h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[5]  = MeshVertex(-w2, +h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[6]  = MeshVertex(+w2, +h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[7]  = MeshVertex(+w2, -h2, -d2,   0.0f,  0.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Top
        v[8]  = MeshVertex(-w2, +h2, -d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[9]  = MeshVertex(-w2, +h2, +d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[10] = MeshVertex(+w2, +h2, +d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[11] = MeshVertex(+w2, +h2, -d2,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Bottom
        v[12] = MeshVertex(-w2, -h2, +d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[13] = MeshVertex(-w2, -h2, -d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[14] = MeshVertex(+w2, -h2, -d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[15] = MeshVertex(+w2, -h2, +d2,   0.0f, -1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Left
        v[16] = MeshVertex(-w2, +h2, -d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[17] = MeshVertex(-w2, -h2, -d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[18] = MeshVertex(-w2, -h2, +d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[19] = MeshVertex(-w2, +h2, +d2,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,   1.0f, flipV ? 1.0f : 0.0f);
        //Right
        v[20] = MeshVertex(+w2, +h2, +d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   0.0f, flipV ? 1.0f : 0.0f);
        v[21] = MeshVertex(+w2, -h2, +d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   0.0f, flipV ? 0.0f : 1.0f);
        v[22] = MeshVertex(+w2, -h2, -d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   1.0f, flipV ? 0.0f : 1.0f);
        v[23] = MeshVertex(+w2, +h2, -d2,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,   1.0f, flipV ? 1.0f : 0.0f);

        for (int i = 0; i < 24; i++)
        {
            meshData.AddVertex(v[i]);
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
        meshData.indices32.assign(&i[0], &i[36]);

        //Subdivisions.
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        for (uint32 i = 0; i < numSubdivisions; ++i)
            subdivide(meshData, rightHand);
    }

    void VulkanMeshGeometry::CreateSphere(MeshData& meshData, 
                                          float radius, 
                                          uint32 sliceCount, 
                                          uint32 stackCount,
                                          bool flipV,
                                          bool rightHand)
    {
        //MeshVertex
        MeshVertex topVertex(0.0f, +radius, 0.0f,  0.0f, +1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 1.0f : 0.0f);
        MeshVertex bottomVertex(0.0f, -radius, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 0.0f : 1.0f);
        meshData.AddVertex(topVertex);

        float phiStep = VulkanMath::ms_fPI / stackCount;
        float thetaStep = 2.0f * VulkanMath::ms_fPI / sliceCount;
        for (uint32 i = 1; i <= stackCount - 1; ++i)
        {
            float phi = i * phiStep;
            for (uint32 j = 0; j <= sliceCount; ++j)
            {
                float theta = j * thetaStep;

                MeshVertex v;
                //pos
                v.pos.x = radius * sinf(phi) * cosf(theta);
                v.pos.y = radius * cosf(phi);
                v.pos.z = radius * sinf(phi) * sinf(theta);
                //color
                v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                //normal
                v.normal = glm::normalize(v.pos);
                //tangent
                v.tangent.x = -radius * sinf(phi) * sinf(theta);
                v.tangent.y = 0.0f;
                v.tangent.z = +radius * sinf(phi) * cosf(theta);
                v.tangent = glm::normalize(v.tangent);
                //texCoord
                v.texCoord.x = theta / VulkanMath::ms_fPI_Two;
                v.texCoord.y = flipV ? (1.0f - phi / VulkanMath::ms_fPI) : (phi / VulkanMath::ms_fPI);

                meshData.AddVertex(v);
            }
        }
        meshData.AddVertex(bottomVertex);

        //Index Top
        for (uint32 i = 1; i <= sliceCount; ++i)
        {
            if (rightHand)
            {
                meshData.indices32.push_back(0);
                meshData.indices32.push_back(i);
                meshData.indices32.push_back(i + 1);
            }
            else
            {
                meshData.indices32.push_back(0);
                meshData.indices32.push_back(i + 1);
                meshData.indices32.push_back(i);
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
                    meshData.indices32.push_back(baseIndex + i * ringVertexCount + j);
                    meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);
                    meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);

                    meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
                    meshData.indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
                    meshData.indices32.push_back(baseIndex + i * ringVertexCount + j);
                }
                else
                {
                    meshData.indices32.push_back(baseIndex + i * ringVertexCount + j);
                    meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
                    meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);

                    meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
                    meshData.indices32.push_back(baseIndex + i * ringVertexCount + j);
                    meshData.indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
                }
                
            }
        }

        //Index Bottom
        uint32 southPoleIndex = (uint32)meshData.vertices.size() - 1;
        baseIndex = southPoleIndex - ringVertexCount;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                meshData.indices32.push_back(southPoleIndex);
                meshData.indices32.push_back(baseIndex + i + 1);
                meshData.indices32.push_back(baseIndex + i);
            }
            else
            {
                meshData.indices32.push_back(southPoleIndex);
                meshData.indices32.push_back(baseIndex + i);
                meshData.indices32.push_back(baseIndex + i + 1);
            }
        }
    }

    void VulkanMeshGeometry::CreateGeosphere(MeshData& meshData, 
                                             float radius, 
                                             uint32 numSubdivisions,
                                             bool flipV,
                                             bool rightHand)
    {
        //MeshVertex
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        const float X = 0.525731f;
        const float Z = 0.850651f;
        glm::vec3 pos[12] =
        {
            glm::vec3(   -X,  0.0f,     Z),  glm::vec3(    X,  0.0f,     Z),
            glm::vec3(   -X,  0.0f,    -Z),  glm::vec3(    X,  0.0f,    -Z),
            glm::vec3( 0.0f,     Z,     X),  glm::vec3( 0.0f,     Z,    -X),
            glm::vec3( 0.0f,    -Z,     X),  glm::vec3( 0.0f,    -Z,    -X),
            glm::vec3(    Z,     X,  0.0f),  glm::vec3(   -Z,     X,  0.0f),
            glm::vec3(    Z,    -X,  0.0f),  glm::vec3(   -Z,    -X,  0.0f)
        };

        uint32 k[60] =
        {
            1,  4, 0,   4, 9, 0,   4,  5, 9,   8, 5,  4,   1, 8, 4,
            1, 10, 8,  10, 3, 8,   8,  3, 5,   3, 2,  5,   3, 7, 2,
            3, 10, 7,  10, 6, 7,   6, 11, 7,   6, 0, 11,   6, 1, 0,
            10, 1, 6,  11, 0, 9,   2, 11, 9,   5, 2,  9,  11, 2, 7
        };

        meshData.vertices.resize(12);
        meshData.indices32.assign(&k[0], &k[60]);

        for (uint32 i = 0; i < 12; ++i)
            meshData.vertices[i].pos = pos[i];
        for (uint32 i = 0; i < numSubdivisions; ++i)
            subdivide(meshData, rightHand);

        for (uint32 i = 0; i < meshData.vertices.size(); ++i)
        {
            meshData.vertices[i].normal = glm::normalize(meshData.vertices[i].pos);
            meshData.vertices[i].pos = radius * meshData.vertices[i].normal;

            float theta = atan2f(meshData.vertices[i].pos.z, meshData.vertices[i].pos.x);
            if (theta < 0.0f)
                theta += VulkanMath::ms_fPI_Two;

            float phi = acosf(meshData.vertices[i].pos.y / radius);

            meshData.vertices[i].texCoord.x = theta / VulkanMath::ms_fPI_Two;
            meshData.vertices[i].texCoord.y = phi / VulkanMath::ms_fPI;

            meshData.vertices[i].tangent.x = -radius * sinf(phi) * sinf(theta);
            meshData.vertices[i].tangent.y = 0.0f;
            meshData.vertices[i].tangent.z = +radius * sinf(phi) * cosf(theta);
            meshData.vertices[i].tangent = glm::normalize(meshData.vertices[i].tangent);

            //meshData.aabb.Merge(meshData.vertices[i].pos);
        }
    }

    void VulkanMeshGeometry::CreateCylinder(MeshData& meshData, 
                                            float bottomRadius, 
                                            float topRadius, 
                                            float height, 
                                            uint32 sliceCount, 
                                            uint32 stackCount,
                                            bool flipV,
                                            bool rightHand)
    {
        //MeshVertex
        float stackHeight = height / stackCount;
        float radiusStep = (topRadius - bottomRadius) / stackCount;
        uint32 ringCount = stackCount + 1;
        for (uint32 i = 0; i < ringCount; ++i)
        {
            float y = -0.5f * height + i * stackHeight;
            float r = bottomRadius + i * radiusStep;

            float dTheta = 2.0f * VulkanMath::ms_fPI / sliceCount;
            for (uint32 j = 0; j <= sliceCount; ++j)
            {
                MeshVertex vertex;

                float c = cosf(j * dTheta);
                float s = sinf(j * dTheta);

                vertex.pos = glm::vec3(r * c, y, r * s);
                vertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

                vertex.texCoord.x = (float)j / sliceCount;
                vertex.texCoord.y = flipV ? ((float)i / stackCount) : (1.0f - (float)i / stackCount);

                // Cylinder can be parameterized as follows, where we introduce v
                // parameter that goes in the same direction as the v tex-coord
                // so that the bitangent goes in the same direction as the v tex-coord.
                //   Let r0 be the bottom radius and let r1 be the top radius.
                //   y(v) = h - hv for v in [0,1].
                //   r(v) = r1 + (r0-r1)v
                //
                //   x(t, v) = r(v)*cos(t)
                //   y(t, v) = h - hv
                //   z(t, v) = r(v)*sin(t)
                // 
                //  dx/dt = -r(v)*sin(t)
                //  dy/dt = 0
                //  dz/dt = +r(v)*cos(t)
                //
                //  dx/dv = (r0-r1)*cos(t)
                //  dy/dv = -h
                //  dz/dv = (r0-r1)*sin(t)

                // This is unit length.
                vertex.tangent = glm::vec3(-s, 0.0f, c);

                float dr = bottomRadius - topRadius;
                glm::vec3 bitangent(dr * c, -height, dr * s);

                glm::vec3 T = vertex.tangent;
                glm::vec3 B = bitangent;
                vertex.normal = glm::normalize(glm::cross(T, B));

                meshData.AddVertex(vertex);
            }
        }

        uint32 ringVertexCount = sliceCount + 1;

        for (uint32 i = 0; i < stackCount; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                if (rightHand)
                {
                    meshData.indices32.push_back(i * ringVertexCount + j);
                    meshData.indices32.push_back((i + 1) * ringVertexCount + j + 1);
                    meshData.indices32.push_back((i + 1) * ringVertexCount + j);

                    meshData.indices32.push_back(i * ringVertexCount + j);
                    meshData.indices32.push_back(i * ringVertexCount + j + 1);
                    meshData.indices32.push_back((i + 1) * ringVertexCount + j + 1);
                }
                else
                {
                    meshData.indices32.push_back(i * ringVertexCount + j);
                    meshData.indices32.push_back((i + 1) * ringVertexCount + j);
                    meshData.indices32.push_back((i + 1) * ringVertexCount + j + 1);

                    meshData.indices32.push_back(i * ringVertexCount + j);
                    meshData.indices32.push_back((i + 1) * ringVertexCount + j + 1);
                    meshData.indices32.push_back(i * ringVertexCount + j + 1);
                }
            }
        }

        buildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData, rightHand);
        buildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData, rightHand);
    }

    void VulkanMeshGeometry::CreateCapsule(MeshData& meshData,
                                           bool flipV,
                                           bool rightHand)
    {

    }

    void VulkanMeshGeometry::CreateCone(MeshData& meshData,
                                        bool flipV,
                                        bool rightHand)
    {

    }

    void VulkanMeshGeometry::CreateTorus(MeshData& meshData,
                                         bool flipV,
                                         bool rightHand)
    {

    }

    void VulkanMeshGeometry::CreateSkyBox(MeshData& meshData,
                                          bool flipV,
                                          bool rightHand)
    {

    }

    void VulkanMeshGeometry::CreateSkyDome(MeshData& meshData,
                                           bool flipV,
                                           bool rightHand)
    {

    }

    void VulkanMeshGeometry::CreateTerrain(MeshData& meshData,
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

        //MeshVertex
        float halfW = 0.5f * width;
        float halfH = 0.5f * height;

        float dx = width / (vertexX - 1);
        float dz = height / (vertexZ - 1);

        float du = 1.0f / (vertexX - 1);
        float dv = 1.0f / (vertexZ - 1);

        meshData.vertices.resize(vertexCount);
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
                MeshVertex vertex = MeshVertex(
                    x, height, z,
                    0.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 0.0f,
                    j * du, flipV ? (1.0f - i * dv) : (i * dv));
                meshData.vertices[i * vertexZ + j] = vertex;
            }
        }

        //Index
        meshData.indices32.resize(faceCount * 3);
        uint32 k = 0;
        for (uint32 i = 0; i < vertexZ - 1; ++i)
        {
            for (uint32 j = 0; j < vertexX - 1; ++j)
            {
                if (rightHand)
                {
                    meshData.indices32[k + 0] = i * vertexZ + j;
                    meshData.indices32[k + 1] = (i + 1) * vertexZ + j;
                    meshData.indices32[k + 2] = (i + 1) * vertexZ + j + 1;
                    
                    meshData.indices32[k + 3] = (i + 1) * vertexZ + j + 1;
                    meshData.indices32[k + 4] = i * vertexZ + j + 1;
                    meshData.indices32[k + 5] = i * vertexZ + j;
                }
                else
                {
                    meshData.indices32[k + 0] = i * vertexZ + j;
                    meshData.indices32[k + 1] = i * vertexZ + j + 1;
                    meshData.indices32[k + 2] = (i + 1) * vertexZ + j + 1;
                    
                    meshData.indices32[k + 3] = (i + 1) * vertexZ + j + 1;
                    meshData.indices32[k + 4] = (i + 1) * vertexZ + j;
                    meshData.indices32[k + 5] = i * vertexZ + j;
                }

                k += 6;
            }
        }
    }

    void VulkanMeshGeometry::subdivide(MeshData& meshData, bool rightHand)
    {
        MeshData inputCopy = meshData;

        meshData.vertices.resize(0);
        meshData.indices32.resize(0);

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

        uint32 numTri = (uint32)inputCopy.indices32.size() / 3;
        for (uint32 i = 0; i < numTri; ++i)
        {
            const MeshVertex& v0 = inputCopy.vertices[inputCopy.indices32[i * 3 + 0]];
            const MeshVertex& v1 = inputCopy.vertices[inputCopy.indices32[i * 3 + 1]];
            const MeshVertex& v2 = inputCopy.vertices[inputCopy.indices32[i * 3 + 2]];

            //mid
            MeshVertex m0,m1,m2;
            midPoint(v0, v1, m0);
            midPoint(v1, v2, m1);
            midPoint(v0, v2, m2);

            //new
            meshData.AddVertex(v0); // 0
            meshData.AddVertex(v1); // 1
            meshData.AddVertex(v2); // 2
            meshData.AddVertex(m0); // 3
            meshData.AddVertex(m1); // 4
            meshData.AddVertex(m2); // 5

            if (rightHand)
            {
                meshData.indices32.push_back(i * 6 + 0);
                meshData.indices32.push_back(i * 6 + 3);
                meshData.indices32.push_back(i * 6 + 5);

                meshData.indices32.push_back(i * 6 + 3);
                meshData.indices32.push_back(i * 6 + 4);
                meshData.indices32.push_back(i * 6 + 5);

                meshData.indices32.push_back(i * 6 + 5);
                meshData.indices32.push_back(i * 6 + 4);
                meshData.indices32.push_back(i * 6 + 2);

                meshData.indices32.push_back(i * 6 + 3);
                meshData.indices32.push_back(i * 6 + 1);
                meshData.indices32.push_back(i * 6 + 4);
            }
            else
            {
                meshData.indices32.push_back(i * 6 + 0);
                meshData.indices32.push_back(i * 6 + 5);
                meshData.indices32.push_back(i * 6 + 3);

                meshData.indices32.push_back(i * 6 + 3);
                meshData.indices32.push_back(i * 6 + 5);
                meshData.indices32.push_back(i * 6 + 4);

                meshData.indices32.push_back(i * 6 + 5);
                meshData.indices32.push_back(i * 6 + 2);
                meshData.indices32.push_back(i * 6 + 4);

                meshData.indices32.push_back(i * 6 + 3);
                meshData.indices32.push_back(i * 6 + 4);
                meshData.indices32.push_back(i * 6 + 1);
            }
        }
    }

    void VulkanMeshGeometry::midPoint(const MeshVertex& v0, const MeshVertex& v1, MeshVertex& m)
    {
        m.pos = 0.5f * (v0.pos + v1.pos);
        m.color = 0.5f * (v0.color + v1.color);
        m.normal = glm::normalize(0.5f * (v0.normal + v1.normal));
        m.tangent = glm::normalize(0.5f * (v0.tangent + v1.tangent));
        m.texCoord = 0.5f * (v0.texCoord + v1.texCoord);
    }

    void VulkanMeshGeometry::buildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData, bool rightHand)
    {
        uint32 baseIndex = (uint32)meshData.vertices.size();

        float y = 0.5f * height;
        float dTheta = 2.0f * VulkanMath::ms_fPI / sliceCount;

        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = topRadius * cosf(i * dTheta);
            float z = topRadius * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = z / height + 0.5f;

            meshData.AddVertex(MeshVertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
        }
        meshData.AddVertex(MeshVertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

        uint32 centerIndex = (uint32)meshData.vertices.size() - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                meshData.indices32.push_back(centerIndex);
                meshData.indices32.push_back(baseIndex + i);
                meshData.indices32.push_back(baseIndex + i + 1);
            }
            else
            {
                meshData.indices32.push_back(centerIndex);
                meshData.indices32.push_back(baseIndex + i + 1);
                meshData.indices32.push_back(baseIndex + i);
            }
        }
    }

    void VulkanMeshGeometry::buildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData, bool rightHand)
    {
        uint32 baseIndex = (uint32)meshData.vertices.size();
        float y = -0.5f * height;

        float dTheta = 2.0f * VulkanMath::ms_fPI / sliceCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = bottomRadius * cosf(i * dTheta);
            float z = bottomRadius * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = z / height + 0.5f;

            meshData.AddVertex(MeshVertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
        }
        meshData.AddVertex(MeshVertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

        uint32 centerIndex = (uint32)meshData.vertices.size() - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                meshData.indices32.push_back(centerIndex);
                meshData.indices32.push_back(baseIndex + i + 1);
                meshData.indices32.push_back(baseIndex + i);
            }
            else
            {
                meshData.indices32.push_back(centerIndex);
                meshData.indices32.push_back(baseIndex + i);
                meshData.indices32.push_back(baseIndex + i + 1);
            }
        }
    }

}; //LostPeter