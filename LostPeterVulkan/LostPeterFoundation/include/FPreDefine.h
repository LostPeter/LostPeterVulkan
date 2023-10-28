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
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

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
    class FCapsule;
    class FCircle;
    class FComplex;
    class FCone;
    class FCylinder;
    class FDynamicLib;
	class FDynamicLibManager;
    class FEquationCubic;
    class FEquationQuadratic;
    class FEquationQuartic;
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
    class FMeshCreateParam_LineLine2D;
    class FMeshCreateParam_LineTriangle2D;
    class FMeshCreateParam_LineQuad2D;
    class FMeshCreateParam_LineGrid2D;
    class FMeshCreateParam_LineCircle2D;
    class FMeshCreateParam_FlatTriangle2D;
    class FMeshCreateParam_FlatQuad2D;
    class FMeshCreateParam_FlatCircle2D;
    class FMeshCreateParam_LineLine3D;
    class FMeshCreateParam_LineTriangle3D;
    class FMeshCreateParam_LineQuad3D;
    class FMeshCreateParam_LineGrid3D;
    class FMeshCreateParam_LineCircle3D;
    class FMeshCreateParam_LineAABB3D;
    class FMeshCreateParam_LineSphere3D;
    class FMeshCreateParam_LineCylinder3D;
    class FMeshCreateParam_LineCapsule3D;
    class FMeshCreateParam_LineCone3D;
    class FMeshCreateParam_LineTorus3D;
    class FMeshCreateParam_FlatTriangle3D;
    class FMeshCreateParam_FlatQuad3D;
    class FMeshCreateParam_FlatCircle3D;
    class FMeshCreateParam_FlatAABB3D;
    class FMeshCreateParam_FlatSphere3D;
    class FMeshCreateParam_FlatCylinder3D;
    class FMeshCreateParam_FlatCapsule3D;
    class FMeshCreateParam_FlatCone3D;
    class FMeshCreateParam_FlatTorus3D;
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
    class FPathManager;
    class FPlane;
    class FPlaneBoundedVolume;
    class FPlugin;
	class FPluginManager;
    class FQuad;
    class FRay;
    class FRefCount;
    class FSegment;
    class FSerializer;
    class FSphere;
    class FTimer;
    class FTorus;
    class FTriangle;
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

    typedef std::map<uint32, String2StringMap> FPathGroupMap;
	typedef std::map<uint32, uint32> FPathGroupIDMap;
	typedef std::map<uint32, String> FPathGroupBaseMap;
	typedef std::map<uint32, bool> FPathGroupRecursiveMap;
    typedef std::map<uint32, bool> FPathGroupDelSuffixMap;


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
    ////////////////////////////////// Line2D ////////////////////////////////
        F_MeshGeometry_LineLine2D = 0,                  //0:    LineLine2D
        F_MeshGeometry_LineTriangle2D,                  //1:    LineTriangle2D
        F_MeshGeometry_LineQuad2D,                      //2:    LineQuad2D
        F_MeshGeometry_LineGrid2D,                      //3:    LineGrid2D
        F_MeshGeometry_LineCircle2D,                    //4:    LineCircle2D

    ////////////////////////////////// Flat2D ////////////////////////////////
        F_MeshGeometry_FlatTriangle2D,                  //5:    FlatTriangle2D
        F_MeshGeometry_FlatQuad2D,                      //6:    FlatQuad2D
        F_MeshGeometry_FlatCircle2D,                    //7:    FlatCircle2D

    ////////////////////////////////// Line3D ////////////////////////////////
        F_MeshGeometry_LineLine3D,                      //8:    LineLine3D
        F_MeshGeometry_LineTriangle3D,                  //9:    LineTriangle3D
        F_MeshGeometry_LineQuad3D,                      //10:   LineQuad3D
        F_MeshGeometry_LineGrid3D,                      //11:   LineGrid3D
        F_MeshGeometry_LineCircle3D,                    //12:   LineCircle3D
        F_MeshGeometry_LineAABB3D,                      //13:   LineAABB3D
        F_MeshGeometry_LineSphere3D,                    //14:   LineSphere3D
        F_MeshGeometry_LineCylinder3D,                  //15:   LineCylinder3D
        F_MeshGeometry_LineCapsule3D,                   //16:   LineCapsule3D
        F_MeshGeometry_LineCone3D,                      //17:   LineCone3D
        F_MeshGeometry_LineTorus3D,                     //18:   LineTorus3D

    ////////////////////////////////// Flat3D ////////////////////////////////
        F_MeshGeometry_FlatTriangle3D,                  //19:   FlatTriangle3D
        F_MeshGeometry_FlatQuad3D,                      //20:   FlatQuad3D
        F_MeshGeometry_FlatCircle3D,                    //21:   FlatCircle3D
        F_MeshGeometry_FlatAABB3D,                      //22:   FlatAABB3D
        F_MeshGeometry_FlatSphere3D,                    //23:   FlatSphere3D
        F_MeshGeometry_FlatCylinder3D,                  //24:   FlatCylinder3D
        F_MeshGeometry_FlatCapsule3D,                   //25:   FlatCapsule3D
        F_MeshGeometry_FlatCone3D,                      //26:   FlatCone3D
        F_MeshGeometry_FlatTorus3D,                     //27:   FlatTorus3D

    ////////////////////////////////// Entity ////////////////////////////////
        F_MeshGeometry_EntityTriangle,                  //28:   EntityTriangle
        F_MeshGeometry_EntityQuad,                      //29:   EntityQuad
        F_MeshGeometry_EntityGrid,                      //30:   EntityGrid
        F_MeshGeometry_EntityCircle,                    //31:   EntityCircle
        F_MeshGeometry_EntityAABB,                      //32:   EntityAABB
        F_MeshGeometry_EntitySphere,                    //33:   EntitySphere
        F_MeshGeometry_EntityGeoSphere,                 //34:   EntityGeoSphere
        F_MeshGeometry_EntityCylinder,                  //35:   EntityCylinder
        F_MeshGeometry_EntityCapsule,                   //36:   EntityCapsule
        F_MeshGeometry_EntityCone,                      //37:   EntityCone
        F_MeshGeometry_EntityTorus,                     //38:   EntityTorus
        F_MeshGeometry_EntitySkyBox,                    //39:   EntitySkyBox
        F_MeshGeometry_EntitySkyDome,                   //40:   EntitySkyDome
        F_MeshGeometry_EntityTerrain,                   //41:   EntityTerrain

        F_MeshGeometry_Count,
    };
    const String& F_GetMeshGeometryTypeName(FMeshGeometryType type);
    const String& F_GetMeshGeometryTypeName(int type);
    FMeshGeometryType F_ParseMeshGeometryType(const String& strName);
    bool F_IsMeshGeometryType_Line2D(FMeshGeometryType type);
    bool F_IsMeshGeometryType_Flat2D(FMeshGeometryType type);
    bool F_IsMeshGeometryType_Line3D(FMeshGeometryType type);
    bool F_IsMeshGeometryType_Flat3D(FMeshGeometryType type);
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


    enum FTextureType
    {
        F_Texture_1D = 0,                               //0: 1D
        F_Texture_2D,                                   //1: 2D
        F_Texture_2DArray,                              //2: 2DArray
        F_Texture_3D,                                   //3: 3D
        F_Texture_CubeMap,                              //4: CubeMap

        F_Texture_Count,
    };
    const String& F_GetTextureTypeName(FTextureType type);
    const String& F_GetTextureTypeName(int type);
    FTextureType F_ParseTextureType(const String& strName);


    enum FTexturePixelFormatType
    {
        F_TexturePixelFormat_R8_UNORM = 0,              //0: R8_UNORM
        F_TexturePixelFormat_R16_UNORM,                 //1: R16_UNORM
        F_TexturePixelFormat_R8G8B8A8_SRGB,             //2: R8G8B8A8_SRGB
        F_TexturePixelFormat_R8G8B8A8_UNORM,            //3: R8G8B8A8_UNORM

        F_TextureFormat_Count,
    };
    const String& F_GetTexturePixelFormatTypeName(FTexturePixelFormatType type);
    const String& F_GetTexturePixelFormatTypeName(int type);
    FTexturePixelFormatType F_ParseTexturePixelFormatType(const String& strName);


    enum FTextureFilterSizeType
    {
        F_TextureFilterSize_Min = 0,	                //0: Min		
        F_TextureFilterSize_Mag,				        //1: Mag
        F_TextureFilterSize_Mip,				        //2: Mip

        F_TextureFilterSize_Count,
    };
    const String& F_GetTextureFilterSizeTypeName(FTextureFilterSizeType type);
    const String& F_GetTextureFilterSizeTypeName(int type);
    FTextureFilterSizeType F_ParseTextureFilterSizeType(const String& strName);


    enum FTextureFilterPixelType
    {
        F_TextureFilterPixel_None	= 0,	            //0: None
        F_TextureFilterPixel_Point,			            //1: Point
        F_TextureFilterPixel_Linear,			        //2: Linear
        F_TextureFilterPixel_Anisotropic,		        //3: Anisotropic

        F_TextureFilterPixel_Count,
    };
    const String& F_GetTextureFilterPixelTypeName(FTextureFilterPixelType type);
    const String& F_GetTextureFilterPixelTypeName(int type);
    FTextureFilterPixelType F_ParseTextureFilterPixelType(const String& strName);


     enum FTextureFilterType
    {
        F_TextureFilter_None = 0,					    //0: Min=F_TextureFilterPixel_Point,        Mag=F_TextureFilterPixel_Point,  	   Mip=F_TextureFilterPixel_None
        F_TextureFilter_Bilinear,				        //1: Min=F_TextureFilterPixel_Linear,       Mag=F_TextureFilterPixel_Linear, 	   Mip=F_TextureFilterPixel_Point
        F_TextureFilter_Trilinear,			            //2: Min=F_TextureFilterPixel_Linear,	    Mag=F_TextureFilterPixel_Linear, 	   Mip=F_TextureFilterPixel_Linear
        F_TextureFilter_Anisotropic,			        //3: Min=F_TextureFilterPixel_Anisotropic,  Mag=F_TextureFilterPixel_Anisotropic,  Mip=F_TextureFilterPixel_Linear

        F_TextureFilter_Count,
    };
    const String& F_GetTextureFilterTypeName(FTextureFilterType type);
    const String& F_GetTextureFilterTypeName(int type);
    FTextureFilterType F_ParseTextureFilterType(const String& strName);


    enum FTextureAddressingType
    {
        F_TextureAddressing_Wrap = 0,	                //0: Wrap
        F_TextureAddressing_Mirror,			            //1: Mirror
        F_TextureAddressing_Clamp,			            //2: Clamp
        F_TextureAddressing_Border,			            //3: Border

        F_TextureAddressing_Count,
    };
    const String& F_GetTextureAddressingTypeName(FTextureAddressingType type);
    const String& F_GetTextureAddressingTypeName(int type);
    FTextureAddressingType F_ParseTextureAddressingType(const String& strName);


    enum FTextureBorderColorType
    {
        F_TextureBorderColor_OpaqueBlack = 0,          //0: OpaqueBlack
        F_TextureBorderColor_OpaqueWhite,              //1: OpaqueWhite
        F_TextureBorderColor_TransparentBlack,         //2: TransparentBlack

        F_TextureBorderColor_Count,
    };
    const String& F_GetTextureBorderColorTypeName(FTextureBorderColorType type);
    const String& F_GetTextureBorderColorTypeName(int type);
    FTextureBorderColorType F_ParseTextureBorderColorType(const String& strName);


    enum FMSAASampleCountType
    {
        F_MSAASampleCount_1_Bit = 0,                   //0: 1_Bit
        F_MSAASampleCount_2_Bit,                       //1: 2_Bit
        F_MSAASampleCount_4_Bit,                       //2: 4_Bit
        F_MSAASampleCount_8_Bit,                       //3: 8_Bit
        F_MSAASampleCount_16_Bit,                      //4: 16_Bit
        F_MSAASampleCount_32_Bit,                      //5: 32_Bit
        F_MSAASampleCount_64_Bit,                      //6: 64_Bit

        F_MSAASampleCount_Count,
    };
    const String& F_GetMSAASampleCountTypeName(FMSAASampleCountType type);
    const String& F_GetMSAASampleCountTypeName(int type);
    FMSAASampleCountType F_ParseMSAASampleCountType(const String& strName);


    enum FShaderType
    {
        F_Shader_Vertex = 0,                           //0: Vertex
        F_Shader_TessellationControl,                  //1: TessellationControl
        F_Shader_TessellationEvaluation,               //2: TessellationEvaluation
        F_Shader_Geometry,                             //3: Geometry
        F_Shader_Fragment,                             //4: Fragment
        F_Shader_Compute,                              //5: Compute

        F_Shader_Count,
    };
    const String& F_GetShaderTypeName(FShaderType type);
    const String& F_GetShaderTypeName(int type);
    FShaderType F_ParseShaderType(const String& strName);


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


    enum FCameraType
    {
        F_Camera_Perspective = 0,                       //0: Perspective
        F_Camera_Orthogonal,                            //1: Orthogonal

        F_Camera_Count,
    };
    const String& F_GetCameraTypeName(FCameraType type);
    const String& F_GetCameraTypF_GetCameraTypeNameeTypeName(int type);
    FCameraType F_ParseCameraType(const String& strName);


    enum FRenderPrimitiveType
	{
		F_RenderPrimitive_PointList = 0,	            //0: PointList
		F_RenderPrimitive_LineList,			            //1: LineList
		F_RenderPrimitive_LineStrip,			        //2: LineStrip
		F_RenderPrimitive_TriangleList,		            //3: TriangleList
		F_RenderPrimitive_TriangleStrip,		        //4: TriangleStrip
		F_RenderPrimitive_TriangleFan,			        //5: TriangleFan

        F_RenderPrimitive_Count,
	};
    const String& F_GetRenderPrimitiveTypeName(FRenderPrimitiveType type);
    const String& F_GetRenderPrimitiveTypeName(int type);
    FRenderPrimitiveType F_ParseRenderPrimitiveType(const String& strName);


    enum FCullingType
	{
		F_Culling_None = 0,	                            //0: None
		F_Culling_ClockWise,				            //1: ClockWise
		F_Culling_CounterClockWise,			            //2: CounterClockWise

        F_Culling_Count,
	};
    const String& F_GetCullingTypeName(FCullingType type);
    const String& F_GetCullingTypeName(int type);
    FCullingType F_ParseCullingType(const String& strName);


    enum FPolygonType
	{
		F_Polygon_Point = 0,	                        //0: Point
		F_Polygon_WireFrame,				            //1: WireFrame
		F_Polygon_Solid,					            //2: Solid

        F_Polygon_Count,
	};
    const String& F_GetPolygonTypeName(FPolygonType type);
    const String& F_GetPolygonTypeName(int type);
    FPolygonType F_ParsePolygonType(const String& strName);


    enum FStencilOPType
	{
		F_StencilOP_Keep = 0,                           //0: Keep
		F_StencilOP_Zero,                               //1: Zero
		F_StencilOP_Replace,                            //2: Replace
		F_StencilOP_Increment,                          //3: Increment
		F_StencilOP_Decrement,                          //4: Decrement
		F_StencilOP_IncrementWrap,                      //5: IncrementWrap
		F_StencilOP_DecrementWrap,                      //6: DecrementWrap
		F_StencilOP_Invert,                             //7: Invert

        F_StencilOP_Count,
	};
    const String& F_GetStencilOPTypeName(FStencilOPType type);
    const String& F_GetStencilOPTypeName(int type);
    FStencilOPType F_ParseStencilOPType(const String& strName);


    enum FCompareFuncType
	{
		F_CompareFunc_AlwaysPass = 0,	                //0: AlwaysPass
		F_CompareFunc_AlwaysFail,				        //1: AlwaysFail
		F_CompareFunc_Less,					            //2: Less 
		F_CompareFunc_LessEqual,				        //3: LessEqual
		F_CompareFunc_Equal,					        //4: Equal
		F_CompareFunc_NotEqual,				            //5: NotEqual
		F_CompareFunc_GreaterEqual,			            //6: GreaterEqual
		F_CompareFunc_Greater,					        //7: Greater	

        F_CompareFunc_Count,
	};
    const String& F_GetCompareFuncTypeName(FCompareFuncType type);
    const String& F_GetCompareFuncTypeName(int type);
    FCompareFuncType F_ParseCompareFuncType(const String& strName);


    enum FSceneBlendingType
	{
		F_SceneBlending_Alpha = 0,		                //0: Alpha
		F_SceneBlending_Color,				            //1: Color
		F_SceneBlending_Add,					        //2: Add
		F_SceneBlending_Modulate,				        //3: Modulate
		F_SceneBlending_Replace,				        //4: Replace

        F_SceneBlending_Count,
	};
    const String& F_GetSceneBlendingTypeName(FSceneBlendingType type);
    const String& F_GetSceneBlendingTypeName(int type);
    FSceneBlendingType F_ParseSceneBlendingType(const String& strName);


    enum FSceneBlendingOPType
	{
		F_SceneBlendingOP_Add = 0,		                //0: Add
		F_SceneBlendingOP_Subtract,			            //1: Subtract
		F_SceneBlendingOP_SubtractReverse,	            //2: SubtractReverse
		F_SceneBlendingOP_Min,				            //3: Min
		F_SceneBlendingOP_Max,					        //4: Max

        F_SceneBlendingOP_Count,
	};
    const String& F_GetSceneBlendingOPTypeName(FSceneBlendingOPType type);
    const String& F_GetSceneBlendingOPTypeName(int type);
    FSceneBlendingOPType F_ParseSceneBlendingOPType(const String& strName);


	enum FSceneBlendingFactorType
	{
		F_SceneBlendingFactor_One = 0,                  //0: One
		F_SceneBlendingFactor_Zero,                     //1: Zero
		F_SceneBlendingFactor_SourceColor,              //2: SourceColor
		F_SceneBlendingFactor_DestColor,                //3: DestColor
		F_SceneBlendingFactor_OneMinusSourceColor,      //4: OneMinusSourceColor
		F_SceneBlendingFactor_OneMinusDestColor,        //5: OneMinusDestColor
		F_SceneBlendingFactor_SourceAlpha,              //6: SourceAlpha
		F_SceneBlendingFactor_DestAlpha,                //7: DestAlpha
		F_SceneBlendingFactor_OneMinusSourceAlpha,      //8: OneMinusSourceAlpha
		F_SceneBlendingFactor_OneMinusDestAlpha,        //9: OneMinusDestAlpha 

        F_SceneBlendingFactor_Count,
	};
    const String& F_GetSceneBlendingFactorTypeName(FSceneBlendingFactorType type);
    const String& F_GetSceneBlendingFactorTypeName(int type);
    FSceneBlendingFactorType F_ParseSceneBlendingFactorType(const String& strName);


    enum FLightingType
	{
		F_Lighting_Flat = 0,	                        //0: Flat
		F_Lighting_Gouraud,					            //1: Gouraud
		F_Lighting_Phong,						        //2: Phong
        F_Lighting_Pbr,                                 //3: Pbr

        F_Lighting_Count,
	};
    const String& F_GetLightingTypeName(FLightingType type);
    const String& F_GetLightingTypeName(int type);
    FLightingType F_ParseLightingType(const String& strName);


    enum FRenderPipelineType
    {
        F_RenderPipeline_Forward = 0,                   //0: Forward
        F_RenderPipeline_Deferred,                      //1: Deferred

        F_RenderPipeline_Count,
    };
    const String& F_GetRenderPipelineTypeName(FRenderPipelineType type);
    const String& F_GetRenderPipelineTypeName(int type);
    FRenderPipelineType F_ParseRenderPipelineType(const String& strName);


    enum FRenderQueueType
    {
        F_RenderQueue_Background = 0,                   //0: Background:   0    - 1000
        F_RenderQueue_Opaque = 1,                       //1: Opaque:       1000 - 2000
        F_RenderQueue_Transparent = 2,                  //2: Transparent:  2000 - 3000
        F_RenderQueue_Overlay = 3,                      //3: Overlay:      3000 - 4000

        F_RenderQueue_Count,
    };
    const String& F_GetRenderQueueTypeName(FRenderQueueType type);
    const String& F_GetRenderQueueTypeName(int type);
    FRenderQueueType F_ParseRenderQueueType(const String& strName);

    int F_GetRenderQueueTypeValue(FRenderQueueType type);
    int F_GetRenderQueueTypeValue(int type);
    FRenderQueueType F_ParseRenderQueueTypeByValue(int value);
    

    enum FRenderPassType
    {
        F_RenderPass_ForwardLit = 0,                    //0:  ForwardLit
        F_RenderPass_DeferredLit,                       //1:  DeferredLit  
        F_RenderPass_ShadowCaster,                      //2:  ShadowCaster
        F_RenderPass_DepthOnly,                         //3:  DepthOnly

        F_RenderPass_Count,
    };
    const String& F_GetRenderPassTypeName(FRenderPassType type);
    const String& F_GetRenderPassTypeName(int type);
    FRenderPassType F_ParseRenderPassType(const String& strName);



}; //LostPeterFoundation

#endif