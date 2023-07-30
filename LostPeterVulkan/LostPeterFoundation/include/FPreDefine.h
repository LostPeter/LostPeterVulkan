/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_PRE_DEFINE_H_
#define _F_PRE_DEFINE_H_

#include "FPreIncludeStd.h"
#include "FPreIncludeThird.h"
#include "FTPoint.h"
#include "FTSize.h"
#include "FTRect.h"
#include "FTLine.h"

namespace LostPeterFoundation
{
    ////////////////////////////// Define //////////////////////////////
    #define F_C_PI                              3.14159265f                                         // PI
    #define F_C_PI_HALF				            1.57079632f                                         // 0.5 * PI
    #define	F_C_PI_TWO					        6.28318530717958647692							    // 2.0 * PI
    #define F_C_PI_INV				            0.31830988618379067154                              // 1.0 / PI
    #define	F_C_EXP					            2.71828182845904523536							    // e
	#define	F_C_LOG2_INVERT			            3.32192809488736234787								// 1.0 / log10(2)
	#define	F_C_LN2					            0.693147180559945									// ln(2)
	#define	F_C_LN2_INVERT				        1.44269504089										// 1.0f / ln(2)
			
	#define	F_C_INV3					        0.33333333333333333333							    // 1/3
	#define	F_C_INV6					        0.16666666666666666666								// 1/6
	#define	F_C_INV7					        0.14285714285714285714								// 1/7
	#define	F_C_INV9					        0.11111111111111111111								// 1/9
	#define	F_C_INV255					        0.00392156862745098039								// 1/255

	#define	F_C_SQRT2					        1.4142135623730950488							    // sqrt(2)
	#define	F_C_INVSQRT2				        0.707106781188										// 1 / sqrt(2)
			
	#define	F_C_SQRT3					        1.7320508075688772935								// sqrt(3)
	#define	F_C_INVSQRT3				        0.577350269189										// 1 / sqrt(3)             
    
    #define F_C_MIN_INT8		                ((int8)		-128)
    #define F_C_MAX_INT8		                ((int8)		0x7f)
    #define F_C_MIN_UINT8		                ((uint8)	0x00)
    #define F_C_MAX_UINT8		                ((uint8)	0xff)
    #define F_C_MIN_INT16		                ((int16)	-32768)
    #define F_C_MAX_INT16		                ((int16)	0x7fff)
    #define	F_C_MIN_UINT16		                ((uint16)	0x0000)
    #define F_C_MAX_UINT16		                ((uint16)	0xffff)
    #define F_C_MIN_INT32		                ((int32)	0x80000000)
    #define F_C_MAX_INT32		                ((int32)	0x7fffffff)
    #define	F_C_MIN_UINT32		                ((uint32)	0x00000000)
    #define F_C_MAX_UINT32		                ((uint32)	0xffffffff)
    #define F_C_MIN_INT64		                ((int64)	0x8000000000000000)
    #define F_C_MAX_INT64		                ((int64)	0x7fffffffffffffff)
    #define F_C_MIN_UINT64		                ((uint64)	0x0000000000000000)
    #define F_C_MAX_UINT64		                ((uint64)	0xffffffffffffffff)
    #define	F_C_MIN_FLOAT				        (-FLT_MAX)	
    #define	F_C_MAX_FLOAT				        FLT_MAX		
    
    #define F_C_POS_INFINITY_NEG                (-FLT_MAX)
    #define F_C_POS_INFINITY                    FLT_MAX

    #define F_C_DELTA				            0.00001f

    #define F_C_NUMBER_SMALL		            1.e-8f
    #define F_C_NUMBER_SMALL_KINDA	            1.e-4f
    #define F_C_NUMBER_BIG			            3.4e+38f
    #define F_C_NUMBER_EULERS                   2.71828182845904523536f   

    #define F_Malloc(size)                      malloc(size); 
    #define F_Free(p)                           if (p != nullptr) { free(p); }
    #define F_Assert(expr)                      assert(expr);

    #define F_ARRAYSIZE(_ARR)                   ((int)(sizeof(_ARR)/sizeof(*_ARR)))    
    #define F_OFFSETOF(_TYPE,_MEMBER)           ((size_t)&(((_TYPE*)0)->_MEMBER))     
    #define F_DELETE(p)                         { if(p) { delete p; p=nullptr; }}
    #define F_DELETE_T(p)                       { if(p) { delete[] p; p=nullptr; }}


    ////////////////////////////// Typedef /////////////////////////////
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

