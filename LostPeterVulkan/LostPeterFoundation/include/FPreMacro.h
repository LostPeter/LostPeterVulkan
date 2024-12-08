/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_PRE_MACRO_H_
#define _F_PRE_MACRO_H_

//Compiler
#define F_COMPILER_MSVC					1
#define F_COMPILER_GNUC					2
#define F_COMPILER_GCCE					3
#define F_COMPILER_CLANG				4
#define F_COMPILER_WINSCW				5

//Platform
#define F_PLATFORM_WINDOW				1
#define F_PLATFORM_MAC				    2
#define F_PLATFORM_LINUX				3
#define F_PLATFORM_ANDROID			    4
#define F_PLATFORM_iOS				    5


//CPU
#define F_CPU_UNKNOWN    				0
#define F_CPU_X86        				1
#define F_CPU_ARM        				2
#define F_CPU_PPC        				3
#define F_CPU_MIPS       				4


//Architecture
#define F_ARCHITECTURE_32			    1
#define F_ARCHITECTURE_64			    2


//FixPoint
#define F_FIXPOINT_32					1
#define F_FIXPOINT_64					2


///////////////////////////// Compiler type and version /////////////////////////////
#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined(__ANDROID__)
	#define F_COMPILER					F_COMPILER_GNUC
	#define F_COMPILER_VERSION			470
#elif defined( __GCCE__ )
	#define F_COMPILER					F_COMPILER_GCCE
	#define F_COMPILER_VERSION			_MSC_VER
#elif defined( __WINSCW__ )
	#define F_COMPILER					F_COMPILER_WINSCW
	#define F_COMPILER_VERSION			_MSC_VER
#elif defined( _MSC_VER )
	#define F_COMPILER					F_COMPILER_MSVC
	#define F_COMPILER_VERSION			_MSC_VER
#elif defined( __clang__ )
	#define F_COMPILER					F_COMPILER_CLANG
	#define F_COMPILER_VERSION (((__clang_major__)*100) + \
	(__clang_minor__ * 10) + \
	__clang_patchlevel__)
#elif defined( __GNUC__ )
	#define F_COMPILER					F_COMPILER_GNUC
	#define F_COMPILER_VERSION (((__GNUC__)*100) + \
	(__GNUC_MINOR__ * 10) + \
	__GNUC_PATCHLEVEL__)
#else
	#pragma error "UnKnown compiler! Abort! Abort!"
#endif


///////////////////////////// Platform //////////////////////////////////////////////
#if defined(__ANDROID__)				//android
	#undef  F_PLATFORM
	#define F_PLATFORM				F_PLATFORM_ANDROID
#elif defined(__APPLE_CC__)				//ios
	#undef  F_PLATFORM
	#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
		#define F_PLATFORM			F_PLATFORM_iOS
	#else
		#define F_PLATFORM			F_PLATFORM_MAC
	#endif
#elif defined(LINUX)					//linux
	#undef  F_PLATFORM
	#define F_PLATFORM				F_PLATFORM_LINUX
#elif defined(WIN32) 					//win32
	#undef  F_PLATFORM
	#define F_PLATFORM				F_PLATFORM_WINDOW
	#pragma warning (disable:4127)  
#else
	#pragma error "UnKnown platform! Abort! Abort!"
#endif


///////////////////////////// CPU ///////////////////////////////////////////////////
#if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64) || defined(_M_AMD64)
	#define F_CPU 		F_CPU_X86
#elif defined(__ppc__) || defined(__ppc64__) || defined(_M_PPC)
	#define F_CPU 		F_CPU_PPC
#elif defined(__arm__) || defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
	#define F_CPU 		F_CPU_ARM
#elif defined(__mips__) || defined(__mips64) || defined(__mips64_) || defined(_M_MIPS)
	#define F_CPU 		F_CPU_MIPS
#else
	#define F_CPU 		F_CPU_UNKNOWN
#endif


///////////////////////////// Architecture //////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
	#define F_ARCHITECTURE			F_ARCHITECTURE_64
#else
	#define F_ARCHITECTURE			F_ARCHITECTURE_32
#endif


