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


//Architecture
#define F_ARCHITECTURE_32			    1
#define F_ARCHITECTURE_64			    2


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

//Forceinline
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


///////////////////////////// Platform //////////////////////////////////////////////
#if defined(__APPLE_CC__)				//ios
	#undef  F_PLATFORM
	#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
		#define F_PLATFORM			F_PLATFORM_iOS
	#else
		#define F_PLATFORM			F_PLATFORM_MAC
	#endif
#elif defined(__ANDROID__)				//android
	#undef  F_PLATFORM
	#define F_PLATFORM				F_PLATFORM_ANDROID
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

///////////////////////////// Settings //////////////////////////////////////////////
#ifdef NDEBUG
	#define F_DEBUG					0
#else
	#define F_DEBUG					1
#endif

#endif