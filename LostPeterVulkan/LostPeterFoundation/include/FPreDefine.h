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
#include "FPtrListNode.h"
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


    ////////////////////////////// Macro ///////////////////////////////
    #define	F_M_MIN(a,b)       		    ((a) < (b) ? (a) : (b))				
	#define	F_M_MIN3(a,b,c)    		    ((a) < (b) ? F_M_MIN(a,c) : F_M_MIN(b,c))
	#define	F_M_MAX(a,b)       		    ((a) > (b) ? (a) : (b))				
	#define	F_M_MAX3(a,b,c)			    ((a) > (b) ? DF_M_MAX(a,c) : DF_M_MAX(b,c))

	#define	F_M_GetR(clr)				(clr & 0xff)
	#define	F_M_GetG(clr)				((clr >> 8) & 0xff)
	#define	F_M_GetB(clr)				((clr >> 16) & 0xff)

	#define	F_M_SQUAR(x)				((x)*(x))							
	#define	F_M_CUBE(x)				    ((x)*(x)*(x))				

	#define F_M_IR(x)					((uint32&)(x))				
	#define F_M_SIR(x)					((int32&)(x))				
	#define F_M_AIR(x)					(F_M_IR(x)&0x7fffffff)		
	#define F_M_FR(x)					((float&)(x))				
	#define	F_M_IS_NEGATIVE_FLOAT(x)	(F_M_IR(x)&0x80000000)

	#define	F_M_BigLittleSwap16(A)      ((((uint16)(A) & 0xff00) >> 8) | (((uint16)(A) & 0x00ff) << 8))
	#define	F_M_BigLittleSwap32(A)      ((((uint32)(A) & 0xff000000) >> 24) | (((uint32)(A) & 0x00ff0000) >> 8) | \
									    (((uint32)(A) & 0x0000ff00) << 8) | (((uint32)(A) & 0x000000ff) << 24))
		
	#define F_M_FloatToFixed(x)		    ((int)((x) * (float)(1 << 16) + 0.5f))
	#define F_M_FixedToFloat(x)		    ((float)(x) / (float)(1 << 16))	

	#define F_M_FOURCC(c0, c1, c2, c3)  (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))
	
	#if F_COMPILER == F_COMPILER_MSVC
		#define F_M_Rotl32(x,y) 		_rotl(x, y)
		#define F_M_Rotl64(x,y) 		_rotl64(x, y)
		#define F_M_BigConstant(x) 	    (x)
	#else
		F_FORCEINLINE uint32 F_F_Rotl32(uint32 x, int8 r)
		{
			return (x << r) | (x >> (32 - r));
		}
		F_FORCEINLINE uint64 F_F_Rotl64(uint64 x, int8 r)
		{
			return (x << r) | (x >> (64 - r));
		}

		#define F_M_Rotl32(x,y) 		F_F_Rotl32(x, y)
		#define F_M_Rotl64(x,y) 		F_F_Rotl64(x, y)
		#define F_M_BigConstant(x) 	    (x##LLU)
	#endif

    /////////////////////////////////////// Function /////////////////////////////////////
    template<class T>	F_FORCEINLINE const T&	F_F_Min	        (const T& a, const T& b)			{ return b < a ? b : a;	}
	template<class T>	F_FORCEINLINE const T&	F_F_Max	        (const T& a, const T& b)			{ return a < b ? b : a;	}
	template<class T>	F_FORCEINLINE void		F_F_SetMin		(T& a, const T& b)					{ if(a>b)	a = b;		}
	template<class T>	F_FORCEINLINE void		F_F_SetMax		(T& a, const T& b)					{ if(a<b)	a = b;		}
	template<class T>   F_FORCEINLINE void		F_F_Swap		(T& lhs, T& rhs)					{ T tmp;tmp = lhs;lhs = rhs;rhs = tmp; }
	template<class T>   F_FORCEINLINE void		F_F_ClampRoof	(T& x, const T& max)				{ if (x > max) x = max; }
	template<class T>	F_FORCEINLINE void		F_F_ClampFloor	(T& x, const T& min)				{ if (x < min) x = min; }
	template<class T>	F_FORCEINLINE void		F_F_Clamp		(T& x, const T& min, const T& max)	{ if (x < min) x = min;if (x > max) x = max; }
	template<class T>	F_FORCEINLINE T		    F_F_ClampT		(T x, T min,T max)					{ T tmp=x; if (x < min) tmp = min;if(x > max) tmp = max; return tmp; }
	
	template <class T>
	static T* F_F_RawOffsetPointer(T* ptr, ptrdiff_t offset)
	{
		return (T*)((char*)(ptr) + offset);
	}

	template <class T>
	static void F_F_AdvanceRawPointer(T*& ptr, ptrdiff_t offset)
	{
		ptr = F_F_RawOffsetPointer(ptr,offset);
	}

	// block read
	F_FORCEINLINE uint32 F_F_Getblock32(const uint32* p, int i)
	{
		return p[i];
	}
	F_FORCEINLINE uint64 DF_F_Getblock64(const uint64* p, int i)
	{
		return p[i];
	}

	// mix
	F_FORCEINLINE uint32 F_F_Mix32(uint32 h)
	{
		h ^= h >> 16;
		h *= 0x85ebca6b;
		h ^= h >> 13;
		h *= 0xc2b2ae35;
		h ^= h >> 16;

		return h;
	}
	F_FORCEINLINE uint64 F_F_Mix64(uint64 k)
	{
		k ^= k >> 33;
		k *= F_M_BigConstant(0xff51afd7ed558ccd);
		k ^= k >> 33;
		k *= F_M_BigConstant(0xc4ceb9fe1a85ec53);
		k ^= k >> 33;

		return k;
	}

	// linear interp
	F_FORCEINLINE double F_F_LinearInterp(double n0,double n1,double a)	
	{
		return ((1.0 - a) * n0) + (a * n1); 
	}

	//cubic interp
	F_FORCEINLINE double F_F_CubicInterp(double n0, double n1, double n2, double n3, double a)
	{
		double p = (n3 - n2) - (n0 - n1);
		double q = (n0 - n1) - p;
		double r = n2 - n0;
		double s = n1;

		return p * a * a * a + q * a * a + r * a + s;
	}

	//maps a value onto a cubic S-curve. a = [0.0,1.0]
	F_FORCEINLINE double F_F_SCurve3(double a)
	{
		return (a * a * (3.0 - 2.0 * a));
	}

	//maps a value onto a quintic S-curve. a = [0.0,1.0]
	F_FORCEINLINE double F_F_SCurve5(double a)
	{
		double a3 = a * a * a;
		double a4 = a3 * a;
		double a5 = a4 * a;
		return (6.0 * a5) - (15.0 * a4) + (10.0 * a3);
	}

	//blend two 8bit channel
	F_FORCEINLINE unsigned char F_F_BlendChannel(const unsigned char channel0, const unsigned char channel1, float alpha)
	{
		float c0 = (float)channel0 / 255.0f;
		float c1 = (float)channel1 / 255.0f;
		return (unsigned char)(((c1 * alpha) + (c0 * (1.0f - alpha))) * 255.0f);
	}

	// word -> byte binary stream
	F_FORCEINLINE unsigned char* F_F_UnpackWord(unsigned char* bytes, unsigned short integer)
	{
	#if F_ENDIAN == F_ENDIAN_LITTLE
		bytes[0] = (unsigned char)((integer & 0x00ff)     );
		bytes[1] = (unsigned char)((integer & 0xff00) >> 8);
	#else	
		bytes[1] = (unsigned char)((integer & 0x00ff)     );
		bytes[0] = (unsigned char)((integer & 0xff00) >> 8);
	#endif
		return bytes;
	}
	
	// dword -> byte binary stream
	F_FORCEINLINE unsigned char* F_F_UnpackUint(unsigned char* bytes, unsigned int integer)
	{
	#if F_ENDIAN == F_ENDIAN_LITTLE	
		bytes[0] = (unsigned char)((integer & 0x000000ff)      );
		bytes[1] = (unsigned char)((integer & 0x0000ff00) >> 8 );
		bytes[2] = (unsigned char)((integer & 0x00ff0000) >> 16);
		bytes[3] = (unsigned char)((integer & 0xff000000) >> 24);
	#else
		bytes[3] = (unsigned char)((integer & 0x000000ff)      );
		bytes[2] = (unsigned char)((integer & 0x0000ff00) >> 8 );
		bytes[1] = (unsigned char)((integer & 0x00ff0000) >> 16);
		bytes[0] = (unsigned char)((integer & 0xff000000) >> 24);
	#endif
		return bytes;
	}

	// float -> byte binary stream
	F_FORCEINLINE unsigned char* F_F_UnpackFloat(unsigned char* bytes, float value)
	{
		unsigned char* pBytes = (unsigned char*)(&value);
	#if F_ENDIAN == F_ENDIAN_LITTLE	
		bytes[0] = *pBytes++;
		bytes[1] = *pBytes++;
		bytes[2] = *pBytes++;
		bytes[3] = *pBytes++;
	#else		
		bytes[3] = *pBytes++;
		bytes[2] = *pBytes++;
		bytes[1] = *pBytes++;
		bytes[0] = *pBytes++;
	#endif
		return bytes;
	}
	
	//=1,big;=0,little
	F_FORCEINLINE int F_F_CheckCPUendian()
	{
		union
		{
			unsigned long int i;
			unsigned char s[4];
		}c;
		c.i = 0x12345678;

		return (0x12 == c.s[0]);
	}

	// local byte -> net byte (similar to htonl func)
	F_FORCEINLINE unsigned short int F_F_htons(unsigned short int h)
	{
		return F_F_CheckCPUendian() ? h : F_M_BigLittleSwap16(h);
	}
	F_FORCEINLINE unsigned long int F_F_htonl(unsigned long int h)
	{
		return F_F_CheckCPUendian() ? h : F_M_BigLittleSwap32(h);
	}

	// net byte -> local byte (similar to ntohl func)
	F_FORCEINLINE unsigned short int F_F_ntohs(unsigned short int n)
	{
		return F_F_CheckCPUendian() ? n : F_M_BigLittleSwap16(n);
	}
	F_FORCEINLINE unsigned long int F_F_ntohl(unsigned long int n)
	{
		return F_F_CheckCPUendian() ? n : F_M_BigLittleSwap32(n);
	}

    ////////////////////////////// Typedef /////////////////////////////
    typedef uint32 RGBA;
	typedef uint32 ARGB;
	typedef uint32 ABGR;
	typedef uint32 BGRA;

    typedef std::chrono::steady_clock::time_point TimePoint;
    typedef std::string String;
    typedef std::vector<String> StringVector;
    typedef std::list<String> StringList;
    typedef std::set<String> StringSet;
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
    typedef std::map<int32, bool> Int2boolMap;
	typedef std::map<int32, int32> Int2IntMap;

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
    class FCodec;
    class FCodecManager;
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
    class FPixelBox;
    class FPixelFormat;
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
    enum FPlatformType
	{
		F_Platform_Windows = 0,                         //0:    Windows
		F_Platform_MacOS,                               //1:    MacOS
		F_Platform_Linux,                               //2:    Linux
		F_Platform_Android,                             //3:    Android
		F_Platform_iOS,                                 //4:    iOS
	};
	foundationExport const String& F_GetPlatformTypeName(FPlatformType type);
    foundationExport const String& F_GetPlatformTypeName(int type);


    enum FFileXMLType
	{
		F_FileXML_RapidXML = 0,
		F_FileXML_TinyXML,
	};


    enum FTransformType
    {
        F_Transform_Local = 0,                          //0:    Local
        F_Transform_Parent,                             //1:    Parent
        F_Transform_World,                              //2:    World
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
    foundationExport const String& F_GetMeshTypeName(FMeshType type);
    foundationExport const String& F_GetMeshTypeName(int type);
    foundationExport FMeshType F_ParseMeshType(const String& strName);


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
    foundationExport const String& F_GetMeshGeometryTypeName(FMeshGeometryType type);
    foundationExport const String& F_GetMeshGeometryTypeName(int type);
    foundationExport FMeshGeometryType F_ParseMeshGeometryType(const String& strName);
    foundationExport bool F_IsMeshGeometryType_Line2D(FMeshGeometryType type);
    foundationExport bool F_IsMeshGeometryType_Flat2D(FMeshGeometryType type);
    foundationExport bool F_IsMeshGeometryType_Line3D(FMeshGeometryType type);
    foundationExport bool F_IsMeshGeometryType_Flat3D(FMeshGeometryType type);
    foundationExport bool F_IsMeshGeometryType_Entity(FMeshGeometryType type);


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
    foundationExport const String& F_GetMeshVertexTypeName(FMeshVertexType type);
    foundationExport const String& F_GetMeshVertexTypeName(int type);
    foundationExport FMeshVertexType F_ParseMeshVertexType(const String& strName);


    enum FTextureType
    {
        F_Texture_1D = 0,                               //0: 1D
        F_Texture_2D,                                   //1: 2D
        F_Texture_2DArray,                              //2: 2DArray
        F_Texture_3D,                                   //3: 3D
        F_Texture_CubeMap,                              //4: CubeMap

        F_Texture_Count,
    };
    foundationExport const String& F_GetTextureTypeName(FTextureType type);
    foundationExport const String& F_GetTextureTypeName(int type);
    foundationExport FTextureType F_ParseTextureType(const String& strName);


    enum FTexturePixelFormatType
    {
        F_TexturePixelFormat_R8_UNORM = 0,              //0: R8_UNORM
        F_TexturePixelFormat_R16_UNORM,                 //1: R16_UNORM
        F_TexturePixelFormat_R8G8B8A8_SRGB,             //2: R8G8B8A8_SRGB
        F_TexturePixelFormat_R8G8B8A8_UNORM,            //3: R8G8B8A8_UNORM

        F_TextureFormat_Count,
    };
    foundationExport const String& F_GetTexturePixelFormatTypeName(FTexturePixelFormatType type);
    foundationExport const String& F_GetTexturePixelFormatTypeName(int type);
    foundationExport FTexturePixelFormatType F_ParseTexturePixelFormatType(const String& strName);


    enum FTextureFilterSizeType
    {
        F_TextureFilterSize_Min = 0,	                //0: Min		
        F_TextureFilterSize_Mag,				        //1: Mag
        F_TextureFilterSize_Mip,				        //2: Mip

        F_TextureFilterSize_Count,
    };
    foundationExport const String& F_GetTextureFilterSizeTypeName(FTextureFilterSizeType type);
    foundationExport const String& F_GetTextureFilterSizeTypeName(int type);
    foundationExport FTextureFilterSizeType F_ParseTextureFilterSizeType(const String& strName);


    enum FTextureFilterPixelType
    {
        F_TextureFilterPixel_None	= 0,	            //0: None
        F_TextureFilterPixel_Point,			            //1: Point
        F_TextureFilterPixel_Linear,			        //2: Linear
        F_TextureFilterPixel_Anisotropic,		        //3: Anisotropic

        F_TextureFilterPixel_Count,
    };
    foundationExport const String& F_GetTextureFilterPixelTypeName(FTextureFilterPixelType type);
    foundationExport const String& F_GetTextureFilterPixelTypeName(int type);
    foundationExport FTextureFilterPixelType F_ParseTextureFilterPixelType(const String& strName);


     enum FTextureFilterType
    {
        F_TextureFilter_None = 0,					    //0: Min=F_TextureFilterPixel_Point,        Mag=F_TextureFilterPixel_Point,  	   Mip=F_TextureFilterPixel_None
        F_TextureFilter_Bilinear,				        //1: Min=F_TextureFilterPixel_Linear,       Mag=F_TextureFilterPixel_Linear, 	   Mip=F_TextureFilterPixel_Point
        F_TextureFilter_Trilinear,			            //2: Min=F_TextureFilterPixel_Linear,	    Mag=F_TextureFilterPixel_Linear, 	   Mip=F_TextureFilterPixel_Linear
        F_TextureFilter_Anisotropic,			        //3: Min=F_TextureFilterPixel_Anisotropic,  Mag=F_TextureFilterPixel_Anisotropic,  Mip=F_TextureFilterPixel_Linear

        F_TextureFilter_Count,
    };
    foundationExport const String& F_GetTextureFilterTypeName(FTextureFilterType type);
    foundationExport const String& F_GetTextureFilterTypeName(int type);
    foundationExport FTextureFilterType F_ParseTextureFilterType(const String& strName);


    enum FTextureAddressingType
    {
        F_TextureAddressing_Wrap = 0,	                //0: Wrap
        F_TextureAddressing_Mirror,			            //1: Mirror
        F_TextureAddressing_Clamp,			            //2: Clamp
        F_TextureAddressing_Border,			            //3: Border

        F_TextureAddressing_Count,
    };
    foundationExport const String& F_GetTextureAddressingTypeName(FTextureAddressingType type);
    foundationExport const String& F_GetTextureAddressingTypeName(int type);
    foundationExport FTextureAddressingType F_ParseTextureAddressingType(const String& strName);


    enum FTextureBorderColorType
    {
        F_TextureBorderColor_OpaqueBlack = 0,          //0: OpaqueBlack
        F_TextureBorderColor_OpaqueWhite,              //1: OpaqueWhite
        F_TextureBorderColor_TransparentBlack,         //2: TransparentBlack

        F_TextureBorderColor_Count,
    };
    foundationExport const String& F_GetTextureBorderColorTypeName(FTextureBorderColorType type);
    foundationExport const String& F_GetTextureBorderColorTypeName(int type);
    foundationExport FTextureBorderColorType F_ParseTextureBorderColorType(const String& strName);


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
    foundationExport const String& F_GetMSAASampleCountTypeName(FMSAASampleCountType type);
    foundationExport const String& F_GetMSAASampleCountTypeName(int type);
    foundationExport FMSAASampleCountType F_ParseMSAASampleCountType(const String& strName);


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
    foundationExport const String& F_GetShaderTypeName(FShaderType type);
    foundationExport const String& F_GetShaderTypeName(int type);
    foundationExport FShaderType F_ParseShaderType(const String& strName);


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
    foundationExport const String& F_GetCameraTypeName(FCameraType type);
    foundationExport const String& F_GetCameraTypF_GetCameraTypeNameeTypeName(int type);
    foundationExport FCameraType F_ParseCameraType(const String& strName);


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
    foundationExport const String& F_GetRenderPrimitiveTypeName(FRenderPrimitiveType type);
    foundationExport const String& F_GetRenderPrimitiveTypeName(int type);
    foundationExport FRenderPrimitiveType F_ParseRenderPrimitiveType(const String& strName);


    enum FCullingType
	{
		F_Culling_None = 0,	                            //0: None
		F_Culling_ClockWise,				            //1: ClockWise
		F_Culling_CounterClockWise,			            //2: CounterClockWise

        F_Culling_Count,
	};
    foundationExport const String& F_GetCullingTypeName(FCullingType type);
    foundationExport const String& F_GetCullingTypeName(int type);
    foundationExport FCullingType F_ParseCullingType(const String& strName);


    enum FPolygonType
	{
		F_Polygon_Point = 0,	                        //0: Point
		F_Polygon_WireFrame,				            //1: WireFrame
		F_Polygon_Solid,					            //2: Solid

        F_Polygon_Count,
	};
    foundationExport const String& F_GetPolygonTypeName(FPolygonType type);
    foundationExport const String& F_GetPolygonTypeName(int type);
    foundationExport FPolygonType F_ParsePolygonType(const String& strName);


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
    foundationExport const String& F_GetStencilOPTypeName(FStencilOPType type);
    foundationExport const String& F_GetStencilOPTypeName(int type);
    foundationExport FStencilOPType F_ParseStencilOPType(const String& strName);


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
    foundationExport const String& F_GetCompareFuncTypeName(FCompareFuncType type);
    foundationExport const String& F_GetCompareFuncTypeName(int type);
    foundationExport FCompareFuncType F_ParseCompareFuncType(const String& strName);


    enum FSceneBlendingType
	{
		F_SceneBlending_Alpha = 0,		                //0: Alpha
		F_SceneBlending_Color,				            //1: Color
		F_SceneBlending_Add,					        //2: Add
		F_SceneBlending_Modulate,				        //3: Modulate
		F_SceneBlending_Replace,				        //4: Replace

        F_SceneBlending_Count,
	};
    foundationExport const String& F_GetSceneBlendingTypeName(FSceneBlendingType type);
    foundationExport const String& F_GetSceneBlendingTypeName(int type);
    foundationExport FSceneBlendingType F_ParseSceneBlendingType(const String& strName);


    enum FSceneBlendingOPType
	{
		F_SceneBlendingOP_Add = 0,		                //0: Add
		F_SceneBlendingOP_Subtract,			            //1: Subtract
		F_SceneBlendingOP_SubtractReverse,	            //2: SubtractReverse
		F_SceneBlendingOP_Min,				            //3: Min
		F_SceneBlendingOP_Max,					        //4: Max

        F_SceneBlendingOP_Count,
	};
    foundationExport const String& F_GetSceneBlendingOPTypeName(FSceneBlendingOPType type);
    foundationExport const String& F_GetSceneBlendingOPTypeName(int type);
    foundationExport FSceneBlendingOPType F_ParseSceneBlendingOPType(const String& strName);


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
    foundationExport const String& F_GetSceneBlendingFactorTypeName(FSceneBlendingFactorType type);
    foundationExport const String& F_GetSceneBlendingFactorTypeName(int type);
    foundationExport FSceneBlendingFactorType F_ParseSceneBlendingFactorType(const String& strName);


    enum FLightingType
	{
		F_Lighting_Flat = 0,	                        //0: Flat
		F_Lighting_Gouraud,					            //1: Gouraud
		F_Lighting_Phong,						        //2: Phong
        F_Lighting_Pbr,                                 //3: Pbr

        F_Lighting_Count,
	};
    foundationExport const String& F_GetLightingTypeName(FLightingType type);
    foundationExport const String& F_GetLightingTypeName(int type);
    foundationExport FLightingType F_ParseLightingType(const String& strName);


    enum FRenderPipelineType
    {
        F_RenderPipeline_Forward = 0,                   //0: Forward
        F_RenderPipeline_Deferred,                      //1: Deferred

        F_RenderPipeline_Count,
    };
    foundationExport const String& F_GetRenderPipelineTypeName(FRenderPipelineType type);
    foundationExport const String& F_GetRenderPipelineTypeName(int type);
    foundationExport FRenderPipelineType F_ParseRenderPipelineType(const String& strName);


    enum FRenderQueueType
    {
        F_RenderQueue_Background = 0,                   //0: Background:   0    - 1000
        F_RenderQueue_Opaque = 1,                       //1: Opaque:       1000 - 2000
        F_RenderQueue_Transparent = 2,                  //2: Transparent:  2000 - 3000
        F_RenderQueue_Overlay = 3,                      //3: Overlay:      3000 - 4000

        F_RenderQueue_Count,
    };
    foundationExport const String& F_GetRenderQueueTypeName(FRenderQueueType type);
    foundationExport const String& F_GetRenderQueueTypeName(int type);
    foundationExport FRenderQueueType F_ParseRenderQueueType(const String& strName);

    foundationExport int F_GetRenderQueueTypeValue(FRenderQueueType type);
    foundationExport int F_GetRenderQueueTypeValue(int type);
    foundationExport FRenderQueueType F_ParseRenderQueueTypeByValue(int value);
    

    enum FRenderPassType
    {
        F_RenderPass_ForwardLit = 0,                    //0:  ForwardLit
        F_RenderPass_DeferredLit,                       //1:  DeferredLit  
        F_RenderPass_ShadowCaster,                      //2:  ShadowCaster
        F_RenderPass_DepthOnly,                         //3:  DepthOnly

        F_RenderPass_Count,
    };
    foundationExport const String& F_GetRenderPassTypeName(FRenderPassType type);
    foundationExport const String& F_GetRenderPassTypeName(int type);
    foundationExport FRenderPassType F_ParseRenderPassType(const String& strName);


    enum FFrameBufferType 
	{
		F_FrameBuffer_Color		= 0x1,	                //Color
		F_FrameBuffer_Depth		= 0x2,	                //Depth
		F_FrameBuffer_Stencil	= 0x4,	                //Stencil
	};
    

    enum FPixelFormatType                                       // RGBA -> [R->4][G->3][B->2][A->1]  High -> Low
    {
        F_PixelFormat_Unknown = 0,                              //0:   Unknown

        // BYTE CHANNEL    
        F_PixelFormat_BYTE_R4G4_UNORM_PACK8,		            //1:   BYTE_R4G4_UNORM_PACK8	        8bits packed, 4bits Red, 4bits Green (packed unsigned normalized format)                                       
        F_PixelFormat_BYTE_L8_UNORM,                            //2:   BYTE_L8_UNORM                    8bits, 8bits Red, Luminance (unsigned normalized format)
        F_PixelFormat_BYTE_A8_UNORM,                            //3:   BYTE_A8_UNORM		            8bits, 8bits Alpha (unsigned normalized format)
        F_PixelFormat_BYTE_R8_UNORM,                            //4:   BYTE_R8_UNORM                    8bits, 8bits Red (unsigned normalized format)
        F_PixelFormat_BYTE_R8_SNORM,                            //5:   BYTE_R8_SNORM                    8bits, 8bits Red (signed normalized format)
        F_PixelFormat_BYTE_R8_USCALED,                          //6:   BYTE_R8_USCALED                  8bits, 8bits Red (unsigned scaled integer format)
        F_PixelFormat_BYTE_R8_SSCALED,                          //7:   BYTE_R8_SSCALED                  8bits, 8bits Red (signed scaled integer format)
        F_PixelFormat_BYTE_R8_UINT,                             //8:   BYTE_R8_UINT                     8bits, 8bits Red (unsigned integer format)
        F_PixelFormat_BYTE_R8_SINT,                             //9:   BYTE_R8_SINT                     8bits, 8bits Red (signed integer format)
        F_PixelFormat_BYTE_R8_SRGB,                             //10:  BYTE_R8_SRGB                     8bits, 8bits Red (unsigned normalized format with sRGB nonlinear encoding)
        
        F_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16,	            //11:  BYTE_R4G4B4A4_UNORM_PACK16       16bits packed, 4bits Red, 4bits Green, 4bits Blue, 4bits Alpha (packed unsigned normalized format)
		F_PixelFormat_BYTE_B4G4R4A4_UNORM_PACK16,	            //12:  BYTE_B4G4R4A4_UNORM_PACK16       16bits packed, 4bits Blue, 4bits Green, 4bits Red, 4bits Alpha (packed unsigned normalized format)
        F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16,	                //13:  BYTE_R5G6B5_UNORM_PACK16	        16bits packed, 5bits Red, 6bits Green, 5bits Blue (packed unsigned normalized format)
		F_PixelFormat_BYTE_B5G6R5_UNORM_PACK16,		            //14:  BYTE_B5G6R5_UNORM_PACK16	        16bits packed, 5bits Blue, 6bits Green, 5bits Red (packed unsigned normalized format)
		F_PixelFormat_BYTE_R5G5B5A1_UNORM_PACK16,	            //15:  BYTE_R5G5B5A1_UNORM_PACK16       16bits packed, 5bits Red, 5bits Green, 5bits Blue, 1bit Alpha (packed unsigned normalized format)
		F_PixelFormat_BYTE_B5G5R5A1_UNORM_PACK16,		        //16:  BYTE_B5G5R5A1_UNORM_PACK16	    16bits packed, 5bits Blue, 5bits Green, 5bits Red, 1bit Alpha (packed unsigned normalized format)
		F_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16,	            //17:  BYTE_A1R5G5B5_UNORM_PACK16       16bits packed, 1bit Alpha, 5bits Red, 5bits Green, 5bits Blue (packed unsigned normalized format)
        F_PixelFormat_BYTE_AL_UNORM,			                //18:  BYTE_AL_UNORM	                16bits, 8bits Alpha in Green, 8bits luminance in Red (unsigned normalized format)
        F_PixelFormat_BYTE_G8R8_UNORM,                          //19:  BYTE_G8R8_UNORM	                16bits, 8bits Green, 8bits Red (unsigned normalized format)
        F_PixelFormat_BYTE_G8R8_SNORM,                          //20:  BYTE_G8R8_SNORM	                16bits, 8bits Green, 8bits Red (signed normalized format)
        F_PixelFormat_BYTE_G8R8_USCALED,                        //21:  BYTE_G8R8_USCALED	            16bits, 8bits Green, 8bits Red (unsigned scaled integer format)
        F_PixelFormat_BYTE_G8R8_SSCALED,                        //22:  BYTE_G8R8_SSCALED	            16bits, 8bits Green, 8bits Red (signed scaled integer format)
        F_PixelFormat_BYTE_G8R8_UINT,                           //23:  BYTE_G8R8_UINT	                16bits, 8bits Green, 8bits Red (unsigned integer format)
        F_PixelFormat_BYTE_G8R8_SINT,                           //24:  BYTE_G8R8_SINT	                16bits, 8bits Green, 8bits Red (signed integer format)
        F_PixelFormat_BYTE_G8R8_SRGB,                           //25:  BYTE_G8R8_SRGB	                16bits, 8bits Green, 8bits Red (unsigned normalized format with sRGB nonlinear encoding) 
        
        F_PixelFormat_BYTE_R8G8B8_UNORM,                        //26:  BYTE_R8G8B8_UNORM	            24bits, 8bits Red, 8bits Green, 8bits Blue (unsigned normalized format)
        F_PixelFormat_BYTE_R8G8B8_SNORM,                        //27:  BYTE_R8G8B8_SNORM	            24bits, 8bits Red, 8bits Green, 8bits Blue (signed normalized format)
        F_PixelFormat_BYTE_R8G8B8_USCALED,                      //28:  BYTE_R8G8B8_USCALED	            24bits, 8bits Red, 8bits Green, 8bits Blue (unsigned scaled integer format)
        F_PixelFormat_BYTE_R8G8B8_SSCALED,                      //29:  BYTE_R8G8B8_SSCALED	            24bits, 8bits Red, 8bits Green, 8bits Blue (signed scaled integer format)
        F_PixelFormat_BYTE_R8G8B8_UINT,                         //30:  BYTE_R8G8B8_UINT	                24bits, 8bits Red, 8bits Green, 8bits Blue (unsigned integer format)
        F_PixelFormat_BYTE_R8G8B8_SINT,                         //31:  BYTE_R8G8B8_SINT	                24bits, 8bits Red, 8bits Green, 8bits Blue (signed integer format)
        F_PixelFormat_BYTE_R8G8B8_SRGB,                         //32:  BYTE_R8G8B8_SRGB	                24bits, 8bits Red, 8bits Green, 8bits Blue (unsigned normalized format with sRGB nonlinear encoding)
        F_PixelFormat_BYTE_B8G8R8_UNORM,                        //33:  BYTE_B8G8R8_UNORM	            24bits, 8bits Blue, 8bits Green, 8bits Red (unsigned normalized format)
        F_PixelFormat_BYTE_B8G8R8_SNORM,                        //34:  BYTE_B8G8R8_SNORM	            24bits, 8bits Blue, 8bits Green, 8bits Red (signed normalized format)
        F_PixelFormat_BYTE_B8G8R8_USCALED,                      //35:  BYTE_B8G8R8_USCALED	            24bits, 8bits Blue, 8bits Green, 8bits Red (unsigned scaled integer format)
        F_PixelFormat_BYTE_B8G8R8_SSCALED,                      //36:  BYTE_B8G8R8_SSCALED	            24bits, 8bits Blue, 8bits Green, 8bits Red (signed scaled integer format)
        F_PixelFormat_BYTE_B8G8R8_UINT,                         //37:  BYTE_B8G8R8_UINT	                24bits, 8bits Blue, 8bits Green, 8bits Red (unsigned integer format)
        F_PixelFormat_BYTE_B8G8R8_SINT,                         //38:  BYTE_B8G8R8_SINT	                24bits, 8bits Blue, 8bits Green, 8bits Red (signed integer format)
        F_PixelFormat_BYTE_B8G8R8_SRGB,                         //39:  BYTE_B8G8R8_SRGB	                24bits, 8bits Blue, 8bits Green, 8bits Red (unsigned normalized format with sRGB nonlinear encoding)

        F_PixelFormat_BYTE_A8B8G8R8_UNORM_PACK32,               //40:  BYTE_A8B8G8R8_UNORM_PACK32       32bits packed, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (packed unsigned normalized format)
        F_PixelFormat_BYTE_A8B8G8R8_SNORM_PACK32,               //41:  BYTE_A8B8G8R8_SNORM_PACK32       32bits packed, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (packed signed normalized format)
        F_PixelFormat_BYTE_A8B8G8R8_USCALED_PACK32,             //42:  BYTE_A8B8G8R8_USCALED_PACK32     32bits packed, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (packed unsigned scaled integer format)
        F_PixelFormat_BYTE_A8B8G8R8_SSCALED_PACK32,             //43:  BYTE_A8B8G8R8_SSCALED_PACK32     32bits packed, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (packed signed scaled integer format)
        F_PixelFormat_BYTE_A8B8G8R8_UINT_PACK32,                //44:  BYTE_A8B8G8R8_UINT_PACK32        32bits packed, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (packed unsigned integer format)
        F_PixelFormat_BYTE_A8B8G8R8_SINT_PACK32,                //45:  BYTE_A8B8G8R8_SINT_PACK32        32bits packed, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (packed signed integer format)
        F_PixelFormat_BYTE_A8B8G8R8_SRGB_PACK32,                //46:  BYTE_A8B8G8R8_SRGB_PACK32        32bits packed, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (packed unsigned normalized format with sRGB nonlinear encoding)
        F_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32,            //47:  BYTE_A2R10G10B10_UNORM_PACK32    32bits packed, 2bits Alpha, 10bits Red, 10bits Green, 10bits Blue (packed unsigned normalized format)
        F_PixelFormat_BYTE_A2R10G10B10_SNORM_PACK32,            //48:  BYTE_A2R10G10B10_SNORM_PACK32    32bits packed, 2bits Alpha, 10bits Red, 10bits Green, 10bits Blue (packed signed normalized format)
        F_PixelFormat_BYTE_A2R10G10B10_USCALED_PACK32,          //49:  BYTE_A2R10G10B10_USCALED_PACK32  32bits packed, 2bits Alpha, 10bits Red, 10bits Green, 10bits Blue (packed unsigned scaled integer format)
        F_PixelFormat_BYTE_A2R10G10B10_SSCALED_PACK32,          //50:  BYTE_A2R10G10B10_SSCALED_PACK32  32bits packed, 2bits Alpha, 10bits Red, 10bits Green, 10bits Blue (packed signed scaled integer format)
        F_PixelFormat_BYTE_A2R10G10B10_UINT_PACK32,             //51:  BYTE_A2R10G10B10_UINT_PACK32     32bits packed, 2bits Alpha, 10bits Red, 10bits Green, 10bits Blue (packed unsigned integer format)
        F_PixelFormat_BYTE_A2R10G10B10_SINT_PACK32,             //52:  BYTE_A2R10G10B10_SINT_PACK32     32bits packed, 2bits Alpha, 10bits Red, 10bits Green, 10bits Blue (packed signed integer format)
        F_PixelFormat_BYTE_A2B10G10R10_UNORM_PACK32,            //53:  BYTE_A2B10G10R10_UNORM_PACK32    32bits packed, 2bits Alpha, 10bits Blue, 10bits Green, 10bits Red (packed unsigned normalized format)
        F_PixelFormat_BYTE_A2B10G10R10_SNORM_PACK32,            //54:  BYTE_A2B10G10R10_SNORM_PACK32    32bits packed, 2bits Alpha, 10bits Blue, 10bits Green, 10bits Red (packed signed normalized format)
        F_PixelFormat_BYTE_A2B10G10R10_USCALED_PACK32,          //55:  BYTE_A2B10G10R10_USCALED_PACK32  32bits packed, 2bits Alpha, 10bits Blue, 10bits Green, 10bits Red (packed unsigned scaled integer format)
        F_PixelFormat_BYTE_A2B10G10R10_SSCALED_PACK32,          //56:  BYTE_A2B10G10R10_SSCALED_PACK32  32bits packed, 2bits Alpha, 10bits Blue, 10bits Green, 10bits Red (packed signed scaled integer format)
        F_PixelFormat_BYTE_A2B10G10R10_UINT_PACK32,             //57:  BYTE_A2B10G10R10_UINT_PACK32     32bits packed, 2bits Alpha, 10bits Blue, 10bits Green, 10bits Red (packed unsigned integer format)
        F_PixelFormat_BYTE_A2B10G10R10_SINT_PACK32,             //58:  BYTE_A2B10G10R10_SINT_PACK32     32bits packed, 2bits Alpha, 10bits Blue, 10bits Green, 10bits Red (packed signed integer format)
        F_PixelFormat_BYTE_A8R8G8B8_UNORM,                      //59:  BYTE_A8R8G8B8_UNORM    		    32bits, 8bits Alpha, 8bits Red, 8bits Green, 8bits Blue (unsigned normalized format)
        F_PixelFormat_BYTE_A8R8G8B8_SNORM,                      //60:  BYTE_A8R8G8B8_SNORM    		    32bits, 8bits Alpha, 8bits Red, 8bits Green, 8bits Blue (signed normalized format)
        F_PixelFormat_BYTE_A8R8G8B8_USCALED,                    //61:  BYTE_A8R8G8B8_USCALED    		32bits, 8bits Alpha, 8bits Red, 8bits Green, 8bits Blue (unsigned scaled integer format)
        F_PixelFormat_BYTE_A8R8G8B8_SSCALED,                    //62:  BYTE_A8R8G8B8_SSCALED    		32bits, 8bits Alpha, 8bits Red, 8bits Green, 8bits Blue (signed scaled integer format)
        F_PixelFormat_BYTE_A8R8G8B8_UINT,                       //63:  BYTE_A8R8G8B8_UINT    		    32bits, 8bits Alpha, 8bits Red, 8bits Green, 8bits Blue (unsigned integer format)
        F_PixelFormat_BYTE_A8R8G8B8_SINT,                       //64:  BYTE_A8R8G8B8_SINT    		    32bits, 8bits Alpha, 8bits Red, 8bits Green, 8bits Blue (signed integer format)
        F_PixelFormat_BYTE_A8R8G8B8_SRGB,                       //65:  BYTE_A8R8G8B8_SRGB    		    32bits, 8bits Alpha, 8bits Red, 8bits Green, 8bits Blue (unsigned normalized format with sRGB nonlinear encoding)
        F_PixelFormat_BYTE_A8B8G8R8_UNORM,                      //66:  BYTE_A8B8G8R8_UNORM    		    32bits, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (unsigned normalized format)
        F_PixelFormat_BYTE_A8B8G8R8_SNORM,                      //67:  BYTE_A8B8G8R8_SNORM    		    32bits, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (signed normalized format)
        F_PixelFormat_BYTE_A8B8G8R8_USCALED,                    //68:  BYTE_A8B8G8R8_USCALED    		32bits, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (unsigned scaled integer format)
        F_PixelFormat_BYTE_A8B8G8R8_SSCALED,                    //69:  BYTE_A8B8G8R8_SSCALED    		32bits, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (signed scaled integer format)
        F_PixelFormat_BYTE_A8B8G8R8_UINT,                       //70:  BYTE_A8B8G8R8_UINT    		    32bits, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (unsigned integer format)
        F_PixelFormat_BYTE_A8B8G8R8_SINT,                       //71:  BYTE_A8B8G8R8_SINT    		    32bits, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (signed integer format)
        F_PixelFormat_BYTE_A8B8G8R8_SRGB,                       //72:  BYTE_A8B8G8R8_SRGB    		    32bits, 8bits Alpha, 8bits Blue, 8bits Green, 8bits Red (unsigned normalized format with sRGB nonlinear encoding)

        // SHORT CHANNEL
        F_PixelFormat_SHORT_L16_UNORM,                          //73:  SHORT_L16_UNORM                  16bits, 16bits Red, Luminance (unsigned normalized format)
        F_PixelFormat_SHORT_R16_UNORM,				            //74:  SHORT_R16_UNORM			        16bits, 16bits Red (unsigned normalized format)
        F_PixelFormat_SHORT_R16_SNORM,				            //75:  SHORT_R16_SNORM			        16bits, 16bits Red (signed normalized format)
        F_PixelFormat_SHORT_R16_USCALED,				        //76:  SHORT_R16_USCALED			    16bits, 16bits Red (unsigned scaled integer format)
        F_PixelFormat_SHORT_R16_SSCALED,				        //77:  SHORT_R16_SSCALED			    16bits, 16bits Red (signed scaled integer format)
        F_PixelFormat_SHORT_R16_UINT,				            //78:  SHORT_R16_UINT			        16bits, 16bits Red (unsigned integer format)
        F_PixelFormat_SHORT_R16_SINT,				            //79:  SHORT_R16_SINT			        16bits, 16bits Red (signed integer format)
        F_PixelFormat_SHORT_R16_SFLOAT,				            //80:  SHORT_R16_SFLOAT			        16bits, 16bits Red (signed floating-point format)

        F_PixelFormat_SHORT_G16R16_UNORM,                       //81:  SHORT_G16R16_UNORM               32bits, 16bits Green, 16bits Red (unsigned normalized format)
        F_PixelFormat_SHORT_G16R16_SNORM,				        //82:  SHORT_G16R16_SNORM			    32bits, 16bits Green, 16bits Red (signed normalized format)
        F_PixelFormat_SHORT_G16R16_USCALED,		                //83:  SHORT_G16R16_USCALED			    32bits, 16bits Green, 16bits Red (unsigned scaled integer format)
        F_PixelFormat_SHORT_G16R16_SSCALED,		                //84:  SHORT_G16R16_SSCALED			    32bits, 16bits Green, 16bits Red (signed scaled integer format)
        F_PixelFormat_SHORT_G16R16_UINT,				        //85:  SHORT_G16R16_UINT			    32bits, 16bits Green, 16bits Red (unsigned integer format)
        F_PixelFormat_SHORT_G16R16_SINT,				        //86:  SHORT_G16R16_SINT			    32bits, 16bits Green, 16bits Red (signed integer format)
        F_PixelFormat_SHORT_G16R16_SFLOAT,				        //87:  SHORT_G16R16_SFLOAT			    32bits, 16bits Green, 16bits Red (signed floating-point format)
        
        F_PixelFormat_SHORT_B16G16R16_UNORM,                    //88:  SHORT_B16G16R16_UNORM            48bits, 16bits Blue, 16bits Green, 16bits Red (unsigned normalized format)
        F_PixelFormat_SHORT_B16G16R16_SNORM,	                //89:  SHORT_B16G16R16_SNORM			48bits, 16bits Blue, 16bits Green, 16bits Red (signed normalized format)
        F_PixelFormat_SHORT_B16G16R16_USCALED,		            //90:  SHORT_B16G16R16_USCALED			48bits, 16bits Blue, 16bits Green, 16bits Red (unsigned scaled integer format)
        F_PixelFormat_SHORT_B16G16R16_SSCALED,		            //91:  SHORT_B16G16R16_SSCALED			48bits, 16bits Blue, 16bits Green, 16bits Red (signed scaled integer format)
        F_PixelFormat_SHORT_B16G16R16_UINT,		                //92:  SHORT_B16G16R16_UINT			    48bits, 16bits Blue, 16bits Green, 16bits Red (unsigned integer format)
        F_PixelFormat_SHORT_B16G16R16_SINT,			            //93:  SHORT_B16G16R16_SINT			    48bits, 16bits Blue, 16bits Green, 16bits Red (signed integer format)
        F_PixelFormat_SHORT_B16G16R16_SFLOAT,			        //94:  SHORT_B16G16R16_SFLOAT			48bits, 16bits Blue, 16bits Green, 16bits Red (signed floating-point format)
        
        F_PixelFormat_SHORT_A16B16G16R16_UNORM,                 //95:  SHORT_A16B16G16R16_UNORM         64bits, 16bits Alpha, 16bits Blue, 16bits Green, 16bits Red (unsigned normalized format)
        F_PixelFormat_SHORT_A16B16G16R16_SNORM,	                //96:  SHORT_A16B16G16R16_SNORM			64bits, 16bits Alpha, 16bits Blue, 16bits Green, 16bits Red (signed normalized format)
        F_PixelFormat_SHORT_A16B16G16R16_USCALED,		        //97:  SHORT_A16B16G16R16_USCALED	    64bits, 16bits Alpha, 16bits Blue, 16bits Green, 16bits Red (unsigned scaled integer format)
        F_PixelFormat_SHORT_A16B16G16R16_SSCALED,		        //98:  SHORT_A16B16G16R16_SSCALED		64bits, 16bits Alpha, 16bits Blue, 16bits Green, 16bits Red (signed scaled integer format)
        F_PixelFormat_SHORT_A16B16G16R16_UINT,		            //99:  SHORT_A16B16G16R16_UINT		    64bits, 16bits Alpha, 16bits Blue, 16bits Green, 16bits Red (unsigned integer format)
        F_PixelFormat_SHORT_A16B16G16R16_SINT,			        //100: SHORT_A16B16G16R16_SINT			64bits, 16bits Alpha, 16bits Blue, 16bits Green, 16bits Red (signed integer format)
        F_PixelFormat_SHORT_A16B16G16R16_SFLOAT,	            //101: SHORT_A16B16G16R16_SFLOAT	    64bits, 16bits Alpha, 16bits Blue, 16bits Green, 16bits Red (signed floating-point format)

        // INT CHANNEL
        F_PixelFormat_INT_R32_UINT,                             //102: INT_R32_UINT			            32bits, 32bits Red (unsigned integer format)
        F_PixelFormat_INT_R32_SINT,                             //103: INT_R32_SINT			            32bits, 32bits Red (signed integer format)
        F_PixelFormat_INT_R32_SFLOAT,                           //104: INT_R32_SFLOAT			        32bits, 32bits Red (signed floating-point format)

        F_PixelFormat_INT_G32R32_UINT,                          //105: INT_G32R32_UINT		            64bits, 32bits Green, 32bits Red (unsigned integer format)
        F_PixelFormat_INT_G32R32_SINT,                          //106: INT_G32R32_SINT			        64bits, 32bits Green, 32bits Red (signed integer format)
        F_PixelFormat_INT_G32R32_SFLOAT,                        //107: INT_G32R32_SFLOAT			    64bits, 32bits Green, 32bits Red (signed floating-point format)

        F_PixelFormat_INT_B32G32R32_UINT,                       //108: INT_B32G32R32_UINT		        96bits, 32bits Blue, 32bits Green, 32bits Red (unsigned integer format)
        F_PixelFormat_INT_B32G32R32_SINT,                       //109: INT_B32G32R32_SINT			    96bits, 32bits Blue, 32bits Green, 32bits Red (signed integer format)
        F_PixelFormat_INT_B32G32R32_SFLOAT,                     //110: INT_B32G32R32_SFLOAT			    96bits, 32bits Blue, 32bits Green, 32bits Red (signed floating-point format)

        F_PixelFormat_INT_A32B32G32R32_UINT,                    //111: INT_A32B32G32R32_UINT		    128bits, 32bits Alpha, 32bits Blue, 32bits Green, 32bits Red (unsigned integer format)
        F_PixelFormat_INT_A32B32G32R32_SINT,                    //112: INT_A32B32G32R32_SINT			128bits, 32bits Alpha, 32bits Blue, 32bits Green, 32bits Red (signed integer format)
        F_PixelFormat_INT_A32B32G32R32_SFLOAT,                  //113: INT_A32B32G32R32_SFLOAT			128bits, 32bits Alpha, 32bits Blue, 32bits Green, 32bits Red (signed floating-point format)

        // LONG CHANNEL
        F_PixelFormat_LONG_R64_UINT,                            //114: LONG_R64_UINT			        64bits, 64bits Red (unsigned integer format)
        F_PixelFormat_LONG_R64_SINT,                            //115: LONG_R64_SINT			        64bits, 64bits Red (signed integer format)
        F_PixelFormat_LONG_R64_SFLOAT,                          //116: LONG_R64_SFLOAT			        64bits, 64bits Red (signed floating-point format)

        F_PixelFormat_LONG_G64R64_UINT,                         //117: LONG_G64R64_UINT		            128bits, 64bits Green, 64bits Red (unsigned integer format)
        F_PixelFormat_LONG_G64R64_SINT,                         //118: LONG_G64R64_SINT			        128bits, 64bits Green, 64bits Red (signed integer format)
        F_PixelFormat_LONG_G64R64_SFLOAT,                       //119: LONG_G64R64_SFLOAT			    128bits, 64bits Green, 64bits Red (signed floating-point format)

        F_PixelFormat_LONG_B64G64R64_UINT,                      //120: LONG_B64G64R64_UINT		        192bits, 64bits Blue, 64bits Green, 64bits Red (unsigned integer format)
        F_PixelFormat_LONG_B64G64R64_SINT,                      //121: LONG_B64G64R64_SINT			    192bits, 64bits Blue, 64bits Green, 64bits Red (signed integer format)
        F_PixelFormat_LONG_B64G64R64_SFLOAT,                    //122: LONG_B64G64R64_SFLOAT			192bits, 64bits Blue, 64bits Green, 64bits Red (signed floating-point format)

        F_PixelFormat_LONG_A64B64G32R64_UINT,                   //123: LONG_A64B64G32R64_UINT		    256bits, 64bits Alpha, 64bits Blue, 64bits Green, 64bits Red (unsigned integer format)
        F_PixelFormat_LONG_A64B64G32R64_SINT,                   //124: LONG_A64B64G32R64_SINT			256bits, 64bits Alpha, 64bits Blue, 64bits Green, 64bits Red (signed integer format)
        F_PixelFormat_LONG_A64B64G32R64_SFLOAT,                 //125: LONG_A64B64G32R64_SFLOAT			256bits, 64bits Alpha, 64bits Blue, 64bits Green, 64bits Red (signed floating-point format)

        // FLOAT
        F_PixelFormat_FLOAT_B10G11R11_UFLOAT_PACK32,            //126: FLOAT_B10G11R11_UFLOAT_PACK32    32bits packed, 10bits Blue, 11bits Green, 11bits Red (packed unsigned floating-point format)
        F_PixelFormat_FLOAT_E5B9G9R9_UFLOAT_PACK32,             //127: FLOAT_E5B9G9R9_UFLOAT_PACK32     32bits packed, 5bits exponent, 9bits Blue, 9bits Green, 9bits Red (packed unsigned floating-point format)

        // STENCIL
        F_PixelFormat_STENCIL_S8_UINT,                          //128: STENCIL_S8_UINT                  8bits, 8bits stencil (unsigned integer format)

        // DEPTH
        F_PixelFormat_DEPTH_D16_UNORM,                          //129: DEPTH_D16_UNORM                  16bits, 16bits depth (unsigned normalized format)
        F_PixelFormat_DEPTH_D24_UNORM,                          //130: DEPTH_D24_UNORM                  32bits, 24bits depth (unsigned normalized format), 8bits unused
        F_PixelFormat_DEPTH_D32_SFLOAT,                         //131: DEPTH_D32_SFLOAT                 32bits, 32bits depth (signed floating-point format)

        // DEPTHSTENCIL
		F_PixelFormat_DEPTHSTENCIL_D16_UNORM_S8_UINT,           //132: EPTHSTENCIL_D16_UNORM_S8_UINT    24bits, 16bits unsigned normalized depth, 8bits unsigned integer stencil
		F_PixelFormat_DEPTHSTENCIL_D24_UNORM_S8_UINT,           //133: DEPTHSTENCIL_D24_UNORM_S8_UINT   32bits, 24bits unsigned normalized depth, 8bits unsigned integer stencil
		F_PixelFormat_DEPTHSTENCIL_D32_SFLOAT_S8_UINT,	        //134: DEPTHSTENCIL_D32_SFLOAT_S8_UINT  64bits, 32bits signed float depth, 8bits unsigned integer stencil, 24bits unused
        
        // DXT
        F_PixelFormat_DXT1,                                     //135: DXT1   DDS DXT1
        F_PixelFormat_DXT2,                                     //136: DXT2   DDS DXT2
        F_PixelFormat_DXT3,                                     //137: DXT3   DDS DXT3
        F_PixelFormat_DXT4,                                     //138: DXT4   DDS DXT4
        F_PixelFormat_DXT5,                                     //139: DXT5   DDS DXT5

        // BC
        F_PixelFormat_BC4_UNORM,                                //140: DDS BC4 format (unsigned normalized)
        F_PixelFormat_BC4_SNORM,                                //141: DDS BC4 format (signed normalized)
        F_PixelFormat_BC5_UNORM,                                //142: DDS BC5 format (unsigned normalized)
        F_PixelFormat_BC5_SNORM,                                //143: DDS BC5 format (signed normalized)
        F_PixelFormat_BC6H_UF16,                                //144: DDS BC6H format (unsigned 16bits normalized)
        F_PixelFormat_BC6H_SF16,                                //145: DDS BC6H format (signed 16bits normalized)
        F_PixelFormat_BC7_UNORM,                                //146: DDS BC7 format (unsigned normalized)

        // PVRTC (PowerVR)
        F_PixelFormat_PVRTC_RGB2,				                //147: PVRTC RGB 2 bpp
        F_PixelFormat_PVRTC_RGBA2,			                    //148: PVRTC RGBA 2 bpp
        F_PixelFormat_PVRTC_RGB4,				                //149: PVRTC RGB 4 bpp
        F_PixelFormat_PVRTC_RGBA4,			                    //150: PVRTC RGBA 4 bpp
        F_PixelFormat_PVRTC2_2BPP,			                    //151: PVRTC Version 2, 2 bpp
        F_PixelFormat_PVRTC2_4BPP,			                    //152: PVRTC Version 2, 4 bpp

		// ETC
        F_PixelFormat_ETC1_RGB8,				                //153: ETC1, RGB8
        F_PixelFormat_ETC2_RGB8,				                //154: ETC2, RGB8
        F_PixelFormat_ETC2_RGBA8,				                //155: ETC2, RGBA8
        F_PixelFormat_ETC2_RGB8A1,			                    //156: ETC2, RGBA1

		// ATC
        F_PixelFormat_ATC_RGB,				                    //157: ATC, RGB
        F_PixelFormat_ATC_RGBA_EXPLICIT_ALPHA,                  //158: ATC, RGBA
        F_PixelFormat_ATC_RGBA_INTERPOLATED_ALPHA,              //159: ATC, RGBA

        // ASTC
        F_PixelFormat_ASTC_RGBA_4X4_LDR,		                //160: ASTC, RGBA,block size  4X4
        F_PixelFormat_ASTC_RGBA_5X4_LDR,		                //161: ASTC, RGBA,block size  5X4
        F_PixelFormat_ASTC_RGBA_5X5_LDR,		                //162: ASTC, RGBA,block size  5X5
        F_PixelFormat_ASTC_RGBA_6X5_LDR,		                //163: ASTC, RGBA,block size  6X5
        F_PixelFormat_ASTC_RGBA_6X6_LDR,		                //164: ASTC, RGBA,block size  6X6
        F_PixelFormat_ASTC_RGBA_8X5_LDR,		                //165: ASTC, RGBA,block size  8X5
        F_PixelFormat_ASTC_RGBA_8X6_LDR,		                //166: ASTC, RGBA,block size  8X6
        F_PixelFormat_ASTC_RGBA_8X8_LDR,		                //167: ASTC, RGBA,block size  8X8
        F_PixelFormat_ASTC_RGBA_10X5_LDR,		                //168: ASTC, RGBA,block size  10X5
        F_PixelFormat_ASTC_RGBA_10X6_LDR,		                //169: ASTC, RGBA,block size  10X6
        F_PixelFormat_ASTC_RGBA_10X8_LDR,		                //170: ASTC, RGBA,block size  10X8
        F_PixelFormat_ASTC_RGBA_10X10_LDR,	                    //171: ASTC, RGBA,block size  10X10
        F_PixelFormat_ASTC_RGBA_12X10_LDR,	                    //172: ASTC, RGBA,block size  12X10
        F_PixelFormat_ASTC_RGBA_12X12_LDR,	                    //173: ASTC, RGBA,block size  12X12

		F_PixelFormat_Count,                                    //174: Count

    #if VULKAN_ENDIAN == VULKAN_ENDIAN_BIG
		F_PixelFormat_BYTE_RGB_UNORM  = F_PixelFormat_BYTE_R8G8B8_UNORM,
		F_PixelFormat_BYTE_BGR_UNORM  = F_PixelFormat_BYTE_B8G8R8_UNORM,
		F_PixelFormat_BYTE_BGRA_UNORM = F_PixelFormat_BYTE_A8B8G8R8_UNORM,
		F_PixelFormat_BYTE_RGBA_UNORM = F_PixelFormat_BYTE_A8R8G8B8_UNORM,
	#else
		F_PixelFormat_BYTE_RGB_UNORM  = F_PixelFormat_BYTE_B8G8R8_UNORM,
		F_PixelFormat_BYTE_BGR_UNORM  = F_PixelFormat_BYTE_R8G8B8_UNORM,
		F_PixelFormat_BYTE_BGRA_UNORM = F_PixelFormat_BYTE_A8R8G8B8_UNORM,
		F_PixelFormat_BYTE_RGBA_UNORM = F_PixelFormat_BYTE_A8B8G8R8_UNORM,
	#endif
    };

    struct FPixelFormatDes
	{
		String name;
		uint8 nElemBytes;
		uint32 eFlags;
		uint32 eComponentType;
		uint32 nComponentCount;
        bool isSupported;
		
		uint8 nRbits;
		uint8 nGbits;
		uint8 nBbits;
		uint8 nAbits;
		
		uint64 nRmask;
		uint64 nGmask;
		uint64 nBmask;
		uint64 nAmask;
	
		uint8 nRshift;
		uint8 nGshift;
		uint8 nBshift;
		uint8 nAshift;
	};


    enum FPixelFormatFlagType
	{
        F_PixelFormatFlag_IsNative		    = 0x00000001,   //0: IsNative
        F_PixelFormatFlag_IsCompressed	    = 0x00000002,   //1: IsCompressed
        F_PixelFormatFlag_IsInteger	        = 0x00000004,   //2: IsInteger
        F_PixelFormatFlag_IsFloat	        = 0x00000008,   //3: IsFloat
		F_PixelFormatFlag_IsLuminance	    = 0x00000010,   //4: IsLuminance
        F_PixelFormatFlag_IsStencil         = 0x00000020,   //5: IsStencil
		F_PixelFormatFlag_IsDepth		    = 0x00000040,   //6: IsDepth
        F_PixelFormatFlag_IsDepthStencil    = 0x00000080,   //7: IsDepthStencil
		F_PixelFormatFlag_HasAlpha		    = 0x00000100,   //8: HasAlpha
	};


    enum FPixelFormatComponentType
	{
		F_PixelFormatComponent_ByteU = 0,              //0: Byte unsigned
        F_PixelFormatComponent_ByteS,                  //1: Byte signed
		F_PixelFormatComponent_ShortU,                 //2: Short unsigned
        F_PixelFormatComponent_ShortS,                 //3: Short signed
        F_PixelFormatComponent_IntU,				   //4: Int unsigned
		F_PixelFormatComponent_IntS,				   //5: Int signed
        F_PixelFormatComponent_LongU,				   //6: Long unsigned
		F_PixelFormatComponent_LongS,				   //7: Long signed
		F_PixelFormatComponent_Float16,                //8: Float 16
		F_PixelFormatComponent_Float32,                //9: Float 32
        F_PixelFormatComponent_Double,                 //10: Double
	};
    const String& F_GetPixelFormatComponentTypeName(FPixelFormatComponentType type);
    const String& F_GetPixelFormatComponentTypeName(int type);


}; //LostPeterFoundation

#endif