///////////////////////////// Endian ////////////////////////////////////////////////
#ifdef F_CONFIG_ENDIAN_BIG
	#define F_ENDIAN			    F_ENDIAN_BIG
#else
	#define F_ENDIAN				F_ENDIAN_LITTLE
#endif


///////////////////////////// FixPoint //////////////////////////////////////////////
#if defined(F_CONFIG_FIXPOINT_64)
	#define F_FIXPOINT					F_FIXPOINT_64
	#define F_FIXPOINT_FRACTIONAL_BITS 	32
#else
	#define F_FIXPOINT					F_FIXPOINT_32
	#define F_FIXPOINT_FRACTIONAL_BITS 	16
#endif


///////////////////////////// Alignment /////////////////////////////////////////////
#if F_COMPILER == F_COMPILER_MSVC
	#define F_ALIGNED_DECLARE(type, var, alignment)  __declspec(align(alignment)) type var
#elif F_COMPILER == F_COMPILER_GNUC
	#define F_ALIGNED_DECLARE(type, var, alignment)  type var __attribute__((__aligned__(alignment)))
#else
	#define F_ALIGNED_DECLARE(type, var, alignment)  type var
#endif


///////////////////////////// SIMD //////////////////////////////////////////////////
#if F_CPU == F_CPU_X86
	#define F_SIMD_ALIGNMENT  16
#else
	#define F_SIMD_ALIGNMENT  16
#endif

#define F_SIMD_ALIGNED_DECLARE(type, var)   F_ALIGNED_DECLARE(type, var, F_SIMD_ALIGNMENT)

// Support SSE ?
#if F_DOUBLE_PRECISION == 0 && F_CPU == F_CPU_X86 && F_COMPILER == F_COMPILER_MSVC
	#define F_HAVE_SSE  	1
#elif F_DOUBLE_PRECISION == 0 && F_CPU == F_CPU_X86 && (F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG) && F_PLATFORM != F_PLATFORM_iOS
	#define F_HAVE_SSE  	1
#endif

// Support VFP ?
#if F_DOUBLE_PRECISION == 0 && F_CPU == F_CPU_ARM && (F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG) && defined(__VFP_FP__)
	#define F_HAVE_VFP  	1
#endif

// Support NEON ?
#if F_DOUBLE_PRECISION == 0 && F_CPU == F_CPU_ARM && (F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG) && defined(__ARM_NEON__)
	#define F_HAVE_NEON  	1
#endif

// Support MSA ?
#if F_DOUBLE_PRECISION == 0 && F_CPU == F_CPU_MIPS && (F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG) && defined(__mips_msa)
	#define F_HAVE_MSA  	1
#endif


#ifndef F_HAVE_SSE
	#define F_HAVE_SSE  	0
#endif

#ifndef F_HAVE_VFP
	#define F_HAVE_VFP  	0
#endif

#ifndef F_HAVE_NEON
	#define F_HAVE_NEON  	0
#endif

#ifndef F_HAVE_MSA
	#define F_HAVE_MSA  	0
#endif


///////////////////////////// Settings //////////////////////////////////////////////
#ifdef NDEBUG
	#define F_DEBUG					0
#else
	#define F_DEBUG					1
#endif


#if F_DEBUG == 1
	#define F_BUILD_SUFFIX			"_d"
#else
	#define F_BUILD_SUFFIX			""
#endif


#if F_PLATFORM == F_PLATFORM_WINDOW
	#if defined(FOUNDATION_EXPORTS)
		#define foundationExport        __declspec(dllexport) 
	#else
		#define foundationExport      	__declspec(dllimport) 
	#endif
#elif F_PLATFORM == F_PLATFORM_ANDROID
	#define foundationExport			__attribute__ ((visibility("default")))
#else
	#define foundationExport
#endif


#if F_COMPILER == F_COMPILER_MSVC
	#if F_COMPILER_VERSION >= 1200
		#define F_FORCEINLINE			__forceinline
	#endif
#elif defined(__MINGW32__)
	#if !defined(F_FORCEINLINE)
		#define F_FORCEINLINE			__inline
	#endif
#else
	#define F_FORCEINLINE				__inline
#endif


#endif