    typedef std::chrono::steady_clock::time_point TimePoint;
    typedef std::string String;
    typedef std::vector<String> StringVector;
    typedef std::list<String> StringList;
    typedef std::map<String, String> String2StringMap;
    typedef std::vector<const char*> ConstCharPtrVector;
    typedef std::vector<char> CharVector;
    typedef std::vector<uint16> Indices16Vector;
    typedef std::vector<uint32> Indices32Vector;
    
    typedef std::vector<bool> BoolVector;
	typedef std::vector<int8> Int8Vector;
	typedef std::vector<uint8> Uint8Vector;
	typedef std::vector<int16> Int16Vector;
	typedef std::vector<uint16> Uint16Vector;
	typedef std::vector<int32> Int32Vector;
	typedef std::vector<uint32> Uint32Vector;
	typedef std::vector<float> FloatVector;
	typedef std::vector<double>	DoubleVector;
    typedef std::deque<TimePoint> FrameTimePointQueue;

    typedef FTPoint<int32> FPointI;
	typedef FTPoint<float> FPointF;
    typedef	FTSize<int32> FSizeI;
	typedef	FTSize<float> FSizeF;
    typedef FTRect<int32> FRectI;
	typedef FTRect<float> FRectF;
    typedef FTLine<int32> FLineI;
	typedef FTLine<float> FLineF;
    typedef std::vector<FPointI> FPointIVector;
    typedef std::vector<FPointF> FPointFVector;
    typedef std::vector<FSizeI> FSizeIVector;
    typedef std::vector<FSizeF> FSizeFVector;
    typedef std::vector<FRectI> FRectIVector;
    typedef std::vector<FRectF> FRectFVector;
    typedef std::vector<FLineI> FLineIVector;
    typedef std::vector<FLineF> FLineFVector;


    typedef glm::vec2 FVector2;
    typedef glm::vec3 FVector3;
    typedef glm::vec4 FVector4;
    typedef glm::mat3 FMatrix3;
    typedef glm::mat4 FMatrix4;
    typedef glm::quat FQuaternion;
    typedef glm::vec4 FColor;
    typedef std::vector<FVector2> FVector2Vector;
    typedef std::vector<FVector3> FVector3Vector;
    typedef std::vector<FVector4> FVector4Vector;
    typedef std::vector<FMatrix3> FMatrix3Vector;
    typedef std::vector<FMatrix4> FMatrix4Vector;
    typedef std::vector<FQuaternion> FQuaternionVector;
    typedef std::vector<FColor> FColorVector;
    typedef std::map<size_t, FVector3> FIndex2Vector3Map;


    struct FVertex_Pos2Color4;
    struct FVertex_Pos3Color4;
    struct FVertex_Pos3Normal3;
    struct FVertex_Pos2Color4Tex2;
    struct FVertex_Pos3Color4Tex2;
    struct FVertex_Pos3Color4Normal3Tex2;
    struct FVertex_Pos3Color4Normal3Tex4;
    struct FVertex_Pos3Color4Normal3Tangent3Tex2;
    struct FVertex_Pos3Color4Normal3Tangent3Tex4;
    struct FVertex_Pos3Normal3Tangent3BlendWI8Tex2;
    struct FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2;

    struct FMeshDataPC;
    struct FMeshDataPCT;
    struct FMeshData;
    struct FMeshDataUV2;
    struct FMeshDataSkin;

