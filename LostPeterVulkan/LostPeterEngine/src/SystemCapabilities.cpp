/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SystemCapabilities.h"

#if F_CPU == F_CPU_X86
	#include "libcpuid.h"
#elif F_CPU == F_CPU_ARM

#elif F_CPU == F_CPU_PPC

#elif F_CPU == F_CPU_MIPS

#endif

#if F_COMPILER == F_COMPILER_MSVC
	#include <excpt.h>
	#include <instrin.h>
#elif F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG
	#include <signal.h>
	#include <setjmp.h>

	#if F_PLATFORM == F_PLATFORM_ANDROID
		#include <cpu-features.h>
	#elif F_CPU == F_CPU_ARM && F_PLATFORM == F_PLATFORM_iOS
		#include <sys/sysctl.h>
		#if __MACH__
			#include <mach/machine.h>
			#ifndef CPU_SUBTYPE_ARM64_V8
                #define CPU_SUBTYPE_ARM64_V8 ((cpu_subtype_t) 1)
            #endif
            #ifndef CPU_SUBTYPE_ARM_V8
                #define CPU_SUBTYPE_ARM_V8 ((cpu_subtype_t) 13)
            #endif
		#endif
	#endif	
#endif

template<> LostPeterEngine::SystemCapabilities* LostPeterFoundation::FSingleton<LostPeterEngine::SystemCapabilities>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
#if F_CPU == F_CPU_X86
	struct CpuidResult
    {
        // Note: DO NOT CHANGE THE ORDER, some code based on that.
        uint32 _eax;
        uint32 _ebx;
        uint32 _edx;
        uint32 _ecx;
    };

	#if F_COMPILER == F_COMPILER_MSVC
		#pragma warning(push)
		#pragma warning(disable: 4035)
	#endif

	static int32 s_IsSupportCpuid()
	{
	#if F_COMPILER == F_COMPILER_MSVC
		#if _MSC_VER >= 1400 && defined(_M_X64)
			return true;
		#else
        	// If can modify flag register bit 21, the cpu is supports CPUID instruction
			__asm
			{
				// Read EFLAG
				pushfd
				pop     eax
				mov     ecx, eax

				// Modify bit 21
				xor     eax, 0x200000
				push    eax
				popfd

				// Read back EFLAG
				pushfd
				pop     eax

				// Restore EFLAG
				push    ecx
				popfd

				// Check bit 21 modifiable
				xor     eax, ecx
				neg     eax
				sbb     eax, eax

				// Return values in eax, no return statement requirement here for VC.
			}
    	#endif
	#elif F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG
		#if F_ARCHITECTURE == F_ARCHITECTURE_64
           return true;
       	#else
			unsigned oldFlags, newFlags;
			__asm__
			(
				"pushfl         \n\t"
				"pop    %0      \n\t"
				"mov    %0, %1  \n\t"
				"xor    %2, %0  \n\t"
				"push   %0      \n\t"
				"popfl          \n\t"
				"pushfl         \n\t"
				"pop    %0      \n\t"
				"push   %1      \n\t"
				"popfl          \n\t"
				: "=r" (oldFlags), "=r" (newFlags)
				: "n" (0x200000)
			);
			return oldFlags != newFlags;
       	#endif // 64
	#else
        //Not supports other compiler
        return false;
	#endif
	}

	static uint32 s_PerformCpuid(int32 query, CpuidResult& result)
	{
	#if F_COMPILER == F_COMPILER_MSVC
		int32 CPUInfo[4];
        __cpuid(CPUInfo, query);
        result._eax = CPUInfo[0];
        result._ebx = CPUInfo[1];
        result._ecx = CPUInfo[2];
        result._edx = CPUInfo[3];
        return result._eax;
	#elif F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG
		#if F_ARCHITECTURE == F_ARCHITECTURE_64
			__asm__
			(
				"cpuid": "=a" (result._eax), "=b" (result._ebx), "=c" (result._ecx), "=d" (result._edx) : "a" (query)
			);
        #else
			__asm__
			(
				"pushl  %%ebx           \n\t"
				"cpuid                  \n\t"
				"movl   %%ebx, %%edi    \n\t"
				"popl   %%ebx           \n\t"
				: "=a" (result._eax), "=D" (result._ebx), "=c" (result._ecx), "=d" (result._edx)
				: "a" (query)
			);
       	#endif 
        return result._eax;
	#else
		//Not supports other compiler
		return 0;
	#endif
	}

	#if F_COMPILER == F_COMPILER_MSVC
		#pragma warning(pop)
	#endif


	#if F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG
		#if F_ARCHITECTURE == F_ARCHITECTURE_32 && F_CPU == F_CPU_X86
			static jmp_buf sIllegalJmpBuf;
			static void s_IllegalHandler(int32 x)
			{
				(void)(x); // Unused
				longjmp(sIllegalJmpBuf, 1);
			}
		#endif
	#endif


	static bool s_CheckOperatingSystemSupportSSE()
	{
	#if F_COMPILER == F_COMPILER_MSVC
		#if _MSC_VER >= 1400 && defined(_M_X64)
            return true;
		#else
			__try
			{
				__asm orps  xmm0, xmm0
				return true;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
		#endif
	#elif F_COMPILER == F_COMPILER_GNUC || F_COMPILER == F_COMPILER_CLANG
		#if F_ARCHITECTURE == F_ARCHITECTURE_64 
            return true;
        #else
        // Does gcc have __try/__except similar mechanism?
        // Use signal, setjmp/longjmp instead.
        void (*oldHandler)(int32);
        oldHandler = signal(SIGILL, _illegalHandler);

        if (setjmp(sIllegalJmpBuf))
        {
            signal(SIGILL, oldHandler);
            return false;
        }
        else
        {
            __asm__ __volatile__ ("orps %xmm0, %xmm0");
            signal(SIGILL, oldHandler);
            return true;
        }
       #endif
	#else
        //Not supports other compiler, assumed is supported by default
        return true;
	#endif
	}


	static uint32 s_QueryCpuFeatures()
	{
	#define CPUID_FUNC_VENDOR_ID                 0x0
	#define CPUID_FUNC_STANDARD_FEATURES         0x1
	#define CPUID_FUNC_EXTENSION_QUERY           0x80000000
	#define CPUID_FUNC_EXTENDED_FEATURES         0x80000001
	#define CPUID_FUNC_ADVANCED_POWER_MANAGEMENT 0x80000007

	#define CPUID_STD_FPU               (1<<0)
	#define CPUID_STD_TSC               (1<<4)
	#define CPUID_STD_CMOV              (1<<15)
	#define CPUID_STD_MMX               (1<<23)
	#define CPUID_STD_SSE               (1<<25)
	#define CPUID_STD_SSE2              (1<<26)
	#define CPUID_STD_HTT               (1<<28)     // EDX[28] - Bit 28 set indicates  Hyper-Threading Technology is supported in hardware.

	#define CPUID_STD_SSE3              (1<<0)      // ECX[0]  - Bit 0 of standard function 1 indicate SSE3 supported
	#define CPUID_STD_SSE41             (1<<19)     // ECX[19] - Bit 0 of standard function 1 indicate SSE41 supported
	#define CPUID_STD_SSE42             (1<<20)     // ECX[20] - Bit 0 of standard function 1 indicate SSE42 supported

	#define CPUID_FAMILY_ID_MASK        0x0F00      // EAX[11:8] - Bit 11 thru 8 contains family  processor id
	#define CPUID_EXT_FAMILY_ID_MASK    0x0F00000   // EAX[23:20] - Bit 23 thru 20 contains extended family processor id
	#define CPUID_PENTIUM4_ID           0x0F00      // Pentium 4 family processor id

	#define CPUID_EXT_3DNOW             (1<<31)
	#define CPUID_EXT_AMD_3DNOWEXT      (1<<30)
	#define CPUID_EXT_AMD_MMXEXT        (1<<22)


	#define CPUID_APM_INVARIANT_TSC     (1<<8)      // EDX[8] - Bit 8 of function 0x80000007 indicates support for invariant TSC.

        uint32 features = 0;
        if (s_IsSupportCpuid())
        {
            CpuidResult result;
            if (s_PerformCpuid(CPUID_FUNC_VENDOR_ID, result))
            {
                if (memcmp(&result._ebx, "GenuineIntel", 12) == 0)
                {
                    if (result._eax > 2)
                        features |= E_CpuFeature_PRO;

                    s_PerformCpuid(CPUID_FUNC_STANDARD_FEATURES, result);
                    if (result._edx & CPUID_STD_FPU)
                        features |= E_CpuFeature_FPU;
                    if (result._edx & CPUID_STD_TSC)
                        features |= E_CpuFeature_TSC;
                    if (result._edx & CPUID_STD_CMOV)
                        features |= E_CpuFeature_CMOV;
                    if (result._edx & CPUID_STD_MMX)
                        features |= E_CpuFeature_MMX;
                    if (result._edx & CPUID_STD_SSE)
                        features |= E_CpuFeature_MMXEXT | E_CpuFeature_SSE;
                    if (result._edx & CPUID_STD_SSE2)
                        features |= E_CpuFeature_SSE2;
                    if (result._ecx & CPUID_STD_SSE3)
                        features |= E_CpuFeature_SSE3;
                    if (result._ecx & CPUID_STD_SSE41)
                        features |= E_CpuFeature_SSE41;
                    if (result._ecx & CPUID_STD_SSE42)
                        features |= E_CpuFeature_SSE42;

                    if ((result._eax & CPUID_EXT_FAMILY_ID_MASK) ||
                        (result._eax & CPUID_FAMILY_ID_MASK) == CPUID_PENTIUM4_ID)
                    {
                        if (result._edx & CPUID_STD_HTT)
                            features |= E_CpuFeature_HTT;
                    }

                    const uint32 maxExtensionFunctionSupport = s_PerformCpuid(CPUID_FUNC_EXTENSION_QUERY, result);
                    if (maxExtensionFunctionSupport >= CPUID_FUNC_ADVANCED_POWER_MANAGEMENT)
                    {
                        s_PerformCpuid(CPUID_FUNC_ADVANCED_POWER_MANAGEMENT, result);

                        if (result._edx & CPUID_APM_INVARIANT_TSC)
                            features |= E_CpuFeature_INVARIANT_TSC;
                    }
                }
                else if (memcmp(&result._ebx, "AuthenticAMD", 12) == 0)
                {
                    features |= E_CpuFeature_PRO;

                    s_PerformCpuid(CPUID_FUNC_STANDARD_FEATURES, result);
                    if (result._edx & CPUID_STD_FPU)
                        features |= E_CpuFeature_FPU;
                    if (result._edx & CPUID_STD_TSC)
                        features |= E_CpuFeature_TSC;
                    if (result._edx & CPUID_STD_CMOV)
                        features |= E_CpuFeature_CMOV;
                    if (result._edx & CPUID_STD_MMX)
                        features |= E_CpuFeature_MMX;
                    if (result._edx & CPUID_STD_SSE)
                        features |= E_CpuFeature_SSE;
                    if (result._edx & CPUID_STD_SSE2)
                        features |= E_CpuFeature_SSE2;
                    if (result._ecx & CPUID_STD_SSE3)
                        features |= E_CpuFeature_SSE3;

                    const uint32 maxExtensionFunctionSupport = s_PerformCpuid(CPUID_FUNC_EXTENSION_QUERY, result);
                    if (maxExtensionFunctionSupport >= CPUID_FUNC_EXTENDED_FEATURES)
                    {
                        s_PerformCpuid(CPUID_FUNC_EXTENDED_FEATURES, result);

                        if (result._edx & CPUID_EXT_3DNOW)
                            features |= E_CpuFeature_3DNOW;
                        if (result._edx & CPUID_EXT_AMD_3DNOWEXT)
                            features |= E_CpuFeature_3DNOWEXT;
                        if (result._edx & CPUID_EXT_AMD_MMXEXT)
                            features |= E_CpuFeature_MMXEXT;
                    }

                    if (maxExtensionFunctionSupport >= CPUID_FUNC_ADVANCED_POWER_MANAGEMENT)
                    {
                        s_PerformCpuid(CPUID_FUNC_ADVANCED_POWER_MANAGEMENT, result);

                        if (result._edx & CPUID_APM_INVARIANT_TSC)
                            features |= E_CpuFeature_TSC;
                    }
                }
            }
        }

        return features;
	}

	static uint32 s_DetectCpuFeatures()
    {
        uint32 features = s_QueryCpuFeatures();

        const uint32 sse_features = 0
            | E_CpuFeature_SSE
            | E_CpuFeature_SSE2
            | E_CpuFeature_SSE3
            | E_CpuFeature_SSE41
            | E_CpuFeature_SSE42;

        if ((features & sse_features) && !s_CheckOperatingSystemSupportSSE())
        {
            features &= ~sse_features;
        }

        return features;
    }

	static String s_DetectCpuIdentifier()
    {
        if (s_IsSupportCpuid())
        {
            CpuidResult result;
            uint32 nExIds;
            char CPUString[0x20];
            char CPUBrandString[0x40];

            std::ostringstream detailedIdentStr;

            if (s_PerformCpuid(0, result))
            {
                memset(CPUString, 0, sizeof(CPUString));
                memset(CPUBrandString, 0, sizeof(CPUBrandString));

                //*((int32*)CPUString) = result._ebx;
                //*((int32*)(CPUString+4)) = result._edx;
                //*((int32*)(CPUString+8)) = result._ecx;
                memcpy(CPUString, &result._ebx, sizeof(int32));
                memcpy(CPUString+4, &result._edx, sizeof(int32));
                memcpy(CPUString+8, &result._ecx, sizeof(int32));

                detailedIdentStr << CPUString;

                // Calling s_PerformCpuid with 0x80000000 as the query argument
                // gets the number of valid extended IDs.
                nExIds = s_PerformCpuid(0x80000000, result);

                for (uint32 i = 0x80000000; i <= nExIds; ++i)
                {
                    s_PerformCpuid(i, result);

                    // Interpret CPU brand string and cache information.
                    if (i == 0x80000002)
                    {
                        memcpy(CPUBrandString + 0, &result._eax, sizeof(result._eax));
                        memcpy(CPUBrandString + 4, &result._ebx, sizeof(result._ebx));
                        memcpy(CPUBrandString + 8, &result._ecx, sizeof(result._ecx));
                        memcpy(CPUBrandString + 12, &result._edx, sizeof(result._edx));
                    }
                    else if (i == 0x80000003)
                    {
                        memcpy(CPUBrandString + 16 + 0, &result._eax, sizeof(result._eax));
                        memcpy(CPUBrandString + 16 + 4, &result._ebx, sizeof(result._ebx));
                        memcpy(CPUBrandString + 16 + 8, &result._ecx, sizeof(result._ecx));
                        memcpy(CPUBrandString + 16 + 12, &result._edx, sizeof(result._edx));
                    }
                    else if (i == 0x80000004)
                    {
                        memcpy(CPUBrandString + 32 + 0, &result._eax, sizeof(result._eax));
                        memcpy(CPUBrandString + 32 + 4, &result._ebx, sizeof(result._ebx));
                        memcpy(CPUBrandString + 32 + 8, &result._ecx, sizeof(result._ecx));
                        memcpy(CPUBrandString + 32 + 12, &result._edx, sizeof(result._edx));
                    }
                }

                String brand(CPUBrandString);
                FUtilString::Trim(brand);
                if (!brand.empty())
                    detailedIdentStr << ": " << brand;

                return detailedIdentStr.str().c_str();
            }
        }

        return "X86";
    }

	static String s_GetCPUTypeName(int32 nType)
	{
		switch (nType)
		{
		case VENDOR_INTEL:
			return "Intel";
		case VENDOR_AMD:
			return "AMD";
		case VENDOR_CYRIX:
			return "Cyrix";
		case VENDOR_NEXGEN:
			return "NexGen";
		case VENDOR_TRANSMETA:
			return "Transmeta";
		case VENDOR_UMC:
			return "x86 UMC";
		case VENDOR_CENTAUR:
			return "x86 IDT";
		case VENDOR_RISE:
			return "x86 Rise";
		case VENDOR_SIS:
			return "x86 SIS";
		case VENDOR_NSC:
			return "x86 NSC";
		}

		return "Unknown";
	}

#elif F_PLATFORM == F_PLATFORM_ANDROID

	static uint32 s_DetectCpuFeatures()
    {
        uint32 features = 0;
	#if F_CPU == F_CPU_ARM
        uint64_t cpufeatures = android_getCpuFeatures();
        if (cpufeatures & ANDROID_CPU_ARM_FEATURE_NEON)
        {
            features |= E_CpuFeature_NEON;
        }
        
        if (cpufeatures & ANDROID_CPU_ARM_FEATURE_VFPv3) 
        {
            features |= E_CpuFeature_VFP;
        }

	#elif F_CPU == F_CPU_X86
        // see https://developer.android.com/ndk/guides/abis.html
        features |= E_CpuFeature_SSE;
        features |= E_CpuFeature_SSE2;
        features |= E_CpuFeature_SSE3;

	#endif
        return features;
    }

    static String s_DetectCpuIdentifier()
    {
        String cpuID;
        AndroidCpuFamily cpuInfo = android_getCpuFamily();
        
        switch (cpuInfo) 
		{
		case ANDROID_CPU_FAMILY_ARM64:
			cpuID = "ARM64";
			break;
		case ANDROID_CPU_FAMILY_ARM:
			{
				if (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_ARMv7) 
				{
					cpuID = "ARMv7";
				}
				else
				{
					cpuID = "Unknown ARM";
				}
			}
			break;
		case ANDROID_CPU_FAMILY_X86:
			cpuID = "Unknown X86";
			break;   
		default:
			cpuID = "Unknown";
			break;
        }
        return cpuID;
    }

#elif F_CPU == F_CPU_ARM

	static uint32 s_DetectCpuFeatures()
    {
        // Use preprocessor definitions to determine architecture and CPU features
        uint32 features = 0;
	#if defined(__ARM_NEON__)
		#if F_PLATFORM == F_PLATFORM_iOS
			int32 hasNEON;
			size_t len = sizeof(size_t);
			sysctlbyname("hw.optional.neon", &hasNEON, &len, NULL, 0);

			if(hasNEON)
		#endif
			features |= E_CpuFeature_NEON;
	#elif defined(__VFP_FP__)
			features |= E_CpuFeature_VFP;
	#endif
        return features;
    }

    static String s_DetectCpuIdentifier()
    {
        String cpuID;
	#if F_PLATFORM == F_PLATFORM_iOS
        // Get the size of the CPU subtype struct
        size_t size;
        sysctlbyname("hw.cpusubtype", NULL, &size, NULL, 0);

        // Get the ARM CPU subtype
        cpu_subtype_t cpusubtype = 0;
        sysctlbyname("hw.cpusubtype", &cpusubtype, &size, NULL, 0);

        switch(cpusubtype)
        {
		case CPU_SUBTYPE_ARM_V6:
			cpuID = "ARMv6";
			break;
		case CPU_SUBTYPE_ARM_V7:
			cpuID = "ARMv7";
			break;
		case CPU_SUBTYPE_ARM_V7F:
			cpuID = "ARM Cortex-A9";
			break;
		case CPU_SUBTYPE_ARM_V7S:
			cpuID = "ARM Swift";
			break;
		case CPU_SUBTYPE_ARM_V8:
			cpuID = "ARMv8";
			break;
		case CPU_SUBTYPE_ARM64_V8:
			cpuID = "ARM64v8";
			break;
		default:
			cpuID = "Unknown ARM";
			break;
        }
	#endif
        return cpuID;
    }

#elif F_CPU == F_CPU_MIPS

	static uint32 s_DetectCpuFeatures()
    {
        // Use preprocessor definitions to determine architecture and CPU features
        uint32 features = 0;
	#if defined(__mips_msa)
        features |= E_CpuFeature_MSA;
	#endif
        return features;
    }

    static String s_DetectCpuIdentifier()
    {
        String cpuID = "MIPS";

        return cpuID;
    }

#else 

    static uint32 s_DetectCpuFeatures()
    {
        return 0;
    }

    static String s_DetectCpuIdentifier()
    {
        return "Unknown";
    }

#endif

	const String& SystemCapabilities::GetCpuIdentifier()
	{
		static const String s_Identifier = s_DetectCpuIdentifier();
        return s_Identifier;
	}

	uint32 SystemCapabilities::GetCpuFeatures()
	{	
		static const uint32 s_Features = s_DetectCpuFeatures();
        return s_Features;
	}

	bool SystemCapabilities::HasCpuFeature(ECpuFeatureType eCPUFeature)
	{
		return (GetCpuFeatures() & eCPUFeature) != 0;
	}

    SystemCapabilities* SystemCapabilities::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	SystemCapabilities& SystemCapabilities::GetSingleton()
	{  
		F_Assert(ms_Singleton && "SystemCapabilities::GetSingleton")
		return (*ms_Singleton);     
	}


    SystemCapabilities::SystemCapabilities()
        : Base("SystemCapabilities")
        , m_bCPUInfoInit(false)
    {

    }

    SystemCapabilities::~SystemCapabilities()
    {
        Destroy();
    }

    bool SystemCapabilities::HasCPUCapability(int32 nKey)
    {
        Int2IntMap::iterator itFind = m_mapCPUCapabilities.find(nKey);
		if (itFind == m_mapCPUCapabilities.end())
			return false;

		return itFind->second > 0 ? true : false;
    }
	bool SystemCapabilities::HasCPUCapability(ECpuInfoType eType)
	{
		return HasCPUCapability((int32)eType);
	}

    void SystemCapabilities::Destroy()
    {

    }

    void SystemCapabilities::Init()
    {
        if (!this->m_bCPUInfoInit)
        {
            initCPUInfos();
			initMemoryInfos();
			m_bCPUInfoInit = true;
        }
		logInfos();		
    }

    void SystemCapabilities::initCPUInfos()
    {
		String strVendor = "";
		String strBrand = "";
		String strCPUCodeName = "";
		String strVenderType = "";
		int32 nVenderType = 0;
		int32 nNumCores = 0;
		int32 nLogicalCPUs = 0;
		int32 nTotalLC = 0;
		int32 nL1DataCache = 0;
		int32 nL1InstCache = 0;
		int32 nL2Cache = 0;
		int32 nL3Cache = 0;
		int32 nL1Cache_DataAsso = 0;
		int32 nL2Cache_Asso = 0;
		int32 nL3Cache_Asso = 0;
		int32 nL1Cache_DataLine = 0;
		int32 nL2Cache_Line = 0;
		int32 nL3Cache_Line = 0;
		int32 nSSESize = 0;

		bool isSupport_SSE = false;
		bool isSupport_SSE2 = false;
		bool isSupport_SSE3 = false;
		bool isSupport_SSE41 = false;
		bool isSupport_SSE42 = false;
		bool isSupport_MMX = false;
		bool isSupport_MMXEXT = false;
		bool isSupport_3DNOW = false;
		bool isSupport_3DNOWEXT = false;
		bool isSupport_CMOV = false;
		bool isSupport_TSC = false;
		bool isSupport_INVARIANT_TSC = false;
		bool isSupport_FPU = false;
		bool isSupport_PRO = false;
		bool isSupport_HTT = false;

		bool isSupport_VFP = false;
		bool isSupport_NEON = false;
		
		bool isSupport_MSA = false;

	#if F_CPU == F_CPU_X86
		cpu_id_t cpuID; 
		if (0 == cpu_identify(0, &cpuID))
		{
			strVendor = cpuID.vendor_str;
			strBrand = cpuID.brand_str;
			strCPUCodeName = cpuID.cpu_codename;
			nVenderType = (int32)cpuID.vendor;
			strVenderType = s_GetCPUTypeName(nVenderType);

			nNumCores = (int32)cpuID.num_cores;
			nLogicalCPUs = (int32)cpuID.num_logical_cpus;
			nTotalLC = (int32)cpuID.total_logical_cpus;
			nL1DataCache = (int32)cpuID.l1_data_cache;
			nL1InstCache = (int32)cpuID.l1_instruction_cache;
			nL2Cache = (int32)cpuID.l2_cache;
			nL3Cache = (int32)cpuID.l3_cache;
			nL1Cache_DataAsso = (int32)cpuID.l1_assoc;
			nL2Cache_Asso = (int32)cpuID.l2_assoc;
			nL3Cache_Asso = (int32)cpuID.l3_assoc;
			nL1Cache_DataLine = (int32)cpuID.l1_cacheline;
			nL2Cache_Line = (int32)cpuID.l2_cacheline;
			nL3Cache_Line = (int32)cpuID.l3_cacheline;
			nSSESize = (int32)cpuID.sse_size;
		}

		isSupport_SSE = HasCpuFeature(E_CpuFeature_SSE);
		isSupport_SSE2 = HasCpuFeature(E_CpuFeature_SSE2);
		isSupport_SSE3 = HasCpuFeature(E_CpuFeature_SSE3);
		isSupport_SSE41 = HasCpuFeature(E_CpuFeature_SSE41);
		isSupport_SSE42 = HasCpuFeature(E_CpuFeature_SSE42);
		isSupport_MMX = HasCpuFeature(E_CpuFeature_MMX);
		isSupport_MMXEXT = HasCpuFeature(E_CpuFeature_MMXEXT);
		isSupport_3DNOW = HasCpuFeature(E_CpuFeature_3DNOW);
		isSupport_3DNOWEXT = HasCpuFeature(E_CpuFeature_3DNOWEXT);
		isSupport_CMOV = HasCpuFeature(E_CpuFeature_CMOV);
		isSupport_TSC = HasCpuFeature(E_CpuFeature_TSC);
		isSupport_INVARIANT_TSC = HasCpuFeature(E_CpuFeature_INVARIANT_TSC);
		isSupport_FPU = HasCpuFeature(E_CpuFeature_FPU);
		isSupport_PRO = HasCpuFeature(E_CpuFeature_PRO);
		isSupport_HTT = HasCpuFeature(E_CpuFeature_HTT);

	#elif F_CPU == F_CPU_ARM
		isSupport_VFP = HasCpuFeature(E_CpuFeature_VFP);
		isSupport_NEON = HasCpuFeature(E_CpuFeature_NEON);

	#elif F_CPU == F_CPU_PPC


	#elif F_CPU == F_CPU_MIPS
		isSupport_MSA = HasCpuFeature(E_CpuFeature_MSA);

	#endif

		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_Vender)]			= "CPU Vendor: " + strVendor;															//0													
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_Brand)]				= "CPU Brand: " + strBrand;																//1
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_Code)]				= "CPU Code: " + strCPUCodeName;														//2
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_VenderType)]		= "CPU Type: " + strVenderType;															//3
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_NumCore)]			= FUtilString::FormatString("CPU NumCores: %d", nNumCores);							    //4
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_LogicalCpus)]		= FUtilString::FormatString("CPU LogicalCPUs: %d", nLogicalCPUs);						//5
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_TotalLogicalCpus)]	= FUtilString::FormatString("CPU TotalLogicalCPUs: %d", nTotalLC);						//6
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L1DataCache)]		= FUtilString::FormatString("CPU L1DataCache: %d K", nL1DataCache);					    //7
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L1InstCache)]		= FUtilString::FormatString("CPU L1InstCache: %d K", nL1InstCache);					    //8
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L2Cache)]			= FUtilString::FormatString("CPU L2Cache: %d K", nL2Cache);							    //9
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L3Cache)]			= FUtilString::FormatString("CPU L3Cache: %d K", nL3Cache);							    //10
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L1Cache_DataAsso)]	= FUtilString::FormatString("CPU L1Cache_DataAsso: %d", nL1Cache_DataAsso);			    //11
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L2Cache_Asso)]		= FUtilString::FormatString("CPU L2Cache_Asso: %d", nL2Cache_Asso);					    //12
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L3Cache_Asso)]		= FUtilString::FormatString("CPU L3Cache_Asso: %d", nL3Cache_Asso);					    //13
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L1Cache_DataLine)]	= FUtilString::FormatString("CPU L1Cache_DataLine: %d", nL1Cache_DataLine);			    //14
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L2Cache_Line)]		= FUtilString::FormatString("CPU L2Cache_Line: %d", nL2Cache_Line);					    //15
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_L3Cache_Line)]		= FUtilString::FormatString("CPU L3Cache_Line: %d", nL3Cache_Line);					    //16
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_SSESize)]			= FUtilString::FormatString("CPU SSESize: %d", nSSESize);								//17
		
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_SSE)]				= String("CPU SSE: ") + (isSupport_SSE ? "Yes" : "No");									//18
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_SSE2)]				= String("CPU SSE2: ") + (isSupport_SSE2 ? "Yes" : "No");								//19
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_SSE3)]				= String("CPU SSE3: ") + (isSupport_SSE3 ? "Yes" : "No");								//20
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_SSE41)]				= String("CPU SSE41: ") + (isSupport_SSE41 ? "Yes" : "No");								//21
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_SSE42)]				= String("CPU SSE42: ") + (isSupport_SSE42 ? "Yes" : "No");								//22
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_MMX)]				= String("CPU MMX: ") + (isSupport_MMX ? "Yes" : "No");									//23		
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_MMXEXT)]			= String("CPU MMXEXT: ") + (isSupport_MMXEXT ? "Yes" : "No");							//24
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_3DNOW)]				= String("CPU 3DNOW: ") + (isSupport_3DNOW ? "Yes" : "No");								//25
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_3DNOWEXT)]			= String("CPU 3DNOWEXT: ") + (isSupport_3DNOWEXT ? "Yes" : "No");						//26
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_CMOV)]				= String("CPU CMOV: ") + (isSupport_CMOV ? "Yes" : "No");								//27
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_TSC)]				= String("CPU TSC: ") + (isSupport_TSC ? "Yes" : "No");									//28
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_INVARIANT_TSC)]		= String("CPU INVARIANT_TSC: ") + (isSupport_INVARIANT_TSC ? "Yes" : "No");				//29
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_FPU)]				= String("CPU FPU: ") + (isSupport_FPU ? "Yes" : "No");									//30
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_PRO)]				= String("CPU PRO: ") + (isSupport_PRO ? "Yes" : "No");									//31
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_HTT)]				= String("CPU HTT: ") + (isSupport_HTT ? "Yes" : "No");									//32
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_VFP)]				= String("CPU VFP: ") + (isSupport_VFP ? "Yes" : "No");									//33
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_NEON)]				= String("CPU NEON: ") + (isSupport_NEON ? "Yes" : "No");								//34
		m_mapCPUInfo[E_GetCpuInfoTypeName(E_CpuInfo_MSA)]				= String("CPU MSA: ") + (isSupport_MSA ? "Yes" : "No");									//35
		
		String2StringMap::iterator it,itEnd;
		itEnd = m_mapCPUInfo.end();
		for (it = m_mapCPUInfo.begin(); it != itEnd; ++it)
		{
			m_aCPUKeys.push_back(it->first);
		}
		
		m_mapCPUCapabilities[E_CpuInfo_VenderType]		    =  nVenderType;						//3
		m_mapCPUCapabilities[E_CpuInfo_NumCore]			    =  nNumCores;						//4
		m_mapCPUCapabilities[E_CpuInfo_LogicalCpus]		    =  nLogicalCPUs;					//5
		m_mapCPUCapabilities[E_CpuInfo_TotalLogicalCpus]	=  nTotalLC;						//6
		m_mapCPUCapabilities[E_CpuInfo_L1DataCache]		    =  nL1DataCache;					//7
		m_mapCPUCapabilities[E_CpuInfo_L1InstCache]		    =  nL1InstCache;					//8
		m_mapCPUCapabilities[E_CpuInfo_L2Cache]  			=  nL2Cache;						//9
		m_mapCPUCapabilities[E_CpuInfo_L3Cache]  			=  nL3Cache;						//10
		m_mapCPUCapabilities[E_CpuInfo_L1Cache_DataAsso]	=  nL1Cache_DataAsso;				//11
		m_mapCPUCapabilities[E_CpuInfo_L2Cache_Asso]		=  nL2Cache_Asso;					//12
		m_mapCPUCapabilities[E_CpuInfo_L3Cache_Asso]		=  nL3Cache_Asso;					//13
		m_mapCPUCapabilities[E_CpuInfo_L1Cache_DataLine]	=  nL1Cache_DataLine;				//14
		m_mapCPUCapabilities[E_CpuInfo_L2Cache_Line]		=  nL2Cache_Line;					//15
		m_mapCPUCapabilities[E_CpuInfo_L3Cache_Line]		=  nL3Cache_Line;					//16
		m_mapCPUCapabilities[E_CpuInfo_SSESize]			    =  nSSESize;						//17

		m_mapCPUCapabilities[E_CpuInfo_SSE]				    =  isSupport_SSE ? 1 : 0;			//18
		m_mapCPUCapabilities[E_CpuInfo_SSE2]				=  isSupport_SSE2 ? 1 : 0;			//19
		m_mapCPUCapabilities[E_CpuInfo_SSE3]				=  isSupport_SSE3 ? 1 : 0;			//20
		m_mapCPUCapabilities[E_CpuInfo_SSE41]				=  isSupport_SSE41 ? 1 : 0;			//21
		m_mapCPUCapabilities[E_CpuInfo_SSE42]				=  isSupport_SSE42 ? 1 : 0;			//22
		m_mapCPUCapabilities[E_CpuInfo_MMX]				    =  isSupport_MMX ? 1 : 0;			//23
		m_mapCPUCapabilities[E_CpuInfo_MMXEXT]			    =  isSupport_MMXEXT ? 1 : 0;		//24
		m_mapCPUCapabilities[E_CpuInfo_3DNOW]				=  isSupport_3DNOW ? 1 : 0;			//25
		m_mapCPUCapabilities[E_CpuInfo_3DNOWEXT]			=  isSupport_3DNOWEXT ? 1 : 0;		//26
		m_mapCPUCapabilities[E_CpuInfo_CMOV]				=  isSupport_CMOV ? 1 : 0;			//27
		m_mapCPUCapabilities[E_CpuInfo_TSC]				    =  isSupport_TSC ? 1 : 0;			//28
		m_mapCPUCapabilities[E_CpuInfo_INVARIANT_TSC]		=  isSupport_INVARIANT_TSC ? 1 : 0;	//29
		m_mapCPUCapabilities[E_CpuInfo_FPU]				    =  isSupport_FPU ? 1 : 0;			//30
		m_mapCPUCapabilities[E_CpuInfo_PRO]				    =  isSupport_PRO ? 1 : 0;			//31
		m_mapCPUCapabilities[E_CpuInfo_HTT]				    =  isSupport_HTT ? 1 : 0;			//32
		m_mapCPUCapabilities[E_CpuInfo_VFP]				    =  isSupport_VFP ? 1 : 0;			//33
		m_mapCPUCapabilities[E_CpuInfo_NEON]				=  isSupport_NEON ? 1 : 0;			//34
		m_mapCPUCapabilities[E_CpuInfo_MSA]				    =  isSupport_MSA ? 1 : 0;			//35
	}
	void SystemCapabilities::initMemoryInfos()
	{

	}

	void SystemCapabilities::logInfos()
    {
		F_LogInfo("---------- CPU Identifier & Features ----------");
		{
			int32 count = (int32)E_CpuInfo_Count;
			for (int32 i = 0; i < count; i++)
			{
				const String& name = E_GetCpuInfoTypeName(i);
				F_LogInfo(m_mapCPUInfo[name].c_str());
			}
		}
		F_LogInfo("---------- CPU Identifier & Features ----------");
    }

}; //LostPeterEngine