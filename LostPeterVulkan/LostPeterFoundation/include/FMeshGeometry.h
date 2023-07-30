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

#ifndef _F_MESH_GEOMETRY_H_
#define _F_MESH_GEOMETRY_H_

#include "FPreDefine.h"
#include "FMeshData.h"

namespace LostPeterFoundation
{
    ///////////////////////////////////////// FMeshCreateParam ///////////////////////////////////////////
    class LPF_Export FMeshCreateParam
    {
    public:
        FMeshCreateParam(bool _flipV, bool _rightHand);
        virtual ~FMeshCreateParam();

    public:
        bool flipV;
        bool rightHand;
        
    public:
        virtual String ToName() = 0;
    };

    //Line
    class LPF_Export FMeshCreateParam_Line : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Line();
        virtual ~FMeshCreateParam_Line();

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName();
    };

    //LineQuad
    class LPF_Export FMeshCreateParam_LineQuad : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineQuad();
        FMeshCreateParam_LineQuad(float _centerX,
                                  float _centerY,
                                  float _width,
                                  float _height);
        virtual ~FMeshCreateParam_LineQuad();

    public:
        static String ms_nameType;

    public:
        float centerX;
        float centerY;
        float width;
        float height;

    public:
        virtual String ToName();
    };

    //LineAABB
    class LPF_Export FMeshCreateParam_LineAABB : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineAABB();
        FMeshCreateParam_LineAABB(float _width,
                                  float _height,
                                  float _depth);
        virtual ~FMeshCreateParam_LineAABB();

    public:
        static String ms_nameType;

    public:
        float width;
        float height;
        float depth;

    public:
        virtual String ToName();
    };

    //LineSphere
    class LPF_Export FMeshCreateParam_LineSphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineSphere();
        FMeshCreateParam_LineSphere(float _radius,
                                    uint32 _sliceCount,
                                    uint32 _stackCount);
        virtual ~FMeshCreateParam_LineSphere();

    public:
        static String ms_nameType;

    public:
        float radius;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName();
    };

    //Triangle
    class LPF_Export FMeshCreateParam_Triangle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Triangle();
        FMeshCreateParam_Triangle(bool _flipV, bool _rightHand);
        virtual ~FMeshCreateParam_Triangle();

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName();
    };

    //Quad
    class LPF_Export FMeshCreateParam_Quad : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Quad();
        FMeshCreateParam_Quad(float _centerX,
                              float _centerY,
                              float _width,
                              float _height,
                              float _depth,
                              bool _flipV,
                              bool _rightHand);
        virtual ~FMeshCreateParam_Quad();

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
    class LPF_Export FMeshCreateParam_Grid : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Grid();
        FMeshCreateParam_Grid(float _width,
                              float _height,
                              uint32 _m,
                              uint32 _n,
                              bool _flipV,
                              bool _rightHand);
        virtual ~FMeshCreateParam_Grid();

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
    class LPF_Export FMeshCreateParam_Circle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Circle();
        FMeshCreateParam_Circle(float _radius,
                                uint32 _segment,
                                bool _flipV,
                                bool _rightHand);
        virtual ~FMeshCreateParam_Circle();

    public:
        static String ms_nameType;

    public:
        float radius;
        uint32 segment;

    public:
        virtual String ToName();
    };

    //AABB
    class LPF_Export FMeshCreateParam_AABB : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_AABB();
        FMeshCreateParam_AABB(float _width,
                              float _height,
                              float _depth,
                              uint32 _numSubdivisions,
                              bool _flipV,
                              bool _rightHand);
    virtual ~FMeshCreateParam_AABB();

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
    class LPF_Export FMeshCreateParam_Sphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Sphere();
        FMeshCreateParam_Sphere(float _radius,
                                uint32 _sliceCount,
                                uint32 _stackCount,
                                bool _flipV,
                                bool _rightHand);
        virtual ~FMeshCreateParam_Sphere();

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
    class LPF_Export FMeshCreateParam_GeoSphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_GeoSphere();
        FMeshCreateParam_GeoSphere(float _radius,
                                   uint32 _numSubdivisions,
                                   bool _flipV,
                                   bool _rightHand);
        virtual ~FMeshCreateParam_GeoSphere();

    public:
        static String ms_nameType;

    public:
        float radius;
        uint32 numSubdivisions;

    public:
        virtual String ToName();
    };

    //Cylinder
    class LPF_Export FMeshCreateParam_Cylinder : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Cylinder();
        FMeshCreateParam_Cylinder(float _bottomRadius,
                                  float _topRadius,
                                  float _height,
                                  float _heightOffset,
                                  uint32 _sliceCount,
                                  uint32 _stackCount,
                                  bool _flipV,
                                  bool _rightHand);
        virtual ~FMeshCreateParam_Cylinder();

    public:
        static String ms_nameType;

    public:
        float bottomRadius;
        float topRadius;
        float height;
        float heightOffset;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName();
    };

    //Capsule
    class LPF_Export FMeshCreateParam_Capsule : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Capsule();
        FMeshCreateParam_Capsule(float _radius,
                                 float _height,
                                 float _heightOffset,
                                 uint32 _numRings,
                                 uint32 _numSegments,
                                 uint32 _numSegHeight,
                                 bool _flipV,
                                 bool _rightHand);
        virtual ~FMeshCreateParam_Capsule();

    public:
        static String ms_nameType;

    public:
        float radius;
        float height;
        float heightOffset;

        uint32 numRings;
        uint32 numSegments;
        uint32 numSegHeight;

    public:
        virtual String ToName();
    };

    //Cone
    class LPF_Export FMeshCreateParam_Cone : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Cone();
        FMeshCreateParam_Cone(float _radius,
                              float _height,
                              float _heightOffset,
                              uint32 _numSegBase,
                              uint32 _numSegHeight,
                              bool _flipV,
                              bool _rightHand);
        virtual ~FMeshCreateParam_Cone();

    public:
        static String ms_nameType;

    public:
        float radius;
        float height;
        float heightOffset;

        uint32 numSegBase;
	    uint32 numSegHeight;

    public:
        virtual String ToName();
    };

    //Torus
    class LPF_Export FMeshCreateParam_Torus : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Torus();
        FMeshCreateParam_Torus(float _radius,
                               float _sectionRadius,
                               uint32 _numSegSection,
                               uint32 _numSegCircle,
                               bool _flipV,
                               bool _rightHand);
        virtual ~FMeshCreateParam_Torus();

    public:
        static String ms_nameType;

    public:
        float radius;
        float sectionRadius;

        uint32 numSegSection;
        uint32 numSegCircle;

    public:
        virtual String ToName();
    };

    //SkyBox
    class LPF_Export FMeshCreateParam_SkyBox : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_SkyBox();
        virtual ~FMeshCreateParam_SkyBox();

    public:
        static String ms_nameType;

    public:
        

    public:
        virtual String ToName();
    };

    //SkyDome
    class LPF_Export FMeshCreateParam_SkyDome : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_SkyDome();
        virtual ~FMeshCreateParam_SkyDome();

    public:
        static String ms_nameType;

    public:
        

    public:
        virtual String ToName();
    };

    //Terrain
    class LPF_Export FMeshCreateParam_Terrain : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Terrain();
        FMeshCreateParam_Terrain(float _offsetX,
                                 float _offsetZ,
                                 float _width,
                                 float _height,
                                 uint32 _vertexX,
                                 uint32 _vertexZ,
                                 float* _pHeight,
                                 uint32 _heightDataGap,
                                 bool _flipV,
                                 bool _rightHand);    
        virtual ~FMeshCreateParam_Terrain();

    public:
        static String ms_nameType;

    public:
        float offsetX;
        float offsetZ;
        float width;
        float height;
        uint32 vertexX;
        uint32 vertexZ;
        float* pHeight;
        uint32 heightDataGap;

    public:
        virtual String ToName();
    };

    ///////////////////////////////////////// FMeshGeometry //////////////////////////////////////////////
    class LPF_Export FMeshGeometry
	{
	public:
        static bool CreateGeometry(FMeshData& meshData, FMeshGeometryType eMeshGeometry);
		static bool CreateGeometry(FMeshData& meshData, FMeshGeometryType eMeshGeometry, FMeshCreateParam* pParam);

    ////////////////////////////////// Line //////////////////////////////////
        //Line
        static void CreateLine(FMeshData& meshData, FMeshCreateParam_Line* pParam)
        {
            CreateLine(meshData);
        }
        static void CreateLine(FMeshData& meshData);

        //LineQuad
        static void CreateLineQuad(FMeshData& meshData, FMeshCreateParam_LineQuad* pParam)
        {
            CreateLineQuad(meshData,
                           pParam->centerX,
                           pParam->centerY,
                           pParam->width,
                           pParam->height);
        }
        static void CreateLineQuad(FMeshData& meshData,
                                   float centerX,
                                   float centerY,
                                   float width,
                                   float height);

        //LineAABB
        static void CreateLineAABB(FMeshData& meshData, FMeshCreateParam_LineAABB* pParam)
        {
            CreateLineAABB(meshData, 
                           pParam->width, 
                           pParam->height, 
                           pParam->depth);
        }
        static void CreateLineAABB(FMeshData& meshData,
                                   float width,
                                   float height,
                                   float depth);

        //LineSphere
        static void CreateLineSphere(FMeshData& meshData, FMeshCreateParam_LineSphere* pParam)
        {
            CreateLineSphere(meshData, 
                             pParam->radius, 
                             pParam->sliceCount, 
                             pParam->stackCount);
        }
        static void CreateLineSphere(FMeshData& meshData,
                                     float radius,
                                     uint32 sliceCount,
                                     uint32 stackCount);


    ////////////////////////////////// Flat //////////////////////////////////



    ////////////////////////////////// Entity ////////////////////////////////
	    //Triangle
        static void CreateTriangle(FMeshData& meshData, FMeshCreateParam_Triangle* pParam)
        {
            CreateTriangle(meshData, 
                           pParam->flipV, 
                           pParam->rightHand);
        }
        static void CreateTriangle(FMeshData& meshData, 
                                   bool flipV,
                                   bool rightHand);

        //Quad
        static void CreateQuad(FMeshData& meshData, FMeshCreateParam_Quad* pParam)
        {
            CreateQuad(meshData, 
                       pParam->centerX, 
                       pParam->centerY, 
                       pParam->width, 
                       pParam->height, 
                       pParam->depth,  
                       pParam->flipV, 
                       pParam->rightHand);
        }
        static void CreateQuad(FMeshData& meshData,
                               float centerX,
                               float centerY,
                               float width,
                               float height,
                               float depth,
                               bool flipV,
                               bool rightHand);

        //Grid
        static void CreateGrid(FMeshData& meshData, FMeshCreateParam_Grid* pParam)
        {
            CreateGrid(meshData, 
                       pParam->width, 
                       pParam->height, 
                       pParam->m, 
                       pParam->n, 
                       pParam->flipV, 
                       pParam->rightHand);
        }
        static void CreateGrid(FMeshData& meshData,
                               float width,
                               float height,
                               uint32 m,
                               uint32 n,
                               bool flipV,
                               bool rightHand);

        //Circle
        static void CreateCircle(FMeshData& meshData, FMeshCreateParam_Circle* pParam)
        {
            CreateCircle(meshData, 
                         pParam->radius, 
                         pParam->segment, 
                         pParam->flipV, 
                         pParam->rightHand);
        }
        static void CreateCircle(FMeshData& meshData,
                                 float radius,
                                 uint32 segment,
                                 bool flipV,
                                 bool rightHand);

        //AABB
        static void CreateAABB(FMeshData& meshData, FMeshCreateParam_AABB* pParam)
        {
            CreateAABB(meshData, 
                       pParam->width, 
                       pParam->height, 
                       pParam->depth, 
                       pParam->numSubdivisions, 
                       pParam->flipV, 
                       pParam->rightHand);
        }
        static void CreateAABB(FMeshData& meshData,
                               float width,
                               float height,
                               float depth,
                               uint32 numSubdivisions,
                               bool flipV,
                               bool rightHand);

        //Sphere
        static void CreateSphere(FMeshData& meshData, FMeshCreateParam_Sphere* pParam)
        {
            CreateSphere(meshData, 
                         pParam->radius, 
                         pParam->sliceCount, 
                         pParam->stackCount, 
                         pParam->flipV, 
                         pParam->rightHand);
        }
        static void CreateSphere(FMeshData& meshData,
                                 float radius,
                                 uint32 sliceCount,
                                 uint32 stackCount,
                                 bool flipV,
                                 bool rightHand);

        //GeoSphere
        static void CreateGeoSphere(FMeshData& meshData, FMeshCreateParam_GeoSphere* pParam)
        {
            CreateGeoSphere(meshData, 
                            pParam->radius,
                            pParam->numSubdivisions, 
                            pParam->flipV, 
                            pParam->rightHand);
        }
        static void CreateGeoSphere(FMeshData& meshData,
                                    float radius,
                                    uint32 numSubdivisions,
                                    bool flipV,
                                    bool rightHand);

        //Cylinder
        static void CreateCylinder(FMeshData& meshData, FMeshCreateParam_Cylinder* pParam)
        {
            CreateCylinder(meshData, 
                           pParam->bottomRadius, 
                           pParam->topRadius, 
                           pParam->height, 
                           pParam->heightOffset,
                           pParam->sliceCount, 
                           pParam->stackCount, 
                           pParam->flipV, 
                           pParam->rightHand);
        }
        static void CreateCylinder(FMeshData& meshData,
                                   float bottomRadius,
                                   float topRadius,
                                   float height,
                                   float heightOffset,
                                   uint32 sliceCount,
                                   uint32 stackCount,
                                   bool flipV,
                                   bool rightHand);

        //Capsule
        static void CreateCapsule(FMeshData& meshData, FMeshCreateParam_Capsule* pParam)
        {
            CreateCapsule(meshData,
                          pParam->radius, 
                          pParam->height, 
                          pParam->heightOffset,
                          pParam->numRings, 
                          pParam->numSegments, 
                          pParam->numSegHeight, 
                          pParam->flipV, 
                          pParam->rightHand);
        }
        static void CreateCapsule(FMeshData& meshData,
                                  float radius,
                                  float height,
                                  float heightOffset,
                                  uint32 numRings,
                                  uint32 numSegments,
                                  uint32 numSegHeight,
                                  bool flipV,
                                  bool rightHand);

        //Cone
        static void CreateCone(FMeshData& meshData, FMeshCreateParam_Cone* pParam)
        {
            CreateCone(meshData, 
                       pParam->radius, 
                       pParam->height, 
                       pParam->heightOffset,
                       pParam->numSegBase, 
                       pParam->numSegHeight, 
                       pParam->flipV, 
                       pParam->rightHand);
        }
        static void CreateCone(FMeshData& meshData,
                               float radius,
                               float height,
                               float heightOffset,
                               uint32 numSegBase,
                               uint32 numSegHeight,
                               bool flipV,
                               bool rightHand);

        //Torus
        static void CreateTorus(FMeshData& meshData, FMeshCreateParam_Torus* pParam)
        {
            CreateTorus(meshData, 
                        pParam->radius, 
                        pParam->sectionRadius, 
                        pParam->numSegSection, 
                        pParam->numSegCircle, 
                        pParam->flipV, 
                        pParam->rightHand);
        }
        static void CreateTorus(FMeshData& meshData,
                                float radius,
                                float sectionRadius,
                                uint32 numSegSection,
                                uint32 numSegCircle,
                                bool flipV,
                                bool rightHand);

        //SkyBox
        static void CreateSkyBox(FMeshData& meshData, FMeshCreateParam_SkyBox* pParam)
        {
            CreateSkyBox(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateSkyBox(FMeshData& meshData,
                                 bool flipV,
                                 bool rightHand);

        //SkyDome
        static void CreateSkyDome(FMeshData& meshData, FMeshCreateParam_SkyDome* pParam)
        {
            CreateSkyDome(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateSkyDome(FMeshData& meshData,
                                  bool flipV,
                                  bool rightHand);

        //Terrain
        static void CreateTerrain(FMeshData& meshData, FMeshCreateParam_Terrain* pParam)
        {
            CreateTerrain(meshData, 
                          pParam->offsetX, 
                          pParam->offsetZ, 
                          pParam->width, 
                          pParam->height, 
                          pParam->vertexX, 
                          pParam->vertexZ, 
                          pParam->pHeight, 
                          pParam->heightDataGap, 
                          pParam->flipV, 
                          pParam->rightHand);
        }
        static void CreateTerrain(FMeshData& meshData,
                                  float offsetX,
                                  float offsetZ,
                                  float width,
                                  float height,
                                  uint32 vertexX,
                                  uint32 vertexZ,
                                  float* pHeight,
                                  uint32 heightDataGap,
                                  bool flipV,
                                  bool rightHand);

    public:
        static void MidPoint(const FMeshVertex& v0, const FMeshVertex& v1, FMeshVertex& m);
        static void SubDivide(FMeshData& meshData, bool rightHand);

    public:
        static uint32 GetVertexCount(FMeshData& meshData);
        static void ReserveVertexCount(FMeshData& meshData, uint32 count);
        static void ResizeVertexCount(FMeshData& meshData, uint32 count);
        static FMeshVertex& GetVertex(FMeshData& meshData, uint32 index);
        static uint32 AddVertex(FMeshData& meshData, const FMeshVertex& vertex);
        static uint32 AddVertex(FMeshData& meshData, 
                                const FVector3& vPos,
                                const FVector4& color, 
                                const FVector3& normal, 
                                const FVector3& tangent, 
                                const FVector2& texCoord);
        static void SetVertex(FMeshData& meshData, int index, const FMeshVertex& vertex);

        static uint32 GetIndexCount(FMeshData& meshData);
        static void ReserveIndexCount(FMeshData& meshData, uint32 count);
        static void ResizeIndexCount(FMeshData& meshData, uint32 count);
        static uint32 GetIndex(FMeshData& meshData, uint32 index);
        static uint32 AddIndex(FMeshData& meshData, uint32 value);
        static void AddIndices(FMeshData& meshData, uint32 count, uint32* pIndex);
        static void SetIndex(FMeshData& meshData, uint32 index, uint32 value);
        static void AddIndexLine(FMeshData& meshData, uint32 index1, uint32 index2);
        static void AddIndexTriangle(FMeshData& meshData, uint32 index1, uint32 index2, uint32 index3);
        static void SetIndexTriangle(FMeshData& meshData, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3);
	};

}; //LostPeterFoundation

#endif