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


    #define F_ARRAYSIZE(_ARR)                   ((int)(sizeof(_ARR)/sizeof(*_ARR)))    
    #define F_OFFSETOF(_TYPE,_MEMBER)           ((size_t)&(((_TYPE*)0)->_MEMBER))     
    #define F_DELETE(p)                         { if(p) { delete p; p=nullptr; }}
    #define F_DELETE_T(p)                       { if(p) { delete[] p; p=nullptr; }}


////////////////////////////// Enum ////////////////////////////////
    enum FLogType
    {
        F_Log_Console = 0,                         //0:    Console
        F_Log_File,                                //1:    File
    };


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

    typedef glm::vec2 FPoint;
    typedef glm::vec2 FSize;
    typedef glm::vec2 FVector2;
    typedef glm::vec3 FVector3;
    typedef glm::vec4 FVector4;
    typedef glm::mat3 FMatrix3;
    typedef glm::mat4 FMatrix4;
    typedef glm::quat FQuaternion;
    typedef glm::vec4 FColor;
    typedef std::vector<FPoint> FPointVector;
    typedef std::vector<FSize> FSizeVector;
    typedef std::vector<FVector2> FVector2Vector;
    typedef std::vector<FVector3> FVector3Vector;
    typedef std::vector<FVector4> FVector4Vector;
    typedef std::vector<FMatrix3> FMatrix3Vector;
    typedef std::vector<FMatrix4> FMatrix4Vector;
    typedef std::vector<FQuaternion> FQuaternionVector;
    typedef std::vector<FColor> FColorVector;
    typedef std::map<size_t, FVector3> FIndex2Vector3Map;


    class FBitwise;
    class FBox;
    class FCamera;
    class FMath;
    class FLog;
    class FLogConsole;
    class FLogFile;
    class FLogManager;
    class FTimer;
    class FUtil;
    class FUtilString;


    typedef std::vector<FLog*> FLogPtrVector;
    typedef std::map<String, FLog*> VFLogPtrMap;


}; //LostPeterFoundation

#endif