    class FAABB;
    class FAtomicCount;
    class FBitwise;
    class FBox;
    class FCamera;
    class FCylinder;
    class FDynamicLib;
	class FDynamicLibManager;
    class FFileBase;
	class FFileBinary;
	class FFileIO;
	class FFileManager;
	class FFileMemory;
    class FFileXML;
	class FFileXMLTiny;
	class FFileXMLTinyConfig;
    class FFrustum;
    class FMath;
    class FMeshDataLoader;
    class FMeshCreateParam;
    class FMeshCreateParam_Line;
    class FMeshCreateParam_LineTriangle;
    class FMeshCreateParam_LineQuad;
    class FMeshCreateParam_LineGrid;
    class FMeshCreateParam_LineCircle;
    class FMeshCreateParam_LineAABB;
    class FMeshCreateParam_LineSphere;
    class FMeshCreateParam_LineCylinder;
    class FMeshCreateParam_LineCapsule;
    class FMeshCreateParam_LineCone;
    class FMeshCreateParam_LineTorus;
    class FMeshCreateParam_FlatTriangle;
    class FMeshCreateParam_FlatQuad;
    class FMeshCreateParam_FlatCircle;
    class FMeshCreateParam_FlatAABB;
    class FMeshCreateParam_FlatSphere;
    class FMeshCreateParam_FlatCylinder;
    class FMeshCreateParam_FlatCapsule;
    class FMeshCreateParam_FlatCone;
    class FMeshCreateParam_FlatTorus;
    class FMeshCreateParam_EntityTriangle;
    class FMeshCreateParam_EntityQuad;
    class FMeshCreateParam_EntityGrid;
    class FMeshCreateParam_EntityCircle;
    class FMeshCreateParam_EntityAABB;
    class FMeshCreateParam_EntitySphere;
    class FMeshCreateParam_EntityGeoSphere;
    class FMeshCreateParam_EntityCylinder;
    class FMeshCreateParam_EntityCapsule;
    class FMeshCreateParam_EntityCone;
    class FMeshCreateParam_EntityTorus;
    class FMeshCreateParam_EntitySkyBox;
    class FMeshCreateParam_EntitySkyDome;
    class FMeshCreateParam_EntityTerrain;
    class FMeshGeometry;
    class FLog;
    class FLogConsole;
    class FLogFile;
    class FLogManager;
    class FPlane;
    class FPlaneBoundedVolume;
    class FPlugin;
	class FPluginManager;
    class FRay;
    class FRefCount;
    class FSphere;
    class FTimer;
    class FUtil;
    class FUtilString;

    typedef	std::vector<FFileIO*> FFileIOPtrVector;
	typedef	std::vector<FFileMemory*> FFileMemoryPtrVector;
    typedef std::map<FFileIO*, int32> FFileIOPtrMap;
	typedef std::map<FFileMemory*, int32> FFileMemoryPtrMap;

    typedef std::vector<FLog*> FLogPtrVector;
    typedef std::map<String, FLog*> FLogPtrMap;

    typedef std::map<String, FDynamicLib*> FDynamicLibPtrMap;
    typedef std::map<String, FPlugin*> FPluginPtrMap;

    typedef std::vector<FRay> FRayVector; 
    typedef std::vector<FPlane> FPlaneVector; 
    typedef std::list<FPlane> FPlaneList;
	typedef std::vector<FPlaneBoundedVolume> FPlaneBoundedVolumeVector;


    ////////////////////////////// Enum ////////////////////////////////
    enum FFileXMLType
	{
		F_FileXML_RapidXML = 0,
		F_FileXML_TinyXML,
	};

    enum FLogType
    {
        F_Log_Console = 0,                              //0:    Console
        F_Log_File,                                     //1:    File
    };


    enum FMeshType
    {
        F_Mesh_File = 0,                                //0:    File
        F_Mesh_Geometry,                                //1:    Geometry

        F_Mesh_Count
    };
    const String& F_GetMeshTypeName(FMeshType type);
    const String& F_GetMeshTypeName(int type);
    FMeshType F_ParseMeshType(const String& strName);


    enum FMeshGeometryType
    {
    ////////////////////////////////// Line //////////////////////////////////
        F_MeshGeometry_Line = 0,                        //0:    Line
        F_MeshGeometry_LineTriangle,                    //1:    LineTriangle
        F_MeshGeometry_LineQuad,                        //2:    LineQuad
        F_MeshGeometry_LineGrid,                        //3:    LineGrid
        F_MeshGeometry_LineCircle,                      //4:    LineCircle
        F_MeshGeometry_LineAABB,                        //5:    LineAABB
        F_MeshGeometry_LineSphere,                      //6:    LineSphere
        F_MeshGeometry_LineCylinder,                    //7:    LineCylinder
        F_MeshGeometry_LineCapsule,                     //8:    LineCapsule
        F_MeshGeometry_LineCone,                        //9:    LineCone
        F_MeshGeometry_LineTorus,                       //10:   LineTorus

    ////////////////////////////////// Flat //////////////////////////////////
        F_MeshGeometry_FlatTriangle,                    //11:   FlatTriangle
        F_MeshGeometry_FlatQuad,                        //12:   FlatQuad
        F_MeshGeometry_FlatCircle,                      //13:   FlatCircle
        F_MeshGeometry_FlatAABB,                        //14:   FlatAABB
        F_MeshGeometry_FlatSphere,                      //15:   FlatSphere
        F_MeshGeometry_FlatCylinder,                    //16:   FlatCylinder
        F_MeshGeometry_FlatCapsule,                     //17:   FlatCapsule
        F_MeshGeometry_FlatCone,                        //18:   FlatCone
        F_MeshGeometry_FlatTorus,                       //19:   FlatTorus

