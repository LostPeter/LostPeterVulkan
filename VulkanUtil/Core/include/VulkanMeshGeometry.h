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

#ifndef _VULKAN_GEOMETRY_H_
#define _VULKAN_GEOMETRY_H_

#include "PreDefine.h"

namespace LostPeter
{
	///////////////////////////////////////// VulkanMeshCreateParam ///////////////////////////////////////////
    class utilExport VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam(bool _flipV, bool _rightHand);
        virtual ~VulkanMeshCreateParam();

    public:
        bool flipV;
        bool rightHand;
        
    public:
        virtual String ToName() = 0;
    };

    //Triangle
    class utilExport VulkanMeshCreateParam_Triangle : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Triangle();
        VulkanMeshCreateParam_Triangle(bool _flipV, bool _rightHand);
        virtual ~VulkanMeshCreateParam_Triangle();

    public:
        static String ms_nameType;
    public:

    public:
        virtual String ToName();
    };

    //Quad
    class utilExport VulkanMeshCreateParam_Quad : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Quad();
        VulkanMeshCreateParam_Quad(float _centerX,
                                   float _centerY,
                                   float _width,
                                   float _height,
                                   float _depth,
                                   bool _flipV,
                                   bool _rightHand);
        virtual ~VulkanMeshCreateParam_Quad();

    public:
        static String ms_nameType;
    public:
        float centerX;
        float centerY;
        float width;
        float height;
        float depth;

    public:
        virtual String ToName();
    };

    //Grid
    class utilExport VulkanMeshCreateParam_Grid : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Grid();
        VulkanMeshCreateParam_Grid(float _width,
                                   float _height,
                                   uint32 _m,
                                   uint32 _n,
                                   bool _flipV,
                                   bool _rightHand);
        virtual ~VulkanMeshCreateParam_Grid();

    public:
        static String ms_nameType;
    public:
        float width;
        float height;
        uint32 m;
        uint32 n;

    public:
        virtual String ToName();
    };

    //Circle
    class utilExport VulkanMeshCreateParam_Circle : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Circle();
        VulkanMeshCreateParam_Circle(float _radius,
                                     uint32 _segment,
                                     bool _flipV,
                                     bool _rightHand);
        virtual ~VulkanMeshCreateParam_Circle();

    public:
        static String ms_nameType;
    public:
        float radius;
        uint32 segment;

    public:
        virtual String ToName();
    };

    //AABB
    class utilExport VulkanMeshCreateParam_AABB : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_AABB();
        VulkanMeshCreateParam_AABB(float _width,
                                   float _height,
                                   float _depth,
                                   uint32 _numSubdivisions,
                                   bool _flipV,
                                   bool _rightHand);
        virtual ~VulkanMeshCreateParam_AABB();

    public:
        static String ms_nameType;
    public:
        float width;
        float height;
        float depth;
        uint32 numSubdivisions;

    public:
        virtual String ToName();
    };

    //Sphere
    class utilExport VulkanMeshCreateParam_Sphere : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Sphere();
        VulkanMeshCreateParam_Sphere(float _radius,
                                     uint32 _sliceCount,
                                     uint32 _stackCount,
                                     bool _flipV,
                                     bool _rightHand);
        virtual ~VulkanMeshCreateParam_Sphere();

    public:
        static String ms_nameType;
    public:
        float radius;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName();
    };

    //GeoSphere
    class utilExport VulkanMeshCreateParam_GeoSphere : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_GeoSphere();
        VulkanMeshCreateParam_GeoSphere(float _radius,
                                        uint32 _numSubdivisions,
                                        bool _flipV,
                                        bool _rightHand);
        virtual ~VulkanMeshCreateParam_GeoSphere();

    public:
        static String ms_nameType;
    public:
        float radius;
        uint32 numSubdivisions;

    public:
        virtual String ToName();
    };

    //Cylinder
    class utilExport VulkanMeshCreateParam_Cylinder : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Cylinder();
        VulkanMeshCreateParam_Cylinder(float _bottomRadius,
                                       float _topRadius,
                                       float _height,
                                       uint32 _sliceCount,
                                       uint32 _stackCount,
                                       bool _flipV,
                                       bool _rightHand);
        virtual ~VulkanMeshCreateParam_Cylinder();

    public:
        static String ms_nameType;
    public:
        float bottomRadius;
        float topRadius;
        float height;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName();
    };

    //Capsule
    class utilExport VulkanMeshCreateParam_Capsule : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Capsule();
        virtual ~VulkanMeshCreateParam_Capsule();

    public:
        static String ms_nameType;
    public:
        

    public:
        virtual String ToName();
    };

    //Cone
    class utilExport VulkanMeshCreateParam_Cone : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Cone();
        virtual ~VulkanMeshCreateParam_Cone();

    public:
        static String ms_nameType;
    public:
        

    public:
        virtual String ToName();
    };

    //Torus
    class utilExport VulkanMeshCreateParam_Torus : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Torus();
        virtual ~VulkanMeshCreateParam_Torus();

    public:
        static String ms_nameType;
    public:
        

    public:
        virtual String ToName();
    };

    //SkyBox
    class utilExport VulkanMeshCreateParam_SkyBox : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_SkyBox();
        virtual ~VulkanMeshCreateParam_SkyBox();

    public:
        static String ms_nameType;
    public:
        

    public:
        virtual String ToName();
    };

    //SkyDome
    class utilExport VulkanMeshCreateParam_SkyDome : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_SkyDome();
        virtual ~VulkanMeshCreateParam_SkyDome();

    public:
        static String ms_nameType;
    public:
        

    public:
        virtual String ToName();
    };

    //Terrain
    class utilExport VulkanMeshCreateParam_Terrain : public VulkanMeshCreateParam
    {
    public:
        VulkanMeshCreateParam_Terrain();
        VulkanMeshCreateParam_Terrain(float _offsetX,
                                      float _offsetZ,
                                      float _width,
                                      float _height,
                                      uint32 _vertexX,
                                      uint32 _vertexZ,
                                      bool _flipV,
                                      bool _rightHand);    
        virtual ~VulkanMeshCreateParam_Terrain();

    public:
        static String ms_nameType;
    public:
        float offsetX;
        float offsetZ;
        float width;
        float height;
        uint32 vertexX;
        uint32 vertexZ;

    public:
        virtual String ToName();
    };

    ///////////////////////////////////////// VulkanMeshGeometry //////////////////////////////////////////////
    class utilExport VulkanMeshGeometry
	{
	public:
        static bool CreateGeometry(MeshData& meshData, VulkanMeshGeometryType eMeshGeometry);
		static bool CreateGeometry(MeshData& meshData, VulkanMeshGeometryType eMeshGeometry, VulkanMeshCreateParam* pParam);

	 //Triangle
        static void CreateTriangle(MeshData& meshData, VulkanMeshCreateParam_Triangle* pParam)
        {
            CreateTriangle(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateTriangle(MeshData& meshData, 
                                   bool flipV,
                                   bool rightHand);

        //Quad
        static void CreateQuad(MeshData& meshData, VulkanMeshCreateParam_Quad* pParam)
        {
            CreateQuad(meshData, pParam->centerX, pParam->centerY, pParam->width, pParam->height, pParam->depth,  pParam->flipV, pParam->rightHand);
        }
        static void CreateQuad(MeshData& meshData,
                               float centerX,
                               float centerY,
                               float width,
                               float height,
                               float depth,
                               bool flipV,
                               bool rightHand);

        //Grid
        static void CreateGrid(MeshData& meshData, VulkanMeshCreateParam_Grid* pParam)
        {
            CreateGrid(meshData, pParam->width, pParam->height, pParam->m, pParam->n, pParam->flipV, pParam->rightHand);
        }
        static void CreateGrid(MeshData& meshData,
                               float width,
                               float height,
                               uint32 m,
                               uint32 n,
                               bool flipV,
                               bool rightHand);

        //Circle
        static void CreateCircle(MeshData& meshData, VulkanMeshCreateParam_Circle* pParam)
        {
            CreateCircle(meshData, pParam->radius, pParam->segment, pParam->flipV, pParam->rightHand);
        }
        static void CreateCircle(MeshData& meshData,
                                 float radius,
                                 uint32 segment,
                                 bool flipV,
                                 bool rightHand);

        //AABB
        static void CreateAABB(MeshData& meshData, VulkanMeshCreateParam_AABB* pParam)
        {
            CreateAABB(meshData, pParam->width, pParam->height, pParam->depth, pParam->numSubdivisions, pParam->flipV, pParam->rightHand);
        }
        static void CreateAABB(MeshData& meshData,
                               float width,
                               float height,
                               float depth,
                               uint32 numSubdivisions,
                               bool flipV,
                               bool rightHand);

        //Sphere
        static void CreateSphere(MeshData& meshData, VulkanMeshCreateParam_Sphere* pParam)
        {
            CreateSphere(meshData, pParam->radius, pParam->sliceCount, pParam->stackCount, pParam->flipV, pParam->rightHand);
        }
        static void CreateSphere(MeshData& meshData,
                                 float radius,
                                 uint32 sliceCount,
                                 uint32 stackCount,
                                 bool flipV,
                                 bool rightHand);

        //GeoSphere
        static void CreateGeosphere(MeshData& meshData, VulkanMeshCreateParam_GeoSphere* pParam)
        {
            CreateGeosphere(meshData, pParam->radius, pParam->numSubdivisions, pParam->flipV, pParam->rightHand);
        }
        static void CreateGeosphere(MeshData& meshData,
                                    float radius,
                                    uint32 numSubdivisions,
                                    bool flipV,
                                    bool rightHand);

        //Cylinder
        static void CreateCylinder(MeshData& meshData, VulkanMeshCreateParam_Cylinder* pParam)
        {
            CreateCylinder(meshData, pParam->bottomRadius, pParam->topRadius, pParam->height, pParam->sliceCount, pParam->stackCount, pParam->flipV, pParam->rightHand);
        }
        static void CreateCylinder(MeshData& meshData,
                                   float bottomRadius,
                                   float topRadius,
                                   float height,
                                   uint32 sliceCount,
                                   uint32 stackCount,
                                   bool flipV,
                                   bool rightHand);

        //Capsule
        static void CreateCapsule(MeshData& meshData, VulkanMeshCreateParam_Capsule* pParam)
        {
            CreateCapsule(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateCapsule(MeshData& meshData,
                                  bool flipV,
                                  bool rightHand);

        //Cone
        static void CreateCone(MeshData& meshData, VulkanMeshCreateParam_Cone* pParam)
        {
            CreateCone(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateCone(MeshData& meshData,
                               bool flipV,
                               bool rightHand);

        //Torus
        static void CreateTorus(MeshData& meshData, VulkanMeshCreateParam_Torus* pParam)
        {
            CreateTorus(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateTorus(MeshData& meshData,
                                bool flipV,
                                bool rightHand);

        //SkyBox
        static void CreateSkyBox(MeshData& meshData, VulkanMeshCreateParam_SkyBox* pParam)
        {
            CreateSkyBox(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateSkyBox(MeshData& meshData,
                                 bool flipV,
                                 bool rightHand);

        //SkyDome
        static void CreateSkyDome(MeshData& meshData, VulkanMeshCreateParam_SkyDome* pParam)
        {
            CreateSkyDome(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateSkyDome(MeshData& meshData,
                                  bool flipV,
                                  bool rightHand);

        //Terrain
        static void CreateTerrain(MeshData& meshData, VulkanMeshCreateParam_Terrain* pParam)
        {
            CreateTerrain(meshData, pParam->offsetX, pParam->offsetZ, pParam->width, pParam->height, pParam->vertexX, pParam->vertexZ, pParam->flipV, pParam->rightHand);
        }
        static void CreateTerrain(MeshData& meshData,
                                  float offsetX,
                                  float offsetZ,
                                  float width,
                                  float height,
                                  uint32 vertexX,
                                  uint32 vertexZ,
                                  bool flipV,
                                  bool rightHand);

    private:
        static void subdivide(MeshData& meshData, bool rightHand);
        static void midPoint(const MeshVertex& v0, const MeshVertex& v1, MeshVertex& m);
        static void buildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData, bool rightHand);
        static void buildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData, bool rightHand);
	};

}; //LostPeter

#endif