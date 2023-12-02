/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _PRE_MACRO_H_
#define _PRE_MACRO_H_

//Compiler
#define E_COMPILER_MSVC					1
#define E_COMPILER_GNUC					2
#define E_COMPILER_GCCE					3
#define E_COMPILER_CLANG				4

//Platform
#define E_PLATFORM_WINDOW				1
#define E_PLATFORM_MAC					2
#define E_PLATFORM_LINUX				3
#define E_PLATFORM_ANDROID				4
#define E_PLATFORM_iOS					5

//Architecture
#define E_ARCHITECTURE_32				1
#define E_ARCHITECTURE_64				2


///////////////////////////// Compiler type and version /////////////////////////////
#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined(__ANDROID__)
	#define E_COMPILER					E_COMPILER_GNUC
	#define E_COMPILER_VERSION			470
#elif defined( __GCCE__ )
	#define E_COMPILER					E_COMPILER_GCCE
	#define E_COMPILER_VERSION			_MSC_VER
#elif defined( __WINSCW__ )
	#define E_COMPILER					E_COMPILER_WINSCW
	#define E_COMPILER_VERSION			_MSC_VER
#elif defined( _MSC_VER )
	#define E_COMPILER					E_COMPILER_MSVC
	#define E_COMPILER_VERSION			_MSC_VER
#elif defined( __clang__ )
	#define E_COMPILER					E_COMPILER_CLANG
	#define E_COMPILER_VERSION (((__clang_major__)*100) + \
	(__clang_minor__ * 10) + \
	__clang_patchlevel__)
#elif defined( __GNUC__ )
	#define E_COMPILER					E_COMPILER_GNUC
	#define E_COMPILER_VERSION (((__GNUC__)*100) + \
	(__GNUC_MINOR__ * 10) + \
	__GNUC_PATCHLEVEL__)
#else
	#pragma error "UnKnown compiler! Abort! Abort!"
#endif

//Forceinline
#if E_COMPILER == E_COMPILER_MSVC
	#if E_COMPILER_VERSION >= 1200
		#define E_FORCEINLINE			__forceinline
	#endif
#elif defined(__MINGW32__)
	#if !defined(E_FORCEINLINE)
		#define E_FORCEINLINE			__inline
	#endif
#else
	#define E_FORCEINLINE				__inline
#endif


///////////////////////////// Platform //////////////////////////////////////////////
#if defined(__APPLE_CC__)				//ios
	#undef  E_PLATFORM
	#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
		#define E_PLATFORM				E_PLATFORM_iOS
	#else
		#define E_PLATFORM				E_PLATFORM_MAC
	#endif
#elif defined(__ANDROID__)				//android
	#undef  E_PLATFORM
	#define E_PLATFORM					E_PLATFORM_ANDROID
#elif defined(LINUX)					//linux
	#undef  E_PLATFORM
	#define E_PLATFORM					E_PLATFORM_LINUX
#elif defined(WIN32) 					//win32
	#undef  E_PLATFORM
	#define E_PLATFORM					E_PLATFORM_WINDOW
	#pragma warning (disable:4127)  
#else
	#pragma error "UnKnown platform! Abort! Abort!"
#endif


#if E_PLATFORM == E_PLATFORM_WINDOW
	#if defined(ENGINE_EXPORTS)
		#define engineExport        	__declspec(dllexport) 
	#else
		#if defined(ENGINE_IMPORTS)
			#define engineExport      	__declspec(dllimport) 
		#else
			#define engineExport
		#endif
	#endif
#elif E_PLATFORM == E_PLATFORM_ANDROID
	#define engineExport				__attribute__ ((visibility("default")))
#else
	#define engineExport
#endif


///////////////////////////// Architecture //////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
	#define E_ARCHITECTURE				E_ARCHITECTURE_64
#else
	#define E_ARCHITECTURE				E_ARCHITECTURE_32
#endif

///////////////////////////// Architecture //////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
	#define E_ARCHITECTURE				E_ARCHITECTURE_64
#else
	#define E_ARCHITECTURE				E_ARCHITECTURE_32
#endif

///////////////////////////// Endian ////////////////////////////////////////////////
#ifdef E_CONFIG_ENDIAN_BIG
	#define E_ENDIAN					E_ENDIAN_BIG
#else
	#define E_ENDIAN					E_ENDIAN_LITTLE
#endif

///////////////////////////// Settings //////////////////////////////////////////////
#ifdef NDEBUG
	#define E_DEBUG						0
#else
	#define E_DEBUG						1
#endif


#endif