    ////////////////////////////////// Entity ////////////////////////////////
        F_MeshGeometry_EntityTriangle,                  //20:   EntityTriangle
        F_MeshGeometry_EntityQuad,                      //21:   EntityQuad
        F_MeshGeometry_EntityGrid,                      //22:   EntityGrid
        F_MeshGeometry_EntityCircle,                    //23:   EntityCircle
        F_MeshGeometry_EntityAABB,                      //24:   EntityAABB
        F_MeshGeometry_EntitySphere,                    //25:   EntitySphere
        F_MeshGeometry_EntityGeoSphere,                 //26:   EntityGeoSphere
        F_MeshGeometry_EntityCylinder,                  //27:   EntityCylinder
        F_MeshGeometry_EntityCapsule,                   //28:   EntityCapsule
        F_MeshGeometry_EntityCone,                      //29:   EntityCone
        F_MeshGeometry_EntityTorus,                     //30:   EntityTorus
        F_MeshGeometry_EntitySkyBox,                    //31:   EntitySkyBox
        F_MeshGeometry_EntitySkyDome,                   //32:   EntitySkyDome
        F_MeshGeometry_EntityTerrain,                   //33:   EntityTerrain

        F_MeshGeometry_Count,
    };
    const String& F_GetMeshGeometryTypeName(FMeshGeometryType type);
    const String& F_GetMeshGeometryTypeName(int type);
    FMeshGeometryType F_ParseMeshGeometryType(const String& strName);
    bool F_IsMeshGeometryType_Line(FMeshGeometryType type);
    bool F_IsMeshGeometryType_Flat(FMeshGeometryType type);
    bool F_IsMeshGeometryType_Entity(FMeshGeometryType type);


    enum FMeshVertexType
    {
        F_MeshVertex_Pos2Color4 = 0,                           //0:    Pos2Color4
        F_MeshVertex_Pos3Color4,                               //1:    Pos3Color4                              (FMeshVertexPC)
        F_MeshVertex_Pos3Normal3,                              //2:    Pos3Normal3
        F_MeshVertex_Pos3Normal3Tex2,                          //3:    Pos3Normal3Tex2
        F_MeshVertex_Pos2Color4Tex2,                           //4:    Pos2Color4Tex2
        F_MeshVertex_Pos3Color4Tex2,                           //5:    Pos3Color4Tex2                          (FMeshVertexPCT)
        F_MeshVertex_Pos3Color4Normal3Tex2,                    //6:    Pos3Color4Normal3Tex2
        F_MeshVertex_Pos3Color4Normal3Tex4,                    //7:    Pos3Color4Normal3Tex4
        F_MeshVertex_Pos3Color4Normal3Tangent3Tex2,            //8:    Pos3Color4Normal3Tangent3Tex2           (FMeshVertex)
        F_MeshVertex_Pos3Color4Normal3Tangent3Tex4,            //9:    Pos3Color4Normal3Tangent3Tex4           (FMeshVertexUV2)
        F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2,          //10:   Pos3Normal3Tangent3BlendWI8Tex2 
        F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2,    //11:   Pos3Color4Normal3Tangent3BlendWI8Tex2   (FMeshVertexSkin)

        F_MeshVertex_Count
    };
    const String& F_GetMeshVertexTypeName(FMeshVertexType type);
    const String& F_GetMeshVertexTypeName(int type);
    FMeshVertexType F_ParseMeshVertexType(const String& strName);


    enum FPlaneSideType
    {
        F_PlaneSide_None = 0,                           //0: None
        F_PlaneSide_Positive,                           //1: Positive
        F_PlaneSide_Negative,                           //2: Negative
        F_PlaneSide_Both,	                            //3: Both
    };


    enum FFrustumPlaneType
    {
        F_FrustumPlane_Near = 0,                        //0: Near
        F_FrustumPlane_Far,                             //1: Far
        F_FrustumPlane_Left,                            //2: Left
        F_FrustumPlane_Right,                           //3: Right
        F_FrustumPlane_Top,                             //4: Top
        F_FrustumPlane_Bottom,                          //5: Bottom

        F_FrustumPlane_Count
    };


    enum FAABBExtentType
    {
        F_AABBExtent_Null = 0,                          //0: Null
        F_AABBExtent_Finite,                            //1: Finite
        F_AABBExtent_Infinite,                          //2: Infinite
    };


}; //LostPeterFoundation

#endif