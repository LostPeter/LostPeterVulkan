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
#define LP_COMPILER_MSVC				1
#define LP_COMPILER_GNUC				2
#define LP_COMPILER_GCCE				3
#define LP_COMPILER_CLANG				4

//Platform
#define LP_PLATFORM_IOS				    1
#define LP_PLATFORM_ANDROID			    2
#define LP_PLATFORM_WIN32				3
#define LP_PLATFORM_MAC				    4
#define LP_PLATFORM_LINUX				5

//Architecture
#define LP_ARCHITECTURE_32			    1
#define LP_ARCHITECTURE_64			    2


///////////////////////////// Compiler type and version /////////////////////////////
#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined(__ANDROID__)
	#define LP_COMPILER					LP_COMPILER_GNUC
	#define LP_COMPILER_VERSION			470
#elif defined( __GCCE__ )
	#define LP_COMPILER					LP_COMPILER_GCCE
	#define LP_COMPILER_VERSION			_MSC_VER
#elif defined( __WINSCW__ )
	#define LP_COMPILER					LP_COMPILER_WINSCW
	#define LP_COMPILER_VERSION			_MSC_VER
#elif defined( _MSC_VER )
	#define LP_COMPILER					LP_COMPILER_MSVC
	#define LP_COMPILER_VERSION			_MSC_VER
#elif defined( __clang__ )
	#define LP_COMPILER					LP_COMPILER_CLANG
	#define LP_COMPILER_VERSION (((__clang_major__)*100) + \
	(__clang_minor__ * 10) + \
	__clang_patchlevel__)
#elif defined( __GNUC__ )
	#define LP_COMPILER					LP_COMPILER_GNUC
	#define LP_COMPILER_VERSION (((__GNUC__)*100) + \
	(__GNUC_MINOR__ * 10) + \
	__GNUC_PATCHLEVEL__)
#else
	#pragma error "UnKnown compiler! Abort! Abort!"
#endif

//Forceinline
#if LP_COMPILER == LP_COMPILER_MSVC
	#if LP_COMPILER_VERSION >= 1200
		#define LP_FORCEINLINE			__forceinline
	#endif
#elif defined(__MINGW32__)
	#if !defined(LP_FORCEINLINE)
		#define LP_FORCEINLINE			__inline
	#endif
#else
	#define LP_FORCEINLINE				__inline
#endif


///////////////////////////// Platform //////////////////////////////////////////////
#if defined(__APPLE_CC__)				//ios
	#undef  LP_PLATFORM
	#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
		#define LP_PLATFORM			LP_PLATFORM_IOS
	#else
		#define LP_PLATFORM			LP_PLATFORM_MAC
	#endif
#elif defined(__ANDROID__)				//android
	#undef  LP_PLATFORM
	#define LP_PLATFORM				LP_PLATFORM_ANDROID
#elif defined(LINUX)					//linux
	#undef  LP_PLATFORM
	#define LP_PLATFORM				LP_PLATFORM_LINUX
#elif defined(WIN32) 					//win32
	#undef  LP_PLATFORM
	#define LP_PLATFORM				LP_PLATFORM_WIN32
	#pragma warning (disable:4127)  
#else
	#pragma error "UnKnown platform! Abort! Abort!"
#endif


#if LP_PLATFORM == LP_PLATFORM_WIN32
	#if defined(LPF_EXPORTS)
		#define LPF_Export        		__declspec(dllexport) 
	#else
		#if defined(LPF_IMPORTS)
			#define LPF_Export      	__declspec(dllimport) 
		#else
			#define LPF_Export
		#endif
	#endif
#elif LP_PLATFORM == LP_PLATFORM_ANDROID
	#define LPF_Export					__attribute__ ((visibility("default")))
#else
	#define LPF_Export
#endif


///////////////////////////// Architecture //////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
	#define LP_ARCHITECTURE			LP_ARCHITECTURE_64
#else
	#define LP_ARCHITECTURE			LP_ARCHITECTURE_32
#endif

///////////////////////////// Endian ////////////////////////////////////////////////
#ifdef LP_CONFIG_ENDIAN_BIG
	#define LP_ENDIAN			    LP_ENDIAN_BIG
#else
	#define LP_ENDIAN				LP_ENDIAN_LITTLE
#endif

///////////////////////////// Settings //////////////////////////////////////////////
#ifdef NDEBUG
	#define LP_DEBUG					0
#else
	#define LP_DEBUG					1
#endif

#endif