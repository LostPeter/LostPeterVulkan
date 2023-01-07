/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-30
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _PRE_MACRO_H_
#define _PRE_MACRO_H_

//Compiler
#define UTIL_COMPILER_MSVC				1
#define UTIL_COMPILER_GNUC				2
#define UTIL_COMPILER_GCCE				3
#define UTIL_COMPILER_CLANG				4

//Platform
#define UTIL_PLATFORM_IOS				1
#define UTIL_PLATFORM_ANDROID			2
#define UTIL_PLATFORM_WIN32				3
#define UTIL_PLATFORM_MAC				4
#define UTIL_PLATFORM_LINUX				5

//Architecture
#define UTIL_ARCHITECTURE_32			1
#define UTIL_ARCHITECTURE_64			2

///////////////////////////// Platform //////////////////////////////////////////////
#if defined(__APPLE_CC__)				//ios
	#undef  UTIL_PLATFORM
	#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
		#define UTIL_PLATFORM			UTIL_PLATFORM_IOS
	#else
		#define UTIL_PLATFORM			UTIL_PLATFORM_MAC
	#endif
#elif defined(__ANDROID__)				//android
	#undef  UTIL_PLATFORM
	#define UTIL_PLATFORM				UTIL_PLATFORM_ANDROID
#elif defined(LINUX)					//linux
	#undef  UTIL_PLATFORM
	#define UTIL_PLATFORM				UTIL_PLATFORM_LINUX
#elif defined(WIN32) 					//win32
	#undef  UTIL_PLATFORM
	#define UTIL_PLATFORM				UTIL_PLATFORM_WIN32
	#pragma warning (disable:4127)  
#else
	#pragma error "UnKnown platform! Abort! Abort!"
#endif


#if UTIL_PLATFORM == UTIL_PLATFORM_WIN32
	#if defined(UTIL_EXPORTS)
		#define utilExport        		__declspec(dllexport) 
	#else
		#if defined(UTIL_IMPORTS)
			#define utilExport      	__declspec(dllimport) 
		#else
			#define utilExport
		#endif
	#endif
#elif UTIL_PLATFORM == UTIL_PLATFORM_ANDROID
	#define utilExport					__attribute__ ((visibility("default")))
#else
	#define utilExport
#endif


///////////////////////////// Architecture //////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
	#define UTIL_ARCHITECTURE			UTIL_ARCHITECTURE_64
#else
	#define UTIL_ARCHITECTURE			UTIL_ARCHITECTURE_32
#endif

///////////////////////////// Architecture //////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
	#define UTIL_ARCHITECTURE			UTIL_ARCHITECTURE_64
#else
	#define UTIL_ARCHITECTURE			UTIL_ARCHITECTURE_32
#endif

///////////////////////////// Endian ////////////////////////////////////////////////
#ifdef UTIL_CONFIG_ENDIAN_BIG
	#define UTIL_ENDIAN					UTIL_ENDIAN_BIG
#else
	#define UTIL_ENDIAN					UTIL_ENDIAN_LITTLE
#endif

///////////////////////////// Settings //////////////////////////////////////////////
#ifdef NDEBUG
	#define UTIL_DEBUG					0
#else
	#define UTIL_DEBUG					1
#endif


#endif